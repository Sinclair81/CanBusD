
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
#include <esp_task_wdt.h>
#include <AsyncTCP.h>       
#include <ESPAsyncWebServer.h> 
#include <ElegantOTA.h>
#include "driver/twai.h"
#include <CanDecoder.h>
#include <secrets.h>    // USE THIS: Loads central credentials safely from the global Arduino libraries folder
// #include "secrets.h" // OPTIONAL: Fallback for local "secrets.h", fill it out, and uncomment this line if needed

#define WATCHDOG_TIMEOUT_SECONDS 60

// Wi-Fi access data - Adjust in secrets.h
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

// Pins & CAN-IDs for ESP32-S3 (GPIO 15 & 16)
#define TX_PIN GPIO_NUM_15
#define RX_PIN GPIO_NUM_16
#define CAN_REQUEST_ID  0x680
#define CAN_RESPONSE_ID 0x690

// Registered variables for cyclical polling
ListDummy dummy1(                                                            999901, "🌡️ Temperaturen");
MultiTempWithStatus outsideTemperatureSensor(                                   274, "Außentemperatur",                0b11110000, "; ");
MultiTempWithStatus flowTemperatureSensor(                                      268, "Vorlauftemperatur",              0b11110000, "; ");
MultiTempWithStatus returnTemperatureSensor(                                    269, "Rücklauftemperatur",             0b11110000, "; ");
ListDummy dummy2(                                                            999902, "🚰 Warmwasser");
MultiTempWithStatus domesticHotWaterSensor(                                     271, "Temperatur",                     0b11110000, "; ");
SingleTemp domesticHotWaterTemperatureSetpoint(                                 396, "Solltemperatur",                 0b00000001);
Hysteresis domesticHotWaterHysteresis(                                         1085, "Hysterese",                      0xFE,       "; ");
ListDummy dummy3(                                                            999903, "🛡️ Legionellenschutz");
ModeAndState legionellaProtectionActivation(                                    873, "Aktivierung",                    0xFE,       "; ");
TempWithStatus legionellaProtectionTargetTemperatureSetpoint(                   874, "Zieltemperatur",                 0b10000001, "; ");
CanTime legionellaProtectionStartTime(                                          875, "Startzeit");
CanDay legionellaProtectionWeekday(                                             876, "Wochentag");
CanTimeWithSecond legionellaProtectionLastSuccessfulStartTime(                  877, "letzte erfolgreiche Startzeit");
CanDay legionellaProtectionLastSuccessfulWeekday(                               878, "etzter erfolgreicher Wochentag");
ListDummy dummy4(                                                            999904, "🌀 Verdichter");
MultiTempWithStatus compressorInletTemperatureSensor(                           321, "Einlasstemperatur",              0b10000001, "; ");
MultiTempWithStatus compressorOutletTemperatureSensor(                          324, "Auslasstemperatur",              0b10000001, "; ");
PressureWithStatus compressorInletPressureSensor(                               322, "Einlassdruck",                   0b10000001, "; ");
PressureWithStatus compressorOutletPressureSensor(                              325, "Auslassdruck",                   0b10000001, "; ");
CanInt16 compressorSpeedRps(                                                   2569, "Drehzahl",                       "", "rps");
ListDummy dummy5(                                                            999905, "⚙️ System");
FlowSensor allengraSensor(                                                     1043, "Allengra ",                      0b10000001, "; ");
CanInt8 primaryCircuitFanOne(                                                  1775, "Primärkreis Ventilator 1",       "", "");
MultiTempWithStatus economizerLiquidTemperatureSensor(                         2333, "Zwischenkühler Flüssigkeit",     0b10000001, "; ");
MultiTempWithStatus evaporatorVaporTemperatureSensor(                          2334, "Verdampfer Dampftemperatur",     0b10000001, "; ");
ListDummy dummy6(                                                            999906, "📊 Statistik");
HeatPumpCompressor heatPumpCompressorStatistical(                              2369, "Wärmepumpen Verdichter",         0b01010000, "; ");
AdditionalElectricHeater additionalElectricHeaterStatistical(                  2370, "Elektrischer Heizstab",          0b01010000, "; ");
ListDummy dummy7(                                                            999907, "⚡ Elektrische Leistung");
CanInt32 currentElectricalPowerConsumptionRefrigerantCircuit(                  2486, "Kältemittelkreis",               "", "W");
CanInt32 currentElectricalPowerConsumptionElectricHeater(                      2487, "Heizstab",                       "", "W");
CanInt32 currentElectricalPowerConsumptionSystem(                              2488, "System",                         "", "W");
ListDummy dummy8(                                                            999908, "♨️ Thermische Leistung");
CanInt32 currentThermalCapacityRefrigerantCircuit(                             2494, "Kältemittelkreis",               "", "W");
CanInt32 currentThermalCapacityElectricHeater(                                 2495, "Heizstab",                       "", "W");
CanInt32 currentThermalCapacitySystem(                                         2496, "System",                         "", "W");
ListDummy dummy9(                                                            999909, "🔌 Energieverbrauch");
EnergyConsumption energyConsumptionCentralHeating(                              548, "Heizung",                        0b10101000, "; ");
EnergyConsumption energyConsumptionDomesticHotWater(                            565, "Warmwasser",                     0b10101000, "; ");
ListDummy dummy10(                                                           999910, "🔌 Energieverbrauch Heizstab");
EnergyConsumption energyConsumptionDomesticHotWaterElectricHeater(             2517, "Warmwasser",                     0b10101000, "; ");
EnergyConsumption energyConsumptionCentralHeatingElectricHeater(               2526, "Heizung",                        0b10101000, "; ");
ListDummy dummy11(                                                           999911, "🔥 Erzeugte Wärmemenge");
EnergyConsumption generatedCentralHeatingOutput(                               1211, "Heizung",                        0b10101000, "; ");
EnergyConsumption generatedDomesticHotWaterOutput(                             1391, "Warmwasser",                     0b10101000, "; ");
ListDummy dummy12(                                                           999912, "📈 Jahresarbeitszahlen");
SeasonalCoefficient seasonalCoefficientOfPerformanceHeating(                   2622, "Heizen",                         0xFE,       "; ");
SeasonalCoefficient seasonalCoefficientOfPerformanceDomesticHotWater(          2624, "Warmwasser",                     0xFE,       "; ");
SeasonalCoefficient seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater(2625, "Heizen und Warmwasser",          0xFE,       "; ");

// Here, we group all objects together as pointers in an array.
const uint8_t NUM_OBJECTS = 50;
CanObject* canObjects[NUM_OBJECTS] = {
    &dummy1,
    &outsideTemperatureSensor,
    &flowTemperatureSensor,
    &returnTemperatureSensor,
    &dummy2,
    &domesticHotWaterSensor,
    &domesticHotWaterTemperatureSetpoint,
    &domesticHotWaterHysteresis,
    &dummy3,
    &legionellaProtectionActivation,
    &legionellaProtectionTargetTemperatureSetpoint,
    &legionellaProtectionStartTime,
    &legionellaProtectionWeekday,
    &legionellaProtectionLastSuccessfulStartTime,
    &legionellaProtectionLastSuccessfulWeekday,
    &dummy4,
    &compressorInletTemperatureSensor,
    &compressorOutletTemperatureSensor,
    &compressorInletPressureSensor,
    &compressorOutletPressureSensor,
    &compressorSpeedRps,
    &dummy5,
    &allengraSensor,
    &primaryCircuitFanOne,
    &economizerLiquidTemperatureSensor,
    &evaporatorVaporTemperatureSensor,
    &dummy6,
    &heatPumpCompressorStatistical,
    &additionalElectricHeaterStatistical,
    &dummy7,
    &currentElectricalPowerConsumptionRefrigerantCircuit,
    &currentElectricalPowerConsumptionElectricHeater,
    &currentElectricalPowerConsumptionSystem,
    &dummy8,
    &currentThermalCapacityRefrigerantCircuit,
    &currentThermalCapacityElectricHeater,
    &currentThermalCapacitySystem,
    &dummy9,
    &energyConsumptionCentralHeating,
    &energyConsumptionDomesticHotWater,
    &dummy10,
    &energyConsumptionDomesticHotWaterElectricHeater,
    &energyConsumptionCentralHeatingElectricHeater,
    &dummy11,
    &generatedCentralHeatingOutput,
    &generatedDomesticHotWaterOutput,
    &dummy12,
    &seasonalCoefficientOfPerformanceHeating,
    &seasonalCoefficientOfPerformanceDomesticHotWater,
    &seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater
};

unsigned long lastQueryTime = 0;
const unsigned long queryInterval = 60000;
unsigned long lastStepTime = 0;
const unsigned long stepInterval = 150;
uint8_t currentQueryStep = 0;

bool isWaitingForConsecutive = false;
uint16_t activeMultiFrameDid = 0;
uint8_t multiFramePayloadByteMax = 0;
uint8_t multiFramePayloadByteCount = 0;
uint8_t multiFrameBuffer[256] = {0};

AsyncWebServer server(80);

// HTML & CSS for a streamlined table view
// HTML & CSS für eine strukturierte Tabellen-Ansicht mit echten Kategorien / right
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Viessmann UDS Daemon</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
    <style>
        body { font-family: Arial, sans-serif; background: #f4f4f9; color: #333; margin: 0; padding: 20px; display: flex; justify-content: center; }
        .container { max-width: 90vw; max-width: 1600px; background: white; padding: 25px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
        h1 { color: #cc0000; margin-top: 0; margin-bottom: 5px; font-size: 1.8rem; }
        p { color: #666; margin-bottom: 25px; font-size: 0.95rem; }
        table { width: 100%; border-collapse: collapse; margin-top: 10px; box-shadow: 0 2px 4px rgba(0,0,0,0.05); }
        th, td { padding: 10px 15px; text-align: left; border-bottom: 1px solid #ddd; }
        th { background-color: #cc0000; color: white; font-weight: bold; font-size: 0.95rem; }
        tr:hover:not(.category-row) { background-color: #f9f9f9; }
        td:first-child { font-family: monospace; font-weight: bold; color: #777; font-size: 1.05rem; }
        td:nth-child(2) { font-weight: bold; color: #222; font-size: 1.05rem; text-align: right; }
        .val { font-family: monospace; font-weight: bold; color: #008800; font-size: 1.05rem; text-align: left; }
        
        .empty-row { height: 12px }
        .empty-row td { padding: 0px 0px; }
        
        .category-row { background-color: #6b6a6a; color: white; font-weight: bold; }
        .category-row td { padding: 5px 15px; font-size: 1.05rem; color: white; letter-spacing: 3.0px; border-bottom: 3px solid #cc0000; }
        
        .btn-ota { display: inline-block; margin-top: 20px; padding: 10px 20px; background: #0066cc; color: white; text-decoration: none; border-radius: 4px; font-weight: bold; font-size: 0.9rem; float: right; }
        .btn-ota:hover { background: #004499; }
    </style>
    <script>
        function updateData() {
            fetch('/data').then(response => response.json()).then(data => {
                let tbody = document.getElementById('data-table');
                
                data.forEach(item => {
                    let row = document.getElementById('did_' + item.did);
                    
                    if(!row) {
                        row = document.createElement('tr');
                        row.id = 'did_' + item.did;
                        
                        if(item.did >= 999900) {
                            e_row = document.createElement('tr');
                            e_row.id = 'did_e_' + item.did;
                            e_row.className = 'empty-row';
                            let e_tdCat = document.createElement('td');
                            e_tdCat.colSpan = 3;
                            e_row.appendChild(e_tdCat);
                            tbody.appendChild(e_row);

                            row.className = 'category-row';
                            /*
                            let tdCat = document.createElement('td');
                            tdCat.colSpan = 3;
                            tdCat.innerText = item.name;
                            row.appendChild(tdCat);
                            */
                            
                            let tdDid = document.createElement('td');
                            let tdName = document.createElement('td');
                            let tdVal = document.createElement('td');
                            tdVal.innerText = item.name;
                            row.appendChild(tdDid);
                            row.appendChild(tdName);
                            row.appendChild(tdVal);
                            
                        } else {
                            let tdDid = document.createElement('td');
                            tdDid.innerText = item.did;
                            
                            let tdName = document.createElement('td');
                            tdName.innerText = item.name;
                            
                            let tdVal = document.createElement('td');
                            tdVal.className = 'val';
                            tdVal.innerText = '---';
                            
                            row.appendChild(tdDid);
                            row.appendChild(tdName);
                            row.appendChild(tdVal);
                        }
                        tbody.appendChild(row);
                    }
                    if(item.did < 999900) {
                        row.querySelector('.val').innerText = item.value;
                    }
                });
            });
        }
        window.onload = function() {
            updateData();
            setInterval(updateData, 2000);
        };
    </script>
</head>
<body>
    <div class="container">
        <a href="/update" class="btn-ota">🚀 OTA Update</a>
        <h1>Viessmann UDS and InfluxDB Daemon</h1>
        <p>Cyclic CAN bus telemetry polling (interval: 1 minute)</p>
        <table>
            <thead>
                <tr>
                    <th>DID</th>
                    <th style="text-align: right";>Name</th>
                    <th style="text-align: left;">Current value</th>
                </tr>
            </thead>
            <tbody id="data-table">
                <!-- Wird vollautomatisch durch JavaScript befüllt -->
            </tbody>
        </table>
    </div>
</body>
</html>
)rawliteral";

void setupWatchdog() {
    esp_task_wdt_config_t wdt_config = {
        .timeout_ms = WATCHDOG_TIMEOUT_SECONDS * 1000,
        .idle_core_mask = (1 << 0) | (1 << 1),
        .trigger_panic = true
    };
    esp_task_wdt_init(&wdt_config);
    esp_task_wdt_add(NULL); 
}

// Sends a UDS request for a specific DID to the CAN bus
void sendUdsRequest(uint16_t did) {
    if (did > 999900) { return; }

    twai_message_t tx_msg;
    tx_msg.identifier = CAN_REQUEST_ID;
    tx_msg.extd = 0;
    tx_msg.rtr = 0;
    tx_msg.data_length_code = 8;
    tx_msg.data[0] = 0x03; // PCI byte: Length of the UDS data (3 bytes follow)
    tx_msg.data[1] = 0x22; // UDS Service: ReadDataByIdentifier
    tx_msg.data[2] = (uint8_t)(did >> 8);   // DID High Byte
    tx_msg.data[3] = (uint8_t)(did & 0xFF);  // DID Low Byte
    for(int i = 4; i < 8; i++) tx_msg.data[i] = 0x00;

    twai_transmit(&tx_msg, pdMS_TO_TICKS(10));
}

// Sends the ISO-TP Flow Control frame to inform the boiler that it may send subsequent frames.
void sendFlowControl() {
    twai_message_t fc_msg;
    fc_msg.identifier = CAN_REQUEST_ID;
    fc_msg.extd = 0;
    fc_msg.rtr = 0;
    fc_msg.data_length_code = 8;
    fc_msg.data[0] = 0x30; // Flow Control Frame (Clear To Send)
    fc_msg.data[1] = 0x00; // Block Size = 0 (Send everything)
    fc_msg.data[2] = 0x00; // Separation Time = 0ms
    for(int i = 3; i < 8; i++) fc_msg.data[i] = 0x00;
    
    twai_transmit(&fc_msg, pdMS_TO_TICKS(10));
}

// Assigns received raw data to the appropriate class instance based on the DID
void routePayloadToDecoder(uint16_t did, const uint8_t* payload, size_t length) {
    for (uint8_t i = 0; i < NUM_OBJECTS; i++) {
        if (canObjects[i]->getDid() == did) {
            canObjects[i]->parse(payload, length);
            break;
        }
    }
}

// Processes incoming CAN bus frames (ISO-TP transport protocol)
void checkForResponse() {
    twai_message_t rx_msg;
    if (twai_receive(&rx_msg, pdMS_TO_TICKS(1)) == ESP_OK) {
        if (rx_msg.identifier == CAN_RESPONSE_ID) {
            
            // MULTI-FRAME: FIRST FRAME (0x10) -> Boiler announces a long response
            if ((rx_msg.data[0] & 0xF0) == 0x10 && rx_msg.data[2] == 0x62) {
                uint16_t totalLen = ((uint16_t)(rx_msg.data[0] & 0x0F) << 8) | rx_msg.data[1];
                activeMultiFrameDid = ((uint16_t)rx_msg.data[3] << 8) | rx_msg.data[4];
                
                multiFramePayloadByteMax = totalLen - 3; // Total length minus Service (1B) and DID (2B)
                multiFramePayloadByteCount = 0;
                isWaitingForConsecutive = true;
                
                // Save the first 3 bytes of the payload from the first frame (indices 5, 6, 7)
                for (int i = 5; i < 8; i++) {
                    if (multiFramePayloadByteCount < multiFramePayloadByteMax) {
                        multiFrameBuffer[multiFramePayloadByteCount++] = rx_msg.data[i];
                    }
                }
                sendFlowControl(); 
            }
            
            // MULTI-FRAME: SEQUENTIAL FRAMES (0x20 to 0x2F)
            else if ((rx_msg.data[0] & 0xF0) == 0x20 && isWaitingForConsecutive) {
                for (int i = 1; i < 8; i++) {
                    if (multiFramePayloadByteCount < multiFramePayloadByteMax) {
                        multiFrameBuffer[multiFramePayloadByteCount++] = rx_msg.data[i];
                    }
                }
                // Once all expected bytes have arrived, decode them
                if (multiFramePayloadByteCount >= multiFramePayloadByteMax) {
                    isWaitingForConsecutive = false;
                    routePayloadToDecoder(activeMultiFrameDid, multiFrameBuffer, multiFramePayloadByteCount);
                }
            }
            
            // SINGLE-FRAME (0x0X): Response fits entirely within a single data frame
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
            
            // UDS ERROR FRAME OR ABORT
            else if (rx_msg.data[1] == 0x7F) {
                isWaitingForConsecutive = false;
            }
        }
    }
}

// Non-blocking state machine for scheduling CAN requests
void handleDaemonTimer() {
    unsigned long currentMillis = millis();

    if (currentQueryStep >= NUM_OBJECTS) {
        if (currentMillis - lastQueryTime >= queryInterval) {
            lastQueryTime = currentMillis;
            currentQueryStep = 0; // Zurück zu Schritt 0
            lastStepTime = currentMillis;
        }
        return;
    }

    if (currentMillis - lastStepTime >= stepInterval) {
        lastStepTime = currentMillis;

        sendUdsRequest(canObjects[currentQueryStep]->getDid());
        currentQueryStep++;
    }
}


void setup() {
    // Establish a Wi-Fi connection
    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    
    // Initialize TWAI/CAN driver for ESP32-S3
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config); 
    twai_start();

    // Initialize the timestamp so that the query begins immediately upon startup
    lastQueryTime = millis() - queryInterval; 

    // Set up web server endpoints
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(200, "text/html; charset=UTF-8", String(index_html)); 
    });
    
    // JSON endpoint provides the current data, including names, for the dynamic web UI
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "[";
        for (uint8_t i = 0; i < NUM_OBJECTS; i++) {
            json += "{";
            json += "\"did\":"    + String(canObjects[i]->getDid()) + ",";
            json += "\"name\":\""  + canObjects[i]->getName()        + "\",";
            json += "\"value\":\"" + canObjects[i]->getFormattedString() + "\"";
            json += "}";
            if (i < NUM_OBJECTS - 1) {
                json += ",";
            }
        }
        json += "]";
        request->send(200, "application/json", json);
    });

    ElegantOTA.begin(&server); 
    server.begin();

    setupWatchdog();
}

void loop() {
    esp_task_wdt_reset();
    ElegantOTA.loop();
    checkForResponse();
    handleDaemonTimer();
    yield();
}
