EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 9 9
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
L SamacSys_Parts:IRF9Z34NPBF Q?
U 1 1 63F66344
P 2600 2200
AR Path="/62665296/63F66344" Ref="Q?"  Part="1" 
AR Path="/6479D118/63F66344" Ref="Q?"  Part="1" 
AR Path="/61B4978A/63F66344" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/63F66344" Ref="Q48"  Part="1" 
F 0 "Q48" H 3030 2346 50  0000 L CNN
F 1 "IRF9Z34NPBF" H 3030 2255 50  0000 L CNN
F 2 "SamacSys_Parts:TO254P469X1042X1967-3P" H 3050 2150 50  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irf9z34npbf.pdf?fileId=5546d462533600a40153561220af1ddd" H 3050 2050 50  0001 L CNN
F 4 "MOSFET MOSFT PCh -55V -17A 100mOhm 23.3nC" H 3050 1950 50  0001 L CNN "Description"
F 5 "4.69" H 3050 1850 50  0001 L CNN "Height"
F 6 "942-IRF9Z34NPBF" H 3050 1750 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Infineon-IR/IRF9Z34NPBF?qs=9%252BKlkBgLFf16a%2FvlD%252BMCtQ%3D%3D" H 3050 1650 50  0001 L CNN "Mouser Price/Stock"
F 8 "Infineon" H 3050 1550 50  0001 L CNN "Manufacturer_Name"
F 9 "IRF9Z34NPBF" H 3050 1450 50  0001 L CNN "Manufacturer_Part_Number"
	1    2600 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 63F6634A
P 1250 3500
AR Path="/62665296/63F6634A" Ref="R?"  Part="1" 
AR Path="/6479D118/63F6634A" Ref="R?"  Part="1" 
AR Path="/61B4978A/63F6634A" Ref="R?"  Part="1" 
AR Path="/63F3F92C/63F6634A" Ref="R100"  Part="1" 
F 0 "R100" H 1320 3546 50  0000 L CNN
F 1 "1M" H 1320 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 1180 3500 50  0001 C CNN
F 3 "~" H 1250 3500 50  0001 C CNN
	1    1250 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 63F66350
P 1800 3800
AR Path="/62665296/63F66350" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/63F66350" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/63F66350" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/63F66350" Ref="#PWR0134"  Part="1" 
F 0 "#PWR0134" H 1800 3550 50  0001 C CNN
F 1 "GND" H 1805 3627 50  0000 C CNN
F 2 "" H 1800 3800 50  0001 C CNN
F 3 "" H 1800 3800 50  0001 C CNN
	1    1800 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1250 3650 1250 3800
Wire Wire Line
	1250 3800 1800 3800
Wire Wire Line
	1250 3350 1250 3050
Wire Wire Line
	1250 3050 1500 3050
Connection ~ 1800 3800
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 63F6635B
P 1700 3050
AR Path="/62665296/63F6635B" Ref="Q?"  Part="1" 
AR Path="/6479D118/63F6635B" Ref="Q?"  Part="1" 
AR Path="/61B4978A/63F6635B" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/63F6635B" Ref="Q47"  Part="1" 
F 0 "Q47" H 1904 3096 50  0000 L CNN
F 1 "2N7000" H 1904 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 1900 2975 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 1700 3050 50  0001 L CNN
	1    1700 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1800 3250 1800 3800
$Comp
L Device:R R?
U 1 1 63F66362
P 1800 1950
AR Path="/62665296/63F66362" Ref="R?"  Part="1" 
AR Path="/6479D118/63F66362" Ref="R?"  Part="1" 
AR Path="/61B4978A/63F66362" Ref="R?"  Part="1" 
AR Path="/63F3F92C/63F66362" Ref="R101"  Part="1" 
F 0 "R101" H 1870 1996 50  0000 L CNN
F 1 "100k" H 1870 1905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 1730 1950 50  0001 C CNN
F 3 "~" H 1800 1950 50  0001 C CNN
	1    1800 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 63F66368
P 1800 2450
AR Path="/62665296/63F66368" Ref="R?"  Part="1" 
AR Path="/6479D118/63F66368" Ref="R?"  Part="1" 
AR Path="/61B4978A/63F66368" Ref="R?"  Part="1" 
AR Path="/63F3F92C/63F66368" Ref="R102"  Part="1" 
F 0 "R102" H 1870 2496 50  0000 L CNN
F 1 "10k" H 1870 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 1730 2450 50  0001 C CNN
F 3 "~" H 1800 2450 50  0001 C CNN
	1    1800 2450
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:BZX79C10 Z?
U 1 1 63F66374
P 2300 1400
AR Path="/62665296/63F66374" Ref="Z?"  Part="1" 
AR Path="/6479D118/63F66374" Ref="Z?"  Part="1" 
AR Path="/61B4978A/63F66374" Ref="Z?"  Part="1" 
AR Path="/63F3F92C/63F66374" Ref="Z12"  Part="1" 
F 0 "Z12" V 2554 1270 50  0000 R CNN
F 1 "BZX79C10" V 2645 1270 50  0000 R CNN
F 2 "SamacSys_Parts:DIOAD1068W53L380D172" H 2700 1550 50  0001 L CNN
F 3 "https://www.fairchildsemi.com/datasheets/BZ/BZX79C10.pdf" H 2700 1450 50  0001 L CNN
F 4 "BZX79C10, Zener Diode, 10V +/-5 500 mW 0.2A, 2-Pin DO-35" H 2700 1350 50  0001 L CNN "Description"
F 5 "" H 2700 1250 50  0001 L CNN "Height"
F 6 "512-BZX79C10" H 2700 1150 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ON-Semiconductor-Fairchild/BZX79C10?qs=FITO%2F%2FQgYDkGEXVpK3DyhQ%3D%3D" H 2700 1050 50  0001 L CNN "Mouser Price/Stock"
F 8 "ON Semiconductor" H 2700 950 50  0001 L CNN "Manufacturer_Name"
F 9 "BZX79C10" H 2700 850 50  0001 L CNN "Manufacturer_Part_Number"
	1    2300 1400
	0    -1   1    0   
$EndComp
Wire Wire Line
	1800 2850 1800 2600
Wire Wire Line
	1800 2300 1800 2200
Wire Wire Line
	2600 2200 2300 2200
Connection ~ 1800 2200
Wire Wire Line
	1800 2200 1800 2100
Wire Wire Line
	2300 2000 2300 2200
Connection ~ 2300 2200
Wire Wire Line
	2300 2200 1800 2200
Wire Wire Line
	2900 1800 2900 950 
Wire Wire Line
	2900 950  2300 950 
Wire Wire Line
	1800 950  1800 1800
Wire Wire Line
	2300 1400 2300 950 
Connection ~ 2300 950 
Wire Wire Line
	2300 950  1800 950 
Text HLabel 1250 3050 0    50   Input ~ 0
SPOut1TTL
Text HLabel 2900 2750 0    50   Input ~ 0
SPOut1
Wire Wire Line
	2900 2750 2900 2400
$Comp
L power:+12V #PWR?
U 1 1 63F664BF
P 2300 950
AR Path="/63F664BF" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/63F664BF" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/63F664BF" Ref="#PWR0135"  Part="1" 
F 0 "#PWR0135" H 2300 800 50  0001 C CNN
F 1 "+12V" H 2315 1123 50  0000 C CNN
F 2 "" H 2300 950 50  0001 C CNN
F 3 "" H 2300 950 50  0001 C CNN
	1    2300 950 
	-1   0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:IRF9Z34NPBF Q?
U 1 1 62C337B6
P 4550 2200
AR Path="/62665296/62C337B6" Ref="Q?"  Part="1" 
AR Path="/6479D118/62C337B6" Ref="Q?"  Part="1" 
AR Path="/61B4978A/62C337B6" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/62C337B6" Ref="Q50"  Part="1" 
F 0 "Q50" H 4980 2346 50  0000 L CNN
F 1 "IRF9Z34NPBF" H 4980 2255 50  0000 L CNN
F 2 "SamacSys_Parts:TO254P469X1042X1967-3P" H 5000 2150 50  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irf9z34npbf.pdf?fileId=5546d462533600a40153561220af1ddd" H 5000 2050 50  0001 L CNN
F 4 "MOSFET MOSFT PCh -55V -17A 100mOhm 23.3nC" H 5000 1950 50  0001 L CNN "Description"
F 5 "4.69" H 5000 1850 50  0001 L CNN "Height"
F 6 "942-IRF9Z34NPBF" H 5000 1750 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Infineon-IR/IRF9Z34NPBF?qs=9%252BKlkBgLFf16a%2FvlD%252BMCtQ%3D%3D" H 5000 1650 50  0001 L CNN "Mouser Price/Stock"
F 8 "Infineon" H 5000 1550 50  0001 L CNN "Manufacturer_Name"
F 9 "IRF9Z34NPBF" H 5000 1450 50  0001 L CNN "Manufacturer_Part_Number"
	1    4550 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C337BC
P 3200 3500
AR Path="/62665296/62C337BC" Ref="R?"  Part="1" 
AR Path="/6479D118/62C337BC" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C337BC" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C337BC" Ref="R103"  Part="1" 
F 0 "R103" H 3270 3546 50  0000 L CNN
F 1 "1M" H 3270 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 3130 3500 50  0001 C CNN
F 3 "~" H 3200 3500 50  0001 C CNN
	1    3200 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62C337C2
P 3750 3800
AR Path="/62665296/62C337C2" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/62C337C2" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/62C337C2" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/62C337C2" Ref="#PWR0136"  Part="1" 
F 0 "#PWR0136" H 3750 3550 50  0001 C CNN
F 1 "GND" H 3755 3627 50  0000 C CNN
F 2 "" H 3750 3800 50  0001 C CNN
F 3 "" H 3750 3800 50  0001 C CNN
	1    3750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 3650 3200 3800
Wire Wire Line
	3200 3800 3750 3800
Wire Wire Line
	3200 3350 3200 3050
Wire Wire Line
	3200 3050 3450 3050
Connection ~ 3750 3800
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 62C337CD
P 3650 3050
AR Path="/62665296/62C337CD" Ref="Q?"  Part="1" 
AR Path="/6479D118/62C337CD" Ref="Q?"  Part="1" 
AR Path="/61B4978A/62C337CD" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/62C337CD" Ref="Q49"  Part="1" 
F 0 "Q49" H 3854 3096 50  0000 L CNN
F 1 "2N7000" H 3854 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 3850 2975 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 3650 3050 50  0001 L CNN
	1    3650 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 3250 3750 3800
$Comp
L Device:R R?
U 1 1 62C337D4
P 3750 1950
AR Path="/62665296/62C337D4" Ref="R?"  Part="1" 
AR Path="/6479D118/62C337D4" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C337D4" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C337D4" Ref="R104"  Part="1" 
F 0 "R104" H 3820 1996 50  0000 L CNN
F 1 "100k" H 3820 1905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 3680 1950 50  0001 C CNN
F 3 "~" H 3750 1950 50  0001 C CNN
	1    3750 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C337DA
P 3750 2450
AR Path="/62665296/62C337DA" Ref="R?"  Part="1" 
AR Path="/6479D118/62C337DA" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C337DA" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C337DA" Ref="R105"  Part="1" 
F 0 "R105" H 3820 2496 50  0000 L CNN
F 1 "10k" H 3820 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 3680 2450 50  0001 C CNN
F 3 "~" H 3750 2450 50  0001 C CNN
	1    3750 2450
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:BZX79C10 Z?
U 1 1 62C337E6
P 4250 1400
AR Path="/62665296/62C337E6" Ref="Z?"  Part="1" 
AR Path="/6479D118/62C337E6" Ref="Z?"  Part="1" 
AR Path="/61B4978A/62C337E6" Ref="Z?"  Part="1" 
AR Path="/63F3F92C/62C337E6" Ref="Z13"  Part="1" 
F 0 "Z13" V 4504 1270 50  0000 R CNN
F 1 "BZX79C10" V 4595 1270 50  0000 R CNN
F 2 "SamacSys_Parts:DIOAD1068W53L380D172" H 4650 1550 50  0001 L CNN
F 3 "https://www.fairchildsemi.com/datasheets/BZ/BZX79C10.pdf" H 4650 1450 50  0001 L CNN
F 4 "BZX79C10, Zener Diode, 10V +/-5 500 mW 0.2A, 2-Pin DO-35" H 4650 1350 50  0001 L CNN "Description"
F 5 "" H 4650 1250 50  0001 L CNN "Height"
F 6 "512-BZX79C10" H 4650 1150 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ON-Semiconductor-Fairchild/BZX79C10?qs=FITO%2F%2FQgYDkGEXVpK3DyhQ%3D%3D" H 4650 1050 50  0001 L CNN "Mouser Price/Stock"
F 8 "ON Semiconductor" H 4650 950 50  0001 L CNN "Manufacturer_Name"
F 9 "BZX79C10" H 4650 850 50  0001 L CNN "Manufacturer_Part_Number"
	1    4250 1400
	0    -1   1    0   
$EndComp
Wire Wire Line
	3750 2850 3750 2600
Wire Wire Line
	3750 2300 3750 2200
Wire Wire Line
	4550 2200 4250 2200
Connection ~ 3750 2200
Wire Wire Line
	3750 2200 3750 2100
Wire Wire Line
	4250 2000 4250 2200
Connection ~ 4250 2200
Wire Wire Line
	4250 2200 3750 2200
Wire Wire Line
	4850 1800 4850 950 
Wire Wire Line
	4850 950  4250 950 
Wire Wire Line
	3750 950  3750 1800
Wire Wire Line
	4250 1400 4250 950 
Connection ~ 4250 950 
Wire Wire Line
	4250 950  3750 950 
Text HLabel 3200 3050 0    50   Input ~ 0
SPOut2TTL
Text HLabel 4850 2750 0    50   Input ~ 0
SPOut2
Wire Wire Line
	4850 2750 4850 2400
$Comp
L power:+12V #PWR?
U 1 1 62C337FD
P 4250 950
AR Path="/62C337FD" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/62C337FD" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/62C337FD" Ref="#PWR0137"  Part="1" 
F 0 "#PWR0137" H 4250 800 50  0001 C CNN
F 1 "+12V" H 4265 1123 50  0000 C CNN
F 2 "" H 4250 950 50  0001 C CNN
F 3 "" H 4250 950 50  0001 C CNN
	1    4250 950 
	-1   0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:IRF9Z34NPBF Q?
U 1 1 62C37B2F
P 6300 2200
AR Path="/62665296/62C37B2F" Ref="Q?"  Part="1" 
AR Path="/6479D118/62C37B2F" Ref="Q?"  Part="1" 
AR Path="/61B4978A/62C37B2F" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/62C37B2F" Ref="Q52"  Part="1" 
F 0 "Q52" H 6730 2346 50  0000 L CNN
F 1 "IRF9Z34NPBF" H 6730 2255 50  0000 L CNN
F 2 "SamacSys_Parts:TO254P469X1042X1967-3P" H 6750 2150 50  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irf9z34npbf.pdf?fileId=5546d462533600a40153561220af1ddd" H 6750 2050 50  0001 L CNN
F 4 "MOSFET MOSFT PCh -55V -17A 100mOhm 23.3nC" H 6750 1950 50  0001 L CNN "Description"
F 5 "4.69" H 6750 1850 50  0001 L CNN "Height"
F 6 "942-IRF9Z34NPBF" H 6750 1750 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Infineon-IR/IRF9Z34NPBF?qs=9%252BKlkBgLFf16a%2FvlD%252BMCtQ%3D%3D" H 6750 1650 50  0001 L CNN "Mouser Price/Stock"
F 8 "Infineon" H 6750 1550 50  0001 L CNN "Manufacturer_Name"
F 9 "IRF9Z34NPBF" H 6750 1450 50  0001 L CNN "Manufacturer_Part_Number"
	1    6300 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C37B35
P 4950 3500
AR Path="/62665296/62C37B35" Ref="R?"  Part="1" 
AR Path="/6479D118/62C37B35" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C37B35" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C37B35" Ref="R106"  Part="1" 
F 0 "R106" H 5020 3546 50  0000 L CNN
F 1 "1M" H 5020 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 4880 3500 50  0001 C CNN
F 3 "~" H 4950 3500 50  0001 C CNN
	1    4950 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62C37B3B
P 5500 3800
AR Path="/62665296/62C37B3B" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/62C37B3B" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/62C37B3B" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/62C37B3B" Ref="#PWR0138"  Part="1" 
F 0 "#PWR0138" H 5500 3550 50  0001 C CNN
F 1 "GND" H 5505 3627 50  0000 C CNN
F 2 "" H 5500 3800 50  0001 C CNN
F 3 "" H 5500 3800 50  0001 C CNN
	1    5500 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3650 4950 3800
Wire Wire Line
	4950 3800 5500 3800
Wire Wire Line
	4950 3350 4950 3050
Wire Wire Line
	4950 3050 5200 3050
Connection ~ 5500 3800
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 62C37B46
P 5400 3050
AR Path="/62665296/62C37B46" Ref="Q?"  Part="1" 
AR Path="/6479D118/62C37B46" Ref="Q?"  Part="1" 
AR Path="/61B4978A/62C37B46" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/62C37B46" Ref="Q51"  Part="1" 
F 0 "Q51" H 5604 3096 50  0000 L CNN
F 1 "2N7000" H 5604 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 5600 2975 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 5400 3050 50  0001 L CNN
	1    5400 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	5500 3250 5500 3800
$Comp
L Device:R R?
U 1 1 62C37B4D
P 5500 1950
AR Path="/62665296/62C37B4D" Ref="R?"  Part="1" 
AR Path="/6479D118/62C37B4D" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C37B4D" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C37B4D" Ref="R107"  Part="1" 
F 0 "R107" H 5570 1996 50  0000 L CNN
F 1 "100k" H 5570 1905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 5430 1950 50  0001 C CNN
F 3 "~" H 5500 1950 50  0001 C CNN
	1    5500 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C37B53
P 5500 2450
AR Path="/62665296/62C37B53" Ref="R?"  Part="1" 
AR Path="/6479D118/62C37B53" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C37B53" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C37B53" Ref="R108"  Part="1" 
F 0 "R108" H 5570 2496 50  0000 L CNN
F 1 "10k" H 5570 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 5430 2450 50  0001 C CNN
F 3 "~" H 5500 2450 50  0001 C CNN
	1    5500 2450
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:BZX79C10 Z?
U 1 1 62C37B5F
P 6000 1400
AR Path="/62665296/62C37B5F" Ref="Z?"  Part="1" 
AR Path="/6479D118/62C37B5F" Ref="Z?"  Part="1" 
AR Path="/61B4978A/62C37B5F" Ref="Z?"  Part="1" 
AR Path="/63F3F92C/62C37B5F" Ref="Z14"  Part="1" 
F 0 "Z14" V 6254 1270 50  0000 R CNN
F 1 "BZX79C10" V 6345 1270 50  0000 R CNN
F 2 "SamacSys_Parts:DIOAD1068W53L380D172" H 6400 1550 50  0001 L CNN
F 3 "https://www.fairchildsemi.com/datasheets/BZ/BZX79C10.pdf" H 6400 1450 50  0001 L CNN
F 4 "BZX79C10, Zener Diode, 10V +/-5 500 mW 0.2A, 2-Pin DO-35" H 6400 1350 50  0001 L CNN "Description"
F 5 "" H 6400 1250 50  0001 L CNN "Height"
F 6 "512-BZX79C10" H 6400 1150 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ON-Semiconductor-Fairchild/BZX79C10?qs=FITO%2F%2FQgYDkGEXVpK3DyhQ%3D%3D" H 6400 1050 50  0001 L CNN "Mouser Price/Stock"
F 8 "ON Semiconductor" H 6400 950 50  0001 L CNN "Manufacturer_Name"
F 9 "BZX79C10" H 6400 850 50  0001 L CNN "Manufacturer_Part_Number"
	1    6000 1400
	0    -1   1    0   
$EndComp
Wire Wire Line
	5500 2850 5500 2600
Wire Wire Line
	5500 2300 5500 2200
Wire Wire Line
	6300 2200 6000 2200
Connection ~ 5500 2200
Wire Wire Line
	5500 2200 5500 2100
Wire Wire Line
	6000 2000 6000 2200
Connection ~ 6000 2200
Wire Wire Line
	6000 2200 5500 2200
Wire Wire Line
	6600 1800 6600 950 
Wire Wire Line
	6600 950  6000 950 
Wire Wire Line
	5500 950  5500 1800
Wire Wire Line
	6000 1400 6000 950 
Connection ~ 6000 950 
Wire Wire Line
	6000 950  5500 950 
Text HLabel 4950 3050 0    50   Input ~ 0
SPOut3TTL
Text HLabel 6600 2750 0    50   Input ~ 0
SPOut3
Wire Wire Line
	6600 2750 6600 2400
$Comp
L power:+12V #PWR?
U 1 1 62C37B76
P 6000 950
AR Path="/62C37B76" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/62C37B76" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/62C37B76" Ref="#PWR0139"  Part="1" 
F 0 "#PWR0139" H 6000 800 50  0001 C CNN
F 1 "+12V" H 6015 1123 50  0000 C CNN
F 2 "" H 6000 950 50  0001 C CNN
F 3 "" H 6000 950 50  0001 C CNN
	1    6000 950 
	-1   0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:IRF9Z34NPBF Q?
U 1 1 62C3E865
P 8150 2200
AR Path="/62665296/62C3E865" Ref="Q?"  Part="1" 
AR Path="/6479D118/62C3E865" Ref="Q?"  Part="1" 
AR Path="/61B4978A/62C3E865" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/62C3E865" Ref="Q54"  Part="1" 
F 0 "Q54" H 8580 2346 50  0000 L CNN
F 1 "IRF9Z34NPBF" H 8580 2255 50  0000 L CNN
F 2 "SamacSys_Parts:TO254P469X1042X1967-3P" H 8600 2150 50  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irf9z34npbf.pdf?fileId=5546d462533600a40153561220af1ddd" H 8600 2050 50  0001 L CNN
F 4 "MOSFET MOSFT PCh -55V -17A 100mOhm 23.3nC" H 8600 1950 50  0001 L CNN "Description"
F 5 "4.69" H 8600 1850 50  0001 L CNN "Height"
F 6 "942-IRF9Z34NPBF" H 8600 1750 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Infineon-IR/IRF9Z34NPBF?qs=9%252BKlkBgLFf16a%2FvlD%252BMCtQ%3D%3D" H 8600 1650 50  0001 L CNN "Mouser Price/Stock"
F 8 "Infineon" H 8600 1550 50  0001 L CNN "Manufacturer_Name"
F 9 "IRF9Z34NPBF" H 8600 1450 50  0001 L CNN "Manufacturer_Part_Number"
	1    8150 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C3E86B
P 6800 3500
AR Path="/62665296/62C3E86B" Ref="R?"  Part="1" 
AR Path="/6479D118/62C3E86B" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C3E86B" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C3E86B" Ref="R109"  Part="1" 
F 0 "R109" H 6870 3546 50  0000 L CNN
F 1 "1M" H 6870 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 6730 3500 50  0001 C CNN
F 3 "~" H 6800 3500 50  0001 C CNN
	1    6800 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 62C3E871
P 7350 3800
AR Path="/62665296/62C3E871" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/62C3E871" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/62C3E871" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/62C3E871" Ref="#PWR0140"  Part="1" 
F 0 "#PWR0140" H 7350 3550 50  0001 C CNN
F 1 "GND" H 7355 3627 50  0000 C CNN
F 2 "" H 7350 3800 50  0001 C CNN
F 3 "" H 7350 3800 50  0001 C CNN
	1    7350 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6800 3650 6800 3800
Wire Wire Line
	6800 3800 7350 3800
Wire Wire Line
	6800 3350 6800 3050
Wire Wire Line
	6800 3050 7050 3050
Connection ~ 7350 3800
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 62C3E87C
P 7250 3050
AR Path="/62665296/62C3E87C" Ref="Q?"  Part="1" 
AR Path="/6479D118/62C3E87C" Ref="Q?"  Part="1" 
AR Path="/61B4978A/62C3E87C" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/62C3E87C" Ref="Q53"  Part="1" 
F 0 "Q53" H 7454 3096 50  0000 L CNN
F 1 "2N7000" H 7454 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 7450 2975 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 7250 3050 50  0001 L CNN
	1    7250 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 3250 7350 3800
$Comp
L Device:R R?
U 1 1 62C3E883
P 7350 1950
AR Path="/62665296/62C3E883" Ref="R?"  Part="1" 
AR Path="/6479D118/62C3E883" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C3E883" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C3E883" Ref="R110"  Part="1" 
F 0 "R110" H 7420 1996 50  0000 L CNN
F 1 "100k" H 7420 1905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 7280 1950 50  0001 C CNN
F 3 "~" H 7350 1950 50  0001 C CNN
	1    7350 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 62C3E889
P 7350 2450
AR Path="/62665296/62C3E889" Ref="R?"  Part="1" 
AR Path="/6479D118/62C3E889" Ref="R?"  Part="1" 
AR Path="/61B4978A/62C3E889" Ref="R?"  Part="1" 
AR Path="/63F3F92C/62C3E889" Ref="R111"  Part="1" 
F 0 "R111" H 7420 2496 50  0000 L CNN
F 1 "10k" H 7420 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 7280 2450 50  0001 C CNN
F 3 "~" H 7350 2450 50  0001 C CNN
	1    7350 2450
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:BZX79C10 Z?
U 1 1 62C3E895
P 7850 1400
AR Path="/62665296/62C3E895" Ref="Z?"  Part="1" 
AR Path="/6479D118/62C3E895" Ref="Z?"  Part="1" 
AR Path="/61B4978A/62C3E895" Ref="Z?"  Part="1" 
AR Path="/63F3F92C/62C3E895" Ref="Z15"  Part="1" 
F 0 "Z15" V 8104 1270 50  0000 R CNN
F 1 "BZX79C10" V 8195 1270 50  0000 R CNN
F 2 "SamacSys_Parts:DIOAD1068W53L380D172" H 8250 1550 50  0001 L CNN
F 3 "https://www.fairchildsemi.com/datasheets/BZ/BZX79C10.pdf" H 8250 1450 50  0001 L CNN
F 4 "BZX79C10, Zener Diode, 10V +/-5 500 mW 0.2A, 2-Pin DO-35" H 8250 1350 50  0001 L CNN "Description"
F 5 "" H 8250 1250 50  0001 L CNN "Height"
F 6 "512-BZX79C10" H 8250 1150 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ON-Semiconductor-Fairchild/BZX79C10?qs=FITO%2F%2FQgYDkGEXVpK3DyhQ%3D%3D" H 8250 1050 50  0001 L CNN "Mouser Price/Stock"
F 8 "ON Semiconductor" H 8250 950 50  0001 L CNN "Manufacturer_Name"
F 9 "BZX79C10" H 8250 850 50  0001 L CNN "Manufacturer_Part_Number"
	1    7850 1400
	0    -1   1    0   
$EndComp
Wire Wire Line
	7350 2850 7350 2600
Wire Wire Line
	7350 2300 7350 2200
Wire Wire Line
	8150 2200 7850 2200
Connection ~ 7350 2200
Wire Wire Line
	7350 2200 7350 2100
Wire Wire Line
	7850 2000 7850 2200
Connection ~ 7850 2200
Wire Wire Line
	7850 2200 7350 2200
Wire Wire Line
	8450 1800 8450 950 
Wire Wire Line
	8450 950  7850 950 
Wire Wire Line
	7350 950  7350 1800
Wire Wire Line
	7850 1400 7850 950 
Connection ~ 7850 950 
Wire Wire Line
	7850 950  7350 950 
Text HLabel 6800 3050 0    50   Input ~ 0
SPOut4TTL
Text HLabel 8450 2750 0    50   Input ~ 0
SPOut4
Wire Wire Line
	8450 2750 8450 2400
$Comp
L power:+12V #PWR?
U 1 1 62C3E8AC
P 7850 950
AR Path="/62C3E8AC" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/62C3E8AC" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/62C3E8AC" Ref="#PWR0141"  Part="1" 
F 0 "#PWR0141" H 7850 800 50  0001 C CNN
F 1 "+12V" H 7865 1123 50  0000 C CNN
F 2 "" H 7850 950 50  0001 C CNN
F 3 "" H 7850 950 50  0001 C CNN
	1    7850 950 
	-1   0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:IRF9Z34NPBF Q?
U 1 1 642D97AE
P 10000 2200
AR Path="/62665296/642D97AE" Ref="Q?"  Part="1" 
AR Path="/6479D118/642D97AE" Ref="Q?"  Part="1" 
AR Path="/61B4978A/642D97AE" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/642D97AE" Ref="Q55"  Part="1" 
F 0 "Q55" H 10430 2346 50  0000 L CNN
F 1 "IRF9Z34NPBF" H 10430 2255 50  0000 L CNN
F 2 "SamacSys_Parts:TO254P469X1042X1967-3P" H 10450 2150 50  0001 L CNN
F 3 "https://www.infineon.com/dgdl/irf9z34npbf.pdf?fileId=5546d462533600a40153561220af1ddd" H 10450 2050 50  0001 L CNN
F 4 "MOSFET MOSFT PCh -55V -17A 100mOhm 23.3nC" H 10450 1950 50  0001 L CNN "Description"
F 5 "4.69" H 10450 1850 50  0001 L CNN "Height"
F 6 "942-IRF9Z34NPBF" H 10450 1750 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Infineon-IR/IRF9Z34NPBF?qs=9%252BKlkBgLFf16a%2FvlD%252BMCtQ%3D%3D" H 10450 1650 50  0001 L CNN "Mouser Price/Stock"
F 8 "Infineon" H 10450 1550 50  0001 L CNN "Manufacturer_Name"
F 9 "IRF9Z34NPBF" H 10450 1450 50  0001 L CNN "Manufacturer_Part_Number"
	1    10000 2200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 642D97B4
P 8650 3500
AR Path="/62665296/642D97B4" Ref="R?"  Part="1" 
AR Path="/6479D118/642D97B4" Ref="R?"  Part="1" 
AR Path="/61B4978A/642D97B4" Ref="R?"  Part="1" 
AR Path="/63F3F92C/642D97B4" Ref="R114"  Part="1" 
F 0 "R114" H 8720 3546 50  0000 L CNN
F 1 "1M" H 8720 3455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 8580 3500 50  0001 C CNN
F 3 "~" H 8650 3500 50  0001 C CNN
	1    8650 3500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 642D97BA
P 9200 3800
AR Path="/62665296/642D97BA" Ref="#PWR?"  Part="1" 
AR Path="/6479D118/642D97BA" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/642D97BA" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/642D97BA" Ref="#PWR0148"  Part="1" 
F 0 "#PWR0148" H 9200 3550 50  0001 C CNN
F 1 "GND" H 9205 3627 50  0000 C CNN
F 2 "" H 9200 3800 50  0001 C CNN
F 3 "" H 9200 3800 50  0001 C CNN
	1    9200 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 3650 8650 3800
Wire Wire Line
	8650 3800 9200 3800
Wire Wire Line
	8650 3350 8650 3050
Wire Wire Line
	8650 3050 8900 3050
Connection ~ 9200 3800
Wire Wire Line
	9200 3250 9200 3800
$Comp
L Device:R R?
U 1 1 642D97CC
P 9200 1950
AR Path="/62665296/642D97CC" Ref="R?"  Part="1" 
AR Path="/6479D118/642D97CC" Ref="R?"  Part="1" 
AR Path="/61B4978A/642D97CC" Ref="R?"  Part="1" 
AR Path="/63F3F92C/642D97CC" Ref="R115"  Part="1" 
F 0 "R115" H 9270 1996 50  0000 L CNN
F 1 "100k" H 9270 1905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 9130 1950 50  0001 C CNN
F 3 "~" H 9200 1950 50  0001 C CNN
	1    9200 1950
	1    0    0    -1  
$EndComp
$Comp
L Device:R R?
U 1 1 642D97D2
P 9200 2450
AR Path="/62665296/642D97D2" Ref="R?"  Part="1" 
AR Path="/6479D118/642D97D2" Ref="R?"  Part="1" 
AR Path="/61B4978A/642D97D2" Ref="R?"  Part="1" 
AR Path="/63F3F92C/642D97D2" Ref="R116"  Part="1" 
F 0 "R116" H 9270 2496 50  0000 L CNN
F 1 "10k" H 9270 2405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P2.54mm_Vertical" V 9130 2450 50  0001 C CNN
F 3 "~" H 9200 2450 50  0001 C CNN
	1    9200 2450
	1    0    0    -1  
$EndComp
$Comp
L SamacSys_Parts:BZX79C10 Z?
U 1 1 642D97DE
P 9700 1400
AR Path="/62665296/642D97DE" Ref="Z?"  Part="1" 
AR Path="/6479D118/642D97DE" Ref="Z?"  Part="1" 
AR Path="/61B4978A/642D97DE" Ref="Z?"  Part="1" 
AR Path="/63F3F92C/642D97DE" Ref="Z16"  Part="1" 
F 0 "Z16" V 9954 1270 50  0000 R CNN
F 1 "BZX79C10" V 10045 1270 50  0000 R CNN
F 2 "SamacSys_Parts:DIOAD1068W53L380D172" H 10100 1550 50  0001 L CNN
F 3 "https://www.fairchildsemi.com/datasheets/BZ/BZX79C10.pdf" H 10100 1450 50  0001 L CNN
F 4 "BZX79C10, Zener Diode, 10V +/-5 500 mW 0.2A, 2-Pin DO-35" H 10100 1350 50  0001 L CNN "Description"
F 5 "" H 10100 1250 50  0001 L CNN "Height"
F 6 "512-BZX79C10" H 10100 1150 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/ON-Semiconductor-Fairchild/BZX79C10?qs=FITO%2F%2FQgYDkGEXVpK3DyhQ%3D%3D" H 10100 1050 50  0001 L CNN "Mouser Price/Stock"
F 8 "ON Semiconductor" H 10100 950 50  0001 L CNN "Manufacturer_Name"
F 9 "BZX79C10" H 10100 850 50  0001 L CNN "Manufacturer_Part_Number"
	1    9700 1400
	0    -1   1    0   
$EndComp
Wire Wire Line
	9200 2850 9200 2600
Wire Wire Line
	9200 2300 9200 2200
Wire Wire Line
	10000 2200 9700 2200
Connection ~ 9200 2200
Wire Wire Line
	9200 2200 9200 2100
Wire Wire Line
	9700 2000 9700 2200
Connection ~ 9700 2200
Wire Wire Line
	9700 2200 9200 2200
Wire Wire Line
	10300 1800 10300 950 
Wire Wire Line
	10300 950  9700 950 
Wire Wire Line
	9200 950  9200 1800
Wire Wire Line
	9700 1400 9700 950 
Connection ~ 9700 950 
Wire Wire Line
	9700 950  9200 950 
Text HLabel 8650 3050 0    50   Input ~ 0
SPOut5TTL
Text HLabel 10300 2750 0    50   Input ~ 0
SPOut5
Wire Wire Line
	10300 2750 10300 2400
$Comp
L power:+12V #PWR?
U 1 1 642D97F5
P 9700 950
AR Path="/642D97F5" Ref="#PWR?"  Part="1" 
AR Path="/61B4978A/642D97F5" Ref="#PWR?"  Part="1" 
AR Path="/63F3F92C/642D97F5" Ref="#PWR0149"  Part="1" 
F 0 "#PWR0149" H 9700 800 50  0001 C CNN
F 1 "+12V" H 9715 1123 50  0000 C CNN
F 2 "" H 9700 950 50  0001 C CNN
F 3 "" H 9700 950 50  0001 C CNN
	1    9700 950 
	-1   0    0    -1  
$EndComp
$Comp
L Transistor_FET:2N7000 Q?
U 1 1 642D97C5
P 9100 3050
AR Path="/62665296/642D97C5" Ref="Q?"  Part="1" 
AR Path="/6479D118/642D97C5" Ref="Q?"  Part="1" 
AR Path="/61B4978A/642D97C5" Ref="Q?"  Part="1" 
AR Path="/63F3F92C/642D97C5" Ref="Q2"  Part="1" 
F 0 "Q2" H 9304 3096 50  0000 L CNN
F 1 "2N7000" H 9304 3005 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-92_Inline" H 9300 2975 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/NDS7002A-D.PDF" H 9100 3050 50  0001 L CNN
	1    9100 3050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
