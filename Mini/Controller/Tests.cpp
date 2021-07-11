#include "TestHelpers.h"
#include "MockArduino.h"
#include <iostream>
#include <exception>
#include <functional>
using namespace std;

class Test {
public:
	Test(const char* name, function<void()> testFunction) :
		name(name),
		testFunction(testFunction) {
	}

	bool Run() const {
		try {
			mockArduino.ClearPressedKeys();
			Loop(3);
			mockArduino.Initialize();
			testFunction();
			return true;
		} catch (const exception& error) {
			cout << name << ": " << error.what() << '\n';
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
	PressKey(Hand::Left, Finger::PinkyExtraTop, 2);
	ReleaseKey(Hand::Left, Finger::PinkyExtraTop, 1);
	mockArduino.AssertKeyboardReports({});
}

TEST_METHOD(RegisterStableKey) {
	PressKey(Hand::Left, Finger::PinkyExtraTop, 3);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC }
	});
}

TEST_METHOD(ReleaseKey) {
	TestKey(Hand::Left, Finger::PinkyExtraTop, KEY_ESC);
}

TEST_METHOD(RetainKeyPressOrder) {
	PressKey(Hand::Left, Finger::PinkyExtraTop);
	PressKey(Hand::Left, Finger::PinkyHome);
	PressKey(Hand::Left, Finger::IndexExtraBottom);
	ReleaseKey(Hand::Left, Finger::PinkyHome);
	PressKey(Hand::Left, Finger::MiddleBottom);
	ReleaseKey(Hand::Left, Finger::PinkyExtraTop);
	ReleaseKey(Hand::Left, Finger::MiddleBottom);
	ReleaseKey(Hand::Left, Finger::IndexExtraBottom);
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
	ClickKey(Hand::Right, Finger::ThumbOuter, [](){
		ClickKey(Hand::Right, Finger::IndexHome);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_F5 },
		{}
	});
}

TEST_METHOD(Layer1SyntheticKey) {
	TestKey(Hand::Right, Finger::ThumbOuter, KEY_MINUS, KEY_RIGHT_SHIFT);
}

TEST_METHOD(Layer2Shift) {
	ClickKey(Hand::Left, Finger::ThumbOuter, [](){
		ClickKey(Hand::Right, Finger::IndexHome);
	});
	mockArduino.AssertKeyboardReports({
		{ KEY_RIGHT_SHIFT, KEY_RIGHT_BRACE },
		{}
	});
}

TEST_METHOD(Layer2SyntheticKey) {
	TestKey(Hand::Left, Finger::ThumbOuter, KEY_SPACE);
}

TEST_METHOD(Layer3Shift) {
	ClickKey(Hand::Right, Finger::PinkyHome, [](){
		ClickKey(Hand::Right, Finger::IndexHome);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_LEFT },
		{}
	});
}

TEST_METHOD(Layer3SyntheticKey) {
	TestKey(Hand::Right, Finger::PinkyHome, KEY_SEMICOLON);
}

TEST_METHOD(Layer4Shift) {
	ClickKey(Hand::Left, Finger::ThumbGridBottomSecond, [](){
		ClickKey(Hand::Left, Finger::PinkyHome);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ENTER },
		{}
	});
}

TEST_METHOD(Layer4SyntheticKey) {
	TestKey(Hand::Left, Finger::ThumbGridBottomSecond, KEY_MENU);
}

END_TESTS()
