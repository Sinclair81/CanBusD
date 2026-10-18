
/*************************************************************************************************************************
 *   Viessmann DID 2 InfluxDB @ Sinclari81 2026                                                                          *
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
#include <InfluxDbClient.h>
#include <CanDecoder.h>
#include <secrets.h>    // USE THIS: Loads central credentials safely from the global Arduino libraries folder
// #include "secrets.h" // OPTIONAL: Fallback for local "secrets.h", fill it out, and uncomment this line if needed

InfluxDBClient client(SECRET_INFLUXDB_URL, SECRET_INFLUXDB_ORG, SECRET_INFLUXDB_BUCKET, SECRET_INFLUXDB_TOKEN);

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
MultiTempWithStatus outsideTemperatureSensor(                                   274, "Außentemperatur",                0b11100001, "; ");
MultiTempWithStatus flowTemperatureSensor(                                      268, "Vorlauftemperatur",              0b11100001, "; ");
MultiTempWithStatus returnTemperatureSensor(                                    269, "Rücklauftemperatur",             0b11100001, "; ");
ListDummy dummy2(                                                            999902, "🚰 Warmwasser");
MultiTempWithStatus domesticHotWaterSensor(                                     271, "Temperatur",                     0b11100001, "; ");
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
MultiPressureWithStatus compressorInletPressureSensor(                          322, "Einlassdruck",                   0b10000001, "; ");
MultiPressureWithStatus compressorOutletPressureSensor(                         325, "Auslassdruck",                   0b10000001, "; ");
CanInt16 compressorSpeedRps(                                                   2569, "Drehzahl",                       "", "rps");
ListDummy dummy5(                                                            999905, "⚙️ System");
MultiPressureWithStatus waterPressureSensor(                                    318, "Wasserdruck",                    0b11100001, "; ");
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
SeasonalCoefficient seasonalCoefficientOfPerformanceHeating(                   2622, "Heizen",                         0b11100001, "; ");
SeasonalCoefficient seasonalCoefficientOfPerformanceDomesticHotWater(          2624, "Warmwasser",                     0b11100001, "; ");
SeasonalCoefficient seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater(2625, "Heizen und Warmwasser",          0b11100001, "; ");

// Here, we group all objects together as pointers in an array.
const uint8_t NUM_OBJECTS = 51;
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
    &waterPressureSensor,
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

uint32_t influxSentDids[NUM_OBJECTS] = {0};
uint8_t influxSentCount = 0;

unsigned long lastQueryTime = 0;
const unsigned long queryInterval = 60000;
unsigned long lastStepTime = 0;
const unsigned long stepInterval = 300;
uint8_t currentQueryStep = 0;

bool isWaitingForConsecutive = false;
uint16_t activeMultiFrameDid = 0;
uint8_t multiFramePayloadByteMax = 0;
uint8_t multiFramePayloadByteCount = 0;
uint8_t multiFrameBuffer[256] = {0};

AsyncWebServer server(80);

/*
const uint8_t apple_icon_png[] PROGMEM = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x30,
    0x08, 0x02, 0x00, 0x00, 0x00, 0xD8, 0x60, 0x6E, 0xD0, 0x00, 0x00, 0x00,
    0x6F, 0x49, 0x44, 0x41, 0x54, 0x78, 0x9C, 0xED, 0xD2, 0xC9, 0x0D, 0xC0,
    0x40, 0x08, 0x43, 0x51, 0xFA, 0xAF, 0x2D, 0x3D, 0x4D, 0x4E, 0x29, 0x80,
    0x8C, 0x6C, 0x16, 0x7D, 0xE4, 0x13, 0xE2, 0xF0, 0x04, 0xC4, 0x13, 0xD1,
    0x2A, 0xF5, 0x02, 0x40, 0x80, 0x00, 0xB5, 0x06, 0x9D, 0xAF, 0x2E, 0xFB,
    0x7B, 0x41, 0x06, 0xD3, 0x9F, 0x33, 0x4B, 0x4D, 0x80, 0x00, 0x01, 0x6A,
    0x07, 0x92, 0x6A, 0xE6, 0x83, 0xD4, 0x9A, 0xE1, 0x20, 0x83, 0x66, 0x32,
    0xC8, 0xA3, 0x01, 0x04, 0x08, 0x50, 0x23, 0x90, 0xCD, 0x94, 0xE3, 0x1B,
    0x4C, 0x39, 0x90, 0x21, 0xF5, 0x82, 0xE1, 0xA0, 0xA3, 0xA9, 0x45, 0x1B,
    0x02, 0x94, 0x19, 0xD5, 0xFD, 0xCD, 0x96, 0x0D, 0x01, 0xEA, 0x91, 0x7A,
    0x01, 0xA0, 0x64, 0x5E, 0x28, 0x44, 0x87, 0x2B, 0xFF, 0xA4, 0x1C, 0xD9,
    0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};
const size_t apple_icon_png_len = sizeof(apple_icon_png);
*/

const uint8_t apple_icon_png[] PROGMEM = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x90,
    0x08, 0x02, 0x00, 0x00, 0x00, 0x68, 0x24, 0x75, 0xEF, 0x00, 0x00, 0x01,
    0x96, 0x49, 0x44, 0x41, 0x54, 0x78, 0xDA, 0xED, 0xDC, 0x41, 0x8E, 0xC2,
    0x30, 0x10, 0x44, 0xD1, 0xDC, 0xFF, 0x6C, 0xDC, 0x29, 0x5C, 0x80, 0x4D,
    0x84, 0x15, 0xBB, 0xBA, 0x1E, 0xFA, 0x4B, 0xC4, 0x62, 0x9E, 0xE2, 0xB4,
    0x63, 0x86, 0xEB, 0x73, 0x5D, 0x0A, 0xCA, 0x9F, 0x00, 0x98, 0x80, 0x09,
    0x18, 0x30, 0x01, 0x13, 0x30, 0x60, 0x02, 0x26, 0x60, 0xC0, 0x04, 0x4C,
    0xC0, 0x80, 0x09, 0x98, 0x80, 0x01, 0x13, 0x30, 0x01, 0x03, 0x26, 0x60,
    0x02, 0x06, 0x4C, 0xC0, 0x04, 0x0C, 0x98, 0x80, 0x09, 0x18, 0xB0, 0x9C,
    0xEE, 0x5F, 0xAF, 0x47, 0xEF, 0x7F, 0xFA, 0xF9, 0xC0, 0x16, 0x83, 0xED,
    0x7D, 0x3F, 0xB0, 0x95, 0x60, 0xFF, 0x5F, 0x8E, 0xC0, 0x36, 0xAF, 0x87,
    0xC0, 0xDC, 0xC0, 0x80, 0x6D, 0x02, 0x48, 0xB9, 0xBC, 0x62, 0xC0, 0xDE,
    0x5F, 0x0F, 0x8D, 0xF5, 0xE6, 0x43, 0x60, 0x65, 0xE3, 0x46, 0x0C, 0x98,
    0x71, 0x23, 0x1E, 0xAC, 0x73, 0xDC, 0xC8, 0x00, 0x33, 0x6E, 0xC4, 0x83,
    0xD5, 0x8E, 0x1B, 0x01, 0x60, 0x9E, 0x6E, 0xD4, 0x81, 0x0D, 0xBB, 0xBC,
    0xF2, 0xC0, 0x9A, 0xC7, 0x8D, 0xD3, 0xC1, 0x8C, 0x1B, 0xF1, 0x60, 0xE5,
    0xE3, 0xC6, 0x28, 0xB0, 0x86, 0x71, 0xE3, 0x68, 0x30, 0xEB, 0x61, 0x1D,
    0xD8, 0xC8, 0xF5, 0x30, 0x09, 0xCC, 0x7C, 0x38, 0x07, 0xAC, 0xE7, 0x06,
    0x76, 0x28, 0x98, 0x07, 0x1C, 0xC0, 0x86, 0xDC, 0xC0, 0xC6, 0x82, 0x4D,
    0x9D, 0x38, 0x80, 0x01, 0x03, 0x06, 0x0C, 0x98, 0xA1, 0x03, 0x18, 0x30,
    0x60, 0xF6, 0x61, 0x9E, 0x74, 0x00, 0x73, 0x18, 0x06, 0xCC, 0xA0, 0xE8,
    0x3C, 0xCC, 0x01, 0xA6, 0x13, 0x16, 0x60, 0x56, 0x45, 0x60, 0xBE, 0xE6,
    0x66, 0x55, 0x04, 0x66, 0x55, 0xAC, 0xFE, 0x67, 0x08, 0xDF, 0xAD, 0xB7,
    0x83, 0x06, 0x66, 0xF4, 0x98, 0x0D, 0xE6, 0x22, 0x9B, 0x00, 0xD6, 0x7C,
    0xDA, 0x92, 0xF1, 0x00, 0xCD, 0x7C, 0x1F, 0x0F, 0x56, 0x3B, 0xDF, 0xB7,
    0x80, 0xF9, 0xE9, 0x22, 0xAB, 0x22, 0x30, 0xAB, 0x22, 0x30, 0xBF, 0x97,
    0x68, 0x55, 0x04, 0x66, 0x07, 0xDD, 0x03, 0x56, 0xB8, 0x83, 0x06, 0x16,
    0xB6, 0x2A, 0x26, 0x81, 0x39, 0x6D, 0x01, 0x06, 0xCC, 0xA0, 0x08, 0x0C,
    0x18, 0x30, 0x60, 0xC0, 0x80, 0x19, 0x3A, 0xEC, 0xC3, 0x80, 0x79, 0xD2,
    0x01, 0x4C, 0xC0, 0x80, 0x09, 0x98, 0x80, 0x01, 0x13, 0x30, 0xC5, 0x80,
    0xDD, 0xF9, 0x2F, 0x60, 0xC0, 0x80, 0x01, 0x03, 0x06, 0x0C, 0x18, 0x30,
    0x60, 0xB2, 0x0F, 0x13, 0x30, 0x60, 0x02, 0x16, 0x33, 0x95, 0x00, 0x03,
    0x06, 0x0C, 0x18, 0x30, 0x60, 0x02, 0x06, 0x4C, 0xC0, 0x04, 0x4C, 0xC0,
    0x80, 0x09, 0x98, 0x80, 0x01, 0x13, 0x30, 0x01, 0x03, 0x26, 0x60, 0x02,
    0x06, 0x4C, 0xC0, 0x04, 0x0C, 0x98, 0x80, 0x09, 0x18, 0x30, 0x01, 0x13,
    0x30, 0x60, 0x02, 0x26, 0x60, 0xC0, 0x04, 0x4C, 0xEB, 0xFA, 0x02, 0xCC,
    0xBC, 0x3A, 0xF7, 0xEE, 0xB8, 0x7B, 0x3A, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};
const size_t apple_icon_png_len = sizeof(apple_icon_png);

// HTML & CSS for a streamlined table view
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>Viessmann DID 2 InfluxDB</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent">
    <meta name="apple-mobile-web-app-title" content="Wärmepumpe">
    <link rel="icon" type="image/png" href="/favicon.png">
    <link rel="apple-touch-icon" type="image/png" href="/apple-touch-icon.png">
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

        .influx-cell { text-align: center; font-size: 1.2rem; font-weight: bold; }
        .check-icon { color: #00aa00; }
        
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
                            e_tdCat.colSpan = 4;
                            e_row.appendChild(e_tdCat);
                            tbody.appendChild(e_row);

                            row.className = 'category-row';
                            let tdDid = document.createElement('td');
                            let tdName = document.createElement('td');
                            let tdVal = document.createElement('td');
                            tdVal.innerText = item.name;
                            let tdInflux = document.createElement('td');
                            row.appendChild(tdDid);
                            row.appendChild(tdName);
                            row.appendChild(tdVal);
                            row.appendChild(tdInflux);
                            
                        } else {
                            let tdDid = document.createElement('td');
                            tdDid.innerText = item.did;
                            
                            let tdName = document.createElement('td');
                            tdName.innerText = item.name;
                            
                            let tdVal = document.createElement('td');
                            tdVal.className = 'val';
                            tdVal.innerText = '---';

                            let tdInflux = document.createElement('td');
                            tdInflux.className = 'influx-cell';
                            
                            row.appendChild(tdDid);
                            row.appendChild(tdName);
                            row.appendChild(tdVal);
                            row.appendChild(tdInflux);
                        }
                        tbody.appendChild(row);
                    }
                    if(item.did < 999900) {
                        row.querySelector('.val').innerText = item.value;

                        let influxCell = row.querySelector('.influx-cell');
                        if (item.influx) {
                            influxCell.innerHTML = '<span class="check-icon">✔</span>';
                        } else {
                            influxCell.innerHTML = '';
                        }
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
        <h1>Viessmann DID 2 InfluxDB</h1>
        <p>Cyclic CAN bus telemetry polling (interval: 1 minute)</p>
        <table>
            <thead>
                <tr>
                    <th>DID</th>
                    <th style="text-align: right";>Name</th>
                    <th style="text-align: left;">Current value</th>
                    <th style="text-align: center;">InfluxDB</th>
                </tr>
            </thead>
            <tbody id="data-table">
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

void addDidToInfluxList(uint32_t did) {
    for (uint8_t i = 0; i < influxSentCount; i++) {
        if (influxSentDids[i] == did) return; 
    }
    if (influxSentCount < NUM_OBJECTS) {
        influxSentDids[influxSentCount] = did;
        influxSentCount++;
    }
}
bool isDidInInfluxList(uint32_t did) {
    for (uint8_t i = 0; i < influxSentCount; i++) {
        if (influxSentDids[i] == did) return true;
    }
    return false;
}
void clearInfluxList() {
    influxSentCount = 0; 
    memset(influxSentDids, 0, sizeof(influxSentDids));
}

void sendToInfluxDB() {
    clearInfluxList();

    Point sensor1("Temperaturen");
    sensor1.addField("did_" + String(outsideTemperatureSensor.getDid()), outsideTemperatureSensor.getActual());
    sensor1.addField("did_" + String(flowTemperatureSensor.getDid()),    flowTemperatureSensor.getActual());
    sensor1.addField("did_" + String(returnTemperatureSensor.getDid()),  returnTemperatureSensor.getActual());
    sensor1.addField("did_" + String(domesticHotWaterSensor.getDid()),   domesticHotWaterSensor.getActual());
    if (client.writePoint(sensor1)) {
        addDidToInfluxList(outsideTemperatureSensor.getDid());
        addDidToInfluxList(flowTemperatureSensor.getDid());
        addDidToInfluxList(returnTemperatureSensor.getDid());
        addDidToInfluxList(domesticHotWaterSensor.getDid());
    }

    Point sensor2("Verdichter");
    sensor2.addField("did_" + String(compressorInletTemperatureSensor.getDid()),  compressorInletTemperatureSensor.getActual());
    sensor2.addField("did_" + String(compressorOutletTemperatureSensor.getDid()), compressorOutletTemperatureSensor.getActual());
    sensor2.addField("did_" + String(compressorInletPressureSensor.getDid()),     compressorInletPressureSensor.getActual());
    sensor2.addField("did_" + String(compressorOutletPressureSensor.getDid()),    compressorOutletPressureSensor.getActual());
    sensor2.addField("did_" + String(compressorSpeedRps.getDid()),                compressorSpeedRps.getInt());
    if (client.writePoint(sensor2)) {
        addDidToInfluxList(compressorInletTemperatureSensor.getDid());
        addDidToInfluxList(compressorOutletTemperatureSensor.getDid());
        addDidToInfluxList(compressorInletPressureSensor.getDid());
        addDidToInfluxList(compressorOutletPressureSensor.getDid());
        addDidToInfluxList(compressorSpeedRps.getDid());
    }

    Point sensor3("Statistik");
    sensor3.addField("did_" + String(heatPumpCompressorStatistical.getDid())       + "_starts", heatPumpCompressorStatistical.getStarts());
    sensor3.addField("did_" + String(heatPumpCompressorStatistical.getDid())       + "_hours",  heatPumpCompressorStatistical.getHours());
    sensor3.addField("did_" + String(additionalElectricHeaterStatistical.getDid()) + "_starts", additionalElectricHeaterStatistical.getStarts());
    sensor3.addField("did_" + String(additionalElectricHeaterStatistical.getDid()) + "_hours",  additionalElectricHeaterStatistical.getHours());
    if (client.writePoint(sensor3)) {
        addDidToInfluxList(heatPumpCompressorStatistical.getDid());
        addDidToInfluxList(additionalElectricHeaterStatistical.getDid());
    }

    Point sensor4("Leistung");
    sensor4.addField("did_" + String(currentElectricalPowerConsumptionRefrigerantCircuit.getDid()), currentElectricalPowerConsumptionRefrigerantCircuit.getInt());
    sensor4.addField("did_" + String(currentElectricalPowerConsumptionElectricHeater.getDid()),     currentElectricalPowerConsumptionElectricHeater.getInt());
    sensor4.addField("did_" + String(currentElectricalPowerConsumptionSystem.getDid()),             currentElectricalPowerConsumptionSystem.getInt());
    sensor4.addField("did_" + String(currentThermalCapacityRefrigerantCircuit.getDid()),            currentThermalCapacityRefrigerantCircuit.getInt());
    sensor4.addField("did_" + String(currentThermalCapacityElectricHeater.getDid()),                currentThermalCapacityElectricHeater.getInt());
    sensor4.addField("did_" + String(currentThermalCapacitySystem.getDid()),                        currentThermalCapacitySystem.getInt());
    if (client.writePoint(sensor4)) {
        addDidToInfluxList(currentElectricalPowerConsumptionRefrigerantCircuit.getDid());
        addDidToInfluxList(currentElectricalPowerConsumptionElectricHeater.getDid());
        addDidToInfluxList(currentElectricalPowerConsumptionSystem.getDid());
        addDidToInfluxList(currentThermalCapacityRefrigerantCircuit.getDid());
        addDidToInfluxList(currentThermalCapacityElectricHeater.getDid());
        addDidToInfluxList(currentThermalCapacitySystem.getDid());
    }

    Point sensor5("Energieverbrauch");
    sensor5.addField("did_" + String(energyConsumptionCentralHeating.getDid()),                 energyConsumptionCentralHeating.getToday());
    sensor5.addField("did_" + String(energyConsumptionDomesticHotWater.getDid()),               energyConsumptionDomesticHotWater.getToday());
    sensor5.addField("did_" + String(energyConsumptionDomesticHotWaterElectricHeater.getDid()), energyConsumptionDomesticHotWaterElectricHeater.getToday());
    sensor5.addField("did_" + String(energyConsumptionCentralHeatingElectricHeater.getDid()),   energyConsumptionCentralHeatingElectricHeater.getToday());
    if (client.writePoint(sensor5)) {
        addDidToInfluxList(energyConsumptionCentralHeating.getDid());
        addDidToInfluxList(energyConsumptionDomesticHotWater.getDid());
        addDidToInfluxList(energyConsumptionDomesticHotWaterElectricHeater.getDid());
        addDidToInfluxList(energyConsumptionCentralHeatingElectricHeater.getDid());
    }

    Point sensor6("Waermemenge");
    sensor6.addField("did_" + String(generatedCentralHeatingOutput.getDid()),   generatedCentralHeatingOutput.getToday());
    sensor6.addField("did_" + String(generatedDomesticHotWaterOutput.getDid()), generatedDomesticHotWaterOutput.getToday());
    if (client.writePoint(sensor6)) {
        addDidToInfluxList(generatedCentralHeatingOutput.getDid());
        addDidToInfluxList(generatedDomesticHotWaterOutput.getDid());
    }

    Point sensor7("Jahresarbeitszahlen");
    sensor7.addField("did_" + String(seasonalCoefficientOfPerformanceHeating.getDid()),                    seasonalCoefficientOfPerformanceHeating.getCurrentYear());
    sensor7.addField("did_" + String(seasonalCoefficientOfPerformanceDomesticHotWater.getDid()),           seasonalCoefficientOfPerformanceDomesticHotWater.getCurrentYear());
    sensor7.addField("did_" + String(seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater.getDid()), seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater.getCurrentYear());
    if (client.writePoint(sensor7)) {
        addDidToInfluxList(seasonalCoefficientOfPerformanceHeating.getDid());
        addDidToInfluxList(seasonalCoefficientOfPerformanceDomesticHotWater.getDid());
        addDidToInfluxList(seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater.getDid());
    }
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
            sendToInfluxDB();
            lastQueryTime = currentMillis;
            currentQueryStep = 0;
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

    // Set up web server endpoints
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ 
        request->send(200, "text/html; charset=UTF-8", String(index_html)); 
    });
    
    // JSON endpoint provides the current data, including names, for the dynamic web UI
    server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
        String json = "[";
        for (uint8_t i = 0; i < NUM_OBJECTS; i++) {
            uint32_t currentDid = canObjects[i]->getDid();
            json += "{";
            json += "\"did\":"    + String(currentDid) + ",";
            json += "\"name\":\""  + canObjects[i]->getName()        + "\",";
            json += "\"value\":\"" + canObjects[i]->getFormattedString() + "\",";
            json += "\"influx\":"  + String(isDidInInfluxList(currentDid) ? "true" : "false");
            json += "}";
            if (i < NUM_OBJECTS - 1) {
                json += ",";
            }
        }
        json += "]";
        request->send(200, "application/json", json);
    });

    server.on("/apple-touch-icon.png", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", apple_icon_png, apple_icon_png_len);
        request->send(response);
    });

    server.on("/favicon.png", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncWebServerResponse *response = request->beginResponse_P(200, "image/png", apple_icon_png, apple_icon_png_len);
        request->send(response);
    });

    ElegantOTA.begin(&server); 
    server.begin();

    setupWatchdog();

    // Initialize the timestamp so that the query begins immediately upon startup
    lastQueryTime = millis();
    lastStepTime = millis() - stepInterval;
}

void loop() {
    esp_task_wdt_reset();
    ElegantOTA.loop();
    checkForResponse();
    handleDaemonTimer();
    yield();
}
