#include "TestHelpers.h"
#include "Arduino.h"
#include "MockArduino.h"
#include <stdexcept>
using namespace std;

int GetFingerRow(Finger finger) {
	switch (finger) {
		case Finger::PinkyExtraTop:
		case Finger::PinkyExtraBottom:
			return 7;
		case Finger::PinkyTop:
		case Finger::PinkyHome:
		case Finger::PinkyBottom:
			return 6;
		case Finger::RingTop:
		case Finger::RingHome:
		case Finger::RingBottom:
			return 5;
		case Finger::MiddleTop:
		case Finger::MiddleHome:
		case Finger::MiddleBottom:
			return 4;
		case Finger::IndexTop:
		case Finger::IndexHome:
		case Finger::IndexBottom:
		case Finger::ThumbGridTopSecond:
			return 3;
		case Finger::IndexExtraTop:
		case Finger::IndexExtraHome:
		case Finger::IndexExtraBottom:
		case Finger::ThumbGridTopFirst:
			return 2;
		case Finger::ThumbInner:
		case Finger::ThumbGridBottomFirst:
			return 1;
		case Finger::ThumbGridBottomSecond:
		case Finger::ThumbOuter:
			return 0;
	}
	throw runtime_error("Invalid finger");
}

int GetFingerColumn(Finger finger) {
	switch (finger) {
		case Finger::PinkyExtraTop:
		case Finger::PinkyTop:
		case Finger::RingTop:
		case Finger::MiddleTop:
		case Finger::IndexTop:
		case Finger::IndexExtraTop:
			return 0;
		case Finger::PinkyExtraBottom:
		case Finger::PinkyHome:
		case Finger::RingHome:
		case Finger::MiddleHome:
		case Finger::IndexHome:
		case Finger::IndexExtraHome:
			return 1;
		case Finger::PinkyBottom:
		case Finger::RingBottom:
		case Finger::MiddleBottom:
		case Finger::IndexBottom:
		case Finger::IndexExtraBottom:
		case Finger::ThumbInner:
		case Finger::ThumbOuter:
			return 2;
		case Finger::ThumbGridTopFirst:
		case Finger::ThumbGridTopSecond:
		case Finger::ThumbGridBottomFirst:
		case Finger::ThumbGridBottomSecond:
			return 3;
	}
	throw runtime_error("Invalid finger");
}

void Loop(int count) {
	for (auto index = 0; index < count; ++index) {
		loop();
	}
}

void PressKey(Hand hand, Finger finger, int loopCount) {
	mockArduino.PressKey(hand == Hand::Left, GetFingerRow(finger), GetFingerColumn(finger));
	Loop(loopCount);
}

void ReleaseKey(Hand hand, Finger finger, int loopCount) {
	mockArduino.ReleaseKey(hand == Hand::Left, GetFingerRow(finger), GetFingerColumn(finger));
	Loop(loopCount);
}

void ClickKey(Hand hand, Finger finger, function<void()> actionWhilePressed) {
	PressKey(hand, finger);
	actionWhilePressed();
	ReleaseKey(hand, finger);
}

void TestKey(Hand hand, Finger finger, unsigned int keyCode, unsigned int modifiers) {
	ClickKey(hand, finger);
	mockArduino.AssertKeyboardReports({
		{ modifiers, keyCode },
		{}
	});
}
