
/*************************************************************************************************************************
 *   Viessmann UDS Daemon @ Sinclari81 2026                                                                              *
 *************************************************************************************************************************
 * 
 * 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  
 * Industrial ESP32-S3 Control Board With RS485 And CAN Communication Interfaces,
 * Supports Wi-Fi / Bluetooth, Built-in Multiple Protection Circuits
 * 
 * -> ESP32-S3-RS485-CAN
 * 
 * https://www.waveshare.com/esp32-s3-rs485-can.htm?srsltid=AU7gw4UBb7m5bKDP9epj_9h0QPumj4ANPL_AVdqtJ7dzOMvFQwdwILDE
 * 
 * Arduino IDE:
 * - Board:           ESP32S3 Dev Module
 * - USB CDC On Boot: Enabled
 * - Flash Size:      16MB (128Mb)
 * - PartitionSchema: 16M Flash (3MB APP/9.9MB FATFS)
 * - PSRAM:           OPI PSRAM
 * 
 * Arduino LIBs:
 * - Async TCP (ESP32Async)
 * - ESP Async WebServer (ESP32Async)
 * - ElegantOTA (Ayush Sharma)
 * 
 */

#include <WiFi.h>
#include <AsyncTCP.h>       
#include <ESPAsyncWebServer.h> 
#include <ElegantOTA.h>
#include "driver/twai.h"
#include "CanDecoder.h"
#include "secrets.h"

// Wi-Fi access data - Adjust in secrets.h
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

// Pins & CAN-IDs for ESP32-S3 (GPIO 15 & 16)
#define TX_PIN GPIO_NUM_15
#define RX_PIN GPIO_NUM_16
#define CAN_REQUEST_ID  0x680
#define CAN_RESPONSE_ID 0x690

// Registrierte Variablen zur zyklischen Abfrage
Utf8String          identification(  377, "ViessmannIdentificationNumber");
SingleTemp          hotWaterSetpoint(396, "DomesticHotWaterTemperatureSetpoint");
MultiTempWithStatus outsideSensor(   274, "OutsideTemperatureSensor");
Pump                heatingPump(     381, "CentralHeatingPump");

// State-Machine Variablen für den blockierungsfreien UDS-Ablauf
unsigned long lastQueryTime = 0;
const unsigned long queryInterval = 60000; // 1 Minute Gesamtintervall
unsigned long lastStepTime = 0;
const unsigned long stepInterval = 150;    // Sichere 150ms Pause zwischen den DIDs OHNE Blockade
uint8_t currentQueryStep = 0;             // Aktueller Schritt im Ablauf (0 = ID, 1 = Setpoint, 2 = Outside, 3 = Pump, 4 = Warten)

bool isWaitingForConsecutive = false;
uint16_t activeMultiFrameDid = 0;
uint8_t multiFramePayloadByteMax = 0;
uint8_t multiFramePayloadByteCount = 0;
uint8_t multiFrameBuffer[256] = {0};

AsyncWebServer server(80);

// HTML & CSS für die schlanke Tabellen-Ansicht
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Viessmann UDS Daemon</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
    <style>
        body { font-family: Arial, sans-serif; background: #f4f4f9; color: #333; margin: 0; padding: 20px; display: flex; justify-content: center; }
        .container { width: 100%; max-width: 900px; background: white; padding: 25px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
        h1 { color: #cc0000; margin-top: 0; margin-bottom: 5px; font-size: 1.8rem; }
        p { color: #666; margin-bottom: 25px; font-size: 0.95rem; }
        table { width: 100%; border-collapse: collapse; margin-top: 10px; box-shadow: 0 2px 4px rgba(0,0,0,0.05); }
        th, td { padding: 12px 15px; text-align: left; border-bottom: 1px solid #ddd; }
        th { background-color: #cc0000; color: white; font-weight: bold; font-size: 0.95rem; }
        tr:hover { background-color: #f9f9f9; }
        td:first-child { font-family: monospace; font-weight: bold; color: #555; width: 80px; }
        td:nth-child(2) { font-weight: 600; color: #222; }
        .val { font-family: monospace; font-weight: bold; color: #008800; font-size: 1.05rem; }
        .btn-ota { display: inline-block; margin-top: 20px; padding: 10px 20px; background: #0066cc; color: white; text-decoration: none; border-radius: 4px; font-weight: bold; font-size: 0.9rem; float: right; }
        .btn-ota:hover { background: #004499; }
    </style>
    <script>
        function updateData() {
            fetch('/data').then(response => response.json()).then(data => {
                data.forEach(item => {
                    let row = document.getElementById('did_' + item.did);
                    if(row) {
                        row.querySelector('.val').innerText = item.value;
                    }
                });
            });
        }
        setInterval(updateData, 2000); // UI alle 2 Sekunden aktualisieren
    </script>
</head>
<body>
    <div class="container">
        <a href="/update" class="btn-ota">🚀 OTA Update</a>
        <h1>Viessmann UDS Daemon</h1>
        <p>Zyklische CAN-Bus Telemetrieabfrage (Intervall: 1 Minute)</p>
        <table>
            <thead>
                <tr>
                    <th>DID</th>
                    <th>Name</th>
                    <th>Aktueller Wert (getFormattedString)</th>
                </tr>
            </thead>
            <tbody>
                <tr id="did_377"><td>377</td><td>ViessmannIdentificationNumber</td><td class="val">---</td></tr>
                <tr id="did_396"><td>396</td><td>DomesticHotWaterTemperatureSetpoint</td><td class="val">---</td></tr>
                <tr id="did_274"><td>274</td><td>OutsideTemperatureSensor</td><td class="val">---</td></tr>
                <tr id="did_381"><td>381</td><td>CentralHeatingPump</td><td class="val">---</td></tr>
            </tbody>
        </table>
    </div>
</body>
</html>
)rawliteral";

// Sendet eine UDS-Abfrage für eine bestimmte DID an den CAN-Bus
void sendUdsRequest(uint16_t did) {
    twai_message_t tx_msg;
    tx_msg.identifier = CAN_REQUEST_ID;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    tx_msg.data[0] = 0x03; // PCI-Byte: Länge der UDS-Daten (3 Bytes folgend)
    tx_msg.data[1] = 0x22; // UDS Service: ReadDataByIdentifier
    tx_msg.data[2] = (uint8_t)(did >> 8);   // DID High Byte
    tx_msg.data[3] = (uint8_t)(did & 0xFF);  // DID Low Byte
    for(int i = 4; i < 8; i++) tx_msg.data[i] = 0x00;

    twai_transmit(&tx_msg, pdMS_TO_TICKS(10));
}

// Sendet den ISO-TP Flow Control Frame, um dem Heizkessel zu sagen, er darf Folgeframes schicken
void sendFlowControl() {
    twai_message_t fc_msg;
    fc_msg.identifier = CAN_REQUEST_ID;
    fc_msg.extd = 0;
    fc_msg.rtr = 0;
    fc_msg.data_length_code = 8;
    fc_msg.data[0] = 0x30; // Flow Control Frame (Clear To Send)
    fc_msg.data[1] = 0x00; // Block Size = 0 (Sende alles)
    fc_msg.data[2] = 0x00; // Separation Time = 0ms
    for(int i = 3; i < 8; i++) fc_msg.data[i] = 0x00;
    
    twai_transmit(&fc_msg, pdMS_TO_TICKS(10));
}

// Ordnet empfangene Rohdaten anhand der DID der passenden Klasseninstanz zu
void routePayloadToDecoder(uint16_t did, const uint8_t* payload, size_t length) {
    switch (did) {
        case 377: identification.parse(payload, length); break;
        case 396: hotWaterSetpoint.parse(payload); break;
        case 274: outsideSensor.parse(payload); break;
        case 381: heatingPump.parse(payload); break;
    }
}

// Verarbeitet eintreffende CAN-Bus Frames (ISO-TP Transportprotokoll)
void checkForResponse() {
    twai_message_t rx_msg;
    if (twai_receive(&rx_msg, pdMS_TO_TICKS(1)) == ESP_OK) {
        if (rx_msg.identifier == CAN_RESPONSE_ID) {
            
            // MULTI-FRAME: ERSTER FRAME (0x10) -> Kessel kündigt lange Antwort an
            if ((rx_msg.data[0] & 0xF0) == 0x10 && rx_msg.data[2] == 0x62) {
                uint16_t totalLen = ((uint16_t)(rx_msg.data[0] & 0x0F) << 8) | rx_msg.data[1];
                activeMultiFrameDid = ((uint16_t)rx_msg.data[3] << 8) | rx_msg.data[4];
                
                multiFramePayloadByteMax = totalLen - 3; // Gesamtlänge minus Service(1B) und DID(2B)
                multiFramePayloadByteCount = 0;
                isWaitingForConsecutive = true;
                
                // Erste 3 Bytes der Nutzdaten aus dem First-Frame sichern (Index 5, 6, 7)
                for (int i = 5; i < 8; i++) {
                    if (multiFramePayloadByteCount < multiFramePayloadByteMax) {
                        multiFrameBuffer[multiFramePayloadByteCount++] = rx_msg.data[i];
                    }
                }
                sendFlowControl(); 
            }
            
            // MULTI-FRAME: FOLGEFRAMES (0x20 bis 0x2F)
            else if ((rx_msg.data[0] & 0xF0) == 0x20 && isWaitingForConsecutive) {
                for (int i = 1; i < 8; i++) {
                    if (multiFramePayloadByteCount < multiFramePayloadByteMax) {
                        multiFrameBuffer[multiFramePayloadByteCount++] = rx_msg.data[i];
                    }
                }
                // Wenn alle erwarteten Bytes da sind, decodieren
                if (multiFramePayloadByteCount >= multiFramePayloadByteMax) {
                    isWaitingForConsecutive = false;
                    routePayloadToDecoder(activeMultiFrameDid, multiFrameBuffer, multiFramePayloadByteCount);
                }
            }
            
            // SINGLE-FRAME (0x0X): Antwort passt komplett in ein Datenframe
            else if (rx_msg.data[1] == 0x62) {
                isWaitingForConsecutive = false; 
                uint8_t framePayloadByteMax = rx_msg.data[0] - 3; 
                uint16_t parsedDid = ((uint16_t)rx_msg.data[2] << 8) | rx_msg.data[3];
                
                uint8_t singleFrameBuffer[8] = {0};
                for (int i = 0; i < framePayloadByteMax; i++) {
                    if (i < 4) {
                        singleFrameBuffer[i] = rx_msg.data[4 + i];
                    }
                }
                routePayloadToDecoder(parsedDid, singleFrameBuffer, framePayloadByteMax);
            }
            
            // UDS FEHLER FRAME ODER ABBRUCH
            else if (rx_msg.data[1] == 0x7F) {
                isWaitingForConsecutive = false;
            }
        }
    }
}

// Blockierungsfreie State-Machine zur Taktung der CAN-Abfragen
void handleDaemonTimer() {
    unsigned long currentMillis = millis();

    // 1. Prüfen, ob die 1 Minute Wartezeit um ist, um die Kette neu zu starten
    if (currentQueryStep == 4) {
        if (currentMillis - lastQueryTime >= queryInterval) {
            lastQueryTime = currentMillis;
            currentQueryStep = 0; // Zurück zu Schritt 0 (Identifikation starten)
            lastStepTime = currentMillis;
        }
        return; // Im Warteschritt tun wir sonst nichts
    }

    // 2. Die einzelnen Abfrage-Schritte im Abstand von 'stepInterval' (150ms) abarbeiten
    if (currentMillis - lastStepTime >= stepInterval) {
        lastStepTime = currentMillis;

        switch (currentQueryStep) {
            case 0:
                sendUdsRequest(identification.getDid());
                currentQueryStep = 1;
                break;
            case 1:
                sendUdsRequest(hotWaterSetpoint.getDid());
                currentQueryStep = 2;
                break;
            case 2:
                sendUdsRequest(outsideSensor.getDid());
                currentQueryStep = 3;
                break;
            case 3:
                sendUdsRequest(heatingPump.getDid());
                currentQueryStep = 4; // Kette beendet, in den Wartemodus wechseln
                break;
        }
    }
}

void setup() {
    // Wi-Fi Verbindung aufbauen
    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    // TWAI/CAN-Treiber für ESP32-S3 initialisieren
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config); 
    twai_start();

    // Zeitstempel initialisieren, damit die Abfrage sofort beim Start loslegt
    lastQueryTime = millis() - queryInterval; 

    // Webserver Endpunkte einrichten
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(200, "text/html; charset=UTF-8", String(index_html)); 
    });
    
    // JSON-Endpunkt liefert die aktuellen Werte für das Web-UI Frontend
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "[";
        json += "{\"did\":" + String(identification.getDid())   + ",\"value\":\"" + String(identification.getString()) + "\"},";
        json += "{\"did\":" + String(hotWaterSetpoint.getDid()) + ",\"value\":\"" + hotWaterSetpoint.getFormattedString() + "\"},";
        json += "{\"did\":" + String(outsideSensor.getDid())    + ",\"value\":\"" + outsideSensor.getFormattedString() + "\"},";
        json += "{\"did\":" + String(heatingPump.getDid())      + ",\"value\":\"" + heatingPump.getFormattedString() + "\"}";
        json += "]";
        request->send(200, "application/json", json);
    });

    ElegantOTA.begin(&server); 
    server.begin(); 
}

void loop() {
    ElegantOTA.loop();
    checkForResponse();  // Läuft jetzt durchgehend mit maximaler Geschwindigkeit
    handleDaemonTimer(); // Schaltet ohne Delays die Abfragen sequenziell durch
    yield();
}
