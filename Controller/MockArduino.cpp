#include "MockArduino.h"
#include <stdexcept>
#include <string>
using namespace std;

KeyboardSingleton Keyboard;
MockArduino mockArduino;

template <typename Type>
void AreEqual(Type expected, Type actual, const string& name)
{
	if (expected != actual)
		throw runtime_error(
			name + ": " +
			"Expected " + to_string(expected) +
			", Actual " + to_string(actual) +
			".");
}

void delay(int milliseconds)
{
	mockArduino.Delay(milliseconds);
}

void pinMode(int pin, int mode)
{
	mockArduino.PinMode(pin, mode);
}

void digitalWrite(int pin, int signal)
{
	mockArduino.DigitalWrite(pin, signal);
}

int digitalRead(int pin)
{
	return mockArduino.DigitalRead(pin);
}

void keyboardCallback(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6)
{
	mockArduino.KeyboardCallback(modifiers, key1, key2, key3, key4, key5, key6);
}

KeyboardReport::KeyboardReport()
	: modifiers(0), key1(0), key2(0), key3(0), key4(0), key5(0), key6(0)
{
}

KeyboardReport::KeyboardReport(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6)
	: modifiers(modifiers), key1(key1), key2(key2), key3(key3), key4(key4), key5(key5), key6(key6)
{
}

bool KeyboardReport::operator==(const KeyboardReport& other) const
{
	return modifiers == other.modifiers &&
		key1 == other.key1 &&
		key2 == other.key2 &&
		key3 == other.key3 &&
		key4 == other.key4 &&
		key5 == other.key5 &&
		key6 == other.key6;
}

bool KeyboardReport::operator!=(const KeyboardReport& other) const
{
	return !operator==(other);
}

string to_string(const KeyboardReport& keyboardReport)
{
	return "Mod=" + to_string(keyboardReport.modifiers) +
		", [" + to_string(keyboardReport.key1) +
		", " + to_string(keyboardReport.key2) +
		", " + to_string(keyboardReport.key3) +
		", " + to_string(keyboardReport.key4) +
		", " + to_string(keyboardReport.key5) +
		", " + to_string(keyboardReport.key6) +
		"]";
}

void MockArduino::Initialize()
{
	delays.clear();
	pinModes.clear();
	pinSignals.clear();
	keyboardReports.clear();
}

void MockArduino::Delay(int milliseconds)
{
	delays.push_back(milliseconds);
}

void MockArduino::PinMode(int pin, int mode)
{
	pinModes[pin] = mode;
}

void MockArduino::DigitalWrite(int pin, int signal)
{
	pinSignals[pin] = signal;
}

int MockArduino::DigitalRead(int pin)
{
	//TODO:
	return HIGH;
}

void MockArduino::KeyboardCallback(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6)
{
	keyboardReports.emplace_back(modifiers, key1, key2, key3, key4, key5, key6);
}

void MockArduino::AssertDelays(const vector<int>& expected)
{
	AreEqual(expected.size(), delays.size(), "Delay Count");
	for (auto index = 0; index < expected.size(); ++index)
		AreEqual(expected[index], delays[index], "Delay[" + to_string(index) + "]");
}

void MockArduino::AssertPinModes(const vector<int>& expected)
{
	AreEqual(expected.size(), pinModes.size(), "Mode Count");
	for (auto index = 0; index < expected.size(); ++index)
		AreEqual(expected[index], pinModes[index], "Mode[" + to_string(index) + "]");
}

void MockArduino::AssertPinSignals(const vector<int>& expected)
{
	AreEqual(expected.size(), pinSignals.size(), "Signal Count");
	for (auto index = 0; index < expected.size(); ++index)
		AreEqual(expected[index], pinSignals[index], "Signal[" + to_string(index) + "]");
}

void MockArduino::AssertKeyboardReports(const vector<KeyboardReport>& expected)
{
	AreEqual(expected.size(), keyboardReports.size(), "Keyboard Report Count");
	for (auto index = 0; index < expected.size(); ++index)
		AreEqual(expected[index], keyboardReports[index], "KeyboardReport[" + to_string(index) + "]");
}
