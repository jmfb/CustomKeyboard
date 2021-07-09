#include "MockArduino.h"
#include <stdexcept>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

KeyboardSingleton Keyboard;
MockArduino mockArduino;

template <typename Type>
void AreEqual(Type expected, Type actual, const string& name) {
	if (expected != actual) {
		throw runtime_error(
			name + ": " +
			"Expected " + to_string(expected) +
			", Actual " + to_string(actual) +
			".");
	}
}

void Assert(bool condition, const string& name) {
	if (!condition) {
		throw runtime_error("Assert failed! " + name);
	}
}

void delay(int milliseconds) {
	mockArduino.Delay(milliseconds);
}

void pinMode(int pin, int mode) {
	mockArduino.PinMode(pin, mode);
}

void digitalWrite(int pin, int signal) {
	mockArduino.DigitalWrite(pin, signal);
}

int digitalRead(int pin) {
	return mockArduino.DigitalRead(pin);
}

void keyboardCallback(
	uint8_t modifiers,
	uint8_t key1,
	uint8_t key2,
	uint8_t key3,
	uint8_t key4,
	uint8_t key5,
	uint8_t key6
) {
	mockArduino.KeyboardCallback(modifiers, key1, key2, key3, key4, key5, key6);
}

KeyboardReport::KeyboardReport()
	: modifiers(0),
	key1(0),
	key2(0),
	key3(0),
	key4(0),
	key5(0),
	key6(0) {
}

KeyboardReport::KeyboardReport(
	unsigned int modifiers,
	unsigned int key1,
	unsigned int key2,
	unsigned int key3,
	unsigned int key4,
	unsigned int key5,
	unsigned int key6
) : modifiers(static_cast<uint8_t>(modifiers)),
	key1(static_cast<uint8_t>(key1)),
	key2(static_cast<uint8_t>(key2)),
	key3(static_cast<uint8_t>(key3)),
	key4(static_cast<uint8_t>(key4)),
	key5(static_cast<uint8_t>(key5)),
	key6(static_cast<uint8_t>(key6)) {
}

bool KeyboardReport::operator==(const KeyboardReport& other) const {
	return
		modifiers == other.modifiers &&
		key1 == other.key1 &&
		key2 == other.key2 &&
		key3 == other.key3 &&
		key4 == other.key4 &&
		key5 == other.key5 &&
		key6 == other.key6;
}

bool KeyboardReport::operator!=(const KeyboardReport& other) const {
	return !operator==(other);
}

string to_string(const KeyboardReport& keyboardReport) {
	return "Mod=" + to_string(keyboardReport.modifiers) +
		", [" + to_string(keyboardReport.key1) +
		", " + to_string(keyboardReport.key2) +
		", " + to_string(keyboardReport.key3) +
		", " + to_string(keyboardReport.key4) +
		", " + to_string(keyboardReport.key5) +
		", " + to_string(keyboardReport.key6) +
		"]";
}

void MockArduino::Initialize() {
	delays.clear();
	pinModes.clear();
	keyboardReports.clear();
	pressedKeys.clear();
}

void MockArduino::Delay(int milliseconds) {
	delays.push_back(milliseconds);
}

void MockArduino::PinMode(int pin, int mode) {
	pinModes[pin] = mode;
}

void MockArduino::DigitalWrite(int pin, int signal) {
	pinSignals[pin] = signal;
	if (pin == 4 && signal == LOW) { // Load
		auto row = GetSelectedRow();
		while (!leftShiftRegister.empty()) {
			leftShiftRegister.pop();
		}
		while (!rightShiftRegister.empty()) {
			rightShiftRegister.pop();
		}
		for (auto column = 0; column < 4; ++column) {
			leftShiftRegister.push(IsPressed(true, row, column));
			rightShiftRegister.push(IsPressed(false, row, column));
		}
	} else if (pin == 5 && signal == HIGH && pinSignals[4] == HIGH) { // Shift
		Assert(!leftShiftRegister.empty(), "Attempt to shift past left register end");
		Assert(!rightShiftRegister.empty(), "Attempt to shift past right register end");
		leftShiftRegister.pop();
		rightShiftRegister.pop();
	}
}

int MockArduino::DigitalRead(int pin) {
	switch (pin) {
		case 7:
			Assert(!leftShiftRegister.empty(), "Attempt to read past left register end");
			return leftShiftRegister.front() ? LOW : HIGH;
		case 8:
			Assert(!rightShiftRegister.empty(), "Attempt to read past right register end");
			return rightShiftRegister.front() ? LOW : HIGH;
		default:
			Assert(false, "Invalid read pin.");
			return HIGH;
	}
}

void MockArduino::KeyboardCallback(
	uint8_t modifiers,
	uint8_t key1,
	uint8_t key2,
	uint8_t key3,
	uint8_t key4,
	uint8_t key5,
	uint8_t key6
) {
	keyboardReports.emplace_back(modifiers, key1, key2, key3, key4, key5, key6);
}

void MockArduino::AssertDelays(const vector<int>& expected) {
	AreEqual(expected.size(), delays.size(), "Delay Count");
	for (auto index = 0; index < expected.size(); ++index) {
		AreEqual(expected[index], delays[index], "Delay[" + to_string(index) + "]");
	}
}

void MockArduino::AssertPinModes(const vector<int>& expected) {
	AreEqual(expected.size(), pinModes.size(), "Mode Count");
	for (auto index = 0; index < expected.size(); ++index) {
		AreEqual(expected[index], pinModes[index], "Mode[" + to_string(index) + "]");
	}
}

void MockArduino::AssertKeyboardReports(const vector<KeyboardReport>& expected) {
	try {
		AreEqual(expected.size(), keyboardReports.size(), "Keyboard Report Count");
		for (auto index = 0; index < expected.size(); ++index) {
			AreEqual(expected[index], keyboardReports[index], "KeyboardReport[" + to_string(index) + "]");
		}
	} catch (...) {
		cout << "Actual Keyboard Report\n";
		for (auto& report : keyboardReports)
			cout << to_string(report) << '\n';
		throw;
	}
}

void MockArduino::PressKey(bool left, int row, int column) {
	pressedKeys.emplace(left, row, column);
}

void MockArduino::ReleaseKey(bool left, int row, int column) {
	pressedKeys.erase(make_tuple(left, row, column));
}

void MockArduino::ClearPressedKeys() {
	pressedKeys.clear();
}

bool MockArduino::IsPressed(bool left, int row, int column) {
	return pressedKeys.contains(make_tuple(left, row, column));
}

int MockArduino::GetSelectedRow() {
	auto s0 = pinSignals[0];
	Assert(s0 == pinSignals[1], "Left and right S0 should match for row selection purposes.");
	auto s1 = pinSignals[2];
	auto s2 = pinSignals[3];
	auto row = s2 << 2 | s1 << 1 | s0;
	Assert(row >= 0 && row <= 7, "Only row 0-7 are valid.");
	return row;
}
