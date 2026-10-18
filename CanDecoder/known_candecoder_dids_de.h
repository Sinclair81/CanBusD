
SingleTemp domesticHotWaterTemperatureSetpoint(                 396, "Warmwasser Solltemperatur");
SingleTemp externalTargetFlowTemperatureSetpoint(               528, "externer Vorlauftemperatur Sollwert");
SingleTemp solarChargingDomesticHotWaterSetpoint(              1394, "Solarladung Warmwasser Sollwert");
SingleTemp primaryCircuitCurrentTemperatureSetpoint(           1799, "Primärkreis aktuelle Solltemperatur");
SingleTemp minimumSecondaryReturnTemperatureRefrigerantCircuit(2255, "minimale Sekundär Rücklauftemperatur Kältemittelkreis");
SingleTemp domesticHotWaterTemperatureSetpointOffset(          2257, "Warmwasser Solltemperatur Offset");
SingleTemp bivalenceControlAlternativeTemperature(             2831, "Bivalenzsteuerung alternative Temperatur");
SingleTemp domesticHotWaterTemperatureSetpointComfort(         3068, "Warmwasser Solltemperatur Komfort");

TempWithStatus legionellaProtectionTargetTemperatureSetpoint(874, "Legionellenschutz Zieltemperatur Sollwert");
TempWithStatus endResultDomesticHotWaterTemperatureSetpoint(1659, "Endergebnis Warmwasser Solltemperatur");

MultiTempWithStatus flowTemperatureSensor(                          268, "Vorlauftemperatursensor");
MultiTempWithStatus returnTemperatureSensor(                        269, "Rücklauftemperatursensor");
MultiTempWithStatus domesticHotWaterSensor(                         271, "Warmwassersensor");
MultiTempWithStatus outsideTemperatureSensor(                       274, "Außentemperatursensor");
MultiTempWithStatus mixerOneCircuitFlowTemperatureSensor(           284, "Mischerkreis eins Vorlauftemperatursensor");
MultiTempWithStatus primaryHeatExchangerLiquidTemperatureSensor(    320, "Primärwärmetauscher Flüssigkeitstemperatursensor");   // only actual; everything else 0.0
MultiTempWithStatus compressorInletTemperatureSensor(               321, "Kompressor Einlasstemperatursensor");                 // only actual; everything else 0.0
MultiTempWithStatus compressorOutletTemperatureSensor(              324, "Kompressor Auslasstemperatursensor");                 // only actual; everything else 0.0
MultiTempWithStatus secondaryHeatExchangerLiquidTemperatureSensor(  355, "Sekundärwärmetauscher Flüssigkeitstemperatursensor"); // only actual; everything else 0.0
MultiTempWithStatus receiverTemperatureSensor(                     1768, "Sammler Temperatursensor");                           // nicht da
MultiTempWithStatus primaryInletTemperatureSensor(                 1769, "Primäreinlass Temperatursensor");                     // only actual; everything else 0.0
MultiTempWithStatus secondaryOutletTemperatureSensor(              1770, "Sekundärauslass Temperatursensor");                   // only actual; everything else 0.0
MultiTempWithStatus engineRoomTemperatureSensor(                   1771, "Maschinenraum Temperatursensor");                     // only actual; everything else 0.0
MultiTempWithStatus compressorOilTemperatureSensor(                1772, "Kompressoröltemperatursensor");                       // only actual; everything else 0.0
MultiTempWithStatus economizerLiquidTemperatureSensor(             2333, "Economizer Flüssigkeitstemperatursensor");            // only actual; everything else 0.0
MultiTempWithStatus evaporatorVaporTemperatureSensor(              2334, "Verdampfer Dampftemperatursensor");                   // only actual; everything else 0.0
MultiTempWithStatus calculatedOutsideTemperature(                  2457, "berechnete Außentemperatur");
MultiTempWithStatus secondaryHeatExchangerVaporPressureSensor(     2784, "Sekundärwärmetauscher Dampfdrucksensor");             // everything 0.0
MultiTempWithStatus inverterInternalPowerModuleTemperature(        2808, "Inverter internes Leistungsmodul Temperatursensor");  // only actual; everything else 0.0
MultiTempWithStatus outdoorMiddleCoilTemperatureSensor(            3014, "Außenbereich mittlere Registertemperatursensor");     // everything 0.0
MultiTempWithStatus heatSinkTemperatureSensor(                     3015, "Kühlkörper Temperatursensor");                        // everything 0.0
MultiTempWithStatus heatingBufferTemperatureSensor(                3016, "Heizungspuffersensor");                               // everything 0.0
MultiTempWithStatus coolingBufferTemperatureSensor(                3017, "Kühlungspuffersensor");                               // everything 0.0
MultiTempWithStatus heatingCoolingBufferTemperatureSensor(         3018, "Heizung Kühlung Kombipuffersensor");                  // everything 0.0
MultiTempWithStatus domesticHotWaterReturnTemperaturTankLoadSystem(3034, "Warmwasser Rücklauftemperatur Speicherladesystem");   // everything 0.0
MultiTempWithStatus domesticHotWaterFlowTemperaturTankLoadSystem(  3035, "Warmwasser Vorlauftemperatur Speicherladesystem");    // everything 0.0
MultiTempWithStatus domesticHotWaterBufferTopTemperatureSensor(    3234, "Warmwasser Pufferspeicher Temperatursensor oben");

MultiPressureWithStatus waterPressureSensor(           318, "Wasserdrucksensor");
MultiPressureWithStatus compressorInletPressureSensor( 322, "Kompressor Einlassdrucksensor"); // only actual; everything else 0.0
MultiPressureWithStatus compressorOutletPressureSensor(325, "Kompressor Auslassdrucksensor"); // only actual; everything else 0.0

Pump centralHeatingPump(            381, "Heizungskreispumpe");
Pump domesticHotWaterShiftLoadPump(1290, "Warmwasser Schichtladepumpe");

CanInt8 electronicExpansionValveOneTargetPositionPercent( 388, "elektronisches Expansionsventil eins Zielposition",      "", "%");
CanInt8 electronicExpansionValveOneCurrentPositionPercent(389, "elektronisches Expansionsventil eins aktuelle Position", "", "%");
CanInt8 electronicExpansionValveTwoTargetPositionPercent( 390, "elektronisches Expansionsventil zwei Zielposition",      "", "%");
CanInt8 electronicExpansionValveTwoCurrentPositionPercent(391, "elektronisches Expansionsventil zwei aktuelle Position", "", "%");
CanInt8 legionellaProtectionWeekday(                      876, "Legionellenschutz Wochentag");
CanInt8 legionellaProtectionLastSuccessfulWeekday(        878, "Legionellenschutz letzter erfolgreicher Wochentag");
CanInt8 centralHeatingPumpMode(                          1240, "Heizungskreispumpe Modus");
CanInt8 solarRechargeSuppressionImpact(                  1529, "Solarunterdrückung Einfluss bei Nachladung");
CanInt8 refrigerantCircuitFourWayValve(                  1773, "Kältemittelkreis Vierwegeventil");
CanInt8 compressorCrankCaseHeater(                       1774, "Kompressor Kurbelwannenheizung");
CanInt8 primaryCircuitFanOne(                            1775, "Primärkreis Ventilator eins");
CanInt8 primaryCircuitFanTwo(                            1776, "Primärkreis Ventilator zwei");
CanInt8 solarPumpConfigurationSelection(                 1819, "Solarpumpen Konfiguration Auswahl");
CanInt8 refrigerationCircuitStatus(                      2259, "Kältemittelkreis Status");
CanInt8 domesticHotWaterStatus(                          2320, "Warmwasser Status");
CanInt8 compressorSetpointPercent(                       2345, "Kompressor Sollwert",                                    "", "%");
CanInt8 compressorSpeedPercent(                          2346, "Kompressor Drehzahl",                                    "", "%");
CanInt8 energyManagmentSystemResultingControlState(      2350, "Energiemanagementsystem resultierender Regelungsstatus");
CanInt8 centralHeatingPumpType(                          2498, "Heizungskreispumpe Typ");
CanInt8 heatPumpState(                                   2557, "Wärmepumpen Status");
CanInt8 noiseReductionMode(                              2634, "Flüsterbetrieb Modus");                                  // 22:00 <-> 06:00 == 1 
CanInt8 domesticHotWaterEfficiencyMode(                  3029, "Warmwasser Effizienzmodus");
CanInt8 domesticHotWaterSensorForDemand(                 3069, "Warmwassersensor für Anforderung");
CanInt8 bufferTargetOperationMode(                       3070, "Puffer Zielbetriebsmodus");

CanInt16 domesticHotWaterPumpPostRunTime(            534, "Warmwasserpumpe Nachlaufzeit");
CanInt16 desiredThermalEnergyDefrost(               2256, "gewünschte thermische Energie für Abtauung", "", "Wh");
CanInt16 electricalHeaterConfiguration(             2340, "elektrische Heizstab Konfiguration");
CanInt16 compressorSpeedRps(                        2569, "Kompressor Drehzahl",                        "", "rps");
CanInt16 compressorOnTimer(                         2573, "Kompressor Einschaltverzögerung");
CanInt16 refrigerationCycleApplicationState(        2575, "Kältekreislauf Anwendungsstatus");
CanInt16 compressorSetpointRps(                     2580, "Kompressor Sollwert",                        "", "rps");
CanInt16 compressorOffTimer(                        2582, "Kompressor Ausschaltverzögerung");
CanInt16 digitalOutputCooling(                      2586, "digitaler Ausgang Kühlung");
CanInt16 secondaryHeatExchangerMinimumVolumeFlow(   2843, "Sekundärwärmetauscher minimaler Volumenstrom");
CanInt16 domesticHotWaterEfficiencyModeAvailability(3030, "Warmwasser Effizienzmodus Verfügbarkeit");
CanInt16 primaryEnergyFactorElectricity(            3032, "Primärenergiefaktor Strom");

CanInt32 coefficientOfPerformanceConfiguration(              2341, "Leistungszahl Konfiguration");
CanInt32 nominalThermalCapacityHeating(                      2342, "Nennwärmeleistung Heizen",                                "", "W");
CanInt32 nominalThermalCapacityCooling(                      2343, "Nennkälteleistung Kühlen",                                "", "W");
CanInt32 currentElectricalPowerConsumptionRefrigerantCircuit(2486, "aktuelle elektrische Leistungsaufnahme Kältemittelkreis", "", "W");
CanInt32 currentElectricalPowerConsumptionElectricHeater(    2487, "aktuelle elektrische Leistungsaufnahme Heizstab",         "", "W");
CanInt32 currentElectricalPowerConsumptionSystem(            2488, "aktuelle elektrische Leistungsaufnahme System",           "", "W");
CanInt32 currentThermalCapacityRefrigerantCircuit(           2494, "aktuelle thermische Leistung Kältemittelkreis",           "", "W");
CanInt32 currentThermalCapacityElectricHeater(               2495, "aktuelle thermische Leistung Heizstab",                   "", "W");
CanInt32 currentThermalCapacitySystem(                       2496, "aktuelle thermische Leistung System",                     "", "W");
CanInt32 nominalThermalCapacityIndoorUnit(                   2611, "Nennwärmeleistung Inneneinheit",                          "", "W");
CanInt32 maximumPowerElectricalHeater(                       2626, "maximale Leistung elektrischer Heizstab",                 "", "W");
CanInt32 desiredThermalCapacity(                             2629, "gewünschte thermische Leistung");
CanInt32 currentElectricalSystemPowerSetpoint(               2934, "aktueller elektrischer Systemleistung Sollwert");
CanInt32 domesticHotWaterHighDemandDetection(                3066, "Warmwasser hoher Bedarf Erkennung");

TemperatureSetpointMetaDataWithEfficiency domesticHotWaterSetpointMetaData(504, "Warmwasser Sollwert Metadaten"); // 14 byte

ModeAndState domesticHotWaterOperationState(531, "Warmwasser Betriebsstatus");
ModeAndState legionellaProtectionActivation(873, "Legionellenschutz Aktivierung");

EnergyConsumption energyConsumptionCentralHeating(                 548, "Energieverbrauch Heizung");
EnergyConsumption energyConsumptionDomesticHotWater(               565, "Energieverbrauch Warmwasser");
EnergyConsumption energyConsumptionCooling(                        566, "Energieverbrauch Kühlung");
EnergyConsumption generatedCentralHeatingOutput(                  1211, "erzeugte Wärmemenge Heizung");
EnergyConsumption generatedDomesticHotWaterOutput(                1391, "erzeugte Wärmemenge Warmwasser");
EnergyConsumption energyConsumptionDomesticHotWaterElectricHeater(2517, "Energieverbrauch Warmwasser elektrischer Heizstab");
EnergyConsumption energyConsumptionCentralHeatingElectricHeater(  2526, "Energieverbrauch Heizung elektrischer Heizstab");
EnergyConsumption generatedCoolingOutput(                         2529, "erzeugte Kältemenge Kühlung");

Utf8String centralHeatingOneCircuitName(627, "Heizkreis eins Name");

CanTime legionellaProtectionStartTime(875, "Legionellenschutz Startzeit");

CanTimeWithSecond legionellaProtectionLastSuccessfulStartTime(877, "Legionellenschutz letzte erfolgreiche Startzeit");

FlowSensor allengraSensor(1043, "Allengra Sensor");

Hysteresis domesticHotWaterHysteresis(1085, "Warmwasser Hysterese");
Hysteresis bufferHysteresis(          1593, "Puffer Hysterese");

PressureConfiguration waterPressureConfiguration(1721, "Wasserdruck Konfiguration");

PowerAndError heatPumpCompressor(      2351, "Wärmepumpen Kompressor");
PowerAndError additionalElectricHeater(2352, "zusätzlicher elektrischer Heizstab");

HeatPumpCompressor heatPumpCompressorStatistical(2369, "Wärmepumpen Kompressor Statistik");

AdditionalElectricHeater additionalElectricHeaterStatistical(2370, "zusätzlicher elektrischer Heizstab Statistik");

SeasonalCoefficient seasonalCoefficientOfPerformanceHeating(                   2622, "Jahresarbeitszahl Heizen");
SeasonalCoefficient seasonalEnergyEfficiencyRatioCooling(                      2623, "Jahresarbeitszahl Kühlen");
SeasonalCoefficient seasonalCoefficientOfPerformanceDomesticHotWater(          2624, "Jahresarbeitszahl Warmwasser");
SeasonalCoefficient seasonalCoefficientOfPerformanceHeatingAndDomesticHotWater(2625, "Jahresarbeitszahl Heizen und Warmwasser");

ThermalPower thermalPower(1190, "thermische Leistung");

NominalPower nominalPowerElectricalHeater(2574, "Nennleistung elektrischer Heizstab");

TemperatureOffsets smartGridTemperatureOffsets(2543, "Smart Grid Temperatur Offsets");

PumpStatus centralHeatingPumpStatus(2791, "Heizungskreispumpe Status");

SetTemperature bufferMinimumMaximumSetTemperature(3106, "Puffer minimal maximale Solltemperatur");

// {680, "EnergyMeter", "RawCodec", 123, "-", "ro"},
// {1814, "ReceiverLiquidLevelSensor", "RawCodec", 5, "-", "ro"},
// {1846, "HeatPumpCompressorCurrentOperatingPoint", "RawCodec", 4, "-", "ro"},
// {2484, "ElectricalPowerRangeMetaData", "RawCodec", 8, "-", "ro"},
// {2540, "NoiseReductionSettings", "RawCodec", 6, "-", "ro"},
// {2555, "ElectricalPowerSetPoint", "RawCodec", 4, "-", "rw"},
// {2556, "ElectricalEnergyRemainingCapacity", "RawCodec", 4, "-", "ro"},
