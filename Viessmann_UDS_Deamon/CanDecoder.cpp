#include "CanDecoder.h"

// Helper
static float convertBytesToFloat(const uint8_t* bytes) {
    int16_t raw16 = (int16_t)(bytes[0] | (bytes[1] << 8));
    return raw16 / 10.0f;
}
static float convertBytesToFloat32(const uint8_t* bytes) {
    union FloatByteConverter {
        float f;
        uint8_t b[4];
    };
    FloatByteConverter converter;
    converter.b[0] = bytes[0];
    converter.b[1] = bytes[1];
    converter.b[2] = bytes[2];
    converter.b[3] = bytes[3];
    return converter.f;
}
static int16_t convertBytesToInt16(const uint8_t* bytes) {
    return (int16_t)((uint16_t)bytes[0] | 
                     ((uint16_t)bytes[1] << 8));
}
static int32_t convertBytesToInt32(const uint8_t* bytes) {
    return (int32_t)((uint32_t)bytes[0] | 
                     ((uint32_t)bytes[1] << 8) | 
                     ((uint32_t)bytes[2] << 16) | 
                     ((uint32_t)bytes[3] << 24));
}
static int64_t convertBytesToInt64(const uint8_t* bytes) {
    return (int64_t)((uint64_t)bytes[0] | 
                     ((uint64_t)bytes[1] << 8) | 
                     ((uint64_t)bytes[2] << 16) | 
                     ((uint64_t)bytes[3] << 24) | 
                     ((uint64_t)bytes[4] << 32) | 
                     ((uint64_t)bytes[5] << 40) | 
                     ((uint64_t)bytes[6] << 48) | 
                     ((uint64_t)bytes[7] << 56));
}

// ======================================== Class: SingleTemp ========================================

SingleTemp::SingleTemp(uint32_t did, const String& name) : _did(did), _name(name) {
    // Initialisierung der ID und des Namens über die Initialisierungsliste
}

void SingleTemp::parse(const uint8_t* busBytes) {
    _rawBytes[0] = busBytes[0];
    _rawBytes[1] = busBytes[1];
}

float SingleTemp::getTemperature() const {
    return convertBytesToFloat(_rawBytes);
}

String SingleTemp::getFormattedString() const {
    String output = "";
    output += "Actual:" + String(getTemperature(), 1) + "°C";
    return output;
}

// ======================================== Class: TempWithStatus ========================================

TempWithStatus::TempWithStatus(uint32_t did, const String& name) : _did(did), _name(name) {
}

void TempWithStatus::parse(const uint8_t* busBytes) {
    _rawBytesTemp[0] = busBytes[0];
    _rawBytesTemp[1] = busBytes[1];
    _statusByte      = busBytes[2]; // Byte 3
}

float TempWithStatus::getTemperature() const {
    return convertBytesToFloat(_rawBytesTemp);
}

uint8_t TempWithStatus::getStatus() const {
    return _statusByte;
}

String TempWithStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"        + String(getTemperature(), 1) + "°C | ";
    output += "Sensor Status:" + String(getStatus());
    return output;
}

// ======================================== Class: MultiTempWithStatus ========================================

MultiTempWithStatus::MultiTempWithStatus(uint32_t did, const String& name) : _did(did), _name(name) {
}

void MultiTempWithStatus::parse(const uint8_t* busBytes) {
    _rawBytesTemp1[0] = busBytes[0];
    _rawBytesTemp1[1] = busBytes[1];
    _rawBytesTemp2[0] = busBytes[2];
    _rawBytesTemp2[1] = busBytes[3];
    _rawBytesTemp3[0] = busBytes[4];
    _rawBytesTemp3[1] = busBytes[5];
    _rawBytesTemp4[0] = busBytes[6];
    _rawBytesTemp4[1] = busBytes[7];
    _statusByte       = busBytes[8];
}

float MultiTempWithStatus::getTemperature1() const {
    return convertBytesToFloat(_rawBytesTemp1);
}

float MultiTempWithStatus::getTemperature2() const {
    return convertBytesToFloat(_rawBytesTemp2);
}

float MultiTempWithStatus::getTemperature3() const {
    return convertBytesToFloat(_rawBytesTemp3);
}

float MultiTempWithStatus::getTemperature4() const {
    return convertBytesToFloat(_rawBytesTemp4);
}

uint8_t MultiTempWithStatus::getStatus() const {
    return _statusByte;
}

String MultiTempWithStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"        + String(getTemperature1(), 1) + "°C | ";
    output += "Minimum:"       + String(getTemperature2(), 1) + "°C | ";
    output += "Maximum:"       + String(getTemperature3(), 1) + "°C | ";
    output += "Average:"       + String(getTemperature4(), 1) + "°C | ";
    output += "Sensor Status:" + String(getStatus());
    return output;
}

// ======================================== Class: SinglePressure ========================================

SinglePressure::SinglePressure(uint32_t did, const String& name) : _did(did), _name(name) {
    // Initialisierung der ID und des Namens über die Initialisierungsliste
}

void SinglePressure::parse(const uint8_t* busBytes) {
    _rawBytes[0] = busBytes[0];
    _rawBytes[1] = busBytes[1];
}

float SinglePressure::getPressure() const {
    return convertBytesToFloat(_rawBytes);
}

String SinglePressure::getFormattedString() const {
    String output = "";
    output += "Actual:" + String(getPressure(), 1) + "°C";
    return output;
}

// ======================================== Class: PressureWithStatus ========================================

PressureWithStatus::PressureWithStatus(uint32_t did, const String& name) : _did(did), _name(name) {
}

void PressureWithStatus::parse(const uint8_t* busBytes) {
    _rawBytesPressure[0] = busBytes[0];
    _rawBytesPressure[1] = busBytes[1];
    _statusByte      = busBytes[2]; // Byte 3
}

float PressureWithStatus::getPressure() const {
    return convertBytesToFloat(_rawBytesPressure);
}

uint8_t PressureWithStatus::getStatus() const {
    return _statusByte;
}

String PressureWithStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"        + String(getPressure(), 1) + "°C | ";
    output += "Sensor Status:" + String(getStatus());
    return output;
}

// ======================================== Class: MultiPressureWithStatus ========================================

MultiPressureWithStatus::MultiPressureWithStatus(uint32_t did, const String& name) : _did(did), _name(name) {
}

void MultiPressureWithStatus::parse(const uint8_t* busBytes) {
    _rawBytesPressure1[0] = busBytes[0];
    _rawBytesPressure1[1] = busBytes[1];
    _rawBytesPressure2[0] = busBytes[2];
    _rawBytesPressure2[1] = busBytes[3];
    _rawBytesPressure3[0] = busBytes[4];
    _rawBytesPressure3[1] = busBytes[5];
    _rawBytesPressure4[0] = busBytes[6];
    _rawBytesPressure4[1] = busBytes[7];
    _statusByte       = busBytes[8];
}

float MultiPressureWithStatus::getPressure1() const {
    return convertBytesToFloat(_rawBytesPressure1);
}

float MultiPressureWithStatus::getPressure2() const {
    return convertBytesToFloat(_rawBytesPressure2);
}

float MultiPressureWithStatus::getPressure3() const {
    return convertBytesToFloat(_rawBytesPressure3);
}

float MultiPressureWithStatus::getPressure4() const {
    return convertBytesToFloat(_rawBytesPressure4);
}

uint8_t MultiPressureWithStatus::getStatus() const {
    return _statusByte;
}

String MultiPressureWithStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"        + String(getPressure1(), 1) + "°C | ";
    output += "Minimum:"       + String(getPressure2(), 1) + "°C | ";
    output += "Maximum:"       + String(getPressure3(), 1) + "°C | ";
    output += "Average:"       + String(getPressure4(), 1) + "°C | ";
    output += "Sensor Status:" + String(getStatus());
    return output;
}

// ======================================== Class: Pump ========================================

Pump::Pump(uint32_t did, const String& name) : _did(did), _name(name) {
}

void Pump::parse(const uint8_t* busBytes) {
    _stateByte   = busBytes[0];
    _targetByte  = busBytes[1];
    _actualByte  = busBytes[2];
    _unknownByte = busBytes[3];
}

uint8_t Pump::getState() const {
    return _stateByte;
}

uint8_t Pump::getTarget() const {
    return _targetByte;
}

uint8_t Pump::getActual() const {
    return _actualByte;
}

uint8_t Pump::getUnknown() const {
    return _unknownByte;
}

String Pump::getFormattedString() const {
    String output = "";
    output += "State:"   + String(getState())  + " | ";
    output += "Target:"  + String(getTarget()) + "% | ";
    output += "Actual:"  + String(getActual()) + "% | ";
    output += "Unknown:" + String(getUnknown());
    return output;
}

// ======================================== Class: MixerPump ========================================

MixerPump::MixerPump(uint32_t did, const String& name) : _did(did), _name(name) {
}

void MixerPump::parse(const uint8_t* busBytes) {
    _targetPowerStateByte = busBytes[0];
    _targetValueByte      = busBytes[1];
    _powerStateByte       = busBytes[2];
    _errorStateByte       = busBytes[3];
    _actualValueByte      = busBytes[4];
}

uint8_t MixerPump::getTargetPowerState() const {
    return _targetPowerStateByte;
}

uint8_t MixerPump::getTargetValue() const {
    return _targetValueByte;
}

uint8_t MixerPump::getPowerState() const {
    return _powerStateByte;
}

uint8_t MixerPump::getErrorState() const {
    return _errorStateByte;
}

uint8_t MixerPump::getActualValue() const {
    return _actualValueByte;
}

String MixerPump::getFormattedString() const {
    String output = "";
    output += "Target Power State:" + String(getTargetPowerState()) + " | ";
    output += "Target Value:"       + String(getTargetValue())      + " | ";
    output += "Power State:"        + String(getPowerState())       + " | ";
    output += "Error State:"        + String(getErrorState())       + " | ";
    output += "Actual Value:"       + String(getActualValue());
    return output;
}

// ======================================== Class: ModeAndState ========================================

ModeAndState::ModeAndState(uint32_t did, const String& name) : _did(did), _name(name) {
}

void ModeAndState::parse(const uint8_t* busBytes) {
    _modeByte  = busBytes[0];
    _stateByte = busBytes[1];
}

uint8_t ModeAndState::getMode() const {
    return _modeByte;
}

uint8_t ModeAndState::getState() const {
    return _stateByte;
}

String ModeAndState::getFormattedString() const {
    String output = "";
    output += "Mode:"  + String(getMode()) + " | ";
    output += "State:" + String(getState());
    return output;
}

// ======================================== Class: PowerAndError ========================================

PowerAndError::PowerAndError(uint32_t did, const String& name) : _did(did), _name(name) {
}

void PowerAndError::parse(const uint8_t* busBytes) {
    _powerByte = busBytes[0];
    _errorByte = busBytes[1];
}

uint8_t PowerAndError::getPower() const {
    return _powerByte;
}

uint8_t PowerAndError::getError() const {
    return _errorByte;
}

String PowerAndError::getFormattedString() const {
    String output = "";
    output += "Power:" + String(getPower()) + " | ";
    output += "Error:" + String(getError());
    return output;
}

// ======================================== Class: MixerCurve ========================================

MixerCurve::MixerCurve(uint32_t did, const String& name) : _did(did), _name(name) {
}

void MixerCurve::parse(const uint8_t* busBytes) {
    _gradientByte         = busBytes[0];
    _levelByte            = busBytes[1];
    _rawBytesBasePoint[0] = busBytes[2];
    _rawBytesBasePoint[1] = busBytes[3];
}

uint8_t MixerCurve::getGradient() const {
    return _gradientByte;
}

uint8_t MixerCurve::getLevel() const {
    return _levelByte;
}

uint16_t MixerCurve::getBasePoint() const {
    return convertBytesToInt16(_rawBytesBasePoint);
}

String MixerCurve::getFormattedString() const {
    String output = "";
    output += "Gradient:"   + String(getGradient()) + " | ";
    output += "Level:"      + String(getLevel())    + " | ";
    output += "Base Point:" + String(getBasePoint());
    return output;
}

// ======================================== Class: HeatPumpCompressor ========================================

HeatPumpCompressor::HeatPumpCompressor(uint32_t did, const String& name) : _did(did), _name(name) {
}

void HeatPumpCompressor::parse(const uint8_t* busBytes) {
    _rawBytesUnknown1[0] = busBytes[0];
    _rawBytesUnknown1[1] = busBytes[1];
    _rawBytesUnknown1[2] = busBytes[2];
    _rawBytesUnknown1[3] = busBytes[3];
    _rawBytesUnknown1[4] = busBytes[4];
    _rawBytesUnknown1[5] = busBytes[5];
    _rawBytesUnknown1[6] = 0x00; // only 6 Bytes
    _rawBytesUnknown1[7] = 0x00; // only 6 Bytes
    _rawBytesStarts[0]   = busBytes[6];
    _rawBytesStarts[1]   = busBytes[7];
    _rawBytesUnknown2[0] = busBytes[8];
    _rawBytesUnknown2[1] = busBytes[9];
    _rawBytesHours[0]    = busBytes[10];
    _rawBytesHours[1]    = busBytes[11];
    _rawBytesUnknown3[0] = busBytes[12];
    _rawBytesUnknown3[1] = busBytes[13];
}

uint64_t HeatPumpCompressor::getUnknown1() const {
    return convertBytesToInt64(_rawBytesUnknown1);
}

uint16_t HeatPumpCompressor::getStarts() const {
    return convertBytesToInt16(_rawBytesStarts);
}

uint16_t HeatPumpCompressor::getUnknown2() const {
    return convertBytesToInt16(_rawBytesUnknown2);
}

uint16_t HeatPumpCompressor::getHours() const {
    return convertBytesToInt16(_rawBytesHours);
}

uint16_t HeatPumpCompressor::getUnknown3() const {
    return convertBytesToInt16(_rawBytesUnknown3);
}

String HeatPumpCompressor::getFormattedString() const {
    String output = "";
    output += "Unknown 1:" + String(getUnknown1()) + " | ";
    output += "Starts:"    + String(getStarts())   + " | ";
    output += "Unknown 2:" + String(getUnknown2()) + " | ";
    output += "Hours:"     + String(getHours())    + " | ";
    output += "Unknown 3:" + String(getUnknown3());
    return output;
}

// ======================================== Class: AdditionalElectricHeater ========================================

AdditionalElectricHeater::AdditionalElectricHeater(uint32_t did, const String& name) : _did(did), _name(name) {
}

void AdditionalElectricHeater::parse(const uint8_t* busBytes) {
    _rawBytesUnknown1[0] = busBytes[0];
    _rawBytesUnknown1[1] = busBytes[1];
    _rawBytesUnknown1[2] = busBytes[2];
    _rawBytesUnknown1[3] = 0x00; // only 3 Bytes
    _rawBytesStarts[0]   = busBytes[3];
    _rawBytesStarts[1]   = busBytes[4];
    _rawBytesUnknown2[0] = busBytes[5];
    _rawBytesUnknown2[1] = busBytes[6];
    _rawBytesHours[0]    = busBytes[7];
    _rawBytesHours[1]    = busBytes[8];
    _rawBytesUnknown3[0] = busBytes[9];
    _rawBytesUnknown3[1] = busBytes[10];
}

uint32_t AdditionalElectricHeater::getUnknown1() const {
    return convertBytesToInt32(_rawBytesUnknown1);
}

uint16_t AdditionalElectricHeater::getStarts() const {
    return convertBytesToInt16(_rawBytesStarts);
}

uint16_t AdditionalElectricHeater::getUnknown2() const {
    return convertBytesToInt16(_rawBytesUnknown2);
}

uint16_t AdditionalElectricHeater::getHours() const {
    return convertBytesToInt16(_rawBytesHours);
}

uint16_t AdditionalElectricHeater::getUnknown3() const {
    return convertBytesToInt16(_rawBytesUnknown3);
}

String AdditionalElectricHeater::getFormattedString() const {
    String output = "";
    output += "Unknown 1:" + String(getUnknown1()) + " | ";
    output += "Starts:"    + String(getStarts())   + " | ";
    output += "Unknown 2:" + String(getUnknown2()) + " | ";
    output += "Hours:"     + String(getHours())    + " | ";
    output += "Unknown 3:" + String(getUnknown3());
    return output;
}

// ======================================== Class: FlowSensor ========================================

FlowSensor::FlowSensor(uint32_t did, const String& name) : _did(did), _name(name) {
}

void FlowSensor::parse(const uint8_t* busBytes) {
    _rawBytesActual[0]      = busBytes[0];
    _rawBytesActual[1]      = busBytes[1];
    _rawBytesTemperature[0] = busBytes[2];
    _rawBytesTemperature[1] = busBytes[3];
    _unknownByte            = busBytes[4];
}

float FlowSensor::getActual() const {
    return convertBytesToFloat(_rawBytesActual);
}

float FlowSensor::getTemperature() const {
    return convertBytesToFloat(_rawBytesTemperature);
}

uint8_t FlowSensor::getUnknown() const {
    return _unknownByte;
}

String FlowSensor::getFormattedString() const {
    String output = "";
    output += "Actual:"      + String(getActual(), 0)      + "l/h | ";
    output += "Temperature:" + String(getTemperature(), 1) + "°C | ";
    output += "Unknown:"     + String(getUnknown());
    return output;
}

// ======================================== Class: Power ========================================

Power::Power(uint32_t did, const String& name) : _did(did), _name(name) {
}

void Power::parse(const uint8_t* busBytes) {
    _rawBytesPower[0] = busBytes[0];
    _rawBytesPower[1] = busBytes[1];
    _rawBytesPower[2] = busBytes[2];
    _rawBytesPower[3] = busBytes[3];
}

uint32_t Power::getPower() const {
    return convertBytesToInt32(_rawBytesPower);
}

String Power::getFormattedString() const {
    String output = "";
    output += "Power:" + String(getPower()) + "W";
    return output;
}

// ======================================== Class: Utf8String ========================================

Utf8String::Utf8String(uint32_t did, const String& name) 
    : _did(did), _name(name), _currentBytes(0) {
    _buffer[0] = '\0';
}

size_t Utf8String::getUtf8Length(const char* str, size_t incomingLen) const {
    size_t allowed = (incomingLen < 64) ? incomingLen : 64;
    // If we land exactly on a UTF-8 continuation byte (bit pattern 10xxxxxx),
    // we move backwards until the character is complete again.
    while (allowed > 0 && ((str[allowed] & 0xC0) == 0x80)) {
        allowed--;
    }
    return allowed;
}

void Utf8String::parse(const uint8_t* busBytes, size_t length) {
    if (!busBytes || length == 0) {
        _buffer[0] = '\0';
        _currentBytes = 0;
        return;
    }
    _currentBytes = getUtf8Length((const char*)busBytes, length);
    memcpy(_buffer, busBytes, _currentBytes);
    _buffer[_currentBytes] = '\0';
}

const char* Utf8String::getString() const {
    return _buffer;
}

uint32_t Utf8String::getDid() const {
    return _did;
}

String Utf8String::getName() const {
    return _name;
}

size_t Utf8String::getByteLength() const {
    return _currentBytes;
}

// ======================================== Class: CanInt8 ========================================

CanInt8::CanInt8(uint32_t did, const String& name) : _did(did), _name(name) {
}

void CanInt8::parse(const uint8_t* busBytes) {
    _byte = busBytes[0];
}

int8_t CanInt8::getInt() const {
    return (int8_t)_rawBytes;
}

String CanInt8::getFormattedString() const {
    String output = "";
    output += "Int8:" + String(getInt()) + " [%, ENUM, dBm, min, cnt, mbar]";
    return output;
}

// ======================================== Class: CanInt16 ========================================

CanInt16::CanInt16(uint32_t did, const String& name) : _did(did), _name(name) {
}

void CanInt16::parse(const uint8_t* busBytes) {
    _rawBytes[0] = busBytes[0];
    _rawBytes[1] = busBytes[1];
}

int16_t CanInt16::getInt() const {
    return convertBytesToInt16(_rawBytes);
}

String CanInt16::getFormattedString() const {
    String output = "";
    output += "Int16:" + String(getInt()) + " [°C, bar, hPas, l/h, h, rps, rpm, µA, mA, Wh, W, kW, VA, m3]";
    return output;
}

// ======================================== Class: CanInt32 ========================================

CanInt32::CanInt32(uint32_t did, const String& name) : _did(did), _name(name) {
}

void CanInt32::parse(const uint8_t* busBytes) {
    _rawBytes[0] = busBytes[0];
    _rawBytes[1] = busBytes[1];
    _rawBytes[2] = busBytes[2];
    _rawBytes[3] = busBytes[3];
}

int32_t CanInt32::getInt() const {
    return convertBytesToInt32(_rawBytes);
}

String CanInt32::getFormattedString() const {
    String output = "";
    output += "Int32:" + String(getInt()) + " [W, kWh, V, A]";
    return output;
}

// ======================================== Class: CanFloat32 ========================================

CanFloat32::CanFloat32(uint32_t did, const String& name) : _did(did), _name(name) {
}

void CanFloat32::parse(const uint8_t* busBytes) {
    _rawBytes[0] = busBytes[0];
    _rawBytes[1] = busBytes[1];
    _rawBytes[2] = busBytes[2];
    _rawBytes[3] = busBytes[3];
}

float CanFloat32::getFloat() const {
    return convertBytesToFloat32(_rawBytes);
}

String CanFloat32::getFormattedString() const {
    String output = "";
    output += "Float32:" + String(getFloat(), 2) + " [Ah, %]";
    return output;
}
