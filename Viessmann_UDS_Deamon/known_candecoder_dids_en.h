
SingleTemp domesticHotWaterTemperatureSetpoint(                 396, "domestic hot water temperature setpoint");
SingleTemp externalTargetFlowTemperatureSetpoint(               528, "external target flow temperature setpoint");
SingleTemp solarChargingDomesticHotWaterSetpoint(              1394, "solar charging domestic hot water setpoint");
SingleTemp primaryCircuitCurrentTemperatureSetpoint(           1799, "primary circuit current temperature setpoint");
SingleTemp minimumSecondaryReturnTemperatureRefrigerantCircuit(2255, "minimum secondary return temperature refrigerant circuit");
SingleTemp domesticHotWaterTemperatureSetpointOffset(          2257, "domestic hot water temperature setpoint offset");
SingleTemp bivalenceControlAlternativeTemperature(             2831, "bivalence control alternative temperature");
SingleTemp domesticHotWaterTemperatureSetpointComfort(         3068, "domestic hot water temperature setpoint comfort");

TempWithStatus legionellaProtectionTargetTemperatureSetpoint(874, "legionella protection target temperature setpoint");
TempWithStatus endResultDomesticHotWaterTemperatureSetpoint(1659, "end result domestic hot water temperature setpoint");

MultiTempWithStatus flowTemperatureSensor(                          268, "flow temperature sensor");
MultiTempWithStatus returnTemperatureSensor(                        269, "return temperature sensor");
MultiTempWithStatus domesticHotWaterSensor(                         271, "domestic hot water sensor");
MultiTempWithStatus outsideTemperatureSensor(                       274, "outside temperature sensor");
MultiTempWithStatus mixerOneCircuitFlowTemperatureSensor(           284, "mixer one circuit flow temperature sensor");
MultiTempWithStatus primaryHeatExchangerLiquidTemperatureSensor(    320, "primary heat exchanger liquid temperature sensor");      // only actual; everything else 0.0
MultiTempWithStatus compressorInletTemperatureSensor(               321, "compressor inlet temperature sensor");                   // only actual; everything else 0.0
MultiTempWithStatus compressorOutletTemperatureSensor(              324, "compressor outlet temperature sensor");                  // only actual; everything else 0.0
MultiTempWithStatus secondaryHeatExchangerLiquidTemperatureSensor(  355, "secondary heat exchanger liquid temperature sensor");    // only actual; everything else 0.0
MultiTempWithStatus receiverTemperatureSensor(                     1768, "receiver temperature sensor");                           // nicht da
MultiTempWithStatus primaryInletTemperatureSensor(                 1769, "primary inlet temperature sensor");                      // only actual; everything else 0.0
MultiTempWithStatus secondaryOutletTemperatureSensor(              1770, "secondary outlet temperature sensor");                   // only actual; everything else 0.0
MultiTempWithStatus engineRoomTemperatureSensor(                   1771, "engine room temperature sensor");                        // only actual; everything else 0.0
MultiTempWithStatus compressorOilTemperatureSensor(                1772, "compressor oil temperature sensor");                     // only actual; everything else 0.0
MultiTempWithStatus economizerLiquidTemperatureSensor(             2333, "economizer liquid temperature sensor");                  // only actual; everything else 0.0
MultiTempWithStatus evaporatorVaporTemperatureSensor(              2334, "evaporator vapor temperature sensor");                   // only actual; everything else 0.0
MultiTempWithStatus calculatedOutsideTemperature(                  2457, "calculated outside temperature");
MultiTempWithStatus secondaryHeatExchangerVaporPressureSensor(     2784, "secondary heat exchanger vapor pressure sensor");        // everything 0.0
MultiTempWithStatus inverterInternalPowerModuleTemperature(        2808, "inverter internal power module temperature");            // only actual; everything else 0.0
MultiTempWithStatus outdoorMiddleCoilTemperatureSensor(            3014, "outdoor middle coil temperature sensor");                // everything 0.0
MultiTempWithStatus heatSinkTemperatureSensor(                     3015, "heat sink temperature sensor");                          // everything 0.0
MultiTempWithStatus heatingBufferTemperatureSensor(                3016, "heating buffer temperature sensor");                     // everything 0.0
MultiTempWithStatus coolingBufferTemperatureSensor(                3017, "cooling buffer temperature sensor");                     // everything 0.0
MultiTempWithStatus heatingCoolingBufferTemperatureSensor(         3018, "heating cooling buffer temperature sensor");             // everything 0.0
MultiTempWithStatus domesticHotWaterReturnTemperaturTankLoadSystem(3034, "domestic hot water return temperatur tank load system"); // everything 0.0
MultiTempWithStatus domesticHotWaterFlowTemperaturTankLoadSystem(  3035, "domestic hot water flow temperatur tank load system");   // everything 0.0
MultiTempWithStatus domesticHotWaterBufferTopTemperatureSensor(    3234, "domestic hot water buffer top temperature sensor");

PressureWithStatus waterPressureSensor(           318, "water pressure sensor");
PressureWithStatus compressorInletPressureSensor( 322, "compressor inlet pressure sensor");  // only actual; everything else 0.0
PressureWithStatus compressorOutletPressureSensor(325, "compressor outlet pressure sensor"); // only actual; everything else 0.0

Pump centralHeatingPump(            381, "central heating pump");
Pump domesticHotWaterShiftLoadPump(1290, "domestic hot water shift load pump");

CanInt8 electronicExpansionValveOneTargetPositionPercent( 388, "electronic expansion valve one target position percent");
CanInt8 electronicExpansionValveOneCurrentPositionPercent(389, "electronic expansion valve one current position percent");
CanInt8 electronicExpansionValveTwoTargetPositionPercent( 390, "electronic expansion valve two target position percent");
CanInt8 electronicExpansionValveTwoCurrentPositionPercent(391, "electronic expansion valve two current position percent");
CanInt8 legionellaProtectionWeekday(                      876, "legionella protection weekday");
CanInt8 legionellaProtectionLastSuccessfulWeekday(        878, "legionella protection last successful weekday");
CanInt8 centralHeatingPumpMode(                          1240, "central heating pump mode");
CanInt8 solarRechargeSuppressionImpact(                  1529, "solar recharge suppression impact");
CanInt8 refrigerantCircuitFourWayValve(                  1773, "refrigerant circuit four way valve");
CanInt8 compressorCrankCaseHeater(                       1774, "compressor crank case heater");
CanInt8 primaryCircuitFanOne(                            1775, "primary circuit fan one");
CanInt8 primaryCircuitFanTwo(                            1776, "primary circuit fan two");
CanInt8 solarPumpConfigurationSelection(                 1819, "solar pump configuration selection");
CanInt8 refrigerationCircuitStatus(                      2259, "refrigeration circuit status");
CanInt8 domesticHotWaterStatus(                          2320, "domestic hot water status");
CanInt8 compressorSetpointPercent(                       2345, "compressor setpoint percent");
CanInt8 compressorSpeedPercent(                          2346, "compressor speed percent");
CanInt8 energyManagmentSystemResultingControlState(      2350, "energy managment system resulting control state");
CanInt8 centralHeatingPumpType(                          2498, "central heating pump type");
CanInt8 heatPumpState(                                   2557, "heat pump state");
CanInt8 noiseReductionMode(                              2634, "noise reduction mode"); // 22:00 <-> 06:00 == 1 
CanInt8 domesticHotWaterEfficiencyMode(                  3029, "domestic hot water efficiency mode");
CanInt8 domesticHotWaterSensorForDemand(                 3069, "domestic hot water sensor for demand");
CanInt8 bufferTargetOperationMode(                       3070, "buffer target operation mode");

CanInt16 domesticHotWaterPumpPostRunTime(            534, "domestic hot water pump post run time");
CanInt16 desiredThermalEnergyDefrost(               2256, "desired thermal energy defrost"); // "Wh"
CanInt16 electricalHeaterConfiguration(             2340, "electrical heater configuration");
CanInt16 compressorSpeedRps(                        2569, "compressor speed");
CanInt16 compressorOnTimer(                         2573, "compressor on timer");
CanInt16 refrigerationCycleApplicationState(        2575, "refrigeration cycle application state");
CanInt16 compressorSetpointRps(                     2580, "compressor setpoint");
CanInt16 compressorOffTimer(                        2582, "compressor off timer");
CanInt16 digitalOutputCooling(                      2586, "digital output cooling");
CanInt16 secondaryHeatExchangerMinimumVolumeFlow(   2843, "secondary heat exchanger minimum volume flow");
CanInt16 domesticHotWaterEfficiencyModeAvailability(3030, "domestic hot water efficiency mode availability");
CanInt16 primaryEnergyFactorElectricity(            3032, "primary energy factor electricity");

CanInt32 coefficientOfPerformanceConfiguration(              2341, "coefficient of performance configuration");
CanInt32 nominalThermalCapacityHeating(                      2342, "nominal thermal capacity heating");                         // "W"
CanInt32 nominalThermalCapacityCooling(                      2343, "nominal thermal capacity cooling");                         // "W"
CanInt32 currentElectricalPowerConsumptionRefrigerantCircuit(2486, "current electrical power consumption refrigerant circuit"); // "W"
CanInt32 currentElectricalPowerConsumptionElectricHeater(    2487, "current electrical power consumption electric heater");     // "W"
CanInt32 currentElectricalPowerConsumptionSystem(            2488, "current electrical power consumption system");              // "W"
CanInt32 currentThermalCapacityRefrigerantCircuit(           2494, "current thermal capacity refrigerant circuit");             // "W"
CanInt32 currentThermalCapacityElectricHeater(               2495, "current thermal capacity electric heater");                 // "W"
CanInt32 currentThermalCapacitySystem(                       2496, "current thermal capacity system");                          // "W"
CanInt32 nominalThermalCapacityIndoorUnit(                   2611, "nominal thermal capacity indoor unit");                     // "W"
CanInt32 maximumPowerElectricalHeater(                       2626, "maximum power electrical heater");                          // "W"
CanInt32 desiredThermalCapacity(                             2629, "desired thermal capacity");
CanInt32 currentElectricalSystemPowerSetpoint(               2934, "current electrical system power setpoint");
CanInt32 domesticHotWaterHighDemandDetection(                3066, "domestic hot water high demand detection");

TemperatureSetpointMetaDataWithEfficiency domesticHotWaterSetpointMetaData(504, "domestic hot water setpoint meta data"); // 14 byte

ModeAndState domesticHotWaterOperationState(531, "domestic hot water operation state");
ModeAndState legionellaProtectionActivation(873, "legionella protection activation");

EnergyConsumption energyConsumptionCentralHeating(                 548, "energy consumption central heating");
EnergyConsumption energyConsumptionDomesticHotWater(               565, "energy consumption domestic hot water");
EnergyConsumption energyConsumptionCooling(                        566, "energy consumption cooling");
EnergyConsumption generatedCentralHeatingOutput(                  1211, "generated central heating output");
EnergyConsumption generatedDomesticHotWaterOutput(                1391, "generated domestic hot water output");
EnergyConsumption energyConsumptionDomesticHotWaterElectricHeater(2517, "energy consumption domestic hot water electric heater");
EnergyConsumption energyConsumptionCentralHeatingElectricHeater(  2526, "energy consumption central heating electric heater");
EnergyConsumption generatedCoolingOutput(                         2529, "generated cooling output");

Utf8String centralHeatingOneCircuitName(627, "central heating one circuit name");

CanTime legionellaProtectionStartTime(875, "legionella protection start time");

CanTimeWithSecond legionellaProtectionLastSuccessfulStartTime(877, "legionella protection last successful start time");

FlowSensor allengraSensor(1043, "Allengra sensor");

Hysteresis domesticHotWaterHysteresis(1085, "domestic hot water hysteresis");
Hysteresis bufferHysteresis(          1593, "buffer hysteresis");

PressureConfiguration waterPressureConfiguration(1721, "water pressure configuration");

PowerAndError heatPumpCompressor(      2351, "heat pump compressor");
PowerAndError additionalElectricHeater(2352, "additional electric heater");

HeatPumpCompressor heatPumpCompressorStatistical(2369, "heat pump compressor statistical");

AdditionalElectricHeater additionalElectricHeaterStatistical(2370, "additional electric heater statistical");

SeasonalCoefficient seasonalCoefficientOfPerformanceHeating(                   2622, "seasonal coefficient of performance heating");
SeasonalCoefficient seasonalEnergyEfficiencyRatioCooling(                      2623, "seasonal energy efficiency ratio cooling");
SeasonalCoefficient seasonalCoefficientOfPerformanceDomesticHotWater(          2624, "seasonal coefficient of performance domestic hot water");
SeasonalCoefficient seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater(2625, "seasonal coefficient of performance heating and domestic hot water");

ThermalPower thermalPower(1190, "thermal power");

NominalPower nominalPowerElectricalHeater(2574, "nominal power electrical heater");

TemperatureOffsets smartGridTemperatureOffsets(2543, "Smart Grid temperature offsets");

PumpStatus centralHeatingPumpStatus(2791, "central heating pump status");

SetTemperature bufferMinimumMaximumSetTemperature(3106, "buffer minimum maximum set temperature");

// {680, "EnergyMeter", "RawCodec", 123, "-", "ro"},
// {1814, "ReceiverLiquidLevelSensor", "RawCodec", 5, "-", "ro"},
// {1846, "HeatPumpCompressorCurrentOperatingPoint", "RawCodec", 4, "-", "ro"},
// {2484, "ElectricalPowerRangeMetaData", "RawCodec", 8, "-", "ro"},
// {2540, "NoiseReductionSettings", "RawCodec", 6, "-", "ro"},
// {2555, "ElectricalPowerSetPoint", "RawCodec", 4, "-", "rw"},
// {2556, "ElectricalEnergyRemainingCapacity", "RawCodec", 4, "-", "ro"},
