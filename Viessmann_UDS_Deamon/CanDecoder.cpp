#include "CanDecoder.h"

// Helper
static float convertBytesToFloat(const uint8_t* bytes) {
    int16_t raw16 = (int16_t)(bytes[0] | (bytes[1] << 8));
    return raw16 / 10.0f;
}
static int16 convertBytesToInt16(const uint8_t* bytes) {
    return (int16_t)(bytes[0] | (bytes[1] << 8));
}
static int32 convertBytesToInt32(const uint8_t* bytes) {
    return (int32_t)(bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24));
}
static int64 convertBytesToInt64(const uint8_t* bytes) {
    return (int64_t)(bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24) | (bytes[4] << 32) | (bytes[5] << 40) | (bytes[6] << 48) | (bytes[7] << 56));
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


