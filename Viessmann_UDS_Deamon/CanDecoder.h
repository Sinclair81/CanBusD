#ifndef CAN_DECODER_H
#define CAN_DECODER_H

#include <Arduino.h>

// ======================================== Class: SingleTemp ========================================
class SingleTemp {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[2] = {0};

public:
    SingleTemp(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getTemperature() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: TempWithStatus ========================================
class TempWithStatus {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesTemp[2] = {0};
    uint8_t _statusByte = 0;

public:
    TempWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getTemperature() const;
    uint8_t getStatus() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: MultiTempWithStatus ========================================
class MultiTempWithStatus {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesTemp1[2] = {0};
    uint8_t _rawBytesTemp2[2] = {0};
    uint8_t _rawBytesTemp3[2] = {0};
    uint8_t _rawBytesTemp4[2] = {0};
    uint8_t _statusByte = 0;

public:
    MultiTempWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getTemperature1() const;
    float getTemperature2() const;
    float getTemperature3() const;
    float getTemperature4() const;
    uint8_t getStatus() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: SinglePressure ========================================
class SinglePressure {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[2] = {0};

public:
    SinglePressure(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getPressure() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: PressureWithStatus ========================================
class PressureWithStatus {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPressure[2] = {0};
    uint8_t _statusByte = 0;

public:
    PressureWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getPressure() const;
    uint8_t getStatus() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: MultiPressureWithStatus ========================================
class MultiPressureWithStatus {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPressure1[2] = {0};
    uint8_t _rawBytesPressure2[2] = {0};
    uint8_t _rawBytesPressure3[2] = {0};
    uint8_t _rawBytesPressure4[2] = {0};
    uint8_t _statusByte = 0;

public:
    MultiPressureWithStatus(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getPressure1() const;
    float getPressure2() const;
    float getPressure3() const;
    float getPressure4() const;
    uint8_t getStatus() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: Pump ========================================
class Pump {
private:
    uint32_t _did;
    String _name;
    uint8_t _stateByte = 0;
    uint8_t _targetByte = 0;
    uint8_t _actualByte = 0;
    uint8_t _unknownByte = 0;

public:
    Pump(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    uint8_t getState() const;
    uint8_t getTarget() const;
    uint8_t getActual() const;
    uint8_t getUnknown() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: MixerPump ========================================
class MixerPump {
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
    void parse(const uint8_t* busBytes);
    uint8_t getTargetPowerState() const;
    uint8_t getTargetValue() const;
    uint8_t getPowerState() const;
    uint8_t getErrorState() const;
    uint8_t getActualValue() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: ModeAndState ========================================
class ModeAndState {
private:
    uint32_t _did;
    String _name;
    uint8_t _modeByte = 0;
    uint8_t _stateByte = 0;

public:
    ModeAndState(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    uint8_t getMode() const;
    uint8_t getState() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: PowerAndError ========================================
class PowerAndError {
private:
    uint32_t _did;
    String _name;
    uint8_t _powerByte = 0;
    uint8_t _errorByte = 0;

public:
    PowerAndError(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    uint8_t getPower() const;
    uint8_t getError() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: MixerCurve ========================================
class MixerCurve {
private:
    uint32_t _did;
    String _name;
    uint8_t gradientByte = 0;
    uint8_t _levelByte = 0;
    uint8_t _rawBytesBasePoint[2] = {0};

public:
    MixerCurve(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    uint8_t getGradient() const;
    uint8_t getLevel() const;
    uint16_t getBasePoint() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: HeatPumpCompressor ========================================
class HeatPumpCompressor {
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
    void parse(const uint8_t* busBytes);
    uint64_t getUnknown1() const;
    uint16_t getStarts() const;
    uint16_t getUnknown2() const;
    uint16_t getHours() const;
    uint16_t getUnknown3() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: AdditionalElectricHeater ========================================
class AdditionalElectricHeater {
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
    void parse(const uint8_t* busBytes);
    uint32_t getUnknown1() const;
    uint16_t getStarts() const;
    uint16_t getUnknown2() const;
    uint16_t getHours() const;
    uint16_t getUnknown3() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: FlowSensor ========================================
class FlowSensor {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesActual[2]      = {0};
    uint8_t _rawBytesTemperature[2] = {0};
    uint8_t _unknownByte = 0;

public:
    FlowSensor(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getActual() const;
    float getTemperature() const;
    uint8_t getUnknown() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: Power ========================================
class Power {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytesPower[4] = {0};

public:
    Power(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    uint32_t getPower() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: Utf8String ========================================
class Utf8String {
private:
    uint32_t _did;
    String _name;
    char _buffer[65]; // 64 Bytes + 1 Byte '\0'
    size_t _currentBytes;
    size_t getUtf8Length(const char* str, size_t incomingLen) const;

public:
    Utf8String(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes, size_t length);
    const char* getString() const;
    uint32_t getDid() const;
    String getName() const;
    size_t getByteLength() const;
};

// ======================================== Class: CanInt8 ========================================
class CanInt8 {
private:
    uint32_t _did;
    String _name;
    uint8_t _byte = 0;

public:
    CanInt8(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    int8_t getInt() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: CanInt16 ========================================
class CanInt16 {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[2] = {0};

public:
    CanInt16(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    int16_t getInt() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: CanInt32 ========================================
class CanInt32 {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[4] = {0};

public:
    CanInt32(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    int32_t getInt() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

// ======================================== Class: CanFloat32 ========================================
class CanFloat32 {
private:
    uint32_t _did;
    String _name;
    uint8_t _rawBytes[4] = {0};

public:
    CanFloat32(uint32_t did, const String& name);
    void parse(const uint8_t* busBytes);
    float getFloat() const;
    uint32_t getDid() const { return _did; }
    String getName() const { return _name; }
    String getFormattedString() const;
};

#endif // CAN_DECODER_H
