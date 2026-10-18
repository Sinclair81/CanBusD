
SingleTemp domesticHotWaterTemperatureSetpoint(                 396, "DomesticHotWaterTemperatureSetpoint");
SingleTemp externalTargetFlowTemperatureSetpoint(               528, "ExternalTargetFlowTemperatureSetpoint");
SingleTemp solarChargingDomesticHotWaterSetpoint(              1394, "SolarChargingDomesticHotWaterSetpoint");
SingleTemp primaryCircuitCurrentTemperatureSetpoint(           1799, "PrimaryCircuitCurrentTemperatureSetpoint");
SingleTemp minimumSecondaryReturnTemperatureRefrigerantCircuit(2255, "MinimumSecondaryReturnTemperatureRefrigerantCircuit");
SingleTemp domesticHotWaterTemperatureSetpointOffset(          2257, "DomesticHotWaterTemperatureSetpointOffset");
SingleTemp bivalenceControlAlternativeTemperature(             2831, "BivalenceControlAlternativeTemperature");
SingleTemp domesticHotWaterTemperatureSetpointComfort(         3068, "DomesticHotWaterTemperatureSetpointComfort");

TempWithStatus legionellaProtectionTargetTemperatureSetpoint(874, "LegionellaProtectionTargetTemperatureSetpoint");
TempWithStatus endResultDomesticHotWaterTemperatureSetpoint(1659, "EndResultDomesticHotWaterTemperatureSetpoint");

MultiTempWithStatus flowTemperatureSensor(                          268, "FlowTemperatureSensor");
MultiTempWithStatus returnTemperatureSensor(                        269, "ReturnTemperatureSensor");
MultiTempWithStatus domesticHotWaterSensor(                         271, "DomesticHotWaterSensor");
MultiTempWithStatus outsideTemperatureSensor(                       274, "OutsideTemperatureSensor");
MultiTempWithStatus mixerOneCircuitFlowTemperatureSensor(           284, "MixerOneCircuitFlowTemperatureSensor");
MultiTempWithStatus primaryHeatExchangerLiquidTemperatureSensor(    320, "PrimaryHeatExchangerLiquidTemperatureSensor");    // only actual; everything else 0.0
MultiTempWithStatus compressorInletTemperatureSensor(               321, "CompressorInletTemperatureSensor");               // only actual; everything else 0.0
MultiTempWithStatus compressorOutletTemperatureSensor(              324, "CompressorOutletTemperatureSensor");              // only actual; everything else 0.0
MultiTempWithStatus secondaryHeatExchangerLiquidTemperatureSensor(  355, "SecondaryHeatExchangerLiquidTemperatureSensor");  // only actual; everything else 0.0
MultiTempWithStatus receiverTemperatureSensor(                     1768, "ReceiverTemperatureSensor");                      // nicht da
MultiTempWithStatus primaryInletTemperatureSensor(                 1769, "PrimaryInletTemperatureSensor");                  // only actual; everything else 0.0
MultiTempWithStatus secondaryOutletTemperatureSensor(              1770, "SecondaryOutletTemperatureSensor");               // only actual; everything else 0.0
MultiTempWithStatus engineRoomTemperatureSensor(                   1771, "EngineRoomTemperatureSensor");                    // only actual; everything else 0.0
MultiTempWithStatus compressorOilTemperatureSensor(                1772, "CompressorOilTemperatureSensor");                 // only actual; everything else 0.0
MultiTempWithStatus economizerLiquidTemperatureSensor(             2333, "EconomizerLiquidTemperatureSensor");              // only actual; everything else 0.0
MultiTempWithStatus evaporatorVaporTemperatureSensor(              2334, "EvaporatorVaporTemperatureSensor");               // only actual; everything else 0.0
MultiTempWithStatus calculatedOutsideTemperature(                  2457, "CalculatedOutsideTemperature");
MultiTempWithStatus secondaryHeatExchangerVaporPressureSensor(     2784, "SecondaryHeatExchangerVaporPressureSensor");      // everything 0.0
MultiTempWithStatus inverterInternalPowerModuleTemperature(        2808, "InverterInternalPowerModuleTemperature");         // only actual; everything else 0.0
MultiTempWithStatus outdoorMiddleCoilTemperatureSensor(            3014, "OutdoorMiddleCoilTemperatureSensor");             // everything 0.0
MultiTempWithStatus heatSinkTemperatureSensor(                     3015, "HeatSinkTemperatureSensor");                      // everything 0.0
MultiTempWithStatus heatingBufferTemperatureSensor(                3016, "HeatingBufferTemperatureSensor");                 // everything 0.0
MultiTempWithStatus coolingBufferTemperatureSensor(                3017, "CoolingBufferTemperatureSensor");                 // everything 0.0
MultiTempWithStatus heatingCoolingBufferTemperatureSensor(         3018, "HeatingCoolingBufferTemperatureSensor");          // everything 0.0
MultiTempWithStatus domesticHotWaterReturnTemperaturTankLoadSystem(3034, "DomesticHotWaterReturnTemperaturTankLoadSystem"); // everything 0.0
MultiTempWithStatus domesticHotWaterFlowTemperaturTankLoadSystem(  3035, "DomesticHotWaterFlowTemperaturTankLoadSystem");   // everything 0.0
MultiTempWithStatus domesticHotWaterBufferTopTemperatureSensor(    3234, "DomesticHotWaterBufferTopTemperatureSensor");

MultiPressureWithStatus waterPressureSensor(           318, "WaterPressureSensor");
MultiPressureWithStatus compressorInletPressureSensor( 322, "CompressorInletPressureSensor");  // only actual; everything else 0.0
MultiPressureWithStatus compressorOutletPressureSensor(325, "CompressorOutletPressureSensor"); // only actual; everything else 0.0

Pump centralHeatingPump(            381, "CentralHeatingPump");
Pump domesticHotWaterShiftLoadPump(1290, "DomesticHotWaterShiftLoadPump");

CanInt8 electronicExpansionValveOneTargetPositionPercent( 388, "ElectronicExpansionValveOneTargetPositionPercent");
CanInt8 electronicExpansionValveOneCurrentPositionPercent(389, "ElectronicExpansionValveOneCurrentPositionPercent");
CanInt8 electronicExpansionValveTwoTargetPositionPercent( 390, "ElectronicExpansionValveTwoTargetPositionPercent");
CanInt8 electronicExpansionValveTwoCurrentPositionPercent(391, "ElectronicExpansionValveTwoCurrentPositionPercent");
CanInt8 legionellaProtectionWeekday(                      876, "LegionellaProtectionWeekday");
CanInt8 legionellaProtectionLastSuccessfulWeekday(        878, "LegionellaProtectionLastSuccessfulWeekday");
CanInt8 centralHeatingPumpMode(                          1240, "CentralHeatingPumpMode");
CanInt8 solarRechargeSuppressionImpact(                  1529, "SolarRechargeSuppressionImpact");
CanInt8 refrigerantCircuitFourWayValve(                  1773, "RefrigerantCircuitFourWayValve");
CanInt8 compressorCrankCaseHeater(                       1774, "CompressorCrankCaseHeater");
CanInt8 primaryCircuitFanOne(                            1775, "PrimaryCircuitFanOne");
CanInt8 primaryCircuitFanTwo(                            1776, "PrimaryCircuitFanTwo");
CanInt8 solarPumpConfigurationSelection(                 1819, "SolarPumpConfigurationSelection");
CanInt8 refrigerationCircuitStatus(                      2259, "RefrigerationCircuitStatus");
CanInt8 domesticHotWaterStatus(                          2320, "DomesticHotWaterStatus");
CanInt8 compressorSetpointPercent(                       2345, "CompressorSetpointPercent");
CanInt8 compressorSpeedPercent(                          2346, "CompressorSpeedPercent");
CanInt8 energyManagmentSystemResultingControlState(      2350, "EnergyManagmentSystemResultingControlState");
CanInt8 centralHeatingPumpType(                          2498, "CentralHeatingPumpType");
CanInt8 heatPumpState(                                   2557, "HeatPumpState");
CanInt8 noiseReductionMode(                              2634, "NoiseReductionMode"); // 22:00 <-> 06:00 == 1 
CanInt8 domesticHotWaterEfficiencyMode(                  3029, "DomesticHotWaterEfficiencyMode");
CanInt8 domesticHotWaterSensorForDemand(                 3069, "DomesticHotWaterSensorForDemand");
CanInt8 bufferTargetOperationMode(                       3070, "BufferTargetOperationMode");

CanInt16 domesticHotWaterPumpPostRunTime(            534, "DomesticHotWaterPumpPostRunTime");
CanInt16 desiredThermalEnergyDefrost(               2256, "DesiredThermalEnergyDefrost"); // "Wh"
CanInt16 electricalHeaterConfiguration(             2340, "ElectricalHeaterConfiguration");
CanInt16 compressorSpeedRps(                        2569, "CompressorSpeedRps");
CanInt16 compressorOnTimer(                         2573, "CompressorOnTimer");
CanInt16 refrigerationCycleApplicationState(        2575, "RefrigerationCycleApplicationState");
CanInt16 compressorSetpointRps(                     2580, "CompressorSetpointRps");
CanInt16 compressorOffTimer(                        2582, "CompressorOffTimer");
CanInt16 digitalOutputCooling(                      2586, "DigitalOutputCooling");
CanInt16 secondaryHeatExchangerMinimumVolumeFlow(   2843, "SecondaryHeatExchangerMinimumVolumeFlow");
CanInt16 domesticHotWaterEfficiencyModeAvailability(3030, "DomesticHotWaterEfficiencyModeAvailability");
CanInt16 primaryEnergyFactorElectricity(            3032, "PrimaryEnergyFactorElectricity");

CanInt32 coefficientOfPerformanceConfiguration(              2341, "CoefficientOfPerformanceConfiguration");
CanInt32 nominalThermalCapacityHeating(                      2342, "NominalThermalCapacityHeating");                       // "W"
CanInt32 nominalThermalCapacityCooling(                      2343, "NominalThermalCapacityCooling");                       // "W"
CanInt32 currentElectricalPowerConsumptionRefrigerantCircuit(2486, "CurrentElectricalPowerConsumptionRefrigerantCircuit"); // "W"
CanInt32 currentElectricalPowerConsumptionElectricHeater(    2487, "CurrentElectricalPowerConsumptionElectricHeater");     // "W"
CanInt32 currentElectricalPowerConsumptionSystem(            2488, "CurrentElectricalPowerConsumptionSystem");             // "W"
CanInt32 currentThermalCapacityRefrigerantCircuit(           2494, "CurrentThermalCapacityRefrigerantCircuit");            // "W"
CanInt32 currentThermalCapacityElectricHeater(               2495, "CurrentThermalCapacityElectricHeater");                // "W"
CanInt32 currentThermalCapacitySystem(                       2496, "CurrentThermalCapacitySystem");                        // "W"
CanInt32 nominalThermalCapacityIndoorUnit(                   2611, "NominalThermalCapacityIndoorUnit");                    // "W"
CanInt32 maximumPowerElectricalHeater(                       2626, "MaximumPowerElectricalHeater");                        // "W"
CanInt32 desiredThermalCapacity(                             2629, "DesiredThermalCapacity");
CanInt32 currentElectricalSystemPowerSetpoint(               2934, "CurrentElectricalSystemPowerSetpoint");
CanInt32 domesticHotWaterHighDemandDetection(                3066, "DomesticHotWaterHighDemandDetection");

TemperatureSetpointMetaDataWithEfficiency domesticHotWaterSetpointMetaData(504, "DomesticHotWaterSetpointMetaData"); // 14 byte

ModeAndState domesticHotWaterOperationState(531, "DomesticHotWaterOperationState");
ModeAndState legionellaProtectionActivation(873, "LegionellaProtectionActivation");

EnergyConsumption energyConsumptionCentralHeating(                 548, "EnergyConsumptionCentralHeating");
EnergyConsumption energyConsumptionDomesticHotWater(               565, "EnergyConsumptionDomesticHotWater");
EnergyConsumption energyConsumptionCooling(                        566, "EnergyConsumptionCooling");
EnergyConsumption generatedCentralHeatingOutput(                  1211, "GeneratedCentralHeatingOutput");
EnergyConsumption generatedDomesticHotWaterOutput(                1391, "GeneratedDomesticHotWaterOutput");
EnergyConsumption energyConsumptionDomesticHotWaterElectricHeater(2517, "EnergyConsumptionDomesticHotWaterElectricHeater");
EnergyConsumption energyConsumptionCentralHeatingElectricHeater(  2526, "EnergyConsumptionCentralHeatingElectricHeater");
EnergyConsumption generatedCoolingOutput(                         2529, "GeneratedCoolingOutput");

Utf8String centralHeatingOneCircuitName(627, "CentralHeatingOneCircuitName");

CanTime legionellaProtectionStartTime(875, "LegionellaProtectionStartTime");

CanTimeWithSecond legionellaProtectionLastSuccessfulStartTime(877, "LegionellaProtectionLastSuccessfulStartTime");

FlowSensor allengraSensor(1043, "AllengraSensor");

Hysteresis domesticHotWaterHysteresis(1085, "DomesticHotWaterHysteresis");
Hysteresis bufferHysteresis(          1593, "BufferHysteresis");

PressureConfiguration waterPressureConfiguration(1721, "WaterPressureConfiguration");

PowerAndError heatPumpCompressor(      2351, "HeatPumpCompressor");
PowerAndError additionalElectricHeater(2352, "AdditionalElectricHeater");

HeatPumpCompressor heatPumpCompressorStatistical(2369, "HeatPumpCompressorStatistical");

AdditionalElectricHeater additionalElectricHeaterStatistical(2370, "AdditionalElectricHeaterStatistical");

SeasonalCoefficient seasonalCoefficientOfPerformanceHeating(                   2622, "SeasonalCoefficientOfPerformanceHeating");
SeasonalCoefficient seasonalEnergyEfficiencyRatioCooling(                      2623, "SeasonalEnergyEfficiencyRatioCooling");
SeasonalCoefficient seasonalCoefficientOfPerformanceDomesticHotWater(          2624, "SeasonalCoefficientOfPerformanceDomesticHotWater");
SeasonalCoefficient seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater(2625, "SeasonalCoefficientOfPerformanceHeatingAndDomesticHotWater");

ThermalPower thermalPower(1190, "ThermalPower");

NominalPower nominalPowerElectricalHeater(2574, "NominalPowerElectricalHeater");

TemperatureOffsets smartGridTemperatureOffsets(2543, "SmartGridTemperatureOffsets");

PumpStatus centralHeatingPumpStatus(2791, "CentralHeatingPumpStatus");

SetTemperature bufferMinimumMaximumSetTemperature(3106, "BufferMinimumMaximumSetTemperature");

// {680, "EnergyMeter", "RawCodec", 123, "-", "ro"},
// {1814, "ReceiverLiquidLevelSensor", "RawCodec", 5, "-", "ro"},
// {1846, "HeatPumpCompressorCurrentOperatingPoint", "RawCodec", 4, "-", "ro"},
// {2484, "ElectricalPowerRangeMetaData", "RawCodec", 8, "-", "ro"},
// {2540, "NoiseReductionSettings", "RawCodec", 6, "-", "ro"},
// {2555, "ElectricalPowerSetPoint", "RawCodec", 4, "-", "rw"},
// {2556, "ElectricalEnergyRemainingCapacity", "RawCodec", 4, "-", "ro"},
