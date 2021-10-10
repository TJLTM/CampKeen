EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L SamacSys_Parts:CD74HC4050ME4 IC1
U 1 1 61623165
P 5900 5350
F 0 "IC1" H 6450 5615 50  0000 C CNN
F 1 "CD74HC4050ME4" H 6450 5524 50  0000 C CNN
F 2 "SOIC127P600X175-16N" H 6850 5450 50  0001 L CNN
F 3 "https://www.ti.com/lit/pdf/MPDS178" H 6850 5350 50  0001 L CNN
F 4 "Buffers & Line Drivers Hi-Spd CMOS Logic Hex Non-Inv Bfr" H 6850 5250 50  0001 L CNN "Description"
F 5 "1.75" H 6850 5150 50  0001 L CNN "Height"
F 6 "595-CD74HC4050ME4" H 6850 5050 50  0001 L CNN "Mouser Part Number"
F 7 "https://www.mouser.co.uk/ProductDetail/Texas-Instruments/CD74HC4050ME4?qs=xFfolx0DHx14hqY1xeFnNQ%3D%3D" H 6850 4950 50  0001 L CNN "Mouser Price/Stock"
F 8 "Texas Instruments" H 6850 4850 50  0001 L CNN "Manufacturer_Name"
F 9 "CD74HC4050ME4" H 6850 4750 50  0001 L CNN "Manufacturer_Part_Number"
	1    5900 5350
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x09 J1
U 1 1 616242A8
P 4600 4200
F 0 "J1" H 4518 4817 50  0000 C CNN
F 1 "CampKeen" H 4518 4726 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x09_P2.54mm_Vertical" H 4600 4200 50  0001 C CNN
F 3 "~" H 4600 4200 50  0001 C CNN
	1    4600 4200
	-1   0    0    -1  
$EndComp
NoConn ~ 4800 3900
NoConn ~ 4800 4600
NoConn ~ 4800 4500
NoConn ~ 4800 4400
NoConn ~ 6850 4600
NoConn ~ 6850 4500
NoConn ~ 6850 4400
$Comp
L power:GND #PWR01
U 1 1 6162C97F
P 5750 3800
F 0 "#PWR01" H 5750 3550 50  0001 C CNN
F 1 "GND" H 5755 3627 50  0000 C CNN
F 2 "" H 5750 3800 50  0001 C CNN
F 3 "" H 5750 3800 50  0001 C CNN
	1    5750 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 6162F404
P 5850 6150
F 0 "#PWR02" H 5850 5900 50  0001 C CNN
F 1 "GND" H 5855 5977 50  0000 C CNN
F 2 "" H 5850 6150 50  0001 C CNN
F 3 "" H 5850 6150 50  0001 C CNN
	1    5850 6150
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x09 J2
U 1 1 61624CF1
P 7050 4200
F 0 "J2" H 7130 4242 50  0000 L CNN
F 1 "EnergyMeter" H 7130 4151 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x09_P2.54mm_Vertical" H 7050 4200 50  0001 C CNN
F 3 "~" H 7050 4200 50  0001 C CNN
	1    7050 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	4800 3800 5750 3800
Connection ~ 5750 3800
Wire Wire Line
	5750 3800 6850 3800
Wire Wire Line
	5850 6150 5850 6050
Wire Wire Line
	5850 6050 5900 6050
Wire Wire Line
	6850 3900 5900 3900
Wire Wire Line
	5900 3900 5900 5350
Wire Wire Line
	5900 5450 5800 5450
Wire Wire Line
	5800 5450 5800 4200
Wire Wire Line
	5800 4200 6850 4200
Wire Wire Line
	6850 4000 5750 4000
Wire Wire Line
	5750 4000 5750 5850
Wire Wire Line
	5750 5850 5900 5850
Wire Wire Line
	5900 5750 5650 5750
Wire Wire Line
	5650 5750 5650 4100
Wire Wire Line
	5650 4100 6850 4100
Wire Wire Line
	6850 4300 6750 4300
Wire Wire Line
	6750 4300 6750 5000
Wire Wire Line
	6750 5000 7300 5000
Wire Wire Line
	7300 5000 7300 5450
Wire Wire Line
	7300 5450 7000 5450
NoConn ~ 7000 5350
NoConn ~ 7000 5650
NoConn ~ 7000 6050
NoConn ~ 7000 5950
NoConn ~ 7000 5850
NoConn ~ 7000 5750
Wire Wire Line
	5900 5950 5550 5950
Wire Wire Line
	5550 5950 5550 4000
Wire Wire Line
	5550 4000 4800 4000
Wire Wire Line
	5900 5650 5500 5650
Wire Wire Line
	5500 5650 5500 4100
Wire Wire Line
	5500 4100 4800 4100
Wire Wire Line
	5900 5550 5450 5550
Wire Wire Line
	5450 5550 5450 4200
Wire Wire Line
	5450 4200 4800 4200
NoConn ~ 4800 4300
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 616355B7
P 7450 5550
F 0 "J3" H 7530 5592 50  0000 L CNN
F 1 "CS5V" H 7530 5501 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x01_P2.54mm_Vertical" H 7450 5550 50  0001 C CNN
F 3 "~" H 7450 5550 50  0001 C CNN
	1    7450 5550
	1    0    0    -1  
$EndComp
Wire Wire Line
	7250 5550 7000 5550
$EndSCHEMATC
