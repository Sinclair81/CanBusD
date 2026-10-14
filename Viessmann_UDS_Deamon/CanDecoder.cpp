#include "CanDecoder.h"

// Helper
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
static float convertBytesToFloat8(const uint8_t* byte) {
    return byte / 10.0f;
}
static float convertBytesToFloat16(const uint8_t* bytes) {
    int16_t raw16 = convertBytesToInt16(bytes);
    return raw16 / 10.0f;
}
static float convertBytesToFloat32(const uint8_t* bytes) {
    int32_t raw32 = convertBytesToInt32(bytes);
    return raw32 / 10.0f;
}
static float convertBytesToRealFloat32(const uint8_t* bytes) {
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

// ======================================== Class: SingleTemp ========================================

SingleTemp::SingleTemp(uint32_t did, const String& name) : _did(did), _name(name) {
    // Initialisierung der ID und des Namens über die Initialisierungsliste
}

void SingleTemp::parse(const uint8_t* busBytes) {
    _rawBytes[0] = busBytes[0];
    _rawBytes[1] = busBytes[1];
}

float SingleTemp::getTemperature() const {
    return convertBytesToFloat16(_rawBytes);
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
    return convertBytesToFloat16(_rawBytesTemp);
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
    _rawBytesActual[0] = busBytes[0];
    _rawBytesActual[1] = busBytes[1];
    _rawBytesMinimum[0] = busBytes[2];
    _rawBytesMinimum[1] = busBytes[3];
    _rawBytesMaximum[0] = busBytes[4];
    _rawBytesMaximum[1] = busBytes[5];
    _rawBytesAverage[0] = busBytes[6];
    _rawBytesAverage[1] = busBytes[7];
    _statusByte       = busBytes[8];
}

float MultiTempWithStatus::getActual() const {
    return convertBytesToFloat16(_rawBytesActual);
}

float MultiTempWithStatus::getMinimum() const {
    return convertBytesToFloat16(_rawBytesMinimum);
}

float MultiTempWithStatus::getMaximum() const {
    return convertBytesToFloat16(_rawBytesMaximum);
}

float MultiTempWithStatus::getAverage() const {
    return convertBytesToFloat16(_rawBytesAverage);
}

uint8_t MultiTempWithStatus::getStatus() const {
    return _statusByte;
}

String MultiTempWithStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"        + String(getActual(), 1) + "°C | ";
    output += "Minimum:"       + String(getMinimum(), 1) + "°C | ";
    output += "Maximum:"       + String(getMaximum(), 1) + "°C | ";
    output += "Average:"       + String(getAverage(), 1) + "°C | ";
    output += "Sensor Status:" + String(getStatus());
    return output;
}

// ======================================== Class: TemperatureSetpointMetaData ========================================

TemperatureSetpointMetaData::TemperatureSetpointMetaData(uint32_t did, const String& name) : _did(did), _name(name) {
}

void TemperatureSetpointMetaData::parse(const uint8_t* busBytes) {
    _rawBytesLowerLimit[0] = busBytes[0];
    _rawBytesLowerLimit[1] = busBytes[1];
    _rawBytesMinimum[0]    = busBytes[2];
    _rawBytesMinimum[1]    = busBytes[3];
    _rawBytesDefault[0]    = busBytes[4];
    _rawBytesDefault[1]    = busBytes[5];
    _rawBytesMaximum[0]    = busBytes[6];
    _rawBytesMaximum[1]    = busBytes[7];
    _rawBytesUpperLimit[0] = busBytes[8];
    _rawBytesUpperLimit[1] = busBytes[9];
}

float TemperatureSetpointMetaData::getLowerLimit() const {
    return convertBytesToFloat16(_rawBytesLowerLimit);
}

float TemperatureSetpointMetaData::getMinimum() const {
    return convertBytesToFloat16(_rawBytesMinimum);
}

float TemperatureSetpointMetaData::getDefault() const {
    return convertBytesToFloat16(_rawBytesDefault);
}

float TemperatureSetpointMetaData::getMaximum() const {
    return convertBytesToFloat16(_rawBytesMaximum);
}

float TemperatureSetpointMetaData::getUpperLimit() const {
    return convertBytesToFloat16(_rawBytesUpperLimit);
}

String TemperatureSetpointMetaData::getFormattedString() const {
    String output = "";
    output += "LowerLimit:" + String(getLowerLimit(), 1) + "°C | ";
    output += "Minimum:"    + String(getMinimum(), 1)    + "°C | ";
    output += "Default:"    + String(getDefault(), 1)    + "°C | ";
    output += "Maximum:"    + String(getMaximum(), 1)    + "°C | ";
    output += "UpperLimit:" + String(getUpperLimit(), 1) + "°C";
    return output;
}

// ======================================== Class: TemperatureSetpointMetaDataWithEfficiency ========================================

TemperatureSetpointMetaDataWithEfficiency::TemperatureSetpointMetaDataWithEfficiency(uint32_t did, const String& name) : _did(did), _name(name) {
}

void TemperatureSetpointMetaDataWithEfficiency::parse(const uint8_t* busBytes) {
    _rawBytesLowerLimit[0]     = busBytes[0];
    _rawBytesLowerLimit[1]     = busBytes[1];
    _rawBytesMinimum[0]        = busBytes[2];
    _rawBytesMinimum[1]        = busBytes[3];
    _rawBytesDefault[0]        = busBytes[4];
    _rawBytesDefault[1]        = busBytes[5];
    _rawBytesMaximum[0]        = busBytes[6];
    _rawBytesMaximum[1]        = busBytes[7];
    _rawBytesUpperLimit[0]     = busBytes[8];
    _rawBytesUpperLimit[1]     = busBytes[9];
    _rawBytesEfficiencyLowerLimit[0] = busBytes[10];
    _rawBytesEfficiencyLowerLimit[1] = busBytes[11];
    _rawBytesEfficiencyUpperLimit[0] = busBytes[12];
    _rawBytesEfficiencyUpperLimit[1] = busBytes[13];
}

float TemperatureSetpointMetaDataWithEfficiency::getLowerLimit() const {
    return convertBytesToFloat16(_rawBytesLowerLimit);
}

float TemperatureSetpointMetaDataWithEfficiency::getMinimum() const {
    return convertBytesToFloat16(_rawBytesMinimum);
}

float TemperatureSetpointMetaDataWithEfficiency::getDefault() const {
    return convertBytesToFloat16(_rawBytesDefault);
}

float TemperatureSetpointMetaDataWithEfficiency::getMaximum() const {
    return convertBytesToFloat16(_rawBytesMaximum);
}

float TemperatureSetpointMetaDataWithEfficiency::getUpperLimit() const {
    return convertBytesToFloat16(_rawBytesUpperLimit);
}

float TemperatureSetpointMetaDataWithEfficiency::getEfficiencyLowerLimit() const {
    return convertBytesToFloat16(_rawBytesEfficiencyLowerLimit);
}

float TemperatureSetpointMetaDataWithEfficiency::getEfficiencyUpperLimit() const {
    return convertBytesToFloat16(_rawBytesEfficiencyUpperLimit);
}

String TemperatureSetpointMetaDataWithEfficiency::getFormattedString() const {
    String output = "";
    output += "LowerLimit:"           + String(getLowerLimit(), 1)           + "°C | ";
    output += "Minimum:"              + String(getMinimum(), 1)              + "°C | ";
    output += "Default:"              + String(getDefault(), 1)              + "°C | ";
    output += "Maximum:"              + String(getMaximum(), 1)              + "°C | ";
    output += "UpperLimit:"           + String(getUpperLimit(), 1)           + "°C | ";
    output += "EfficiencyLowerLimit:" + String(getEfficiencyLowerLimit(), 1) + "°C | ";
    output += "EfficiencyUpperLimit:" + String(getEfficiencyUpperLimit(), 1) + "°C";
    return output;
}

// ======================================== Class: Hysteresis ========================================

Hysteresis::Hysteresis(uint32_t did, const String& name) : _did(did), _name(name) {
}

void Hysteresis::parse(const uint8_t* busBytes) {
    _rawBytesSetpointSwitchOn[0]  = busBytes[0];
    _rawBytesSetpointSwitchOn[1]  = busBytes[1];
    _rawBytesSetpointSwitchOff[0] = busBytes[2];
    _rawBytesSetpointSwitchOff[1] = busBytes[3];
}

float Hysteresis::getSetpointSwitchOn() const {
    return convertBytesToFloat16(_rawBytesSetpointSwitchOn);
}

float Hysteresis::getSetpointSwitchOff() const {
    return convertBytesToFloat16(_rawBytesSetpointSwitchOff);
}

String Hysteresis::getFormattedString() const {
    String output = "";
    output += "Setpoint Switch On:"  + String(getSetpointSwitchOn(), 1)  + "°C | ";
    output += "Setpoint Switch Off:" + String(getSetpointSwitchOff(), 1) + "°C";
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
    return convertBytesToFloat16(_rawBytes);
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
    return convertBytesToFloat16(_rawBytesPressure);
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
    _rawBytesActual[0] = busBytes[0];
    _rawBytesActual[1] = busBytes[1];
    _rawBytesMinimum[0] = busBytes[2];
    _rawBytesMinimum[1] = busBytes[3];
    _rawBytesMaximum[0] = busBytes[4];
    _rawBytesMaximum[1] = busBytes[5];
    _rawBytesAverage[0] = busBytes[6];
    _rawBytesAverage[1] = busBytes[7];
    _statusByte       = busBytes[8];
}

float MultiPressureWithStatus::getActual() const {
    return convertBytesToFloat16(_rawBytesActual);
}

float MultiPressureWithStatus::getMinimum() const {
    return convertBytesToFloat16(_rawBytesMinimum);
}

float MultiPressureWithStatus::getMaximum() const {
    return convertBytesToFloat16(_rawBytesMaximum);
}

float MultiPressureWithStatus::getAverage() const {
    return convertBytesToFloat16(_rawBytesAverage);
}

uint8_t MultiPressureWithStatus::getStatus() const {
    return _statusByte;
}

String MultiPressureWithStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"        + String(getActual(), 1) + "°C | ";
    output += "Minimum:"       + String(getMinimum(), 1) + "°C | ";
    output += "Maximum:"       + String(getMaximum(), 1) + "°C | ";
    output += "Average:"       + String(getAverage(), 1) + "°C | ";
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
    return convertBytesToFloat16(_rawBytesActual);
}

float FlowSensor::getTemperature() const {
    return convertBytesToFloat16(_rawBytesTemperature);
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

// ======================================== Class: EnergyConsumption ========================================

EnergyConsumption::EnergyConsumption(uint32_t did, const String& name) : _did(did), _name(name) {
}

void EnergyConsumption::parse(const uint8_t* busBytes) {
    _rawBytesToday[0]        = busBytes[0];
    _rawBytesToday[1]        = busBytes[1];
    _rawBytesToday[2]        = busBytes[2];
    _rawBytesToday[3]        = busBytes[3];
    _rawBytesPast7Days[0]    = busBytes[4];
    _rawBytesPast7Days[1]    = busBytes[5];
    _rawBytesPast7Days[2]    = busBytes[6];
    _rawBytesPast7Days[3]    = busBytes[7];
    _rawBytesCurrentMonth[0] = busBytes[8];
    _rawBytesCurrentMonth[1] = busBytes[9];
    _rawBytesCurrentMonth[2] = busBytes[10];
    _rawBytesCurrentMonth[3] = busBytes[11];
    _rawBytesPastMonth[0]    = busBytes[12];
    _rawBytesPastMonth[1]    = busBytes[13];
    _rawBytesPastMonth[2]    = busBytes[14];
    _rawBytesPastMonth[3]    = busBytes[15];
    _rawBytesCurrentYear[0]  = busBytes[16];
    _rawBytesCurrentYear[1]  = busBytes[17];
    _rawBytesCurrentYear[2]  = busBytes[18];
    _rawBytesCurrentYear[3]  = busBytes[19];
    _rawBytesPastYear[0]     = busBytes[20];
    _rawBytesPastYear[1]     = busBytes[21];
    _rawBytesPastYear[2]     = busBytes[22];
    _rawBytesPastYear[3]     = busBytes[23];
}

float EnergyConsumption::getToday() const {
    return convertBytesToFloat32(_rawBytesToday);
}

float EnergyConsumption::getPast7Days() const {
    return convertBytesToFloat32(_rawBytesPast7Days);
}

float EnergyConsumption::getCurrentMonth() const {
    return convertBytesToFloat32(_rawBytesCurrentMonth);
}

float EnergyConsumption::getPastMonth() const {
    return convertBytesToFloat32(_rawBytesPastMonth);
}

float EnergyConsumption::getCurrentYear() const {
    return convertBytesToFloat32(_rawBytesCurrentYear);
}

float EnergyConsumption::getPastYear() const {
    return convertBytesToFloat32(_rawBytesPastYear);
}

String EnergyConsumption::getFormattedString() const {
    String output = "";
    output += "Today:"        + String(getToday(), 1)        + "kWh | ";
    output += "Past7Days:"    + String(getPast7Days(), 1)    + "kWh | ";
    output += "CurrentMonth:" + String(getCurrentMonth(), 1) + "kWh | ";
    output += "PastMonth:"    + String(getPastMonth(), 1)    + "kWh | ";
    output += "CurrentYear:"  + String(getCurrentYear(), 1)  + "kWh | ";
    output += "PastYear:"     + String(getPastYear(), 1)     + "kWh";
    return output;
}

// ======================================== Class: CanTime ========================================

CanTime::CanTime(uint32_t did, const String& name) : _did(did), _name(name) {
}

void CanTime::parse(const uint8_t* busBytes) {
    _rawBytesHour   = busBytes[0];
    _rawBytesMinute = busBytes[1];
}

int8_t CanTime::getHour() const {
    return (int8_t)_rawBytesHour;
}

int8_t CanTime::getMinute() const {
    return (int8_t)_rawBytesMinute;
}

String CanTime::getFormattedString() const {
    String output = "";
    output += (getHour() < 10 ? "0" : "") + String(getHour()) + ":"  + (getMinute() < 10 ? "0" : "") + String(getMinute());
    return output;
}

// ======================================== Class: CanTimeWithSecond ========================================

CanTimeWithSecond::CanTimeWithSecond(uint32_t did, const String& name) : _did(did), _name(name) {
}

void CanTimeWithSecond::parse(const uint8_t* busBytes) {
    _rawBytesHour   = busBytes[0];
    _rawBytesMinute = busBytes[1];
    _rawBytesSecond = busBytes[2];
}

int8_t CanTimeWithSecond::getHour() const {
    return (int8_t)_rawBytesHour;
}

int8_t CanTimeWithSecond::getMinute() const {
    return (int8_t)_rawBytesMinute;
}

int8_t CanTimeWithSecond::getSecond() const {
    return (int8_t)_rawBytesSecond;
}

String CanTimeWithSecond::getFormattedString() const {
    String output = "";
    output += (getHour() < 10 ? "0" : "") + String(getHour()) + ":" + (getMinute() < 10 ? "0" : "") + String(getMinute()) + ":" + (getSecond() < 10 ? "0" : "") + String(getSecond());
    return output;
}

// ======================================== Class: PressureConfiguration ========================================

PressureConfiguration::PressureConfiguration(uint32_t did, const String& name) : _did(did), _name(name) {
}

void PressureConfiguration::parse(const uint8_t* busBytes) {
    _rawBytesDefaultPressure[0]     = busBytes[0];
    _rawBytesDefaultPressure[1]     = busBytes[1];
    _rawBytesNormalPressureDelta[0] = busBytes[2];
    _rawBytesNormalPressureDelta[1] = busBytes[3];
    _rawBytesMaximumPressure[0]     = busBytes[4];
    _rawBytesMaximumPressure[1]     = busBytes[5];
    _rawBytesErrorThreshold[0]      = busBytes[6];
    _rawBytesErrorThreshold[1]      = busBytes[7];
}

float PressureConfiguration::getDefaultPressure() const {
    return convertBytesToFloat16(_rawBytesDefaultPressure);
}

float PressureConfiguration::getNormalPressureDelta() const {
    return convertBytesToFloat16(_rawBytesNormalPressureDelta);
}

float PressureConfiguration::getMaximumPressure() const {
    return convertBytesToFloat16(_rawBytesMaximumPressure);
}

float PressureConfiguration::getErrorThreshold() const {
    return convertBytesToFloat16(_rawBytesErrorThreshold);
}

String PressureConfiguration::getFormattedString() const {
    String output = "";
    output += "Default:"         + String(getDefaultPressure(), 1)     + "bar | ";
    output += "Normal Delta:"    + String(getNormalPressureDelta(), 1) + "bar | ";
    output += "Maximum:"         + String(getMaximumPressure(), 1)     + "bar | ";
    output += "Error Threshold:" + String(getErrorThreshold(), 1)      + "bar";
    return output;
}

// ======================================== Class: SeasonalCoefficient ========================================

SeasonalCoefficient::SeasonalCoefficient(uint32_t did, const String& name) : _did(did), _name(name) {
}

void SeasonalCoefficient::parse(const uint8_t* busBytes) {
    _rawBytesCurrentYear       = busBytes[0];
    _rawBytesEnergyThermal[0]  = busBytes[1];
    _rawBytesEnergyThermal[1]  = busBytes[2];
    _rawBytesEnergyThermal[2]  = busBytes[3];
    _rawBytesEnergyThermal[3]  = busBytes[4];
    _rawBytesEnergyElectric[0] = busBytes[5];
    _rawBytesEnergyElectric[1] = busBytes[6];
    _rawBytesEnergyElectric[2] = busBytes[7];
    _rawBytesEnergyElectric[3] = busBytes[8];
}

float SeasonalCoefficient::getCurrentYear() const {
    return convertBytesToFloat8(_rawBytesCurrentYear);
}

float SeasonalCoefficient::getEnergyThermal() const {
    return convertBytesToFloat32(_rawBytesEnergyThermal);
}

float SeasonalCoefficient::getEnergyElectric() const {
    return convertBytesToFloat32(_rawBytesEnergyElectric);
}

String SeasonalCoefficient::getFormattedString() const {
    String output = "";
    output += "Current Year:"    + String(getCurrentYear(), 1)    + " | ";
    output += "Energy Thermal:"  + String(getEnergyThermal(), 1)  + "kWh | ";
    output += "Energy Electric:" + String(getEnergyElectric(), 1) + "kWh";
    return output;
}

// ======================================== Class: ThermalPower ========================================

ThermalPower::ThermalPower(uint32_t did, const String& name) : _did(did), _name(name) {
}

void ThermalPower::parse(const uint8_t* busBytes) {
    _rawBytesPower[0]   = busBytes[0];
    _rawBytesPower[1]   = busBytes[1];
    _rawBytesUnknown[0] = busBytes[2];
    _rawBytesUnknown[1] = busBytes[3];
}

uint16_t ThermalPower::getPower() const {
    return convertBytesToInt16(_rawBytesPower);
}

uint16_t ThermalPower::getUnknown() const {
    return convertBytesToInt16(_rawBytesUnknown);
}

String ThermalPower::getFormattedString() const {
    String output = "";
    output += "Power:"   + String(getPower()) + "kW | ";
    output += "Unknown:" + String(getUnknown());
    return output;
}

// ======================================== Class: NominalPower ========================================

NominalPower::NominalPower(uint32_t did, const String& name) : _did(did), _name(name) {
}

void NominalPower::parse(const uint8_t* busBytes) {
    _rawBytesPower[0]   = busBytes[0];
    _rawBytesPower[1]   = busBytes[1];
    _rawBytesUnknown[0] = busBytes[2];
    _rawBytesUnknown[1] = busBytes[3];
    _rawBytesUnknown[2] = busBytes[4];
    _rawBytesUnknown[3] = busBytes[5];
    _rawBytesUnknown[4] = busBytes[6];
    _rawBytesUnknown[5] = busBytes[7];
    _rawBytesUnknown[6] = 0;
    _rawBytesUnknown[8] = 0;
}

uint16_t NominalPower::getPower() const {
    return convertBytesToInt16(_rawBytesPower);
}

uint64_t NominalPower::getUnknown() const {
    return convertBytesToInt64(_rawBytesUnknown);
}

String NominalPower::getFormattedString() const {
    String output = "";
    output += "Power:"   + String(getPower()) + "kW | ";
    output += "Unknown:" + String(getUnknown());
    return output;
}

// ======================================== Class: TemperatureOffsets ========================================

TemperatureOffsets::TemperatureOffsets(uint32_t did, const String& name) : _did(did), _name(name) {
}

void TemperatureOffsets::parse(const uint8_t* busBytes) {
    _rawBytesSetpointIncreaseRoomTemperatureHeating[0]   = busBytes[0];
    _rawBytesSetpointIncreaseRoomTemperatureHeating[1]   = busBytes[1];
    _rawBytesSetpointDecreaseRoomTemperatureCooling[0]   = busBytes[2];
    _rawBytesSetpointDecreaseRoomTemperatureCooling[1]   = busBytes[3];
    _rawBytesSetpointIncreaseDHWTemperature[0]           = busBytes[4];
    _rawBytesSetpointIncreaseDHWTemperature[1]           = busBytes[5];
    _rawBytesSetpointIncreaseBufferTemperatureHeating[0] = busBytes[6];
    _rawBytesSetpointIncreaseBufferTemperatureHeating[1] = busBytes[7];
    _rawBytesSetpointDecreaseBufferTemperatureCooling[0] = busBytes[8];
    _rawBytesSetpointDecreaseBufferTemperatureCooling[1] = busBytes[9];
}

float TemperatureOffsets::getSetpointIncreaseRoomTemperatureHeating() const {
    return convertBytesToFloat16(_rawBytesSetpointIncreaseRoomTemperatureHeating);
}

float TemperatureOffsets::getSetpointDecreaseRoomTemperatureCooling() const {
    return convertBytesToFloat16(_rawBytesSetpointDecreaseRoomTemperatureCooling);
}

float TemperatureOffsets::getSetpointIncreaseDHWTemperature() const {
    return convertBytesToFloat16(_rawBytesSetpointIncreaseDHWTemperature);
}

float TemperatureOffsets::getSetpointIncreaseBufferTemperatureHeating() const {
    return convertBytesToFloat16(_rawBytesSetpointIncreaseBufferTemperatureHeating);
}

float TemperatureOffsets::getSetpointDecreaseBufferTemperatureCooling() const {
    return convertBytesToFloat16(_rawBytesSetpointDecreaseBufferTemperatureCooling);
}

String TemperatureOffsets::getFormattedString() const {
    String output = "";
    output += "SetP Inc Room Temp Heating:"   + String(getSetpointIncreaseRoomTemperatureHeating(), 1)   + "°C | ";
    output += "SetP Dec Room Temp Cooling:"   + String(getSetpointDecreaseRoomTemperatureCooling(), 1)   + "°C | ";
    output += "SetP Inc DHW Temp:"            + String(getSetpointIncreaseDHWTemperature(), 1).          + "°C | ";
    output += "SetP Inc Buffer Temp Heating:" + String(getSetpointIncreaseBufferTemperatureHeating(), 1) + "°C | ";
    output += "SetP Dec Buffer Temp Cooling:" + String(getSetpointDecreaseBufferTemperatureCooling(), 1) + "°C";
    return output;
}

// ======================================== Class: PumpStatus ========================================

PumpStatus::PumpStatus(uint32_t did, const String& name) : _did(did), _name(name) {
}

void PumpStatus::parse(const uint8_t* busBytes) {
    _rawByteActual      = busBytes[0];
    _rawBytesUnknown[0] = busBytes[1];
    _rawBytesUnknown[1] = busBytes[2];
    _rawBytesUnknown[2] = busBytes[3];
    _rawBytesUnknown[3] = busBytes[4];
}

uint8_t PumpStatus::getActual() const {
    return (uint8_t)_rawByteActual;
}

uint16_t PumpStatus::getUnknown() const {
    return convertBytesToInt32(_rawBytesUnknown);
}

String PumpStatus::getFormattedString() const {
    String output = "";
    output += "Actual:"  + String(getActual()) + " | ";
    output += "Unknown:" + String(getUnknown());
    return output;
}

// ======================================== Class: SetTemperature ========================================

SetTemperature::SetTemperature(uint32_t did, const String& name) : _did(did), _name(name) {
}

void SetTemperature::parse(const uint8_t* busBytes) {
    _rawBytesMinimum[0] = busBytes[0];
    _rawBytesMinimum[1] = busBytes[1];
    _rawBytesMaximum[0] = busBytes[2];
    _rawBytesMaximum[1] = busBytes[3];
}

float SetTemperature::getMinimum() const {
    return convertBytesToFloat16(_rawBytesMinimum);
}

float SetTemperature::getMaximum() const {
    return convertBytesToFloat16(_rawBytesMaximum);
}

String SetTemperature::getFormattedString() const {
    String output = "";
    output += "Minimum:" + String(getMinimum(), 1) + "°C | ";
    output += "Maximum:" + String(getMaximum(), 1) + "°C";
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
    return (int8_t)_byte;
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
    return convertBytesToRealFloat32(_rawBytes);
}

String CanFloat32::getFormattedString() const {
    String output = "";
    output += "Float32:" + String(getFloat(), 2) + " [Ah, %]";
    return output;
}
