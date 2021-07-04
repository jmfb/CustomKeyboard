#include "Arduino.h"

enum class Pins : uint8_t {
	Select0LeftHand = 0,
	LedDataLeftHand = 0,
	Select0RightHand = 1,
	LedDataRightHand = 1,
	Select1 = 2,
	Select2 = 3,
	ShiftLoad = 4,
	ColumnClock = 5,
	LedClock = 6,
	ReceiveLeftHand = 7,
	ReceiveRightHand = 8
};

void SetUpOutputPin(Pins pin) {
	pinMode(static_cast<uint8_t>(pin), OUTPUT);
	digitalWrite(static_cast<uint8_t>(pin), LOW);
}

void SetUpInputPin(Pins pin) {
	pinMode(static_cast<uint8_t>(pin), INPUT);
}

void setup() {
	SetUpOutputPin(Pins::Select0LeftHand);
	SetUpOutputPin(Pins::Select0RightHand);
	SetUpOutputPin(Pins::Select1);
	SetUpOutputPin(Pins::Select2);
	SetUpOutputPin(Pins::ShiftLoad);
	SetUpOutputPin(Pins::ColumnClock);
	SetUpOutputPin(Pins::LedClock);
	SetUpInputPin(Pins::ReceiveLeftHand);
	SetUpInputPin(Pins::ReceiveRightHand);
	delay(200);
}

enum class Positions : uint8_t {
	None = 0xff,
	First = 0,
	PinkyExtraTop = First,
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
	ThumbOuter,
	Last = ThumbOuter
};

const auto keyCount = static_cast<uint8_t>(Positions::Last) + 1;

const uint8_t rowCount = 8;
const uint8_t columnCount = 4;

const Positions scanPositions[rowCount][columnCount] = {
	{ Positions::None, Positions::None, Positions::ThumbOuter, Positions::ThumbGridBottomSecond },
	{ Positions::None, Positions::None, Positions::ThumbInner, Positions::ThumbGridBottomFirst },
	{ Positions::IndexExtraTop, Positions::IndexExtraHome, Positions::IndexExtraBottom, Positions::ThumbGridTopFirst },
	{ Positions::IndexTop, Positions::IndexHome, Positions::IndexBottom, Positions::ThumbGridTopSecond },
	{ Positions::MiddleTop, Positions::MiddleHome, Positions::MiddleBottom, Positions::None },
	{ Positions::RingTop, Positions::RingHome, Positions::RingBottom, Positions::None },
	{ Positions::PinkyTop, Positions::PinkyHome, Positions::PinkyBottom, Positions::None },
	{ Positions::PinkyExtraTop, Positions::PinkyExtraBottom, Positions::None, Positions::None }
};

const unsigned int leftHandLayer0[keyCount] = {
	KEY_ESC, KEY_TAB,
	KEY_Q, KEY_A, KEY_Z,
	KEY_W, KEY_S, KEY_X,
	KEY_E, KEY_D, KEY_C,
	KEY_R, KEY_F, KEY_V,
	KEY_T, KEY_G, KEY_B,
	KEY_1, KEY_2,
	KEY_3, KEY_4,
	KEY_SPACE, KEY_5
};

const unsigned int rightHandLayer0[keyCount] = {
	KEY_BACKSPACE, KEY_ENTER,
	KEY_P, KEY_SEMICOLON, KEY_SLASH,
	KEY_O, KEY_L, KEY_PERIOD,
	KEY_I, KEY_K, KEY_COMMA,
	KEY_U, KEY_J, KEY_M,
	KEY_Y, KEY_H, KEY_N,
	KEY_6, KEY_7,
	KEY_8, KEY_9,
	KEY_MINUS, KEY_0
};

#if 0
const int leftHyperKey = 1000;
const int rightHyperKey = 1001;

class HyperKey
{
public:
	HyperKey()
		: keyCode(0), shift(false)
	{
	}

	HyperKey(int keyCode, bool shift)
		: keyCode(keyCode), shift(shift)
	{
	}

	int keyCode;
	bool shift;
};

const HyperKey none;
#endif

class KeyState {
public:
	KeyState() {
		for (uint8_t key = 0; key < keyCount; ++key) {
			leftHand[key] = false;
			rightHand[key] = false;
		}
	}

	void Scan() {
		for (uint8_t row = 0; row < rowCount; ++row) {
			ScanRow(row);
		}
	}

	KeyState operator&&(const KeyState& other) const
	{
		KeyState result;
		for (uint8_t key = 0; key < keyCount; ++key) {
			result.leftHand[key] = leftHand[key] && other.leftHand[key];
			result.rightHand[key] = rightHand[key] && other.rightHand[key];
		}
		return result;
	}

	bool operator==(const KeyState& other) const
	{
		for (uint8_t key = 0; key < keyCount; ++key) {
			if (leftHand[key] != other.leftHand[key] || rightHand[key] != other.rightHand[key]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const KeyState& other) const
	{
		return !operator==(other);
	}

private:
	void ScanRow(uint8_t row) {
		SelectRow(row);
		LoadRegister();
		ShiftRegister();
		for (uint8_t column = 0; column < columnCount; ++column) {
			ScanColumn(row, column);
		}
	}

	void ScanColumn(uint8_t row, uint8_t column) {
		auto leftHandPosition = ReadPosition(Pins::ReceiveLeftHand, row, column);
		if (leftHandPosition != Positions::None) {
			leftHand[static_cast<uint8_t>(leftHandPosition)] = true;
		}

		auto rightHandPosition = ReadPosition(Pins::ReceiveRightHand, row, column);
		if (rightHandPosition != Positions::None) {
			rightHand[static_cast<uint8_t>(rightHandPosition)] = true;
		}

		PulseColumnClock();
	}

	bool ReadColumn(Pins pin) {
		return digitalRead(static_cast<uint8_t>(pin)) == LOW;
	}

	Positions ReadPosition(Pins pin, uint8_t row, uint8_t column) {
		return ReadColumn(pin) ?
			scanPositions[row][column] :
			Positions::None;
	}

	void SelectRow(uint8_t row) {
		auto select0 = (row & 0b001) ? HIGH : LOW;
		auto select1 = (row & 0b010) ? HIGH : LOW;
		auto select2 = (row & 0b100) ? HIGH : LOW;
		digitalWrite(static_cast<uint8_t>(Pins::Select0LeftHand), select0);
		digitalWrite(static_cast<uint8_t>(Pins::Select0RightHand), select0);
		digitalWrite(static_cast<uint8_t>(Pins::Select1), select1);
		digitalWrite(static_cast<uint8_t>(Pins::Select2), select2);
		delay(1);
	}

	void LoadRegister() {
		digitalWrite(static_cast<uint8_t>(Pins::ShiftLoad), LOW);
	}

	void ShiftRegister() {
		digitalWrite(static_cast<uint8_t>(Pins::ShiftLoad), HIGH);
	}

	void PulseColumnClock() {
		digitalWrite(static_cast<uint8_t>(Pins::ColumnClock), HIGH);
		digitalWrite(static_cast<uint8_t>(Pins::ColumnClock), LOW);
	}

private:
	bool leftHand[keyCount];
	bool rightHand[keyCount];
};

const uint8_t debounceCount = 3;

class StableScanner {
public:
	StableScanner() {
		nextIndex = 0;
	}

	KeyState Scan() {
		auto index = nextIndex;
		nextIndex = (nextIndex + 1) % debounceCount;
		keyState[index].Scan();
		return keyState[0] && keyState[1] && keyState[2];
	}

private:
	uint8_t nextIndex;
	KeyState keyState[debounceCount];
};

#if 0
const int maxKeyCount = 6;

class PressedKeys
{
public:
	PressedKeys()
		: count(0)
	{
	}

	void Scan(const PressedKeys& previousPressedKeys, const KeyState& keyState)
	{
		HoldPreviousKeys(previousPressedKeys, keyState);
		if (count < maxKeyCount)
			PressNewKeys(previousPressedKeys, keyState);
	}

	bool operator==(const PressedKeys& other) const
	{
		if (count != other.count)
			return false;
		for (int index = 0; index < count; ++index)
			if (keys[index] != other.keys[index])
				return false;
		return true;
	}
	bool operator!=(const PressedKeys& other) const
	{
		return !operator==(other);
	}

	int GetKeyCode(int index) const
	{
		return GetKeyCode(keys[index].row, keys[index].column);
	}

	HyperKey GetHyperKey(int index) const
	{
		return GetHyperKey(keys[index].row, keys[index].column);
	}

private:
	void HoldPreviousKeys(const PressedKeys& previousPressedKeys, const KeyState& keyState)
	{
		for (int index = 0; index < previousPressedKeys.count; ++index)
			if (keyState.IsPressed(previousPressedKeys.keys[index]))
				Press(previousPressedKeys.keys[index]);
	}

	void PressNewKeys(const PressedKeys& previousPressedKeys, const KeyState& keyState)
	{
		for (int row = 0; row < rowCount; ++row)
			for (int column = 0; column < columnCount; ++column)
				DetectPressedKey(keyState, row, column);
	}

	void DetectPressedKey(const KeyState& keyState, int row, int column)
	{
		PressedKey pressedKey(row, column);
		if (keyState.IsPressed(pressedKey) && !IsPressed(pressedKey))
			Press(pressedKey);
	}

	bool IsPressed(const PressedKey& key) const
	{
		for (int index = 0; index < count; ++index)
			if (keys[index] == key)
				return true;
		return false;
	}

	void Press(const PressedKey& key)
	{
		if (count < maxKeyCount)
			keys[count++] = key;
	}

	static int GetKeyCode(int row, int column)
	{
		if (row < topRowCount)
		{
			if (column < leftHandColumnCount)
				return leftHandKeyMap[row][column];
			return numPadKeyMap[row][column - leftHandColumnCount];
		}
		if (column < rightHandColumnCount)
			return rightHandKeyMap[row - topRowCount][column];
		return arrowKeyKeyMap[row - topRowCount][column - rightHandColumnCount];
	}

	static HyperKey GetHyperKey(int row, int column)
	{
		if (row < topRowCount)
		{
			if (column < leftHandColumnCount)
				return leftHandHyperKeyMap[row][column];
			return numPadHyperKeyMap[row][column - leftHandColumnCount];
		}
		if (column < rightHandColumnCount)
			return rightHandHyperKeyMap[row - topRowCount][column];
		return arrowKeyHyperKeyMap[row - topRowCount][column - rightHandColumnCount];
	}

public:
	int count;

private:
	PressedKey keys[maxKeyCount];
};

class KeyReport
{
public:
	KeyReport()
	{
		modifiers = 0;
		for (int key = 0; key < maxKeyCount; ++key)
			keys[key] = 0;
	}

	void Translate(const PressedKeys& pressedKeys)
	{
		bool hyper = false;
		bool hyperShift = false;
		int nextKey = 0;
		for (int index = 0; index < pressedKeys.count; ++index)
		{
			if (hyper)
			{
				HyperKey hyperKey = pressedKeys.GetHyperKey(index);
				hyperShift = hyperKey.shift;
				keys[nextKey++] = static_cast<uint8_t>(hyperKey.keyCode);
			}
			else
			{
				int keyCode = pressedKeys.GetKeyCode(index);
				//TODO: use different hyper key maps based on which hyper key was pressed.
				if (keyCode == leftHyperKey || keyCode == rightHyperKey)
				{
					hyper = true;
					continue;
				}

				if (IsMacroKey(keyCode))
				{
					//TODO: Handle macro keys differently (multi key, hyper key, etc.)
					int mappedKeyCode = ExpandMacroKey(keyCode);
					if (mappedKeyCode != 0)
						keys[nextKey++] = static_cast<uint8_t>(mappedKeyCode);
					continue;
				}

				if (IsModifier(keyCode))
				{
					modifiers |= keyCode;
					continue;
				}

				keys[nextKey++] = static_cast<uint8_t>(keyCode);
			}
		}
		if (hyperShift)
			modifiers |= KEY_LEFT_SHIFT;
	}

	void Transmit() const
	{
		Keyboard.set_modifier(modifiers);
		Keyboard.set_key1(keys[0]);
		Keyboard.set_key2(keys[1]);
		Keyboard.set_key3(keys[2]);
		Keyboard.set_key4(keys[3]);
		Keyboard.set_key5(keys[4]);
		Keyboard.set_key6(keys[5]);
		Keyboard.send_now();
		delay(2);
	}

	bool operator==(const KeyReport& other) const
	{
		for (int index = 0; index < maxKeyCount; ++index)
			if (keys[index] != other.keys[index])
				return false;
		return modifiers == other.modifiers;
	}
	bool operator!=(const KeyReport& other) const
	{
		return !operator==(other);
	}

private:
	static bool IsModifier(int keyCode)
	{
		switch (keyCode)
		{
		case KEY_LEFT_GUI:
		case KEY_RIGHT_GUI:
		case KEY_LEFT_SHIFT:
		case KEY_RIGHT_SHIFT:
		case KEY_LEFT_ALT:
		case KEY_RIGHT_ALT:
		case KEY_LEFT_CTRL
:		case KEY_RIGHT_CTRL:
			return true;
		}
		return false;
	}

	static bool IsMacroKey(int keyCode)
	{
		return keyCode >= minMacroKey && keyCode <= maxMacroKey;
	}

	int ExpandMacroKey(int keyCode)
	{
		//TODO: Can't use KEY_MEDIA_* on windows.  Found a doc saying I need special HID identifier during device detection.
		switch (keyCode)
		{
		case MKEY_N1: return WindowsKey(KEY_D);
		case MKEY_N2: return WindowsKey(KEY_E);
		case MKEY_N3: return WindowsKey(KEY_R);
		case MKEY_N4: return WindowsKey(KEY_PAUSE);
		}
		return 0;
	}

	int WindowsKey(int keyCode)
	{
		modifiers |= KEY_LEFT_GUI;
		return keyCode;
	}

	int Alt(int keyCode)
	{
		modifiers |= KEY_LEFT_ALT;
		return keyCode;
	}

private:
	uint8_t modifiers;
	uint8_t keys[maxKeyCount];
};
#endif

void loop()
{
	static StableScanner stableScanner;
	static KeyState previousKeyState;
#if 0
	static PressedKeys previousPressedKeys;
	static KeyReport previousKeyReport;
#endif
	KeyState keyState = stableScanner.Scan();
	if (keyState == previousKeyState)
		return;
	previousKeyState = keyState;
#if 0
	PressedKeys pressedKeys;
	pressedKeys.Scan(previousPressedKeys, keyState);
	if (pressedKeys == previousPressedKeys)
		return;
	previousPressedKeys = pressedKeys;
	KeyReport keyReport;
	keyReport.Translate(pressedKeys);
	if (keyReport == previousKeyReport)
		return;
	previousKeyReport = keyReport;
	keyReport.Transmit();
#endif
}
