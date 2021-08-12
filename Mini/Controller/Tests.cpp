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
		{ 0, KEY_F5 },
		{}
	});
}

TEST_METHOD(Layer4SyntheticKey) {
	TestKey(Hand::Left, Finger::ThumbGridBottomSecond, KEY_MENU);
}

TEST_METHOD(LayerKeyShiftPlusNonShiftInRapidSuccession) {
	ClickKey(Hand::Left, Finger::ThumbOuter, [](){
		PressKey(Hand::Right, Finger::MiddleHome);
		PressKey(Hand::Right, Finger::RingHome);
		ReleaseKey(Hand::Right, Finger::MiddleHome);
		ReleaseKey(Hand::Right, Finger::RingHome);
	});
	mockArduino.AssertKeyboardReports({
		{ KEY_RIGHT_SHIFT, KEY_0 },
		{ 0, KEY_0, KEY_RIGHT_BRACE }, // Ignore shift for first layer key on subsequent layer keys
		{ 0, KEY_RIGHT_BRACE },
		{}
	});
}

TEST_METHOD(LayerKeysWithSameKeyCodeInRapidSuccession) {
	ClickKey(Hand::Left, Finger::ThumbOuter, [](){
		PressKey(Hand::Right, Finger::IndexHome);
		PressKey(Hand::Right, Finger::RingHome);
		ReleaseKey(Hand::Right, Finger::IndexHome);
		ReleaseKey(Hand::Right, Finger::RingHome);
	});
	mockArduino.AssertKeyboardReports({
		{ KEY_RIGHT_SHIFT, KEY_RIGHT_BRACE },
		{ KEY_RIGHT_SHIFT }, // Early release for first layer key sharing subsequent layer key keyCode
		{ 0, KEY_RIGHT_BRACE },
		{}
	});
}

TEST_METHOD(SemicolonFollowedByRapidSpace) {
	PressKey(Hand::Right, Finger::PinkyHome);
	PressKey(Hand::Right, Finger::ThumbInner);
	ReleaseKey(Hand::Right, Finger::PinkyHome);
	ReleaseKey(Hand::Right, Finger::ThumbInner);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_SEMICOLON },
		{},
		{ 0, KEY_SPACE },
		{}
	});
}

TEST_METHOD(AllLayerKeys) {
	ClickKey(Hand::Left, Finger::PinkyExtraTop);
	ClickKey(Hand::Left, Finger::PinkyExtraBottom);
	ClickKey(Hand::Left, Finger::ThumbInner);
	ClickKey(Hand::Left, Finger::ThumbOuter);
	ClickKey(Hand::Left, Finger::ThumbGridTopFirst);
	ClickKey(Hand::Left, Finger::ThumbGridTopSecond);
	ClickKey(Hand::Left, Finger::ThumbGridBottomFirst);
	ClickKey(Hand::Left, Finger::ThumbGridBottomSecond);

	ClickKey(Hand::Right, Finger::PinkyExtraTop);
	ClickKey(Hand::Right, Finger::PinkyExtraBottom);
	ClickKey(Hand::Right, Finger::ThumbInner);
	ClickKey(Hand::Right, Finger::ThumbOuter);
	ClickKey(Hand::Right, Finger::ThumbGridTopFirst);
	ClickKey(Hand::Right, Finger::ThumbGridTopSecond);
	ClickKey(Hand::Right, Finger::ThumbGridBottomFirst);
	ClickKey(Hand::Right, Finger::ThumbGridBottomSecond);

	mockArduino.AssertKeyboardReports({
		{ 0, KEY_ESC }, {},
		{ 0, KEY_TAB }, {},
		{ KEY_LEFT_SHIFT }, {},
		{ 0, KEY_SPACE }, {},
		{ KEY_LEFT_CTRL }, {},
		{ KEY_LEFT_GUI }, {},
		{ KEY_LEFT_ALT }, {},
		{ 0, KEY_MENU }, {},

		{ 0, KEY_BACKSPACE }, {},
		{ 0, KEY_ENTER }, {},
		{ 0, KEY_SPACE }, {},
		{ KEY_RIGHT_SHIFT, KEY_MINUS }, {},
		{ KEY_RIGHT_CTRL }, {},
		{ KEY_RIGHT_GUI }, {},
		{ KEY_RIGHT_ALT }, {},
		{ 0, KEY_MENU }, {}
	});
}

TEST_METHOD(Layer0LeftHand) {
	ClickKey(Hand::Left, Finger::PinkyTop);
	ClickKey(Hand::Left, Finger::RingTop);
	ClickKey(Hand::Left, Finger::MiddleTop);
	ClickKey(Hand::Left, Finger::IndexTop);
	ClickKey(Hand::Left, Finger::IndexExtraTop);
	ClickKey(Hand::Left, Finger::PinkyHome);
	ClickKey(Hand::Left, Finger::RingHome);
	ClickKey(Hand::Left, Finger::MiddleHome);
	ClickKey(Hand::Left, Finger::IndexHome);
	ClickKey(Hand::Left, Finger::IndexExtraHome);
	ClickKey(Hand::Left, Finger::PinkyBottom);
	ClickKey(Hand::Left, Finger::RingBottom);
	ClickKey(Hand::Left, Finger::MiddleBottom);
	ClickKey(Hand::Left, Finger::IndexBottom);
	ClickKey(Hand::Left, Finger::IndexExtraBottom);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_Q }, {},
		{ 0, KEY_W }, {},
		{ 0, KEY_E }, {},
		{ 0, KEY_R }, {},
		{ 0, KEY_T }, {},
		{ 0, KEY_A }, {},
		{ 0, KEY_S }, {},
		{ 0, KEY_D }, {},
		{ 0, KEY_F }, {},
		{ 0, KEY_G }, {},
		{ 0, KEY_Z }, {},
		{ 0, KEY_X }, {},
		{ 0, KEY_C }, {},
		{ 0, KEY_V }, {},
		{ 0, KEY_B }, {}
	});
}

TEST_METHOD(Layer0RightHand) {
	ClickKey(Hand::Right, Finger::IndexExtraTop);
	ClickKey(Hand::Right, Finger::IndexTop);
	ClickKey(Hand::Right, Finger::MiddleTop);
	ClickKey(Hand::Right, Finger::RingTop);
	ClickKey(Hand::Right, Finger::PinkyTop);
	ClickKey(Hand::Right, Finger::IndexExtraHome);
	ClickKey(Hand::Right, Finger::IndexHome);
	ClickKey(Hand::Right, Finger::MiddleHome);
	ClickKey(Hand::Right, Finger::RingHome);
	ClickKey(Hand::Right, Finger::PinkyHome);
	ClickKey(Hand::Right, Finger::IndexExtraBottom);
	ClickKey(Hand::Right, Finger::IndexBottom);
	ClickKey(Hand::Right, Finger::MiddleBottom);
	ClickKey(Hand::Right, Finger::RingBottom);
	ClickKey(Hand::Right, Finger::PinkyBottom);
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_Y }, {},
		{ 0, KEY_U }, {},
		{ 0, KEY_I }, {},
		{ 0, KEY_O }, {},
		{ 0, KEY_P }, {},
		{ 0, KEY_H }, {},
		{ 0, KEY_J }, {},
		{ 0, KEY_K }, {},
		{ 0, KEY_L }, {},
		{ 0, KEY_SEMICOLON }, {},
		{ 0, KEY_N }, {},
		{ 0, KEY_M }, {},
		{ 0, KEY_COMMA }, {},
		{ 0, KEY_PERIOD }, {},
		{ 0, KEY_SLASH }, {}
	});
}

TEST_METHOD(Layer1LeftHand) {
	ClickKey(Hand::Right, Finger::ThumbOuter, [](){
		ClickKey(Hand::Left, Finger::PinkyTop);
		ClickKey(Hand::Left, Finger::RingTop);
		ClickKey(Hand::Left, Finger::MiddleTop);
		ClickKey(Hand::Left, Finger::IndexTop);
		ClickKey(Hand::Left, Finger::IndexExtraTop);
		ClickKey(Hand::Left, Finger::PinkyHome);
		ClickKey(Hand::Left, Finger::RingHome);
		ClickKey(Hand::Left, Finger::MiddleHome);
		ClickKey(Hand::Left, Finger::IndexHome);
		ClickKey(Hand::Left, Finger::IndexExtraHome);
		ClickKey(Hand::Left, Finger::PinkyBottom);
		ClickKey(Hand::Left, Finger::RingBottom);
		ClickKey(Hand::Left, Finger::MiddleBottom);
		ClickKey(Hand::Left, Finger::IndexBottom);
		ClickKey(Hand::Left, Finger::IndexExtraBottom);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEYPAD_ASTERIX }, {},
		{ 0, KEYPAD_7 }, {},
		{ 0, KEYPAD_8 }, {},
		{ 0, KEYPAD_9 }, {},
		{ 0, KEYPAD_PLUS }, {},
		{ 0, KEYPAD_0 }, {},
		{ 0, KEYPAD_4 }, {},
		{ 0, KEYPAD_5 }, {},
		{ 0, KEYPAD_6 }, {},
		{ 0, KEYPAD_PERIOD }, {},
		{ 0, KEYPAD_SLASH }, {},
		{ 0, KEYPAD_1 }, {},
		{ 0, KEYPAD_2 }, {},
		{ 0, KEYPAD_3 }, {},
		{ 0, KEYPAD_MINUS }, {}
	});
}

TEST_METHOD(Layer1RightHand) {
	ClickKey(Hand::Right, Finger::ThumbOuter, [](){
		ClickKey(Hand::Right, Finger::IndexExtraTop);
		ClickKey(Hand::Right, Finger::IndexTop);
		ClickKey(Hand::Right, Finger::MiddleTop);
		ClickKey(Hand::Right, Finger::RingTop);
		ClickKey(Hand::Right, Finger::PinkyTop);
		ClickKey(Hand::Right, Finger::IndexExtraHome);
		ClickKey(Hand::Right, Finger::IndexHome);
		ClickKey(Hand::Right, Finger::MiddleHome);
		ClickKey(Hand::Right, Finger::RingHome);
		ClickKey(Hand::Right, Finger::PinkyHome);
		ClickKey(Hand::Right, Finger::IndexExtraBottom);
		ClickKey(Hand::Right, Finger::IndexBottom);
		ClickKey(Hand::Right, Finger::MiddleBottom);
		ClickKey(Hand::Right, Finger::RingBottom);
		ClickKey(Hand::Right, Finger::PinkyBottom);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_PRINTSCREEN }, {},
		{ 0, KEY_F1 }, {},
		{ 0, KEY_F2 }, {},
		{ 0, KEY_F3 }, {},
		{ 0, KEY_F4 }, {},
		{ 0, KEY_SCROLL_LOCK }, {},
		{ 0, KEY_F5 }, {},
		{ 0, KEY_F6 }, {},
		{ 0, KEY_F7 }, {},
		{ 0, KEY_F8 }, {},
		{ 0, KEY_PAUSE }, {},
		{ 0, KEY_F9 }, {},
		{ 0, KEY_F10 }, {},
		{ 0, KEY_F11 }, {},
		{ 0, KEY_F12 }, {}
	});
}

TEST_METHOD(Layer2LeftHand) {
	ClickKey(Hand::Left, Finger::ThumbOuter, [](){
		ClickKey(Hand::Left, Finger::PinkyTop);
		ClickKey(Hand::Left, Finger::RingTop);
		ClickKey(Hand::Left, Finger::MiddleTop);
		ClickKey(Hand::Left, Finger::IndexTop);
		ClickKey(Hand::Left, Finger::IndexExtraTop);
		ClickKey(Hand::Left, Finger::PinkyHome);
		ClickKey(Hand::Left, Finger::RingHome);
		ClickKey(Hand::Left, Finger::MiddleHome);
		ClickKey(Hand::Left, Finger::IndexHome);
		ClickKey(Hand::Left, Finger::IndexExtraHome);
		ClickKey(Hand::Left, Finger::PinkyBottom);
		ClickKey(Hand::Left, Finger::RingBottom);
		ClickKey(Hand::Left, Finger::MiddleBottom);
		ClickKey(Hand::Left, Finger::IndexBottom);
		ClickKey(Hand::Left, Finger::IndexExtraBottom);
	});
	mockArduino.AssertKeyboardReports({
		{ KEY_LEFT_SHIFT, KEY_TILDE }, {},
		{ KEY_LEFT_SHIFT, KEY_1 }, {},
		{ KEY_RIGHT_SHIFT, KEY_7 }, {},
		{ KEY_RIGHT_SHIFT, KEY_BACKSLASH }, {},
		{ KEY_LEFT_SHIFT, KEY_3 }, {},
		{ KEY_RIGHT_SHIFT, KEY_COMMA }, {},
		{ 0, KEY_LEFT_BRACE }, {},
		{ KEY_RIGHT_SHIFT, KEY_9 }, {},
		{ KEY_RIGHT_SHIFT, KEY_LEFT_BRACE }, {},
		// Nothing
		// Nothing
		{ KEY_LEFT_SHIFT, KEY_6 }, {},
		{ KEY_LEFT_SHIFT, KEY_5 }, {},
		{ KEY_RIGHT_SHIFT, KEY_8 }, {}
		// Nothing
	});
}

TEST_METHOD(Layer2RightHand) {
	ClickKey(Hand::Left, Finger::ThumbOuter, [](){
		ClickKey(Hand::Right, Finger::IndexExtraTop);
		ClickKey(Hand::Right, Finger::IndexTop);
		ClickKey(Hand::Right, Finger::MiddleTop);
		ClickKey(Hand::Right, Finger::RingTop);
		ClickKey(Hand::Right, Finger::PinkyTop);
		ClickKey(Hand::Right, Finger::IndexExtraHome);
		ClickKey(Hand::Right, Finger::IndexHome);
		ClickKey(Hand::Right, Finger::MiddleHome);
		ClickKey(Hand::Right, Finger::RingHome);
		ClickKey(Hand::Right, Finger::PinkyHome);
		ClickKey(Hand::Right, Finger::IndexExtraBottom);
		ClickKey(Hand::Right, Finger::IndexBottom);
		ClickKey(Hand::Right, Finger::MiddleBottom);
		ClickKey(Hand::Right, Finger::RingBottom);
		ClickKey(Hand::Right, Finger::PinkyBottom);
	});
	mockArduino.AssertKeyboardReports({
		{ KEY_LEFT_SHIFT, KEY_4 }, {},
		{ KEY_RIGHT_SHIFT, KEY_QUOTE }, {},
		{ 0, KEY_QUOTE }, {},
		{ 0, KEY_TILDE }, {},
		{ KEY_LEFT_SHIFT, KEY_2 }, {},
		// Nothing
		{ KEY_RIGHT_SHIFT, KEY_RIGHT_BRACE }, {},
		{ KEY_RIGHT_SHIFT, KEY_0 }, {},
		{ 0, KEY_RIGHT_BRACE }, {},
		{ KEY_RIGHT_SHIFT, KEY_PERIOD }, {},
		// Nothing
		{ KEY_RIGHT_SHIFT, KEY_EQUAL }, {},
		{ 0, KEY_MINUS }, {},
		{ 0, KEY_EQUAL }, {},
		{ 0, KEY_BACKSLASH }, {}
	});
}

TEST_METHOD(Layer3LeftHand) {
	ClickKey(Hand::Right, Finger::PinkyHome, [](){
		ClickKey(Hand::Left, Finger::PinkyTop);
		ClickKey(Hand::Left, Finger::RingTop);
		ClickKey(Hand::Left, Finger::MiddleTop);
		ClickKey(Hand::Left, Finger::IndexTop);
		ClickKey(Hand::Left, Finger::IndexExtraTop);
		ClickKey(Hand::Left, Finger::PinkyHome);
		ClickKey(Hand::Left, Finger::RingHome);
		ClickKey(Hand::Left, Finger::MiddleHome);
		ClickKey(Hand::Left, Finger::IndexHome);
		ClickKey(Hand::Left, Finger::IndexExtraHome);
		ClickKey(Hand::Left, Finger::PinkyBottom);
		ClickKey(Hand::Left, Finger::RingBottom);
		ClickKey(Hand::Left, Finger::MiddleBottom);
		ClickKey(Hand::Left, Finger::IndexBottom);
		ClickKey(Hand::Left, Finger::IndexExtraBottom);
	});
	mockArduino.AssertKeyboardReports({
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing

		// NOTE: Since there are not currently any L.H. Layer 3 key mappings, this test will produce SEMICOLON
		{ 0, KEY_SEMICOLON }, {}
	});
}

TEST_METHOD(Layer3RightHand) {
	ClickKey(Hand::Right, Finger::PinkyHome, [](){
		ClickKey(Hand::Right, Finger::IndexExtraTop);
		ClickKey(Hand::Right, Finger::IndexTop);
		ClickKey(Hand::Right, Finger::MiddleTop);
		ClickKey(Hand::Right, Finger::RingTop);
		ClickKey(Hand::Right, Finger::PinkyTop);
		ClickKey(Hand::Right, Finger::IndexExtraHome);
		ClickKey(Hand::Right, Finger::IndexHome);
		ClickKey(Hand::Right, Finger::MiddleHome);
		ClickKey(Hand::Right, Finger::RingHome);
		// Nope: ClickKey(Hand::Right, Finger::PinkyHome);
		ClickKey(Hand::Right, Finger::IndexExtraBottom);
		ClickKey(Hand::Right, Finger::IndexBottom);
		ClickKey(Hand::Right, Finger::MiddleBottom);
		ClickKey(Hand::Right, Finger::RingBottom);
		ClickKey(Hand::Right, Finger::PinkyBottom);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_INSERT }, {},
		{ 0, KEY_HOME }, {},
		{ 0, KEY_UP }, {},
		{ 0, KEY_END }, {},
		// Nothing
		{ 0, KEY_DELETE }, {},
		{ 0, KEY_LEFT }, {},
		{ 0, KEY_DOWN }, {},
		{ 0, KEY_RIGHT }, {},
		// Nothing
		// Nothing
		{ 0, KEY_PAGE_UP }, {},
		{ 0, KEY_PAGE_DOWN }, {},
		// Nothing
		// Nothing
	});
}

TEST_METHOD(Layer4LeftHand) {
	ClickKey(Hand::Left, Finger::ThumbGridBottomSecond, [](){
		ClickKey(Hand::Left, Finger::PinkyTop);
		ClickKey(Hand::Left, Finger::RingTop);
		ClickKey(Hand::Left, Finger::MiddleTop);
		ClickKey(Hand::Left, Finger::IndexTop);
		ClickKey(Hand::Left, Finger::IndexExtraTop);
		ClickKey(Hand::Left, Finger::PinkyHome);
		ClickKey(Hand::Left, Finger::RingHome);
		ClickKey(Hand::Left, Finger::MiddleHome);
		ClickKey(Hand::Left, Finger::IndexHome);
		ClickKey(Hand::Left, Finger::IndexExtraHome);
		ClickKey(Hand::Left, Finger::PinkyBottom);
		ClickKey(Hand::Left, Finger::RingBottom);
		ClickKey(Hand::Left, Finger::MiddleBottom);
		ClickKey(Hand::Left, Finger::IndexBottom);
		ClickKey(Hand::Left, Finger::IndexExtraBottom);
	});
	mockArduino.AssertKeyboardReports({
		{ 0, KEY_F1 }, {},
		{ 0, KEY_F2 }, {},
		{ 0, KEY_F3 }, {},
		{ 0, KEY_F4 }, {},
		{ 0, KEY_BACKSPACE }, {},
		{ 0, KEY_F5 }, {},
		{ 0, KEY_F6 }, {},
		{ 0, KEY_F7 }, {},
		{ 0, KEY_F8 }, {},
		{ 0, KEY_ENTER }, {},
		{ 0, KEY_F9 }, {},
		{ 0, KEY_F10 }, {},
		{ 0, KEY_F11 }, {},
		{ 0, KEY_F12 }, {}
		// Nothing
	});
}

TEST_METHOD(Layer4RightHand) {
	ClickKey(Hand::Left, Finger::ThumbGridBottomSecond, [](){
		ClickKey(Hand::Right, Finger::IndexExtraTop);
		ClickKey(Hand::Right, Finger::IndexTop);
		ClickKey(Hand::Right, Finger::MiddleTop);
		ClickKey(Hand::Right, Finger::RingTop);
		ClickKey(Hand::Right, Finger::PinkyTop);
		ClickKey(Hand::Right, Finger::IndexExtraHome);
		ClickKey(Hand::Right, Finger::IndexHome);
		ClickKey(Hand::Right, Finger::MiddleHome);
		ClickKey(Hand::Right, Finger::RingHome);
		ClickKey(Hand::Right, Finger::PinkyHome);
		ClickKey(Hand::Right, Finger::IndexExtraBottom);
		ClickKey(Hand::Right, Finger::IndexBottom);
		ClickKey(Hand::Right, Finger::MiddleBottom);
		ClickKey(Hand::Right, Finger::RingBottom);
		ClickKey(Hand::Right, Finger::PinkyBottom);
	});
	mockArduino.AssertKeyboardReports({
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing

		// NOTE: Since there are not currently any R.H. Layer 4 key mappings, this test will produce MENU
		{ 0, KEY_MENU }, {}
	});
}

TEST_METHOD(Layer5LeftHand) {
	ClickKey(Hand::Left, Finger::ThumbInner, [](){
		ClickKey(Hand::Left, Finger::ThumbOuter, [](){
			ClickKey(Hand::Left, Finger::PinkyTop);
			ClickKey(Hand::Left, Finger::RingTop);
			ClickKey(Hand::Left, Finger::MiddleTop);
			ClickKey(Hand::Left, Finger::IndexTop);
			ClickKey(Hand::Left, Finger::IndexExtraTop);
			ClickKey(Hand::Left, Finger::PinkyHome);
			ClickKey(Hand::Left, Finger::RingHome);
			ClickKey(Hand::Left, Finger::MiddleHome);
			ClickKey(Hand::Left, Finger::IndexHome);
			ClickKey(Hand::Left, Finger::IndexExtraHome);
			ClickKey(Hand::Left, Finger::PinkyBottom);
			ClickKey(Hand::Left, Finger::RingBottom);
			ClickKey(Hand::Left, Finger::MiddleBottom);
			ClickKey(Hand::Left, Finger::IndexBottom);
			ClickKey(Hand::Left, Finger::IndexExtraBottom);
		});
	});
	mockArduino.AssertKeyboardReports({
		{ KEY_LEFT_SHIFT }, {},
		// Nothing
		// Nothing
		{ KEY_LEFT_CTRL | KEY_LEFT_GUI, KEY_F4 }, {},
		// Nothing
		// Nothing
		{ KEY_LEFT_CTRL | KEY_LEFT_ALT, KEY_DELETE }, {},
		{ KEY_LEFT_CTRL | KEY_LEFT_GUI, KEY_LEFT }, {},
		{ KEY_LEFT_CTRL | KEY_LEFT_GUI, KEY_D }, {},
		{ KEY_LEFT_CTRL | KEY_LEFT_GUI, KEY_RIGHT }, {},
		{ KEY_LEFT_CTRL | KEY_LEFT_SHIFT, KEY_ESC }, {},
		// Nothing
		// Nothing
		{ KEY_LEFT_SHIFT | KEY_LEFT_GUI, KEY_S }, {},
		// Nothing
		// Nothing
		{ KEY_LEFT_SHIFT }, {}
	});
}

TEST_METHOD(Layer5RightHand) {
	ClickKey(Hand::Left, Finger::ThumbInner, [](){
		ClickKey(Hand::Left, Finger::ThumbOuter, [](){
			ClickKey(Hand::Right, Finger::IndexExtraTop);
			ClickKey(Hand::Right, Finger::IndexTop);
			ClickKey(Hand::Right, Finger::MiddleTop);
			ClickKey(Hand::Right, Finger::RingTop);
			ClickKey(Hand::Right, Finger::PinkyTop);
			ClickKey(Hand::Right, Finger::IndexExtraHome);
			ClickKey(Hand::Right, Finger::IndexHome);
			ClickKey(Hand::Right, Finger::MiddleHome);
			ClickKey(Hand::Right, Finger::RingHome);
			ClickKey(Hand::Right, Finger::PinkyHome);
			ClickKey(Hand::Right, Finger::IndexExtraBottom);
			ClickKey(Hand::Right, Finger::IndexBottom);
			ClickKey(Hand::Right, Finger::MiddleBottom);
			ClickKey(Hand::Right, Finger::RingBottom);
			ClickKey(Hand::Right, Finger::PinkyBottom);
		});
	});
	mockArduino.AssertKeyboardReports({
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing
		// Nothing

		// NOTE: Since there are not currently any R.H. Layer 5 key mappings, this test will produce SHIFT/SHIFT
		{ KEY_LEFT_SHIFT }, {},
		{ KEY_LEFT_SHIFT }, {}
	});
}

END_TESTS()
