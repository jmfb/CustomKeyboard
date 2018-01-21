#include "Arduino.h"
#include "MockArduino.h"
#include <iostream>
#include <exception>
#include <functional>
using namespace std;

void Loop(int count)
{
	for (auto index = 0; index < count; ++index)
		loop();
}

void PressKey(int row, int column)
{
	mockArduino.PressKey(row, column);
	Loop(3);
}

void ReleaseKey(int row, int column)
{
	mockArduino.ReleaseKey(row, column);
	Loop(1);
}

class Test
{
public:
	Test(const char* name, function<void()> testFunction)
		: name(name), testFunction(testFunction)
	{
	}

	bool Run() const
	{
		try
		{
			mockArduino.ClearPressedKeys();
			Loop(3);
			mockArduino.Initialize();
			testFunction();
			return true;
		}
		catch (const exception& error)
		{
			cout << name << ": " << error.what() << '\n';
			return false;
		}
	}

	const char* name;
	function<void()> testFunction;
};

extern vector<Test> tests;

int main()
{
	auto passed = 0;
	auto failed = 0;
	for (auto& test : tests)
		if (test.Run())
			++passed;
		else
			++failed;
	cout << (passed - 1) << " passed, " << failed << " failed.\n"
		<< "Test run " << (failed > 0 ? "failed" : "succeeded") << ".\n";
	return failed > 0 ? 1 : 0;
}

#define BEGIN_TESTS() vector<Test> tests{ { "", [](){}
#define TEST_METHOD(name) }, { #name, []()
#define END_TESTS() } };

BEGIN_TESTS()

TEST_METHOD(InitializePinModesAndSignals)
{
	setup();
	mockArduino.AssertDelays({ 200 });
	mockArduino.AssertPinModes({
		//First 12 row pins marked as output
		OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT,
		OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT,
		OUTPUT, OUTPUT,
		//Next 11 column pins marked as input
		INPUT, INPUT, INPUT, INPUT, INPUT,
		INPUT, INPUT, INPUT, INPUT, INPUT,
		INPUT
	});
	mockArduino.AssertPinSignals({
		//First 12 row pins set to HIGH
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH, HIGH,
		//Next 11 column pins also all set to HIGH
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH, HIGH, HIGH, HIGH, HIGH,
		HIGH
	});
	mockArduino.AssertKeyboardReports({});
}

TEST_METHOD(DoNotRegisterKeysPressedForFewerThanThreeScans)
{
	mockArduino.PressKey(0, 12);
	Loop(2);
	mockArduino.ReleaseKey(0, 12);
	Loop(1);
	mockArduino.AssertKeyboardReports({});
}

TEST_METHOD(RegisterStableKey)
{
	mockArduino.PressKey(0, 12);
	Loop(3);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC }
	});
}

TEST_METHOD(RetainKeyPressOrder)
{
	PressKey(0, 12); //Esc
	PressKey(0, 14); //F1
	PressKey(0, 15); //F2
	ReleaseKey(0, 14);
	PressKey(0, 16); //F3
	ReleaseKey(0, 12);
	ReleaseKey(0, 16);
	ReleaseKey(0, 15);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC },
		{ 0, KEY_ESC, KEY_F1 },
		{ 0, KEY_ESC, KEY_F1, KEY_F2 },
		{ 0, KEY_ESC, KEY_F2 },
		{ 0, KEY_ESC, KEY_F2, KEY_F3 },
		{ 0, KEY_F2, KEY_F3 },
		{ 0, KEY_F2 },
		{}
	});
}

TEST_METHOD(HyperKeyLayout)
{
	PressKey(0, 12); //Esc
	PressKey(5, 15); //Hyper Key
	PressKey(2, 12); //Tab maps to Enter
	PressKey(0, 13); //F1 has no hyper mapping
	ReleaseKey(0, 13);
	ReleaseKey(2, 12);
	ReleaseKey(5, 15);
	ReleaseKey(0, 12);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC },
		{ 0, KEY_ESC, KEY_ENTER },
		{ 0, KEY_ESC },
		{}
	});
}

TEST_METHOD(HyperShift)
{
	PressKey(5, 15); //Hyper Key
	PressKey(3, 16); //F => {
	PressKey(2, 12); //Tab => Enter
	PressKey(3, 17); //G => <
	ReleaseKey(3, 17);
	ReleaseKey(2, 12);
	ReleaseKey(3, 16);
	mockArduino.AssertKeyboardReports({
		{ KEY_LEFT_SHIFT, KEY_LEFT_BRACE },
		{ 0, KEY_LEFT_BRACE, KEY_ENTER },
		{ KEY_LEFT_SHIFT, KEY_LEFT_BRACE, KEY_ENTER, KEY_COMMA },
		{ 0, KEY_LEFT_BRACE, KEY_ENTER },
		{ KEY_LEFT_SHIFT, KEY_LEFT_BRACE },
		{}
	});
}

TEST_METHOD(ConsecutiveHyperKeysMappedToSameOriginalKey)
{
	PressKey(5, 15); //Hyper Key
	PressKey(3, 16); //F => {
	PressKey(3, 14); //S => [
	mockArduino.AssertKeyboardReports({
		{ KEY_LEFT_SHIFT, KEY_LEFT_BRACE },
		//Not sure if OS keyboard driver supports this.
		{ 0, KEY_LEFT_BRACE, KEY_LEFT_BRACE }
	});
}

END_TESTS()
