EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 6 10
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Device:R R?
U 1 1 627B13BC
P 1500 3650
AR Path="/62665296/627B13BC" Ref="R?"  Part="1" 
AR Path="/6479D118/627B13BC" Ref="R?"  Part="1" 
AR Path="/627AE565/627B13BC" Ref="R51"  Part="1" 
F 0 "R51" H 1570 3696 50  0000 L CNN
F 1 "1M" H 1570 3605 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 1430 3650 50  0001 C CNN
F 3 "~" H 1500 3650 50  0001 C CNN
	1    1500 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 627B13C2
P 2050 3950
AR Path="/62665296/627B13C2" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/627B13C2" Ref="#PWR?"  Part="1" 
AR Path="/627AE565/627B13C2" Ref="#PWR098"  Part="1" 
F 0 "#PWR098" H 2050 3700 50  0001 C CNN
F 1 "GND" H 2055 3777 50  0000 C CNN
F 2 "" H 2050 3950 50  0001 C CNN
F 3 "" H 2050 3950 50  0001 C CNN
	1    2050 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3800 1500 3950
Wire Wire Line
	1500 3950 2050 3950
$Comp
L Device:R R?
U 1 1 627B13CA
P 7100 2500
AR Path="/62665296/627B13CA" Ref="R?"  Part="1" 
AR Path="/6479D118/627B13CA" Ref="R?"  Part="1" 
AR Path="/627AE565/627B13CA" Ref="R53"  Part="1" 
F 0 "R53" H 7170 2546 50  0000 L CNN
F 1 "22k" H 7170 2455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 7030 2500 50  0001 C CNN
F 3 "~" H 7100 2500 50  0001 C CNN
	1    7100 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 3500 1500 3200
Wire Wire Line
	1500 3200 1750 3200
Connection ~ 2050 3950
Wire Wire Line
	2050 3000 2050 2900
Wire Wire Line
	7100 2800 7400 2800
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 627B13DF
P 1950 3200
AR Path="/62665296/627B13DF" Ref="Q?"  Part="1" 
AR Path="/6479D118/627B13DF" Ref="Q?"  Part="1" 
AR Path="/627AE565/627B13DF" Ref="Q16"  Part="1" 
F 0 "Q16" H 2154 3246 50  0000 L CNN
F 1 "2N7000" H 2154 3155 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 2150 3125 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 1950 3200 50  0001 L CNN
	1    1950 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 627B13E5
P 1100 3650
AR Path="/62665296/627B13E5" Ref="C?"  Part="1" 
AR Path="/6479D118/627B13E5" Ref="C?"  Part="1" 
AR Path="/627AE565/627B13E5" Ref="C24"  Part="1" 
F 0 "C24" H 1215 3696 50  0000 L CNN
F 1 "22nF" H 1215 3605 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 1138 3500 50  0001 C CNN
F 3 "~" H 1100 3650 50  0001 C CNN
	1    1100 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 3500 1100 3200
Wire Wire Line
	1100 3200 1500 3200
Connection ~ 1500 3200
Wire Wire Line
	1100 3800 1100 3950
Wire Wire Line
	1100 3950 1500 3950
Connection ~ 1500 3950
Text HLabel 1100 3200 0    50   Input ~ 0
AlarmResetIn
Text HLabel 7400 2800 2    50   Input ~ 0
AlarmResetTTL
Text HLabel 2650 6500 0    50   Input ~ 0
KitchenIn
Text HLabel 10400 3400 2    50   Input ~ 0
KitchenTTL
Text HLabel 2150 5400 0    50   Input ~ 0
BathroomIn
Text HLabel 9450 3200 2    50   Input ~ 0
BathroomTTL
$Comp
L Device:R R?
U 1 1 627BA46D
P 2550 5850
AR Path="/62665296/627BA46D" Ref="R?"  Part="1" 
AR Path="/6479D118/627BA46D" Ref="R?"  Part="1" 
AR Path="/627AE565/627BA46D" Ref="R56"  Part="1" 
F 0 "R56" H 2620 5896 50  0000 L CNN
F 1 "1M" H 2620 5805 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 2480 5850 50  0001 C CNN
F 3 "~" H 2550 5850 50  0001 C CNN
	1    2550 5850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 627BA473
P 3100 6150
AR Path="/62665296/627BA473" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/627BA473" Ref="#PWR?"  Part="1" 
AR Path="/627AE565/627BA473" Ref="#PWR0104"  Part="1" 
F 0 "#PWR0104" H 3100 5900 50  0001 C CNN
F 1 "GND" H 3105 5977 50  0000 C CNN
F 2 "" H 3100 6150 50  0001 C CNN
F 3 "" H 3100 6150 50  0001 C CNN
	1    3100 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 6000 2550 6150
Wire Wire Line
	2550 6150 3100 6150
$Comp
L Device:R R?
U 1 1 627BA47B
P 9150 2900
AR Path="/62665296/627BA47B" Ref="R?"  Part="1" 
AR Path="/6479D118/627BA47B" Ref="R?"  Part="1" 
AR Path="/627AE565/627BA47B" Ref="R58"  Part="1" 
F 0 "R58" H 9220 2946 50  0000 L CNN
F 1 "22k" H 9220 2855 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 9080 2900 50  0001 C CNN
F 3 "~" H 9150 2900 50  0001 C CNN
	1    9150 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 5700 2550 5400
Wire Wire Line
	2550 5400 2800 5400
Connection ~ 3100 6150
Wire Wire Line
	9150 3200 9450 3200
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 627BA490
P 3000 5400
AR Path="/62665296/627BA490" Ref="Q?"  Part="1" 
AR Path="/6479D118/627BA490" Ref="Q?"  Part="1" 
AR Path="/627AE565/627BA490" Ref="Q19"  Part="1" 
F 0 "Q19" H 3204 5446 50  0000 L CNN
F 1 "2N7000" H 3204 5355 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3200 5325 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 3000 5400 50  0001 L CNN
	1    3000 5400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 627BA496
P 2150 5850
AR Path="/62665296/627BA496" Ref="C?"  Part="1" 
AR Path="/6479D118/627BA496" Ref="C?"  Part="1" 
AR Path="/627AE565/627BA496" Ref="C27"  Part="1" 
F 0 "C27" H 2265 5896 50  0000 L CNN
F 1 "22nF" H 2265 5805 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 2188 5700 50  0001 C CNN
F 3 "~" H 2150 5850 50  0001 C CNN
	1    2150 5850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5400 2550 5400
Connection ~ 2550 5400
Wire Wire Line
	2150 6150 2550 6150
Connection ~ 2550 6150
Wire Wire Line
	2150 6150 2150 6000
Wire Wire Line
	2150 5700 2150 5400
$Comp
L Device:R R?
U 1 1 627C28FF
P 3050 6950
AR Path="/62665296/627C28FF" Ref="R?"  Part="1" 
AR Path="/6479D118/627C28FF" Ref="R?"  Part="1" 
AR Path="/627AE565/627C28FF" Ref="R52"  Part="1" 
F 0 "R52" H 3120 6996 50  0000 L CNN
F 1 "1M" H 3120 6905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 2980 6950 50  0001 C CNN
F 3 "~" H 3050 6950 50  0001 C CNN
	1    3050 6950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 627C2905
P 3600 7250
AR Path="/62665296/627C2905" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/627C2905" Ref="#PWR?"  Part="1" 
AR Path="/627AE565/627C2905" Ref="#PWR0100"  Part="1" 
F 0 "#PWR0100" H 3600 7000 50  0001 C CNN
F 1 "GND" H 3605 7077 50  0000 C CNN
F 2 "" H 3600 7250 50  0001 C CNN
F 3 "" H 3600 7250 50  0001 C CNN
	1    3600 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 7100 3050 7250
Wire Wire Line
	3050 7250 3600 7250
$Comp
L Device:R R?
U 1 1 627C290D
P 10100 3100
AR Path="/62665296/627C290D" Ref="R?"  Part="1" 
AR Path="/6479D118/627C290D" Ref="R?"  Part="1" 
AR Path="/627AE565/627C290D" Ref="R54"  Part="1" 
F 0 "R54" H 10170 3146 50  0000 L CNN
F 1 "22k" H 10170 3055 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 10030 3100 50  0001 C CNN
F 3 "~" H 10100 3100 50  0001 C CNN
	1    10100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 6800 3050 6500
Wire Wire Line
	3050 6500 3300 6500
Wire Wire Line
	10100 3400 10100 3250
Wire Wire Line
	10100 3400 10400 3400
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 627C2922
P 3500 6500
AR Path="/62665296/627C2922" Ref="Q?"  Part="1" 
AR Path="/6479D118/627C2922" Ref="Q?"  Part="1" 
AR Path="/627AE565/627C2922" Ref="Q17"  Part="1" 
F 0 "Q17" H 3704 6546 50  0000 L CNN
F 1 "2N7000" H 3704 6455 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3700 6425 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 3500 6500 50  0001 L CNN
	1    3500 6500
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 627C2928
P 2650 6950
AR Path="/62665296/627C2928" Ref="C?"  Part="1" 
AR Path="/6479D118/627C2928" Ref="C?"  Part="1" 
AR Path="/627AE565/627C2928" Ref="C25"  Part="1" 
F 0 "C25" H 2765 6996 50  0000 L CNN
F 1 "22nF" H 2765 6905 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 2688 6800 50  0001 C CNN
F 3 "~" H 2650 6950 50  0001 C CNN
	1    2650 6950
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 6500 3050 6500
Connection ~ 3050 6500
Wire Wire Line
	2650 7250 3050 7250
Connection ~ 3050 7250
Wire Wire Line
	2650 7250 2650 7100
Wire Wire Line
	2650 6800 2650 6500
Text HLabel 1650 4300 0    50   Input ~ 0
SpareInterruptIn
$Comp
L Device:R R?
U 1 1 618F12B3
P 2050 4750
AR Path="/62665296/618F12B3" Ref="R?"  Part="1" 
AR Path="/6479D118/618F12B3" Ref="R?"  Part="1" 
AR Path="/627AE565/618F12B3" Ref="R55"  Part="1" 
F 0 "R55" H 2120 4796 50  0000 L CNN
F 1 "1M" H 2120 4705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 1980 4750 50  0001 C CNN
F 3 "~" H 2050 4750 50  0001 C CNN
	1    2050 4750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 618F12B9
P 2600 5050
AR Path="/62665296/618F12B9" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/618F12B9" Ref="#PWR?"  Part="1" 
AR Path="/627AE565/618F12B9" Ref="#PWR0102"  Part="1" 
F 0 "#PWR0102" H 2600 4800 50  0001 C CNN
F 1 "GND" H 2605 4877 50  0000 C CNN
F 2 "" H 2600 5050 50  0001 C CNN
F 3 "" H 2600 5050 50  0001 C CNN
	1    2600 5050
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 4900 2050 5050
Wire Wire Line
	2050 5050 2600 5050
Wire Wire Line
	2050 4600 2050 4300
Wire Wire Line
	2050 4300 2300 4300
Connection ~ 2600 5050
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 618F12D6
P 2500 4300
AR Path="/62665296/618F12D6" Ref="Q?"  Part="1" 
AR Path="/6479D118/618F12D6" Ref="Q?"  Part="1" 
AR Path="/627AE565/618F12D6" Ref="Q18"  Part="1" 
F 0 "Q18" H 2704 4346 50  0000 L CNN
F 1 "2N7000" H 2704 4255 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 2700 4225 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 2500 4300 50  0001 L CNN
	1    2500 4300
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 618F12DC
P 1650 4750
AR Path="/62665296/618F12DC" Ref="C?"  Part="1" 
AR Path="/6479D118/618F12DC" Ref="C?"  Part="1" 
AR Path="/627AE565/618F12DC" Ref="C26"  Part="1" 
F 0 "C26" H 1765 4796 50  0000 L CNN
F 1 "22nF" H 1765 4705 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D4.7mm_W2.5mm_P5.00mm" H 1688 4600 50  0001 C CNN
F 3 "~" H 1650 4750 50  0001 C CNN
	1    1650 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 4300 2050 4300
Connection ~ 2050 4300
Wire Wire Line
	1650 5050 2050 5050
Connection ~ 2050 5050
Wire Wire Line
	1650 5050 1650 4900
Wire Wire Line
	1650 4600 1650 4300
Wire Wire Line
	2050 3400 2050 3950
Wire Wire Line
	3600 6700 3600 7250
Connection ~ 3600 7250
Wire Wire Line
	3100 5600 3100 6150
Wire Wire Line
	2600 4500 2600 5050
Wire Wire Line
	9150 2650 9150 2750
Wire Wire Line
	7100 2250 7100 2350
Wire Wire Line
	10100 2850 10100 2950
Wire Wire Line
	7100 2800 7100 2650
Wire Wire Line
	9150 3200 9150 3050
$Comp
L power:+5V #PWR0123
U 1 1 61C8C410
P 9150 2650
F 0 "#PWR0123" H 9150 2500 50  0001 C CNN
F 1 "+5V" H 9165 2823 50  0000 C CNN
F 2 "" H 9150 2650 50  0001 C CNN
F 3 "" H 9150 2650 50  0001 C CNN
	1    9150 2650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0117
U 1 1 61C8D1D9
P 10100 2850
F 0 "#PWR0117" H 10100 2700 50  0001 C CNN
F 1 "+5V" H 10115 3023 50  0000 C CNN
F 2 "" H 10100 2850 50  0001 C CNN
F 3 "" H 10100 2850 50  0001 C CNN
	1    10100 2850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0115
U 1 1 61C8DF19
P 7100 2250
F 0 "#PWR0115" H 7100 2100 50  0001 C CNN
F 1 "+5V" H 7115 2423 50  0000 C CNN
F 2 "" H 7100 2250 50  0001 C CNN
F 3 "" H 7100 2250 50  0001 C CNN
	1    7100 2250
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0153
U 1 1 61CA1CD2
P 3850 2000
F 0 "#PWR0153" H 3850 1850 50  0001 C CNN
F 1 "+5V" H 3865 2173 50  0000 C CNN
F 2 "" H 3850 2000 50  0001 C CNN
F 3 "" H 3850 2000 50  0001 C CNN
	1    3850 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 2000 3850 2100
$Comp
L Device:R R?
U 1 1 61CA1CCB
P 3850 2250
AR Path="/62665296/61CA1CCB" Ref="R?"  Part="1" 
AR Path="/6479D118/61CA1CCB" Ref="R?"  Part="1" 
AR Path="/627AE565/61CA1CCB" Ref="R121"  Part="1" 
F 0 "R121" H 3920 2296 50  0000 L CNN
F 1 "330" H 3920 2205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 3780 2250 50  0001 C CNN
F 3 "~" H 3850 2250 50  0001 C CNN
	1    3850 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61CA47C5
P 5250 2250
AR Path="/62665296/61CA47C5" Ref="R?"  Part="1" 
AR Path="/6479D118/61CA47C5" Ref="R?"  Part="1" 
AR Path="/627AE565/61CA47C5" Ref="R14"  Part="1" 
F 0 "R14" H 5320 2296 50  0000 L CNN
F 1 "330" H 5320 2205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 5180 2250 50  0001 C CNN
F 3 "~" H 5250 2250 50  0001 C CNN
	1    5250 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 2000 5250 2100
$Comp
L power:+5V #PWR0111
U 1 1 61CA47CC
P 5250 2000
F 0 "#PWR0111" H 5250 1850 50  0001 C CNN
F 1 "+5V" H 5265 2173 50  0000 C CNN
F 2 "" H 5250 2000 50  0001 C CNN
F 3 "" H 5250 2000 50  0001 C CNN
	1    5250 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61CA664F
P 4750 2250
AR Path="/62665296/61CA664F" Ref="R?"  Part="1" 
AR Path="/6479D118/61CA664F" Ref="R?"  Part="1" 
AR Path="/627AE565/61CA664F" Ref="R117"  Part="1" 
F 0 "R117" H 4820 2296 50  0000 L CNN
F 1 "330" H 4820 2205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 4680 2250 50  0001 C CNN
F 3 "~" H 4750 2250 50  0001 C CNN
	1    4750 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 2000 4750 2100
$Comp
L power:+5V #PWR0113
U 1 1 61CA6656
P 4750 2000
F 0 "#PWR0113" H 4750 1850 50  0001 C CNN
F 1 "+5V" H 4765 2173 50  0000 C CNN
F 2 "" H 4750 2000 50  0001 C CNN
F 3 "" H 4750 2000 50  0001 C CNN
	1    4750 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 61CA8277
P 4300 2250
AR Path="/62665296/61CA8277" Ref="R?"  Part="1" 
AR Path="/6479D118/61CA8277" Ref="R?"  Part="1" 
AR Path="/627AE565/61CA8277" Ref="R120"  Part="1" 
F 0 "R120" H 4370 2296 50  0000 L CNN
F 1 "330" H 4370 2205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 4230 2250 50  0001 C CNN
F 3 "~" H 4300 2250 50  0001 C CNN
	1    4300 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2000 4300 2100
$Comp
L power:+5V #PWR0119
U 1 1 61CA827E
P 4300 2000
F 0 "#PWR0119" H 4300 1850 50  0001 C CNN
F 1 "+5V" H 4315 2173 50  0000 C CNN
F 2 "" H 4300 2000 50  0001 C CNN
F 3 "" H 4300 2000 50  0001 C CNN
	1    4300 2000
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:LTV-847 Q60
U 1 1 61CC4462
P 5400 2800
F 0 "Q60" H 6250 3065 50  0000 C CNN
F 1 "LTV-847" H 6250 2974 50  0000 C CNN
F 2 "DIP762W60P254L1984H450Q16N" H 6950 2900 50  0001 L CNN
F 3 "https://datasheet.datasheetarchive.com/originals/distributors/Datasheets-305/79782.pdf" H 6950 2800 50  0001 L CNN
F 4 "PTH 16PDIP DC Input Transistor" H 6950 2700 50  0001 L CNN "Description"
F 5 "4.5" H 6950 2600 50  0001 L CNN "Height"
F 6 "859-LTV-847" H 6950 2500 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Lite-On/LTV-847?qs=HMpxB34gN8NSIrLkafiZAg%3D%3D" H 6950 2400 50  0001 L CNN "Mouser Price/Stock"
F 8 "Lite-On" H 6950 2300 50  0001 L CNN "Manufacturer_Name"
F 9 "LTV-847" H 6950 2200 50  0001 L CNN "Manufacturer_Part_Number"
	1    5400 2800
	1    0    0    -1  
$EndComp
Connection ~ 7100 2800
Wire Wire Line
	7100 2900 7250 2900
Wire Wire Line
	7250 2900 7250 3100
Wire Wire Line
	7250 3500 7100 3500
Wire Wire Line
	7100 3300 7250 3300
Connection ~ 7250 3300
Wire Wire Line
	7250 3300 7250 3500
Wire Wire Line
	7100 3100 7250 3100
Connection ~ 7250 3100
Wire Wire Line
	7250 3100 7250 3300
$Comp
L power:+5V #PWR0121
U 1 1 61C8B4E8
P 8050 2450
F 0 "#PWR0121" H 8050 2300 50  0001 C CNN
F 1 "+5V" H 8065 2623 50  0000 C CNN
F 2 "" H 8050 2450 50  0001 C CNN
F 3 "" H 8050 2450 50  0001 C CNN
	1    8050 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 2450 8050 2550
$Comp
L Device:R R?
U 1 1 618F12C1
P 8050 2700
AR Path="/62665296/618F12C1" Ref="R?"  Part="1" 
AR Path="/6479D118/618F12C1" Ref="R?"  Part="1" 
AR Path="/627AE565/618F12C1" Ref="R57"  Part="1" 
F 0 "R57" H 8120 2746 50  0000 L CNN
F 1 "22k" H 8120 2655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 7980 2700 50  0001 C CNN
F 3 "~" H 8050 2700 50  0001 C CNN
	1    8050 2700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8050 3000 8350 3000
Text HLabel 8350 3000 2    50   Input ~ 0
SpareInterruptTTL
Wire Wire Line
	8050 3000 8050 2850
Wire Wire Line
	7100 3000 8050 3000
Connection ~ 8050 3000
Wire Wire Line
	7100 3200 9150 3200
Connection ~ 9150 3200
Wire Wire Line
	10100 3400 7100 3400
Connection ~ 10100 3400
$Comp
L Diode:1N4007 D?
U 1 1 61CF45AE
P 2600 3450
AR Path="/6479D118/61CF45AE" Ref="D?"  Part="1" 
AR Path="/619922D7/61CF45AE" Ref="D?"  Part="1" 
AR Path="/627AE565/61CF45AE" Ref="D14"  Part="1" 
F 0 "D14" H 2600 3667 50  0000 C CNN
F 1 "1N4007" H 2600 3576 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 2600 3275 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 2600 3450 50  0001 C CNN
	1    2600 3450
	0    -1   -1   0   
$EndComp
$Comp
L Diode:1N4007 D?
U 1 1 61CF45BA
P 3600 3750
AR Path="/6479D118/61CF45BA" Ref="D?"  Part="1" 
AR Path="/619922D7/61CF45BA" Ref="D?"  Part="1" 
AR Path="/627AE565/61CF45BA" Ref="D16"  Part="1" 
F 0 "D16" H 3600 3967 50  0000 C CNN
F 1 "1N4007" H 3600 3876 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 3600 3575 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 3600 3750 50  0001 C CNN
	1    3600 3750
	0    -1   -1   0   
$EndComp
$Comp
L Diode:1N4007 D?
U 1 1 61CF45C0
P 2450 2900
AR Path="/6479D118/61CF45C0" Ref="D?"  Part="1" 
AR Path="/619922D7/61CF45C0" Ref="D?"  Part="1" 
AR Path="/627AE565/61CF45C0" Ref="D13"  Part="1" 
F 0 "D13" H 2450 3117 50  0000 C CNN
F 1 "1N4007" H 2450 3026 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 2450 2725 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 2450 2900 50  0001 C CNN
	1    2450 2900
	1    0    0    1   
$EndComp
Wire Wire Line
	3100 3400 3100 3300
Wire Wire Line
	3600 3600 3600 3500
Wire Wire Line
	5250 2800 5400 2800
Wire Wire Line
	4750 3000 5400 3000
Wire Wire Line
	4300 3200 5400 3200
Wire Wire Line
	3850 3400 5400 3400
Wire Wire Line
	5400 3100 2600 3100
Wire Wire Line
	3100 3300 5400 3300
Wire Wire Line
	5400 3500 3600 3500
$Comp
L power:GND #PWR?
U 1 1 61D587EB
P 7250 3600
AR Path="/62665296/61D587EB" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/61D587EB" Ref="#PWR?"  Part="1" 
AR Path="/627AE565/61D587EB" Ref="#PWR0164"  Part="1" 
F 0 "#PWR0164" H 7250 3350 50  0001 C CNN
F 1 "GND" H 7255 3427 50  0000 C CNN
F 2 "" H 7250 3600 50  0001 C CNN
F 3 "" H 7250 3600 50  0001 C CNN
	1    7250 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 3600 7250 3500
Connection ~ 7250 3500
Wire Wire Line
	5250 2800 5250 2400
Wire Wire Line
	4750 3000 4750 2400
Wire Wire Line
	4300 3200 4300 2400
Wire Wire Line
	3850 3400 3850 2400
$Comp
L Diode:1N4007 D?
U 1 1 61CF45B4
P 3100 3550
AR Path="/6479D118/61CF45B4" Ref="D?"  Part="1" 
AR Path="/619922D7/61CF45B4" Ref="D?"  Part="1" 
AR Path="/627AE565/61CF45B4" Ref="D15"  Part="1" 
F 0 "D15" H 3100 3767 50  0000 C CNN
F 1 "1N4007" H 3100 3676 50  0000 C CNN
F 2 "Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal" H 3100 3375 50  0001 C CNN
F 3 "http://www.vishay.com/docs/88503/1n4001.pdf" H 3100 3550 50  0001 C CNN
	1    3100 3550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 3100 2600 3300
Wire Wire Line
	2600 2900 5400 2900
Wire Wire Line
	2300 2900 2050 2900
Wire Wire Line
	2600 4100 2600 3600
Wire Wire Line
	3100 5200 3100 3700
Wire Wire Line
	3600 6300 3600 3900
$EndSCHEMATC
