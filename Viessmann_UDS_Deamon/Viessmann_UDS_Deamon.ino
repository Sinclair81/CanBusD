
/*************************************************************************************************************************
 *   Viessmann UDS Commander @ Sinclari81 2026                                                                           *
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
#include "known_dids.h"
#include "secrets.h"

// Wi-Fi access data - Please adjust in secrets.h
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASSWORD;

// Pins & CAN-IDs for ESP32-S3 (GPIO 15 & 16)
#define TX_PIN GPIO_NUM_15
#define RX_PIN GPIO_NUM_16
#define CAN_REQUEST_ID  0x680
#define CAN_RESPONSE_ID 0x690

// Global variables for the diagnostic interface
String lastRxDidHex = "---";
String lastRxDidDec = "---";
String lastRxPayload = "---";
String decodedValueStr = "---"; 
String webLog = "";

uint8_t decodeMode = 0;

AsyncWebServer server(80);

bool isWaitingForConsecutive = false;
uint16_t activeMultiFrameDid = 0;
uint8_t multiFramePayloadByteMax = 0;
uint8_t multiFramePayloadByteCount = 0;
String multiFramePayloadAccumulator = "";
uint8_t tempAktuellRawHigh = 0;

// Scanner variables
bool isScanning = false;
uint16_t currentScanDid = 0;
uint16_t currentScanDid_payloadLength = 0; 
uint16_t const maxScanDid = 3500;
unsigned long lastScanStepTime = 0;
const unsigned long scanStepInterval = 1250; 

uint16_t foundDids[10];
uint8_t foundDidCount = 0;

// Dynamic bit register for capturing ALL responding DIDs
uint8_t scanResultBitRegister[438] = {0}; 

void markDidAsFound(uint16_t did) {
    if (did < maxScanDid) {
        scanResultBitRegister[did / 8] |= (1 << (did % 8));
    }
}

bool isDidFound(uint16_t did) {
    if (did < maxScanDid) {
        return (scanResultBitRegister[did / 8] & (1 << (did % 8))) != 0;
    }
    return false;
}

void clearScanRegister() {
    memset(scanResultBitRegister, 0, sizeof(scanResultBitRegister));
}

bool isScanningKnownOnly = false;
size_t currentKnownScanIndex = 0;

String getDidName(uint16_t did) {
    for (size_t i = 0; i < knownDidsCount; i++) {
        if (pgm_read_word(&(knownDidsDatabase[i].did)) == did) {
            return String((const char*)pgm_read_ptr(&(knownDidsDatabase[i].id)));
        }
    }
    return "Unknown";
}

String getDidCodec(uint16_t did) {
    for (size_t i = 0; i < knownDidsCount; i++) {
        if (pgm_read_word(&(knownDidsDatabase[i].did)) == did) {
            return String((const char*)pgm_read_ptr(&(knownDidsDatabase[i].codec)));
        }
    }
    return "Unknown";
}

uint16_t getDidLength(uint16_t did, uint16_t actualPayloadLen) {
    uint16_t fallbackLength = 0;
    bool didFound = false;
    for (size_t i = 0; i < knownDidsCount; i++) {
        if (pgm_read_word(&(knownDidsDatabase[i].did)) == did) {
            uint16_t dbLength = pgm_read_word(&(knownDidsDatabase[i].length));
            didFound = true;
            fallbackLength = dbLength;
            if (dbLength == actualPayloadLen) return dbLength;
        }
    }
    return didFound ? fallbackLength : 0; 
}

String decToHex(const String& decStr, uint8_t width = 4) {
    unsigned long value = decStr.toInt();
    char buffer[16];
    switch (width) {
        case 2: sprintf(buffer, "%02lX", value); break;
        case 4: sprintf(buffer, "%04lX", value); break;
        case 6: sprintf(buffer, "%06lX", value); break;
        case 8: sprintf(buffer, "%08lX", value); break;
        default: sprintf(buffer, "%lX", value); break;
    }
    return String(buffer);
}

size_t hexStringToBytes(String hexStr, uint8_t* byteArr, size_t maxLen) {
    hexStr.replace(" ", ""); 
    hexStr.toUpperCase();
    size_t len = hexStr.length() / 2;
    if (len > maxLen) len = maxLen;
    for (size_t i = 0; i < len; i++) {
        String bytePart = hexStr.substring(i * 2, (i * 2) + 2);
        byteArr[i] = (uint8_t) strtol(bytePart.c_str(), NULL, 16);
    }
    return len;
}

String getFormattedTime() {
    unsigned long totalSeconds = millis() / 1000;
    int seconds = totalSeconds % 60;
    int minutes = (totalSeconds / 60) % 60;
    int hours = (totalSeconds / 3600); 
    char timeBuffer[16];
    sprintf(timeBuffer, "%02d:%02d:%02d", hours, minutes, seconds);
    return String(timeBuffer);
}

void logToWeb(String txt) {
    webLog = webLog + "[" + getFormattedTime() + "] " + txt + "<br>";
    if (webLog.length() > 20000) {
        webLog = webLog.substring(webLog.length() - 20000);
    }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Viessmann UDS Commander</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
    <style>
        body { font-family: Arial, sans-serif; text-align: center; background: #f4f4f9; color: #333; margin: 0; padding: 20px; }
        .container { max-width: 1400px; margin: 0 auto; text-align: left; background: white; padding: 25px; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.1); }
        h1 { color: #cc0000; text-align: center; margin-top: 0; margin-bottom: 20px; }
        .form-group { margin-bottom: 15px; }
        label { font-weight: bold; display: block; margin-bottom: 5px; }
        input[type="text"], select { width: 100%; padding: 10px; border: 1px solid #ccc; border-radius: 4px; box-sizing: border-box; font-family: monospace; font-size: 1rem; background: white; }
        .layout-table { width: 100%; border-collapse: collapse; }
        .layout-table td { vertical-align: top; padding: 0; }
        .layout-left {  }  
        .layout-divider { width: 15px; border-right: 1px solid #ddd; height: 100%; } 
        .layout-right { width: 330px; box-sizing: border-box; } 
        .action-button-group { display: flex; flex-direction: column; gap: 12px; margin-top: 0px; margin-left:15px; }
        .button-group { display: flex; gap: 10px; margin-top: 10px; }
        .btn { display: block; width: 100%; padding: 12px; background: #cc0000; color: white; border: none; border-radius: 4px; font-size: 1.1rem; font-weight: bold; cursor: pointer; text-align: center; text-decoration: none; box-sizing: border-box; }
        .btn:hover { background: #aa0000; }
        .btn-ota { background: #0066cc; }
        .btn-ota:hover { background: #004499; }
        .btn-scan { background: #ff9900; }
        .btn-scan:hover { background: #e68a00; }
        .btn-scan-known { background: #28a745; }
        .btn-scan-known:hover { background: #218838; }
        .btn-scan.active, .btn-scan-known.active { background: #6c757d; cursor: not-allowed; }
        .btn-dl { background: #6f42c1; display: none; } 
        .btn-dl:hover { background: #5a32a3; }
        .result-box { background: #eee; padding: 15px; border-radius: 5px; margin-top: 20px; font-size: 1.1rem; }
        .result-table { width: 100%; border-collapse: collapse; font-family: monospace; }
        .result-table td { padding: 5px 0; vertical-align: top; }
        .result-table td:first-child { width: 240px; font-weight: bold; color: #333; }
        .result-val { font-weight: bold; color: #004499; }
        .result-decoded { font-weight: bold; color: #008800; font-size: 1.2rem; }
        .log { background: #222; color: #00ff00; font-family: monospace; padding: 10px; border-radius: 5px; max-height: 265px; overflow-y: auto; font-size: 0.85rem; margin-top: 20px;}
    </style>
    <script>
        setInterval(function() {
            fetch('/data').then(response => response.json()).then(data => {
                document.getElementById('rx_did').innerText = data.rx_did;
                document.getElementById('rx_payload').innerText = data.rx_payload;
                document.getElementById('rx_decoded').innerText = data.rx_decoded;
                
                let logDiv = document.getElementById('log');
                let isScrolledToBottom = (logDiv.scrollHeight - logDiv.clientHeight - logDiv.scrollTop) <= 10;
                logDiv.innerHTML = data.log;

                if (isScrolledToBottom) {
                    logDiv.scrollTop = logDiv.scrollHeight;
                }

                let fullScanBtn = document.getElementById('scan_btn');
                let knownScanBtn = document.getElementById('scan_known_btn');
                let dlBtn = document.getElementById('dl_btn');
                
                if (data.is_scanning) {
                    fullScanBtn.disabled = true; fullScanBtn.classList.add('active');
                    knownScanBtn.disabled = true; knownScanBtn.classList.add('active');
                    dlBtn.style.display = "none";
                    if (data.max_progress > 2000) { 
                        fullScanBtn.innerText = "⏳ Full scan: " + data.scan_progress + " / " + data.max_progress;
                        knownScanBtn.innerText = "🔍 Scan known DIDs";
                    } else { 
                        knownScanBtn.innerText = "⏳ Scan: " + data.scan_progress + " / " + data.max_progress;
                        fullScanBtn.innerText = "🔍 Scan all DIDs (0-3500)";
                    }
                } else {
                    fullScanBtn.classList.remove('active'); fullScanBtn.disabled = false;
                    fullScanBtn.innerText = "🔍 Scan all DIDs (0-3500)";
                    knownScanBtn.classList.remove('active'); knownScanBtn.disabled = false;
                    knownScanBtn.innerText = "🔍 Scan known DIDs";
                    if(data.has_scan_results) { dlBtn.style.display = "block"; }
                }
            });
        }, 1500);


        function sendCommand() {
            let did = document.getElementById('send_did').value;
            let payload = document.getElementById('send_payload').value;
            fetch('/send?did=' + encodeURIComponent(did) + '&payload=' + encodeURIComponent(payload));
        }
        function triggerScan() { fetch('/scan'); }
        function triggerKnownScan() { fetch('/scanknown'); }
        function changeDecodeMode() {
            let mode = document.getElementById('decode_mode').value;
            fetch('/setmode?mode=' + mode);
        }
    </script>
</head>
<body>
    <div class="container">
        <h1>Viessmann UDS Commander</h1>
        <table class="layout-table">
            <tr>
                <td class="layout-left">
                    <div class="form-group">
                        <label for="send_did">Send DID (DEC, e.g. 274) to CAN ID 0x680 and UDS Service 0x22:</label>
                        <input type="text" id="send_did" value="274">
                    </div>
                    <div class="form-group">
                        <label for="send_payload">Send 8 bytes (HEX, e.g. 03 22 01 12 00 00 00 00) to CAN ID 0x680 (optional):</label>
                        <input type="text" id="send_payload" placeholder="z.B. 03 22 01 12 00 00 00 00">
                    </div>
                    <div class="button-group">
                        <button class="btn" onclick="sendCommand()">🚀 Send command</button>
                    </div>
                </td>
                <td class="layout-divider"></td>
                <td class="layout-right">
                    <div class="action-button-group">
                        <a href="/update" class="btn btn-ota">🚀 OTA update</a>
                        <button id="scan_known_btn" class="btn btn-scan-known" onclick="triggerKnownScan()">🔍 Scan known DIDs</button>
                        <button id="scan_btn" class="btn btn-scan" onclick="triggerScan()">🔍 Scan all DIDs (0-3500)</button>
                        <a id="dl_btn" href="/download_config" class="btn btn-dl" download="known_dids_custom.h">💾 Download config</a>
                    </div>
                </td>
            </tr>
        </table>
        <hr style="margin: 15px 0; border: 0; border-top: 1px solid #ddd;">
        <div class="form-group">
            <label for="decode_mode">Payload decoding:</label>
            <select id="decode_mode" onchange="changeDecodeMode()">
                <option value="0">Off (default)</option> 
                <option value="1">Temperature (e.g.: DID 274 or 396)</option> 
                <option value="2">Pressure (e.g.: DID 813)</option> 
                <option value="3">Pump (e.g.: DID 381)</option> 
                <option value="4">Mixer pump (e.g.: DID 401)</option> 
                <option value="5">Mode & State (e.g.: DID 873)</option> 
                <option value="6">Power & Error (e.g.: DID 2351)</option> 
                <option value="7">Mixer curve (e.g.: DID 881)</option> 
                <option value="8">Compressor statistics (e.g.: DID 2369)</option> 
                <option value="9">E-cartridge statistics (e.g.: DID 2370)</option> 
                <option value="10">Flow sensor (e.g.: DID 1043)</option> 
                <option value="11">Power (e.g.: DID 2486)</option> 
                <option value="12">Utf8 [Text] (e.g.: DID 627)</option> 
                <option value="13">Int8 [%, ENUM, dBm, min, cnt, mbar] (e.g.: DID 2346)</option> 
                <option value="14">Int16 [°C, bar, hPas, l/h, h, rps, rpm, µA, mA, Wh, W, kW, VA, m3] (e.g.: DID 919)</option> 
                <option value="15">Int32 [W, kWh, V, A] (e.g.: DID 2342)</option> 
                <option value="16">Float32 [Ah, %] (e.g.: DID 2990)</option>
            </select>
        </div>
        <div class="result-box">
            <table class="result-table">
                <tr><td>Last received DID:</td><td><span id="rx_did" class="result-val">---</span></td></tr>
                <tr><td>Payload data (HEX):</td><td><span id="rx_payload" class="result-val">---</span></td></tr>
                <tr><td>Decoded value:</td><td><span id="rx_decoded" class="result-decoded">---</span></td></tr>
            </table>
        </div>
        <div class="log" id="log">%LOGCONTENT%</div>
    </div>
</body>
</html>
)rawliteral";

union FloatByteConverter {
    float f;
    uint8_t b[4];
  };

void processDecoding() {
    if (decodeMode == 0) { decodedValueStr = "--- (Off)"; return; }
    uint8_t payloadBytes[32] = {0};
    size_t payloadLen = hexStringToBytes(lastRxPayload, payloadBytes, 16);
    if ((decodeMode == 1) || (decodeMode == 2)) { // Temperature or Pressure
        String sign = (decodeMode == 1) ? "°C" : "bar";
        if (payloadLen == 2) {
            int16_t v1 = ((int16_t)payloadBytes[1] << 8) | payloadBytes[0];
            String s1 = String(v1 / 10.0, 1);
            decodedValueStr = "Actual:" + s1 + sign;
        } else if (payloadLen == 3) {
            int16_t v1 = ((int16_t)payloadBytes[1] << 8) | payloadBytes[0];
            String s1 = String(v1 / 10.0, 1);
            String s2 = String(payloadBytes[2]);
            decodedValueStr = "Actual:" + s1 + sign + " | Sensor Status:" + s2;
        } else if (payloadLen == 9) {
            int16_t v1 = ((int16_t)payloadBytes[1] << 8) | payloadBytes[0];
            int16_t v2 = ((int16_t)payloadBytes[3] << 8) | payloadBytes[2];
            int16_t v3 = ((int16_t)payloadBytes[5] << 8) | payloadBytes[4];
            int16_t v4 = ((int16_t)payloadBytes[7] << 8) | payloadBytes[6];
            String s1 = String(v1 / 10.0, 1);
            String s2 = String(v2 / 10.0, 1);
            String s3 = String(v3 / 10.0, 1);
            String s4 = String(v4 / 10.0, 1);
            String s5 = String(payloadBytes[8]);
            decodedValueStr = "Actual:" + s1 + sign + " | Minimum:" + s2 + sign + " | Maximum:" + s3 + sign + " | Average:" + s4 + sign + " | Sensor Status:" + s5;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 3) { // Pump
        if (payloadLen == 4) {
            String s1 = String(payloadBytes[0]);
            String s2 = String(payloadBytes[1]);
            String s3 = String(payloadBytes[2]);
            String s4 = String(payloadBytes[3]);
            decodedValueStr = "State:" + s1 + " | Target:" + s2 + "% | Actual:" + s3 + "% | Unknown:" + s4;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 4) { // Mixer pump
        if (payloadLen == 5) {
            String s1 = String(payloadBytes[0]);
            String s2 = String(payloadBytes[1]);
            String s3 = String(payloadBytes[2]);
            String s4 = String(payloadBytes[3]);
            String s5 = String(payloadBytes[4]);
            decodedValueStr = "Target Power State:" + s1 + " | Target Value:" + s2 + " | Power State:" + s3 + " | Error State:" + s4 + " | Actual Value:" + s5;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 5) { // Mode & State
        if (payloadLen == 2) {
            String s1 = String(payloadBytes[0]);
            String s2 = String(payloadBytes[1]);
            decodedValueStr = "Mode:" + s1 + " | State:" + s2;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 6) { // Power & Error
        if (payloadLen == 2) {
            String s1 = String(payloadBytes[0]);
            String s2 = String(payloadBytes[1]);
            decodedValueStr = "Power:" + s1 + " | Error:" + s2;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 7) { // Mixer curve
        if (payloadLen == 4) {
            int16_t v3 = ((int16_t)payloadBytes[3] << 8) | payloadBytes[2];
            String s1 = String(payloadBytes[0]);
            String s2 = String(payloadBytes[1]);
            String s3 = String(v3);
            decodedValueStr = "Gradient:" + s1 + " | Level:" + s2 + " | Base Point:" + s3;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 8) { // Compressor statistics
        if (payloadLen == 14) {
            int64_t v1 = ((int64_t)payloadBytes[5] << 40) | ((int64_t)payloadBytes[4] << 32) | ((int64_t)payloadBytes[3] << 24) | ((int64_t)payloadBytes[2] << 16) | ((int64_t)payloadBytes[1] << 8) | payloadBytes[0];
            int16_t v2 = ((int16_t)payloadBytes[7] << 8) | payloadBytes[6];
            int16_t v3 = ((int16_t)payloadBytes[9] << 8) | payloadBytes[8];
            int16_t v4 = ((int16_t)payloadBytes[11] << 8) | payloadBytes[10];
            int16_t v5 = ((int16_t)payloadBytes[13] << 8) | payloadBytes[12];
            String s1 = String(v1);
            String s2 = String(v2);
            String s3 = String(v3);
            String s4 = String(v4);
            String s5 = String(v5);
            decodedValueStr = "Unknown 1:" + s1 + " | Starts:" + s2 + " | Unknown 2:" + s3 + " | Hours:" + s4 + " | Unknown 3:" + s5;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 9) { // E-cartridge statistics
        if (payloadLen == 11) {
            int64_t v1 = ((int64_t)payloadBytes[2] << 16) | ((int64_t)payloadBytes[1] << 8) | payloadBytes[0];
            int16_t v2 = ((int16_t)payloadBytes[7] << 8) | payloadBytes[6];
            int16_t v3 = ((int16_t)payloadBytes[9] << 8) | payloadBytes[8];
            int16_t v4 = ((int16_t)payloadBytes[11] << 8) | payloadBytes[10];
            int16_t v5 = ((int16_t)payloadBytes[13] << 8) | payloadBytes[12];
            String s1 = String(v1);
            String s2 = String(v2);
            String s3 = String(v3);
            String s4 = String(v4);
            String s5 = String(v5);
            decodedValueStr = "Unknown 1:" + s1 + " | Starts:" + s2 + " | Unknown 2:" + s3 + " | Hours:" + s4 + " | Unknown 3:" + s5;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 10) { // Flow sensor
        if (payloadLen == 5) {
            int16_t v1 = ((int16_t)payloadBytes[1] << 8) | payloadBytes[0];
            int16_t v2 = ((int16_t)payloadBytes[3] << 8) | payloadBytes[2];
            int16_t v3 = (int16_t)payloadBytes[4];
            String s1 = String(v1 / 10.0, 0);
            String s2 = String(v2 / 10.0, 1);
            String s3 = String(v3);
            decodedValueStr = "Actual:" + s1 + "l/h | Temperature:" + s2 + "°C | Unknown:" + s3;
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 11) { // Power
        if (payloadLen == 4) {
            int64_t v1 = ((int64_t)payloadBytes[3] << 16) | ((int64_t)payloadBytes[2] << 16) | ((int64_t)payloadBytes[1] << 8) | payloadBytes[0];
            String s1 = String(v1);
            decodedValueStr = "Actual:" + s1 + "W";
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 12) { // Utf8
        if (payloadLen >= 1) {
            char cString[payloadLen + 1]; 
            for (int i = 0; i < payloadLen; i++) {
                cString[i] = (char)payloadBytes[i];
            }
            cString[payloadLen] = '\0';
            decodedValueStr = "Text: " + String(cString);
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 13) { // Int8
        if (payloadLen >= 1) {
            int16_t v1 = payloadBytes[0];
            String s1 = String(v1);
            decodedValueStr = s1 + " [%, ENUM, dBm, min, cnt, mbar]";
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 14) { // Int16
        if (payloadLen >= 2) {
            int16_t v1 = ((int16_t)payloadBytes[1] << 8) | payloadBytes[0];
            String s1 = String(v1);
            decodedValueStr = s1 + " [°C, bar, hPas, l/h, h, rps, rpm, µA, mA, Wh, W, kW, VA, m3]";
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 15) { // Int32
        if (payloadLen >= 4) {
            int32_t v1 = ((int32_t)payloadBytes[3] << 24) | ((int32_t)payloadBytes[2] << 16) | ((int32_t)payloadBytes[1] << 8) | payloadBytes[0];
            String s1 = String(v1);
            decodedValueStr = s1 + " [W, kWh, V, A]";
        } else { decodedValueStr = "Error: Payload too short!"; }
    } else if (decodeMode == 16) { // Float32
        if (payloadLen >= 4) {
            FloatByteConverter converter;
            converter.b[0] = payloadBytes[0]; // LSB
            converter.b[1] = payloadBytes[1];
            converter.b[2] = payloadBytes[2];
            converter.b[3] = payloadBytes[3]; // MSB
            String s1 = String(converter.f, 2);
            decodedValueStr = s1 + " [Ah, %]";
        } else { decodedValueStr = "Error: Payload too short!"; }
    }
}

void handleDownloadConfig(AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginChunkedResponse("text/plain", [](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
        static size_t dbIndex = 0; static uint8_t stage = 0;
        if (index == 0) { dbIndex = 0; stage = 0; }
        String chunk = "";
        if (stage == 0) {
            chunk += "#ifndef KNOWN_DIDS_H\n#define KNOWN_DIDS_H\n\n#include <Arduino.h>\n\n";
            chunk += "struct KnownDID {\n    uint16_t did;\n    const char* id;\n    const char* codec;\n    uint16_t length;\n    const char* unit;\n    const char* access;\n};\n\n";
            chunk += "// Customized Viessmann configuration for your unit\nconst KnownDID knownDidsDatabase[] PROGMEM = {\n";
            stage = 1; chunk.getBytes(buffer, maxLen); return chunk.length();
        }
        if (stage == 1) {
            while (dbIndex < knownDidsCount) {
                uint16_t checkDid = pgm_read_word(&(knownDidsDatabase[dbIndex].did)); dbIndex++;
                if (isDidFound(checkDid)) {
                    char rBuf[256];
                    sprintf(rBuf, "    {%d, \"%s\", \"%s\", %d, \"%s\", \"%s\"},\n", checkDid,
                        (const char*)pgm_read_ptr(&(knownDidsDatabase[dbIndex-1].id)), (const char*)pgm_read_ptr(&(knownDidsDatabase[dbIndex-1].codec)),
                        pgm_read_word(&(knownDidsDatabase[dbIndex-1].length)), (const char*)pgm_read_ptr(&(knownDidsDatabase[dbIndex-1].unit)), (const char*)pgm_read_ptr(&(knownDidsDatabase[dbIndex-1].access)));
                    chunk = String(rBuf); chunk.getBytes(buffer, maxLen); return chunk.length();
                }
            }
            stage = 2;
        }
        if (stage == 2) {
            chunk += "};\n\n";
            chunk += "// Automatically calculate the number of DIDs\n";
            chunk += "const size_t knownDidsCount = sizeof(knownDidsDatabase) / sizeof(KnownDID);\n\n";
            chunk += "#endif\n\n";
            stage = 3;
            chunk.getBytes(buffer, maxLen);
            return chunk.length();
        }
        return 0;
    });
    response->addHeader("Content-Disposition", "attachment; filename=known_dids_custom.h");
    request->send(response);
}

void scanDIDs(uint8_t startMode = 0) {
    if (startMode == 1) { isScanning = true; isScanningKnownOnly = false; currentScanDid = 0; foundDidCount = 0; clearScanRegister(); lastScanStepTime = millis(); logToWeb("▶️ Full scan (0-" + String(maxScanDid) + ") started..."); return; }
    if (startMode == 2) { isScanning = true; isScanningKnownOnly = true; currentKnownScanIndex = 0; foundDidCount = 0; clearScanRegister(); lastScanStepTime = millis(); logToWeb("▶️ Targeted scan started..."); return; }
    if (!isScanning) return;
    if (millis() - lastScanStepTime >= scanStepInterval) {
        lastScanStepTime = millis(); uint16_t didToQuery = 0;
        if (isScanningKnownOnly) {
            if (currentKnownScanIndex >= knownDidsCount) {
                isScanning = false;
                if (foundDidCount > 0) {
                    String dList = ""; for (uint8_t i = 0; i < foundDidCount; i++) dList += String(foundDids[i]) + (i < foundDidCount - 1 ? ", " : "");
                    logToWeb("📋 DIDs found: " + dList);
                }
                logToWeb("⏹️ Targeted DID scan complete. Your known_dids_custom.h is ready for download!"); return;
            }
            didToQuery = pgm_read_word(&(knownDidsDatabase[currentKnownScanIndex].did)); currentScanDid_payloadLength = pgm_read_word(&(knownDidsDatabase[currentKnownScanIndex].length)); currentScanDid = didToQuery; currentKnownScanIndex++;
        } else {
            if (currentScanDid > maxScanDid) {
                isScanning = false;
                if (foundDidCount > 0) {
                    String dList = ""; for (uint8_t i = 0; i < foundDidCount; i++) dList += String(foundDids[i]) + (i < foundDidCount - 1 ? ", " : "");
                    logToWeb("📋 DIDs found: " + dList);
                }
                logToWeb("⏹️ Full scan complete. Your known_dids_custom.h is ready for download!"); return;
            }
            didToQuery = currentScanDid; currentScanDid_payloadLength = 0; currentScanDid++;
        }
        twai_message_t tx_msg; tx_msg.identifier = CAN_REQUEST_ID; tx_msg.extd = 0; tx_msg.rtr = 0; tx_msg.data_length_code = 8;
        tx_msg.data[0] = 0x03; tx_msg.data[1] = 0x22; tx_msg.data[2] = (uint8_t)(didToQuery >> 8); tx_msg.data[3] = (uint8_t)(didToQuery & 0xFF); for(int i = 4; i < 8; i++) tx_msg.data[i] = 0x00;
        twai_transmit(&tx_msg, pdMS_TO_TICKS(10));
    }
}

void sendFlowControl() {
    twai_message_t fc_msg; fc_msg.identifier = CAN_REQUEST_ID; fc_msg.extd = 0; fc_msg.rtr = 0; fc_msg.data_length_code = 8;
    fc_msg.data[0] = 0x30; fc_msg.data[1] = 0x00; fc_msg.data[2] = 0x00; for(int i = 3; i < 8; i++) fc_msg.data[i] = 0x00;
    twai_transmit(&fc_msg, pdMS_TO_TICKS(50));
}

void setup() {
    WiFi.mode(WIFI_STA); WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { delay(500); }
    logToWeb("Wi-Fi active! IP: " + WiFi.localIP().toString());
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(TX_PIN, RX_PIN, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_250KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config); twai_start();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(200, "text/html; charset=UTF-8", String(index_html)); });
    server.on("/setmode", HTTP_GET, [](AsyncWebServerRequest *request){ if (request->hasParam("mode")) { decodeMode = request->getParam("mode")->value().toInt(); processDecoding(); } request->send(200, "text/plain", "OK"); });
    server.on("/scan", HTTP_GET, [](AsyncWebServerRequest *request){ scanDIDs(1); request->send(200, "text/plain", "OK"); });
    server.on("/scanknown", HTTP_GET, [](AsyncWebServerRequest *request){ scanDIDs(2); request->send(200, "text/plain", "OK"); });
    server.on("/download_config", HTTP_GET, handleDownloadConfig);
    
    server.on("/send", HTTP_GET, [](AsyncWebServerRequest *request){
        if (request->hasParam("did")) {
            String dStr = request->getParam("did")->value(); String pStr = request->hasParam("payload") ? request->getParam("payload")->value() : "";
            uint16_t did = (uint16_t) strtol(dStr.c_str(), NULL, 10); twai_message_t tx_msg; tx_msg.identifier = CAN_REQUEST_ID; tx_msg.extd = 0; tx_msg.rtr = 0;
            if (pStr == "") { tx_msg.data[0] = 0x03; tx_msg.data[1] = 0x22; tx_msg.data[2] = (uint8_t)(did >> 8); tx_msg.data[3] = (uint8_t)(did & 0xFF); for(int i = 4; i < 8; i++) tx_msg.data[i] = 0x00; }
            else { uint8_t pBytes[8] = {0}; hexStringToBytes(pStr, pBytes, 8); did = ((uint16_t)pBytes[2] << 8) | pBytes[3]; for(int i = 0; i < 8; i++) tx_msg.data[i] = pBytes[i]; }
            tx_msg.data_length_code = 8; String rTx = ""; for (int i = 0; i < 8; i++) { char bB[4]; sprintf(bB, "%02X ", tx_msg.data[i]); rTx += String(bB); }
            logToWeb("-> Roh-Frame: " + rTx);
            if (twai_transmit(&tx_msg, pdMS_TO_TICKS(100)) == ESP_OK) { logToWeb(">> DID: " + String(did, DEC) + " (0x" + decToHex(String(did), 4) + ")"); } else { logToWeb("Sende-Fehler!"); }
        }
        request->send(200, "text/plain", "OK");
    });

    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
        uint16_t progressVal = currentScanDid; uint16_t maxVal = maxScanDid;
        if (isScanning && isScanningKnownOnly) { progressVal = currentKnownScanIndex; maxVal = knownDidsCount; }
        bool hasResults = false; for (size_t i = 0; i < sizeof(scanResultBitRegister); i++) { if(scanResultBitRegister[i] > 0) { hasResults = true; break; } }
        String displayDid = lastRxDidDec + " (" + lastRxDidHex + ") - " + getDidName(lastRxDidDec.toInt());
        String json = "{\"rx_did\":\"" + displayDid + "\",\"rx_payload\":\"" + lastRxPayload + "\",\"rx_decoded\":\"" + decodedValueStr + "\",\"is_scanning\":" + String(isScanning ? "true" : "false") + ",\"scan_progress\":" + String(progressVal) + ",\"max_progress\":" + String(maxVal) + ",\"has_scan_results\":" + String(hasResults ? "true" : "false") + ",\"log\":\"" + webLog + "\"}";
        request->send(200, "application/json", json);
    });
    ElegantOTA.begin(&server); server.begin(); logToWeb("Commander UI ready.");
}

void checkForResponse() {
    twai_message_t rx_msg;
    if (twai_receive(&rx_msg, pdMS_TO_TICKS(1)) == ESP_OK) {
        if (rx_msg.identifier == CAN_RESPONSE_ID) {
            
            bool gotValidData = false;
            bool scanValidDidFound = false;
            uint16_t scanVerifiedDid = 0;

            // --- 1. RAW LOGGING (Suppressed during scanning) ---
            if (!isScanning) {
                String rawDump = "";
                for(int i=0; i<8; i++) {
                    char byteBuf[4];
                    sprintf(byteBuf, "%02X ", rx_msg.data[i]);
                    rawDump += String(byteBuf);
                }
                logToWeb("<- Raw frame: " + rawDump);
            }

            // --- 2. MULTI-FRAME: FIRST FRAME (0x10) ---
            if ((rx_msg.data[0] & 0xF0) == 0x10 && rx_msg.data[2] == 0x62) {
                uint16_t parsedMultiDid = ((uint16_t)rx_msg.data[3] << 8) | rx_msg.data[4];
                
                if (isScanning) {
                    // In scan mode: Note the DID but do NOT send Flow Control (force abort).
                    if (currentScanDid_payloadLength == 0 || currentScanDid_payloadLength == (uint16_t)rx_msg.data[1] - 3) { 
                        scanVerifiedDid = parsedMultiDid; 
                        scanValidDidFound = true; 
                    } 
                } else {
                    // Normal UI mode: Perform full ISO-TP handshake
                    multiFramePayloadByteMax = (rx_msg.data[1] - 3); // 1 Byte Service + 2 Byte DID
                    multiFramePayloadByteCount = 0;
                    activeMultiFrameDid = parsedMultiDid;
                    
                    multiFramePayloadAccumulator = "";
                    for (int i = 5; i < 8; i++) {
                        char byteBuf[4];
                        sprintf(byteBuf, "%02X ", rx_msg.data[i]);
                        multiFramePayloadAccumulator += String(byteBuf);
                        multiFramePayloadByteCount += 1;
                    }
                    
                    isWaitingForConsecutive = true;
                    sendFlowControl(); 
                }
            }
            
            // --- 3. MULTI-FRAME: ALL SUBSEQUENT FRAMES (0x20 to 0x2F) --- (Only in normal UI mode)
            else if ((rx_msg.data[0] & 0xF0) == 0x20 && isWaitingForConsecutive && !isScanning) {
                for (int i = 1; i < 8; i++) {
                    char byteBuf[4];
                    sprintf(byteBuf, "%02X ", rx_msg.data[i]);
                    if (multiFramePayloadByteCount < multiFramePayloadByteMax) {
                        multiFramePayloadAccumulator += String(byteBuf);
                        multiFramePayloadByteCount += 1;
                    }
                }

                if (multiFramePayloadByteCount == multiFramePayloadByteMax) {
                    char didHexBuf[6];
                    sprintf(didHexBuf, "%04X", activeMultiFrameDid);
                    String currentDidStr = String(didHexBuf);
                    
                    lastRxDidHex = "0x" + currentDidStr;
                    lastRxDidDec = String(activeMultiFrameDid);
                    lastRxPayload = multiFramePayloadAccumulator;
                    gotValidData = true;
                    
                    logToWeb("<< DID: " + lastRxDidDec + " (" + lastRxDidHex + ") | Payload: " + lastRxPayload);
                }
            }
            
            // --- 4. SINGLE-FRAME (0x0X, Service 0x62 is at index 1) ---
            else if (rx_msg.data[1] == 0x62) {
                uint16_t parsedDid = ((uint16_t)rx_msg.data[2] << 8) | rx_msg.data[3];
                
                if (isScanning) {
                    // In scan mode: Register only DID for the buffer.
                    if (currentScanDid_payloadLength == 0 || currentScanDid_payloadLength == (uint16_t)rx_msg.data[0] - 3) {
                        scanVerifiedDid = parsedDid;
                        scanValidDidFound = true;
                    }
                } else {
                    // Normal UI mode: Full display and trigger decoding
                    isWaitingForConsecutive = false; 
                    uint8_t framePayloadByteMax = (rx_msg.data[0] - 3); 
                    uint8_t framePayloadByteCount = 0;
                    
                    char didHexBuf[6];
                    sprintf(didHexBuf, "%04X", parsedDid);
                    String currentDidStr = String(didHexBuf);
                    
                    lastRxDidHex = "0x" + currentDidStr;
                    lastRxDidDec = String(parsedDid);
                    
                    String payloadStr = "";
                    for (int i = 4; i < 8; i++) {
                        char byteBuf[4]; 
                        sprintf(byteBuf, "%02X ", rx_msg.data[i]);
                        if (framePayloadByteCount < framePayloadByteMax) {
                            payloadStr += String(byteBuf);
                            framePayloadByteCount += 1;
                        }
                    }
                    lastRxPayload = payloadStr;
                    gotValidData = true;
                    
                    logToWeb("<< DID: " + lastRxDidDec + " (" + lastRxDidHex + ") | Payload: " + lastRxPayload);
                }
            }

            // --- 5. UDS ERROR FRAMES & SPECIAL CASES (0x7F at Index 1) ---
            else if (rx_msg.data[1] == 0x7F) {
                isWaitingForConsecutive = false; 
                
                // Errors are completely ignored in scan mode.
                if (!isScanning) {
                    lastRxDidHex = "---";
                    lastRxDidDec = "---";
                    lastRxPayload = "---";
                    gotValidData = true;

                    uint8_t nrc = rx_msg.data[3]; 
                    switch (nrc) {
                        case 0x10: logToWeb("<< ERROR - NRC 0x10 - General Reject"); break;
                        case 0x11: logToWeb("<< ERROR - NRC 0x11 - Service Not Supported"); break;
                        case 0x12: logToWeb("<< ERROR - NRC 0x12 - SubFunction Not Supported"); break;
                        case 0x13: logToWeb("<< ERROR - NRC 0x13 - Incorrect Message Length / Invalid Format"); break;
                        case 0x22: logToWeb("<< ERROR - NRC 0x22 - Conditions Not Correct (e.g. engine not running)"); break;
                        case 0x31: logToWeb("<< ERROR - NRC 0x31 - Request Out Of Range (DID does not exist.)"); break;
                        case 0x33: logToWeb("<< ERROR - NRC 0x33 - Security Access Denied (Blocked)"); break;
                        case 0x78: logToWeb("<< ERROR - NRC 0x78 - Request Correctly Received - Response Pending (Please wait)"); break;
                        default:   logToWeb("<< ERROR - UDS NRC 0x" + String(nrc, HEX) + " occurred."); break;
                    }
                }
            }
            
            // --- 6. IGNORE: OUTGOING FLOW CONTROL FRAMES FROM OTHER DEVICES ---
            else if ((rx_msg.data[0] & 0xF0) == 0x30) {
                // Nichts tun
            }

            // --- 7. AUTOMATIC SCANNER ANALYSIS ---
            if (isScanning && scanValidDidFound) {
                markDidAsFound(scanVerifiedDid);
                foundDids[foundDidCount] = scanVerifiedDid;
                foundDidCount++;

                // When the block of 10 is full, push it to the log as CSV.
                if (foundDidCount >= 10) {
                    String didList = "";
                    for (uint8_t i = 0; i < 10; i++) {
                        didList += String(foundDids[i]) + (i < 9 ? ", " : "");
                    }
                    logToWeb("📋 DIDs found: " + didList);
                    foundDidCount = 0; // Reset buffer for the next 10 DIDs
                }
            }

            // Initiate normal decoding
            if (gotValidData) {
                processDecoding();
            }
        }
    }
}

void loop() {
    ElegantOTA.loop();
    checkForResponse();
    scanDIDs(); 
    yield();
}
