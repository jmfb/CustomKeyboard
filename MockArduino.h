#pragma once
#include "Arduino.h"
#include <vector>
#include <map>
#include <string>
using namespace std;

class KeyboardReport
{
public:
	KeyboardReport();
	KeyboardReport(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6);

	bool operator==(const KeyboardReport& other) const;
	bool operator!=(const KeyboardReport& other) const;

	uint8_t modifiers;
	uint8_t key1;
	uint8_t key2;
	uint8_t key3;
	uint8_t key4;
	uint8_t key5;
	uint8_t key6;
};

string to_string(const KeyboardReport& keyboardReport);

class MockArduino
{
public:
	void Initialize();

	void Delay(int milliseconds);
	void PinMode(int pin, int mode);
	void DigitalWrite(int pin, int signal);
	int DigitalRead(int pin);
	void KeyboardCallback(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6);

	void AssertDelays(const vector<int>& expected);
	void AssertPinModes(const vector<int>& expected);
	void AssertPinSignals(const vector<int>& expected);
	void AssertKeyboardReports(const vector<KeyboardReport>& expected);

private:
	vector<int> delays;
	map<int, int> pinModes;
	map<int, int> pinSignals;
	vector<KeyboardReport> keyboardReports;
};

extern MockArduino mockArduino;
