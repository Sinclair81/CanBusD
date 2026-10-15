#ifndef CAN_DECODER_H
#define CAN_DECODER_H

#include <Arduino.h>

// ======================================== Class: CanObject ========================================
class CanObject {
public:
    virtual ~CanObject() {}
    virtual uint32_t getDid() const = 0;
    virtual String getName() const = 0;
    virtual String getFormattedString() const = 0;
    
    virtual void parse(const uint8_t* busBytes) {}
    virtual void parse(const uint8_t* busBytes, size_t length) { parse(busBytes); } // Fallback
};

// ======================================== Class: SingleTemp ========================================
class SingleTemp : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[2] = {0};

public:
    SingleTemp(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getTemperature() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: TempWithStatus ========================================
class TempWithStatus : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesTemp[2] = {0};
    uint8_t _statusByte = 0;

public:
    TempWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getTemperature() const;
    uint8_t getStatus() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: MultiTempWithStatus ========================================
class MultiTempWithStatus : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesActual[2] = {0};
    uint8_t _rawBytesMinimum[2] = {0};
    uint8_t _rawBytesMaximum[2] = {0};
    uint8_t _rawBytesAverage[2] = {0};
    uint8_t _statusByte = 0;

public:
    MultiTempWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getActual() const;
    float getMinimum() const;
    float getMaximum() const;
    float getAverage() const;
    uint8_t getStatus() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: TemperatureSetpointMetaData ========================================
class TemperatureSetpointMetaData : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesLowerLimit[2] = {0};
    uint8_t _rawBytesMinimum[2] = {0};
    uint8_t _rawBytesDefault[2] = {0};
    uint8_t _rawBytesMaximum[2] = {0};
    uint8_t _rawBytesUpperLimit[2] = {0};

public:
    TemperatureSetpointMetaData(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getLowerLimit() const;
    float getMinimum() const;
    float getDefault() const;
    float getMaximum() const;
    float getUpperLimit() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: TemperatureSetpointMetaDataWithEfficiency ========================================
class TemperatureSetpointMetaDataWithEfficiency : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesLowerLimit[2] = {0};
    uint8_t _rawBytesMinimum[2] = {0};
    uint8_t _rawBytesDefault[2] = {0};
    uint8_t _rawBytesMaximum[2] = {0};
    uint8_t _rawBytesUpperLimit[2] = {0};
    uint8_t _rawBytesEfficiencyLowerLimit[2] = {0};
    uint8_t _rawBytesEfficiencyUpperLimit[2] = {0};

public:
    TemperatureSetpointMetaDataWithEfficiency(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getLowerLimit() const;
    float getMinimum() const;
    float getDefault() const;
    float getMaximum() const;
    float getUpperLimit() const;
    float getEfficiencyLowerLimit() const;
    float getEfficiencyUpperLimit() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: Hysteresis ========================================
class Hysteresis : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesSetpointSwitchOn[2]  = {0};
    uint8_t _rawBytesSetpointSwitchOff[2] = {0};

public:
    Hysteresis(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getSetpointSwitchOn() const;
    float getSetpointSwitchOff() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: SinglePressure ========================================
class SinglePressure : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[2] = {0};

public:
    SinglePressure(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getPressure() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: PressureWithStatus ========================================
class PressureWithStatus : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPressure[2] = {0};
    uint8_t _statusByte = 0;

public:
    PressureWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getPressure() const;
    uint8_t getStatus() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: MultiPressureWithStatus ========================================
class MultiPressureWithStatus : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesActual[2] = {0};
    uint8_t _rawBytesMinimum[2] = {0};
    uint8_t _rawBytesMaximum[2] = {0};
    uint8_t _rawBytesAverage[2] = {0};
    uint8_t _statusByte = 0;

public:
    MultiPressureWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getActual() const;
    float getMinimum() const;
    float getMaximum() const;
    float getAverage() const;
    uint8_t getStatus() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: Pump ========================================
class Pump : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _stateByte = 0;
    uint8_t _targetByte = 0;
    uint8_t _actualByte = 0;
    uint8_t _unknownByte = 0;

public:
    Pump(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getState() const;
    uint8_t getTarget() const;
    uint8_t getActual() const;
    uint8_t getUnknown() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: MixerPump ========================================
class MixerPump : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _targetPowerStateByte = 0;
    uint8_t _targetValueByte = 0;
    uint8_t _powerStateByte = 0;
    uint8_t _errorStateByte = 0;
    uint8_t _actualValueByte = 0;

public:
    MixerPump(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getTargetPowerState() const;
    uint8_t getTargetValue() const;
    uint8_t getPowerState() const;
    uint8_t getErrorState() const;
    uint8_t getActualValue() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: ModeAndState ========================================
class ModeAndState : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _modeByte = 0;
    uint8_t _stateByte = 0;

public:
    ModeAndState(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getMode() const;
    uint8_t getState() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: PowerAndError ========================================
class PowerAndError : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _powerByte = 0;
    uint8_t _errorByte = 0;

public:
    PowerAndError(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getPower() const;
    uint8_t getError() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: MixerCurve ========================================
class MixerCurve : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _gradientByte = 0;
    uint8_t _levelByte = 0;
    uint8_t _rawBytesBasePoint[2] = {0};

public:
    MixerCurve(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getGradient() const;
    uint8_t getLevel() const;
    uint16_t getBasePoint() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: HeatPumpCompressor ========================================
class HeatPumpCompressor : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesUnknown1[8] = {0}; // only 6 Bytes
    uint8_t _rawBytesStarts[2]   = {0};
    uint8_t _rawBytesUnknown2[2] = {0};
    uint8_t _rawBytesHours[2]    = {0};
    uint8_t _rawBytesUnknown3[2] = {0};

public:
    HeatPumpCompressor(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint64_t getUnknown1() const;
    uint16_t getStarts() const;
    uint16_t getUnknown2() const;
    uint16_t getHours() const;
    uint16_t getUnknown3() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: AdditionalElectricHeater ========================================
class AdditionalElectricHeater : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesUnknown1[4] = {0}; // only 3 Bytes
    uint8_t _rawBytesStarts[2]   = {0};
    uint8_t _rawBytesUnknown2[2] = {0};
    uint8_t _rawBytesHours[2]    = {0};
    uint8_t _rawBytesUnknown3[2] = {0};

public:
    AdditionalElectricHeater(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint32_t getUnknown1() const;
    uint16_t getStarts() const;
    uint16_t getUnknown2() const;
    uint16_t getHours() const;
    uint16_t getUnknown3() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: FlowSensor ========================================
class FlowSensor : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesActual[2]      = {0};
    uint8_t _rawBytesTemperature[2] = {0};
    uint8_t _unknownByte = 0;

public:
    FlowSensor(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getActual() const;
    float getTemperature() const;
    uint8_t getUnknown() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: Power ========================================
class Power : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPower[4] = {0};

public:
    Power(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint32_t getPower() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: EnergyConsumption ========================================
class EnergyConsumption : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesToday[4]        = {0};
    uint8_t _rawBytesPast7Days[4]    = {0};
    uint8_t _rawBytesCurrentMonth[4] = {0};
    uint8_t _rawBytesPastMonth[4]    = {0};
    uint8_t _rawBytesCurrentYear[4]  = {0};
    uint8_t _rawBytesPastYear[4]     = {0};

public:
    EnergyConsumption(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getToday() const;
    float getPast7Days() const;
    float getCurrentMonth() const;
    float getPastMonth() const;
    float getCurrentYear() const;
    float getPastYear() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: CanTime ========================================
class CanTime : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesHour   = 0;
    uint8_t _rawBytesMinute = 0;

public:
    CanTime(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getHour() const;
    uint8_t getMinute() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: CanTimeWithSecond ========================================
class CanTimeWithSecond : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesHour   = 0;
    uint8_t _rawBytesMinute = 0;
    uint8_t _rawBytesSecond = 0;

public:
    CanTimeWithSecond(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getHour() const;
    uint8_t getMinute() const;
    uint8_t getSecond() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: PressureConfiguration ========================================
class PressureConfiguration : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesDefaultPressure[2]     = {0};
    uint8_t _rawBytesNormalPressureDelta[2] = {0};
    uint8_t _rawBytesMaximumPressure[2]     = {0};
    uint8_t _rawBytesErrorThreshold[2]      = {0};

public:
    PressureConfiguration(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getDefaultPressure() const;
    float getNormalPressureDelta() const;
    float getMaximumPressure() const;
    float getErrorThreshold() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: SeasonalCoefficient ========================================
class SeasonalCoefficient : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesCurrentYear       =  0;
    uint8_t _rawBytesEnergyThermal[4]  = {0};
    uint8_t _rawBytesEnergyElectric[4] = {0};

public:
    SeasonalCoefficient(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getCurrentYear() const;
    float getEnergyThermal() const;
    float getEnergyElectric() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: ThermalPower ========================================
class ThermalPower : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPower[2]   = {0};
    uint8_t _rawBytesUnknown[2] = {0};

public:
    ThermalPower(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint16_t getPower() const;
    uint16_t getUnknown() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: NominalPower ========================================
class NominalPower : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPower[2]   = {0};
    uint8_t _rawBytesUnknown[8] = {0};

public:
    NominalPower(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint16_t getPower() const;
    uint64_t getUnknown() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: TemperatureOffsets ========================================
class TemperatureOffsets : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesSetpointIncreaseRoomTemperatureHeating[2] = {0};
    uint8_t _rawBytesSetpointDecreaseRoomTemperatureCooling[2] = {0};
    uint8_t _rawBytesSetpointIncreaseDHWTemperature[2] = {0};
    uint8_t _rawBytesSetpointIncreaseBufferTemperatureHeating[2] = {0};
    uint8_t _rawBytesSetpointDecreaseBufferTemperatureCooling[2] = {0};

public:
    TemperatureOffsets(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getSetpointIncreaseRoomTemperatureHeating() const;
    float getSetpointDecreaseRoomTemperatureCooling() const;
    float getSetpointIncreaseDHWTemperature() const;
    float getSetpointIncreaseBufferTemperatureHeating() const;
    float getSetpointDecreaseBufferTemperatureCooling() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: PumpStatus ========================================
class PumpStatus : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawByteActual      =  0;
    uint8_t _rawBytesUnknown[4] = {0};

public:
    PumpStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    uint8_t getActual() const;
    uint32_t getUnknown() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: SetTemperature ========================================
class SetTemperature : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesMinimum[2] = {0};
    uint8_t _rawBytesMaximum[2] = {0};

public:
    SetTemperature(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getMinimum() const;
    float getMaximum() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: Utf8String ========================================
class Utf8String : public CanObject {
private:
    uint32_t _did;
    String _name;
    char _buffer[65]; // 64 Bytes + 1 Byte '\0'
    size_t _currentBytes;
    size_t getUtf8Length(const char* str, size_t incomingLen) const;

public:
    Utf8String(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes, size_t length) override;
    const char* getString() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override { return String(getString()); }
    size_t getByteLength() const;
};

// ======================================== Class: CanInt8 ========================================
class CanInt8 : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _byte = 0;

public:
    CanInt8(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    int8_t getInt() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: CanInt16 ========================================
class CanInt16 : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[2] = {0};

public:
    CanInt16(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    int16_t getInt() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: CanInt32 ========================================
class CanInt32 : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[4] = {0};

public:
    CanInt32(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    int32_t getInt() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

// ======================================== Class: CanFloat32 ========================================
class CanFloat32 : public CanObject {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[4] = {0};

public:
    CanFloat32(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes) override;
    float getFloat() const;
    uint32_t getDid() const override { return _did; }
    String getName() const override { return _name; }
    String getFormattedString() const override;
};

#endif // CAN_DECODER_H
