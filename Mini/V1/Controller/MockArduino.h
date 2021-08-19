#pragma once
#include "Arduino.h"
#include <vector>
#include <map>
#include <string>
#include <set>
#include <queue>
#include <tuple>
using namespace std;

class KeyboardReport {
public:
	KeyboardReport();
	KeyboardReport(
		unsigned int modifiers,
		unsigned int key1 = 0,
		unsigned int key2 = 0,
		unsigned int key3 = 0,
		unsigned int key4 = 0,
		unsigned int key5 = 0,
		unsigned int key6 = 0);

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

class MockArduino {
public:
	void Initialize();

	void Delay(int milliseconds);
	void DelayMicroseconds(int microseconds);
	void PinMode(int pin, int mode);
	void DigitalWrite(int pin, int signal);
	int DigitalRead(int pin);
	void KeyboardCallback(
		uint8_t modifiers,
		uint8_t key1,
		uint8_t key2,
		uint8_t key3,
		uint8_t key4,
		uint8_t key5,
		uint8_t key6);

	void AssertPinModes(const vector<int>& expected);
	void AssertKeyboardReports(const vector<KeyboardReport>& expected);

	void PressKey(bool left, int row, int column);
	void ReleaseKey(bool left, int row, int column);
	void ClearPressedKeys();

	bool IsPressed(bool left, int row, int column);
	int GetSelectedRow();

private:
	map<int, int> pinModes;
	map<int, int> pinSignals;
	queue<bool> leftShiftRegister;
	queue<bool> rightShiftRegister;
	vector<KeyboardReport> keyboardReports;
	set<tuple<bool, int, int>> pressedKeys;
};

extern MockArduino mockArduino;
