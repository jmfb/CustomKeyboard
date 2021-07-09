#include "Arduino.h"
#include "MockArduino.h"
#include <iostream>
#include <exception>
#include <functional>
using namespace std;

void Loop(int count) {
	for (auto index = 0; index < count; ++index) {
		loop();
	}
}

void PressKey(bool left, int row, int column) {
	mockArduino.PressKey(left, row, column);
	Loop(3);
}

void ReleaseKey(bool left, int row, int column) {
	mockArduino.ReleaseKey(left, row, column);
	Loop(3);
}

class Test {
public:
	Test(const char* name, function<void()> testFunction)
		: name(name),
		testFunction(testFunction) {
	}

	bool Run() const {
		try {
			cout << name << '\n';
			mockArduino.ClearPressedKeys();
			Loop(3);
			mockArduino.Initialize();
			testFunction();
			return true;
		} catch (const exception& error) {
			cout << error.what() << '\n';
			return false;
		}
	}

	const char* name;
	function<void()> testFunction;
};

extern vector<Test> tests;

int main() {
	auto passed = 0;
	auto failed = 0;
	for (auto& test : tests) {
		if (test.Run()) {
			++passed;
		} else {
			++failed;
		}
	}
	cout << (passed - 1) << " passed, " << failed << " failed.\n"
		<< "Test run " << (failed > 0 ? "failed" : "succeeded") << ".\n";
	return failed > 0 ? 1 : 0;
}

#define BEGIN_TESTS() vector<Test> tests{ { "", [](){}
#define TEST_METHOD(name) }, { #name, []()
#define END_TESTS() } };

BEGIN_TESTS()

TEST_METHOD(InitializePinModesAndSignals) {
	setup();
	mockArduino.AssertPinModes({
		// First 7 pins marked as output
		OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT, OUTPUT,
		// Next 2 receive pins marked input
		INPUT, INPUT
	});
}

TEST_METHOD(DoNotRegisterKeysPressedForFewerThanThreeScans) {
	mockArduino.PressKey(true, 7, 0);
	Loop(2);
	mockArduino.ReleaseKey(true, 7, 0);
	Loop(1);
	mockArduino.AssertKeyboardReports({});
}

TEST_METHOD(RegisterStableKey) {
	mockArduino.PressKey(true, 7, 0);
	Loop(3);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC }
	});
}

TEST_METHOD(ReleaseKey) {
	PressKey(true, 7, 0);
	ReleaseKey(true, 7, 0);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC },
		{}
	});
}

TEST_METHOD(RetainKeyPressOrder) {
	PressKey(true, 7, 0); //Esc
	PressKey(true, 6, 1); //A
	PressKey(true, 2, 2); //B
	ReleaseKey(true, 6, 1);
	PressKey(true, 4, 2); //C
	ReleaseKey(true, 7, 0);
	ReleaseKey(true, 4, 2);
	ReleaseKey(true, 2, 2);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC },
		{ 0, KEY_ESC, KEY_A },
		{ 0, KEY_ESC, KEY_A, KEY_B },
		{ 0, KEY_ESC, KEY_B },
		{ 0, KEY_ESC, KEY_B, KEY_C },
		{ 0, KEY_B, KEY_C },
		{ 0, KEY_B },
		{}
	});
}

TEST_METHOD(Layer1Shift) {
	PressKey(false, 0, 2);
	PressKey(false, 3, 1);
	ReleaseKey(false, 3, 1);
	ReleaseKey(false, 0, 2);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_F5 },
		{}
	});
}

TEST_METHOD(Layer1SyntheticKey) {
	PressKey(false, 0, 2);
	ReleaseKey(false, 0, 2);
	mockArduino.AssertKeyboardReports({
		{ KEY_RIGHT_SHIFT, KEY_MINUS },
		{}
	});
}

END_TESTS()
