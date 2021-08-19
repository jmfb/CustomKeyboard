#include <functional>
using namespace std;

enum class Hand {
	Left,
	Right
};

enum class Finger {
	PinkyExtraTop,
	PinkyExtraBottom,
	PinkyTop,
	PinkyHome,
	PinkyBottom,
	RingTop,
	RingHome,
	RingBottom,
	MiddleTop,
	MiddleHome,
	MiddleBottom,
	IndexTop,
	IndexHome,
	IndexBottom,
	IndexExtraTop,
	IndexExtraHome,
	IndexExtraBottom,
	ThumbGridTopFirst,
	ThumbGridTopSecond,
	ThumbGridBottomFirst,
	ThumbGridBottomSecond,
	ThumbInner,
	ThumbOuter
};

void Loop(int count);
void PressKey(Hand hand, Finger finger, int loopCount = 3);
void ReleaseKey(Hand hand, Finger finger, int loopCount = 3);
void ClickKey(Hand hand, Finger finger, function<void()> actionWhilePressed = [](){});
void TestKey(Hand hand, Finger finger, unsigned int keyCode, unsigned int modifiers = 0);
