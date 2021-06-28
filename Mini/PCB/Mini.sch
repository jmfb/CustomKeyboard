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
L Mini:MiniDINN9 UD9
U 1 1 60D91131
P 10250 5800
F 0 "UD9" H 10300 6481 50  0000 C CNN
F 1 "MiniDINN9" H 10300 6390 50  0000 C CNN
F 2 "" H 10250 5800 50  0001 C CNN
F 3 "" H 10250 5800 50  0001 C CNN
	1    10250 5800
	1    0    0    -1  
$EndComp
$Comp
L Mini:Demultiplexer UDEMUX
U 1 1 60D91AF8
P 10200 3550
F 0 "UDEMUX" H 10175 4431 50  0000 C CNN
F 1 "Demultiplexer" H 10175 4340 50  0000 C CNN
F 2 "" H 10250 3300 50  0001 C CNN
F 3 "" H 10250 3300 50  0001 C CNN
	1    10200 3550
	1    0    0    -1  
$EndComp
$Comp
L Mini:ParallelLoadShiftRegister UREG
U 1 1 60D92831
P 10250 1250
F 0 "UREG" H 10200 111 50  0000 C CNN
F 1 "ParallelLoadShiftRegister" H 10200 20  50  0000 C CNN
F 2 "" H 10250 1250 50  0001 C CNN
F 3 "" H 10250 1250 50  0001 C CNN
	1    10250 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R0
U 1 1 60D9333B
P 9500 4600
F 0 "R0" H 9430 4554 50  0000 R CNN
F 1 "R" H 9430 4645 50  0000 R CNN
F 2 "" V 9430 4600 50  0001 C CNN
F 3 "~" H 9500 4600 50  0001 C CNN
	1    9500 4600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R1
U 1 1 60D93D8F
P 9250 4600
F 0 "R1" H 9320 4646 50  0000 L CNN
F 1 "R" H 9320 4555 50  0000 L CNN
F 2 "" V 9180 4600 50  0001 C CNN
F 3 "~" H 9250 4600 50  0001 C CNN
	1    9250 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 60D94491
P 9000 4600
F 0 "R2" H 9070 4646 50  0000 L CNN
F 1 "R" H 9070 4555 50  0000 L CNN
F 2 "" V 8930 4600 50  0001 C CNN
F 3 "~" H 9000 4600 50  0001 C CNN
	1    9000 4600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 60D94BA3
P 8750 4600
F 0 "R3" H 8820 4646 50  0000 L CNN
F 1 "R" H 8820 4555 50  0000 L CNN
F 2 "" V 8680 4600 50  0001 C CNN
F 3 "~" H 8750 4600 50  0001 C CNN
	1    8750 4600
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KY
U 1 1 60D94E5E
P 1300 1150
F 0 "KY" H 1275 1681 50  0000 C CNN
F 1 "KeyLedDiode" H 1275 1590 50  0000 C CNN
F 2 "" H 1300 1150 50  0001 C CNN
F 3 "" H 1300 1150 50  0001 C CNN
	1    1300 1150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KU
U 1 1 60D957F4
P 2450 1150
F 0 "KU" H 2425 1681 50  0000 C CNN
F 1 "KeyLedDiode" H 2425 1590 50  0000 C CNN
F 2 "" H 2450 1150 50  0001 C CNN
F 3 "" H 2450 1150 50  0001 C CNN
	1    2450 1150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KI
U 1 1 60D95C9B
P 3600 1150
F 0 "KI" H 3575 1681 50  0000 C CNN
F 1 "KeyLedDiode" H 3575 1590 50  0000 C CNN
F 2 "" H 3600 1150 50  0001 C CNN
F 3 "" H 3600 1150 50  0001 C CNN
	1    3600 1150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KO
U 1 1 60D95F88
P 4750 1150
F 0 "KO" H 4725 1681 50  0000 C CNN
F 1 "KeyLedDiode" H 4725 1590 50  0000 C CNN
F 2 "" H 4750 1150 50  0001 C CNN
F 3 "" H 4750 1150 50  0001 C CNN
	1    4750 1150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KP
U 1 1 60D96499
P 5900 1150
F 0 "KP" H 5875 1681 50  0000 C CNN
F 1 "KeyLedDiode" H 5875 1590 50  0000 C CNN
F 2 "" H 5900 1150 50  0001 C CNN
F 3 "" H 5900 1150 50  0001 C CNN
	1    5900 1150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KBACKSPACE
U 1 1 60D969D9
P 7050 1150
F 0 "KBACKSPACE" H 7025 1681 50  0000 C CNN
F 1 "KeyLedDiode" H 7025 1590 50  0000 C CNN
F 2 "" H 7050 1150 50  0001 C CNN
F 3 "" H 7050 1150 50  0001 C CNN
	1    7050 1150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KENTER
U 1 1 60D96F98
P 7050 2300
F 0 "KENTER" H 7025 2831 50  0000 C CNN
F 1 "KeyLedDiode" H 7025 2740 50  0000 C CNN
F 2 "" H 7050 2300 50  0001 C CNN
F 3 "" H 7050 2300 50  0001 C CNN
	1    7050 2300
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KH
U 1 1 60D9766D
P 1300 2300
F 0 "KH" H 1275 2831 50  0000 C CNN
F 1 "KeyLedDiode" H 1275 2740 50  0000 C CNN
F 2 "" H 1300 2300 50  0001 C CNN
F 3 "" H 1300 2300 50  0001 C CNN
	1    1300 2300
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KN
U 1 1 60D97E6E
P 1300 3450
F 0 "KN" H 1275 3981 50  0000 C CNN
F 1 "KeyLedDiode" H 1275 3890 50  0000 C CNN
F 2 "" H 1300 3450 50  0001 C CNN
F 3 "" H 1300 3450 50  0001 C CNN
	1    1300 3450
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KM
U 1 1 60D98A24
P 2450 3450
F 0 "KM" H 2425 3981 50  0000 C CNN
F 1 "KeyLedDiode" H 2425 3890 50  0000 C CNN
F 2 "" H 2450 3450 50  0001 C CNN
F 3 "" H 2450 3450 50  0001 C CNN
	1    2450 3450
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KCOMMA
U 1 1 60D99623
P 3600 3450
F 0 "KCOMMA" H 3575 3981 50  0000 C CNN
F 1 "KeyLedDiode" H 3575 3890 50  0000 C CNN
F 2 "" H 3600 3450 50  0001 C CNN
F 3 "" H 3600 3450 50  0001 C CNN
	1    3600 3450
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KL
U 1 1 60D99ACE
P 4750 2300
F 0 "KL" H 4725 2831 50  0000 C CNN
F 1 "KeyLedDiode" H 4725 2740 50  0000 C CNN
F 2 "" H 4750 2300 50  0001 C CNN
F 3 "" H 4750 2300 50  0001 C CNN
	1    4750 2300
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KPERIOD
U 1 1 60D9A0C4
P 4750 3450
F 0 "KPERIOD" H 4725 3981 50  0000 C CNN
F 1 "KeyLedDiode" H 4725 3890 50  0000 C CNN
F 2 "" H 4750 3450 50  0001 C CNN
F 3 "" H 4750 3450 50  0001 C CNN
	1    4750 3450
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KSEMICOLON
U 1 1 60D9A677
P 5900 2300
F 0 "KSEMICOLON" H 5875 2831 50  0000 C CNN
F 1 "KeyLedDiode" H 5875 2740 50  0000 C CNN
F 2 "" H 5900 2300 50  0001 C CNN
F 3 "" H 5900 2300 50  0001 C CNN
	1    5900 2300
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KFSLASH
U 1 1 60D9ACED
P 5900 3450
F 0 "KFSLASH" H 5875 3981 50  0000 C CNN
F 1 "KeyLedDiode" H 5875 3890 50  0000 C CNN
F 2 "" H 5900 3450 50  0001 C CNN
F 3 "" H 5900 3450 50  0001 C CNN
	1    5900 3450
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KT2_1
U 1 1 60D9B3F5
P 3200 6550
F 0 "KT2_1" H 3175 7081 50  0000 C CNN
F 1 "KeyLedDiode" H 3175 6990 50  0000 C CNN
F 2 "" H 3200 6550 50  0001 C CNN
F 3 "" H 3200 6550 50  0001 C CNN
	1    3200 6550
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KT2_2
U 1 1 60D9B913
P 2050 6550
F 0 "KT2_2" H 2025 7081 50  0000 C CNN
F 1 "KeyLedDiode" H 2025 6990 50  0000 C CNN
F 2 "" H 2050 6550 50  0001 C CNN
F 3 "" H 2050 6550 50  0001 C CNN
	1    2050 6550
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KTG_A
U 1 1 60D9C0A7
P 4550 6100
F 0 "KTG_A" H 4525 6631 50  0000 C CNN
F 1 "KeyLedDiode" H 4525 6540 50  0000 C CNN
F 2 "" H 4550 6100 50  0001 C CNN
F 3 "" H 4550 6100 50  0001 C CNN
	1    4550 6100
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KTG_B
U 1 1 60D9C8B3
P 5700 6100
F 0 "KTG_B" H 5675 6631 50  0000 C CNN
F 1 "KeyLedDiode" H 5675 6540 50  0000 C CNN
F 2 "" H 5700 6100 50  0001 C CNN
F 3 "" H 5700 6100 50  0001 C CNN
	1    5700 6100
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KTG_C
U 1 1 60D9CFDF
P 4550 7150
F 0 "KTG_C" H 4525 7681 50  0000 C CNN
F 1 "KeyLedDiode" H 4525 7590 50  0000 C CNN
F 2 "" H 4550 7150 50  0001 C CNN
F 3 "" H 4550 7150 50  0001 C CNN
	1    4550 7150
	1    0    0    -1  
$EndComp
$Comp
L Mini:KeyLedDiode KTG_D
U 1 1 60D9D818
P 5700 7150
F 0 "KTG_D" H 5675 7681 50  0000 C CNN
F 1 "KeyLedDiode" H 5675 7590 50  0000 C CNN
F 2 "" H 5700 7150 50  0001 C CNN
F 3 "" H 5700 7150 50  0001 C CNN
	1    5700 7150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 800  1300 800 
Wire Wire Line
	3500 800  3600 800 
Wire Wire Line
	4650 800  4750 800 
Wire Wire Line
	5800 800  5900 800 
Wire Wire Line
	6950 800  7050 800 
Wire Wire Line
	1200 1950 1300 1950
Wire Wire Line
	2350 1950 2450 1950
Wire Wire Line
	4650 1950 4750 1950
Wire Wire Line
	5800 1950 5900 1950
Wire Wire Line
	1200 3100 1300 3100
Wire Wire Line
	2350 3100 2450 3100
Wire Wire Line
	3500 3100 3600 3100
Wire Wire Line
	4650 3100 4750 3100
Wire Wire Line
	5800 3100 5900 3100
Wire Wire Line
	6950 1950 7050 1950
Wire Wire Line
	3100 6200 3200 6200
Wire Wire Line
	4450 5750 4550 5750
Wire Wire Line
	5600 5750 5700 5750
Wire Wire Line
	1950 6200 2050 6200
Wire Wire Line
	4450 6800 4550 6800
Wire Wire Line
	5600 6800 5700 6800
Wire Wire Line
	3500 1950 3600 1950
$Comp
L Mini:KeyLedDiode KK
U 1 1 60D990DD
P 3600 2300
F 0 "KK" H 3575 2831 50  0000 C CNN
F 1 "KeyLedDiode" H 3575 2740 50  0000 C CNN
F 2 "" H 3600 2300 50  0001 C CNN
F 3 "" H 3600 2300 50  0001 C CNN
	1    3600 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 800  2450 800 
$Comp
L Mini:KeyLedDiode KJ
U 1 1 60D9856D
P 2450 2300
F 0 "KJ" H 2425 2831 50  0000 C CNN
F 1 "KeyLedDiode" H 2425 2740 50  0000 C CNN
F 2 "" H 2450 2300 50  0001 C CNN
F 3 "" H 2450 2300 50  0001 C CNN
	1    2450 2300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
