#include "Arduino.h"

#if defined(TESTS)
#include <iostream>
using namespace std;
#endif

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
	pinMode(static_cast<uint8_t>(pin), INPUT_PULLUP);
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
	delayMicroseconds(10);
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

constexpr auto keyCount = static_cast<uint8_t>(Positions::Last) + 1;

constexpr uint8_t rowCount = 8;
constexpr uint8_t columnCount = 4;

constexpr Positions scanPositions[rowCount][columnCount] = {
	{ Positions::None, Positions::None, Positions::ThumbOuter, Positions::ThumbGridBottomSecond },
	{ Positions::None, Positions::None, Positions::ThumbInner, Positions::ThumbGridBottomFirst },
	{ Positions::IndexExtraTop, Positions::IndexExtraHome, Positions::IndexExtraBottom, Positions::ThumbGridTopFirst },
	{ Positions::IndexTop, Positions::IndexHome, Positions::IndexBottom, Positions::ThumbGridTopSecond },
	{ Positions::MiddleTop, Positions::MiddleHome, Positions::MiddleBottom, Positions::None },
	{ Positions::RingTop, Positions::RingHome, Positions::RingBottom, Positions::None },
	{ Positions::PinkyTop, Positions::PinkyHome, Positions::PinkyBottom, Positions::None },
	{ Positions::PinkyExtraTop, Positions::PinkyExtraBottom, Positions::None, Positions::None }
};

class HandKeyState {
public:
	HandKeyState() {
		for (uint8_t key = 0; key < keyCount; ++key) {
			keys[key] = false;
		}
	}

	void SetKey(uint8_t row, uint8_t column, bool pressed) {
		auto position = scanPositions[row][column];
		if (position != Positions::None) {
			keys[static_cast<uint8_t>(position)] = pressed;
		}
	}

	HandKeyState operator&&(const HandKeyState& other) const {
		HandKeyState result;
		for (uint8_t key = 0; key < keyCount; ++key) {
			result.keys[key] = keys[key] && other.keys[key];
		}
		return result;
	}

	bool operator==(const HandKeyState& other) const {
		for (uint8_t key = 0; key < keyCount; ++key) {
			if (keys[key] != other.keys[key]) {
				return false;
			}
		}
		return true;
	}

	bool operator!=(const HandKeyState& other) const {
		return !operator==(other);
	}

	bool IsPressed(uint8_t key) const {
		if (key < keyCount) {
			return keys[key];
		}
		return false;
	}

private:
	bool keys[keyCount];
};

enum class HardwareConfiguration {
	V1,
	LeftHand,
	RightHand,
	Unplugged
};

class KeyState {
public:
	void Scan() {
		for (uint8_t row = 0; row < rowCount; ++row) {
			ScanRow(row);
			if (row == 0) {
				ScanHardwareConfiguration();
			}
		}
		if (areHandsInverted) {
			SwapScans();
		}
	}

	KeyState operator&&(const KeyState& other) const {
		KeyState result;
		result.leftHand = leftHand && other.leftHand;
		result.rightHand = rightHand && other.rightHand;
		result.areHandsInverted = areHandsInverted && other.areHandsInverted;
		return result;
	}

	bool operator==(const KeyState& other) const {
		return leftHand == other.leftHand &&
			rightHand == other.rightHand &&
			areHandsInverted == other.areHandsInverted;
	}

	bool operator!=(const KeyState& other) const {
		return !operator==(other);
	}

	bool IsPressed(uint8_t key) const {
		if (key < keyCount) {
			return leftHand.IsPressed(key);
		}
		auto rightHandKey = key - keyCount;
		if (rightHandKey < keyCount) {
			return rightHand.IsPressed(rightHandKey);
		}
		return false;
	}

	bool AreHandsInverted() const {
		return areHandsInverted;
	}

private:
	void ScanHardwareConfiguration() {
		auto leftConfiguration1 = ReadColumn(Pins::ReceiveLeftHand);
		auto rightConfiguration1 = ReadColumn(Pins::ReceiveRightHand);
		PulseColumnClock();
		auto leftConfiguration2 = ReadColumn(Pins::ReceiveLeftHand);
		auto rightConfiguration2 = ReadColumn(Pins::ReceiveRightHand);
		PulseColumnClock();

		auto leftPlugConfiguration = GetHandConfiguration(leftConfiguration1, leftConfiguration2);
		auto rightPlugConfiguration = GetHandConfiguration(rightConfiguration1, rightConfiguration2);
		auto isRightPluggedIntoLeft = leftPlugConfiguration == HardwareConfiguration::RightHand;
		auto isLeftPluggedIntoRight = rightPlugConfiguration == HardwareConfiguration::LeftHand;

		areHandsInverted = isRightPluggedIntoLeft || isLeftPluggedIntoRight;
	}

	static HardwareConfiguration GetHandConfiguration(bool configuration1, bool configuration2) {
		if (!configuration1 && !configuration2) {
			return HardwareConfiguration::V1;
		}
		if (!configuration1 && configuration2) {
			return HardwareConfiguration::RightHand;
		}
		if (configuration1 && !configuration2) {
			return HardwareConfiguration::LeftHand;
		}
		return HardwareConfiguration::Unplugged;
	}

	void SwapScans() {
		HandKeyState temp = leftHand;
		leftHand = rightHand;
		rightHand = temp;
	}

	void ScanRow(uint8_t row) {
		SelectRow(row);
		LoadRegister();
		ShiftRegister();
		for (uint8_t column = 0; column < columnCount; ++column) {
			ScanColumn(row, column);
		}
	}

	void ScanColumn(uint8_t row, uint8_t column) {
		leftHand.SetKey(row, column, ReadColumn(Pins::ReceiveLeftHand));
		rightHand.SetKey(row, column, ReadColumn(Pins::ReceiveRightHand));
		PulseColumnClock();
	}

	void SelectRow(uint8_t row) {
		auto select0 = (row & 0b001) ? HIGH : LOW;
		auto select1 = (row & 0b010) ? HIGH : LOW;
		auto select2 = (row & 0b100) ? HIGH : LOW;
		digitalWrite(static_cast<uint8_t>(Pins::Select0LeftHand), select0);
		digitalWrite(static_cast<uint8_t>(Pins::Select0RightHand), select0);
		digitalWrite(static_cast<uint8_t>(Pins::Select1), select1);
		digitalWrite(static_cast<uint8_t>(Pins::Select2), select2);
		delayMicroseconds(10);
	}

	void LoadRegister() {
		digitalWrite(static_cast<uint8_t>(Pins::ShiftLoad), LOW);
	}

	void ShiftRegister() {
		digitalWrite(static_cast<uint8_t>(Pins::ShiftLoad), HIGH);
	}

	bool ReadColumn(Pins pin) {
		return digitalRead(static_cast<uint8_t>(pin)) == LOW;
	}

	void PulseColumnClock() {
		digitalWrite(static_cast<uint8_t>(Pins::ColumnClock), HIGH);
		digitalWrite(static_cast<uint8_t>(Pins::ColumnClock), LOW);
	}

private:
	HandKeyState leftHand;
	HandKeyState rightHand;
	bool areHandsInverted = false;
};

constexpr uint8_t debounceCount = 3;

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

constexpr uint8_t maxPressedKeys = 10;

class PressedKeys {
public:
	PressedKeys() {
		nextKey = 0;
		for (uint8_t key = 0; key < maxPressedKeys; ++key) {
			keys[key] = 0;
		}
		heldKeys = 0;
		overflow = false;
	}

	void Scan(const PressedKeys& previousPressedKeys, const KeyState& keyState) {
		HoldPreviousKeys(previousPressedKeys, keyState);
		PressNewKeys(keyState);
	}

	bool operator==(const PressedKeys& other) const {
		if (nextKey != other.nextKey) {
			return false;
		}
		for (uint8_t key = 0; key < nextKey; ++key) {
			if (keys[key] != other.keys[key]) {
				return false;
			}
		}
		return heldKeys == other.heldKeys;
	}

	bool operator!=(const PressedKeys& other) const {
		return !operator==(other);
	}

	uint8_t GetCount() const {
		return nextKey;
	}

	uint8_t Get(uint8_t index) const {
		return index < nextKey ? keys[index] : 0;
	}

	bool WasHeld(uint8_t index) const {
		return index < heldKeys;
	}

	bool IsPressed(uint8_t key) const {
		for (uint8_t index = 0; index < nextKey; ++index) {
			if (keys[index] == key) {
				return true;
			}
		}
		return false;
	}

	void Remove(uint8_t key) {
		for (uint8_t index = 0; index < nextKey; ++index) {
			if (keys[index] == key) {
				for (uint8_t nextIndex = index + 1; nextIndex < nextKey; ++nextIndex) {
					keys[nextIndex - 1] = keys[nextIndex];
				}
				--nextKey;
				break;
			}
		}
	}

private:
	void HoldPreviousKeys(const PressedKeys& previousPressedKeys, const KeyState& keyState) {
		for (uint8_t key = 0; key < previousPressedKeys.nextKey; ++key) {
			auto previousKey = previousPressedKeys.keys[key];
			if (keyState.IsPressed(previousKey)) {
				Press(previousKey);
				++heldKeys;
			}
		}
	}

	void PressNewKeys(const KeyState& keyState) {
		for (uint8_t key = 0; key < 2 * keyCount; ++key) {
			if (keyState.IsPressed(key) && !IsPressed(key)) {
				Press(key);
			}
		}
	}

	void Press(uint8_t key) {
		if (nextKey < maxPressedKeys) {
			keys[nextKey++] = key;
		} else {
			overflow = true;
		}
	}

private:
	uint8_t nextKey;
	uint8_t keys[maxPressedKeys];
	uint8_t heldKeys;
	bool overflow;
};

constexpr uint8_t invalidKeyCode = 0xff;

class LayerKey {
public:
	constexpr LayerKey() : keyCode(invalidKeyCode), modifiers(0) {}
	constexpr LayerKey(unsigned int value) : keyCode(static_cast<uint8_t>(value)), modifiers(0) {}
	constexpr LayerKey(unsigned int value, unsigned int modifiers) : keyCode(static_cast<uint8_t>(value)), modifiers(modifiers) {}

	constexpr bool IsValid() const {
		return keyCode != invalidKeyCode;
	}

	constexpr uint8_t GetKeyCode() const {
		return keyCode;
	}

	constexpr uint8_t GetModifiers() const {
		return modifiers;
	}

private:
	uint8_t keyCode;
	uint8_t modifiers;
};

class H {
public:
	constexpr static LayerKey K() { return LayerKey(); }
	constexpr static LayerKey K(unsigned int keyCode) { return LayerKey(keyCode); }

	constexpr static LayerKey LS(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_SHIFT); }
	constexpr static LayerKey RS(unsigned int keyCode) { return LayerKey(keyCode, KEY_RIGHT_SHIFT); }

	constexpr static LayerKey VD(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_GUI | KEY_LEFT_CTRL); }
	constexpr static LayerKey WS(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_GUI | KEY_LEFT_SHIFT); }

	constexpr static LayerKey CA(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_CTRL | KEY_LEFT_ALT); }
	constexpr static LayerKey CS(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_CTRL | KEY_LEFT_SHIFT); }
};

constexpr uint8_t layerRowCount = 3;
constexpr uint8_t layerColumnCount = 5;
constexpr uint8_t layerSize = layerRowCount * layerColumnCount;
constexpr uint8_t layerCount = 6;

constexpr LayerKey leftHandLayers[layerCount][layerSize] = {
	{
		H::K(KEY_Q), H::K(KEY_W), H::K(KEY_E), H::K(KEY_R), H::K(KEY_T),
		H::K(KEY_A), H::K(KEY_S), H::K(KEY_D), H::K(KEY_F), H::K(KEY_G),
		H::K(KEY_Z), H::K(KEY_X), H::K(KEY_C), H::K(KEY_V), H::K(KEY_B)
	},
	{
		H::K(KEYPAD_ASTERIX), H::K(KEYPAD_7), H::K(KEYPAD_8), H::K(KEYPAD_9), H::K(KEYPAD_PLUS),
		H::K(KEYPAD_0),       H::K(KEYPAD_4), H::K(KEYPAD_5), H::K(KEYPAD_6), H::K(KEYPAD_PERIOD),
		H::K(KEYPAD_SLASH),   H::K(KEYPAD_1), H::K(KEYPAD_2), H::K(KEYPAD_3), H::K(KEYPAD_MINUS)
	},
	// |~!&|#|
	// |<[({ |
	// | ^%* |
	{
		H::LS(KEY_TILDE),     H::LS(KEY_1),         H::RS(KEY_7), H::RS(KEY_BACKSLASH),  H::LS(KEY_3),
		H::RS(KEY_COMMA),     H::K(KEY_LEFT_BRACE), H::RS(KEY_9), H::RS(KEY_LEFT_BRACE), H::K(),
		H::K(),               H::LS(KEY_6),         H::LS(KEY_5), H::RS(KEY_8),          H::K()
	},
	{
		H::K(), H::K(), H::K(),  H::K(), H::K(),
		H::K(), H::K(), H::K(),  H::K(), H::K(),
		H::K(), H::K(), H::K(),  H::K(), H::K()
	},
	{
		H::K(KEY_F1), H::K(KEY_F2),  H::K(KEY_F3),  H::K(KEY_F4),  H::K(KEY_BACKSPACE),
		H::K(KEY_F5), H::K(KEY_F6),  H::K(KEY_F7),  H::K(KEY_F8),  H::K(KEY_ENTER),
		H::K(KEY_F9), H::K(KEY_F10), H::K(KEY_F11), H::K(KEY_F12), H::K()
	},
	{
		H::K(),            H::K(),          H::VD(KEY_F4), H::K(),           H::K(),
		H::CA(KEY_DELETE), H::VD(KEY_LEFT), H::VD(KEY_D),  H::VD(KEY_RIGHT), H::CS(KEY_ESC),
		H::K(),            H::K(),          H::WS(KEY_S),  H::K(),           H::K()
	}
};

constexpr LayerKey rightHandLayers[layerCount][layerSize] = {
	{
		H::K(KEY_Y), H::K(KEY_U), H::K(KEY_I),     H::K(KEY_O),      H::K(KEY_P),
		H::K(KEY_H), H::K(KEY_J), H::K(KEY_K),     H::K(KEY_L),      H::K(),	// Nothing, Layer 3 shift
		H::K(KEY_N), H::K(KEY_M), H::K(KEY_COMMA), H::K(KEY_PERIOD), H::K(KEY_SLASH)
	},
	{
		H::K(KEY_PRINTSCREEN), H::K(KEY_F1), H::K(KEY_F2),  H::K(KEY_F3),  H::K(KEY_F4),
		H::K(KEY_SCROLL_LOCK), H::K(KEY_F5), H::K(KEY_F6),  H::K(KEY_F7),  H::K(KEY_F8),
		H::K(KEY_PAUSE),       H::K(KEY_F9), H::K(KEY_F10), H::K(KEY_F11), H::K(KEY_F12)
	},
	// |$"'`@|
	// | })]>|
	// | +-=\|
	{
		H::LS(KEY_4),     H::RS(KEY_QUOTE),       H::K(KEY_QUOTE), H::K(KEY_TILDE),       H::LS(KEY_2),
		H::K(),           H::RS(KEY_RIGHT_BRACE), H::RS(KEY_0),    H::K(KEY_RIGHT_BRACE), H::RS(KEY_PERIOD),
		H::K(),           H::RS(KEY_EQUAL),       H::K(KEY_MINUS), H::K(KEY_EQUAL),       H::K(KEY_BACKSLASH)
	},
	{
		H::K(KEY_INSERT), H::K(KEY_HOME),    H::K(KEY_UP),        H::K(KEY_END),   H::K(),
		H::K(KEY_DELETE), H::K(KEY_LEFT),    H::K(KEY_DOWN),      H::K(KEY_RIGHT), H::K(),
		H::K(),           H::K(KEY_PAGE_UP), H::K(KEY_PAGE_DOWN), H::K(),          H::K()
	},
	{
		H::K(), H::K(), H::K(),  H::K(), H::K(),
		H::K(), H::K(), H::K(),  H::K(), H::K(),
		H::K(), H::K(), H::K(),  H::K(), H::K()
	},
	{
		H::K(), H::K(), H::K(),  H::K(), H::K(),
		H::K(), H::K(), H::K(),  H::K(), H::K(),
		H::K(), H::K(), H::K(),  H::K(), H::K()
	}
};

constexpr uint8_t maxKeyCount = 6;

class KeyReport {
public:
	KeyReport() {
		nextKey = 0;
		modifiers = 0;
		for (uint8_t key = 0; key < maxKeyCount; ++key) {
			keys[key] = 0;
		}
		overflow = false;
	}

	void AddKey(unsigned int keyCode) {
		auto value = static_cast<uint8_t>(keyCode);
		if (IsModifier(keyCode)) {
			modifiers |= value;
		} else if (!IsAlreadyAdded(value)) {
			if (nextKey < maxKeyCount) {
				keys[nextKey++] = value;
			} else {
				overflow = true;
			}
		}
	}

	void AddModifier(uint8_t modifier) {
		modifiers |= modifier;
	}

	bool AddLayerKey(const LayerKey& layerKey, bool wasHeld) {
		if (!layerKey.IsValid()) {
			return false;
		}
		if (!wasHeld) {
			modifiers |= layerKey.GetModifiers();
		}
		AddKey(layerKey.GetKeyCode());
		return true;
	}

	void RemoveLayerKey(const LayerKey& layerKey) {
		auto index = IndexOfKey(layerKey.GetKeyCode());
		if (index < nextKey) {
			for (auto next = index + 1; next < nextKey; ++next) {
				keys[next - 1] = keys[next];
			}
			--nextKey;
			keys[nextKey] = 0;
		}
	}

	bool NeedSyntheticRelease(const LayerKey& layerKey) const {
		return IsAlreadyAdded(layerKey.GetKeyCode());
	}

	static const LayerKey& GetLeftLayerKey(uint8_t layer, uint8_t key) {
		return leftHandLayers[layer][GetLeftHandLayerIndex(key)];
	}

	static const LayerKey& GetRightLayerKey(uint8_t layer, uint8_t key) {
		return rightHandLayers[layer][GetRightHandLayerIndex(key)];
	}

	void Transmit() const {
		Keyboard.set_modifier(modifiers);
		Keyboard.set_key1(keys[0]);
		Keyboard.set_key2(keys[1]);
		Keyboard.set_key3(keys[2]);
		Keyboard.set_key4(keys[3]);
		Keyboard.set_key5(keys[4]);
		Keyboard.set_key6(keys[5]);
		Keyboard.send_now();
		delayMicroseconds(10);
	}

	bool operator==(const KeyReport& other) const {
		for (uint8_t key = 0; key < maxKeyCount; ++key) {
			if (keys[key] != other.keys[key]) {
				return false;
			}
		}
		return modifiers == other.modifiers;
	}

	bool operator!=(const KeyReport& other) const {
		return !operator==(other);
	}

private:
	static bool IsModifier(int keyCode) {
		switch (keyCode) {
			case KEY_LEFT_GUI:
			case KEY_RIGHT_GUI:
			case KEY_LEFT_SHIFT:
			case KEY_RIGHT_SHIFT:
			case KEY_LEFT_ALT:
			case KEY_RIGHT_ALT:
			case KEY_LEFT_CTRL:
			case KEY_RIGHT_CTRL:
				return true;
		}
		return false;
	}

	uint8_t IndexOfKey(uint8_t value) const {
		for (uint8_t key = 0; key < nextKey; ++key) {
			if (keys[key] == value) {
				return key;
			}
		}
		return nextKey;
	}

	bool IsAlreadyAdded(uint8_t value) const {
		return IndexOfKey(value) < nextKey;
	}

	static uint8_t GetLeftHandLayerIndex(uint8_t key) {
		switch (static_cast<Positions>(key)) {
			case Positions::PinkyTop: return 0;
			case Positions::RingTop: return 1;
			case Positions::MiddleTop: return 2;
			case Positions::IndexTop: return 3;
			case Positions::IndexExtraTop: return 4;

			case Positions::PinkyHome: return 5;
			case Positions::RingHome: return 6;
			case Positions::MiddleHome: return 7;
			case Positions::IndexHome: return 8;
			case Positions::IndexExtraHome: return 9;

			case Positions::PinkyBottom: return 10;
			case Positions::RingBottom: return 11;
			case Positions::MiddleBottom: return 12;
			case Positions::IndexBottom: return 13;
			case Positions::IndexExtraBottom: return 14;

			default: return 0;
		}
	}

	static uint8_t GetRightHandLayerIndex(uint8_t key) {
		switch (static_cast<Positions>(key)) {
			case Positions::IndexExtraTop: return 0;
			case Positions::IndexTop: return 1;
			case Positions::MiddleTop: return 2;
			case Positions::RingTop: return 3;
			case Positions::PinkyTop: return 4;

			case Positions::IndexExtraHome: return 5;
			case Positions::IndexHome: return 6;
			case Positions::MiddleHome: return 7;
			case Positions::RingHome: return 8;
			case Positions::PinkyHome: return 9;

			case Positions::IndexExtraBottom: return 10;
			case Positions::IndexBottom: return 11;
			case Positions::MiddleBottom: return 12;
			case Positions::RingBottom: return 13;
			case Positions::PinkyBottom: return 14;

			default: return 0;
		}
	}

private:
	uint8_t nextKey;
	uint8_t modifiers;
	uint8_t keys[maxKeyCount];
	bool overflow;
};

class Helper {
public:
	constexpr static uint8_t LeftKey(Positions position) {
		return static_cast<uint8_t>(position);
	}

	constexpr static uint8_t RightKey(Positions position) {
		return keyCount + static_cast<uint8_t>(position);
	}
};

constexpr auto layer1Shift = Helper::RightKey(Positions::ThumbOuter);
constexpr auto layer2Shift = Helper::LeftKey(Positions::ThumbOuter);
constexpr auto layer3Shift = Helper::RightKey(Positions::PinkyHome);
constexpr auto layer4Shift = Helper::LeftKey(Positions::ThumbGridBottomSecond);
constexpr auto layer5Shift = Helper::LeftKey(Positions::ThumbOuter);

constexpr uint8_t layerShifts[layerCount] = {
	0,
	layer1Shift,
	layer2Shift,
	layer3Shift,
	layer4Shift,
	layer5Shift
};

constexpr LayerKey layerShiftSyntheticKeys[layerCount] = {
	H::K(),
	H::RS(KEY_MINUS),
	H::K(KEY_SPACE),
	H::K(KEY_SEMICOLON),
	H::K(KEY_MENU),
	H::K()
};

constexpr uint8_t modifierKeys[] = {
	static_cast<uint8_t>(MODIFIERKEY_LEFT_CTRL),
	static_cast<uint8_t>(MODIFIERKEY_LEFT_SHIFT),
	static_cast<uint8_t>(MODIFIERKEY_LEFT_ALT),
	static_cast<uint8_t>(MODIFIERKEY_LEFT_GUI),
	static_cast<uint8_t>(MODIFIERKEY_RIGHT_CTRL),
	static_cast<uint8_t>(MODIFIERKEY_RIGHT_SHIFT),
	static_cast<uint8_t>(MODIFIERKEY_RIGHT_ALT),
	static_cast<uint8_t>(MODIFIERKEY_RIGHT_GUI)
};

class KeyboardDriver {
public:
	void ScanAndTransmit() {
		auto keyState = stableScanner.Scan();
		if (keyState == previousKeyState) {
			return;
		}
		previousKeyState = keyState;

		PressedKeys pressedKeys;
		pressedKeys.Scan(previousPressedKeys, keyState);
		if (pressedKeys == previousPressedKeys) {
			return;
		}

		GenerateKeyReports(pressedKeys);
		previousPressedKeys = pressedKeys;
	}

	uint8_t GetLayer() const {
		return layer;
	}

	bool AreHandsInverted() const {
		return previousKeyState.AreHandsInverted();
	}

private:
	void GenerateKeyReports(const PressedKeys& pressedKeys) {
		DetectLayerUnshift(pressedKeys);
		DetectLayerShift(pressedKeys);
		RemoveDeadLayerKeys(pressedKeys);
		KeyReport keyReport;
		for (uint8_t index = 0; index < pressedKeys.GetCount(); ++index) {
			AddToReport(keyReport, pressedKeys.Get(index), pressedKeys.WasHeld(index));
		}
		SendKeyReport(keyReport);
	}

	bool IsLayerShifted() const {
		return layer != 0;
	}

	void DetectLayerUnshift(const PressedKeys& pressedKeys) {
		if (IsLayerShifted() && !pressedKeys.IsPressed(layerShifts[layer])) {
			if (!layerUsed) {
				auto heldMs = millis() - layerStartMs;
				if (heldMs < 150) {
					SendSyntheticKey(layerShiftSyntheticKeys[layer]);
				}
			}
			SwitchLayer(0);
			deadLayerKeys = previousPressedKeys;
		}
	}

	void RemoveDeadLayerKeys(const PressedKeys& pressedKeys) {
		for (uint8_t index = 0; index < deadLayerKeys.GetCount(); ) {
			if (pressedKeys.IsPressed(deadLayerKeys.Get(index))) {
				++index;
			} else {
				deadLayerKeys.Remove(deadLayerKeys.Get(index));
			}
		}
	}

	void DetectLayerShift(const PressedKeys& pressedKeys) {
		if (!IsLayerShifted()) {
			for (uint8_t newLayer = 1; newLayer < layerCount; ++newLayer) {
				if (pressedKeys.IsPressed(layerShifts[newLayer]) &&
					!deadLayerKeys.IsPressed(layerShifts[newLayer])) {
					SwitchLayer(newLayer);
					break;
				}
			}
		}
		if (IsLayerShifted() && pressedKeys.IsPressed(static_cast<uint8_t>(Positions::ThumbInner)) && !layerUsed) {
			if (layer == 2) {
				SwitchLayer(5);
			}
		}
	}

	void SwitchLayer(int value) {
		layer = value;
		layerStartMs = millis();
		layerUsed = false;
	}

	void SendSyntheticKey(const LayerKey& layerKey) {
		if (!layerKey.IsValid()) {
			return;
		}
		KeyReport release = previousKeyReport;
		KeyReport press = previousKeyReport;
		for (auto modifierKey : modifierKeys) {
			if (layerKey.GetModifiers() & modifierKey) {
				press.AddModifier(modifierKey);
				SendKeyReport(press);
			}
		}
		press.AddKey(layerKey.GetKeyCode());
		SendKeyReport(press);
		SendKeyReport(release);
	}

	void SendSyntheticRelease(const LayerKey& layerKey) {
		KeyReport release = previousKeyReport;
		release.RemoveLayerKey(layerKey);
		SendKeyReport(release);
	}

	void AddToReport(KeyReport& keyReport, uint8_t key, bool wasHeld) {
		if (key < keyCount) {
			AddLeftHandKey(keyReport, key, wasHeld);
		} else {
			auto rightHandKey = key - keyCount;
			if (rightHandKey < keyCount) {
				AddRightHandKey(keyReport, rightHandKey, wasHeld);
			}
		}
	}

	void AddLeftHandKey(KeyReport& keyReport, uint8_t key, bool wasHeld) {
		switch (static_cast<Positions>(key)) {
			case Positions::PinkyExtraTop:
				keyReport.AddKey(KEY_ESC);
				break;
			case Positions::PinkyExtraBottom:
				keyReport.AddKey(KEY_TAB);
				break;
			case Positions::ThumbGridTopFirst:
				keyReport.AddKey(KEY_LEFT_CTRL);
				break;
			case Positions::ThumbGridTopSecond:
				keyReport.AddKey(KEY_LEFT_GUI);
				break;
			case Positions::ThumbGridBottomFirst:
				keyReport.AddKey(KEY_LEFT_ALT);
				break;
			case Positions::ThumbGridBottomSecond:
				// Nothing (Layer 4 shift)
				break;
			case Positions::ThumbInner:
				if (layer != 5) {
					keyReport.AddKey(KEY_LEFT_SHIFT);
				}
				break;
			case Positions::ThumbOuter:
				// Nothing (layer 2 shift)
				break;
			default:
				if (!deadLayerKeys.IsPressed(key)) {
					AddLayerKey(keyReport, KeyReport::GetLeftLayerKey(layer, key), wasHeld);
				}
				break;
		}
	}

	void AddRightHandKey(KeyReport& keyReport, uint8_t key, bool wasHeld) {
		switch (static_cast<Positions>(key)) {
			case Positions::PinkyExtraTop:
				keyReport.AddKey(KEY_BACKSPACE);
				break;
			case Positions::PinkyExtraBottom:
				keyReport.AddKey(KEY_ENTER);
				break;
			case Positions::ThumbGridTopFirst:
				keyReport.AddKey(KEY_RIGHT_CTRL);
				break;
			case Positions::ThumbGridTopSecond:
				keyReport.AddKey(KEY_RIGHT_GUI);
				break;
			case Positions::ThumbGridBottomFirst:
				keyReport.AddKey(KEY_RIGHT_ALT);
				break;
			case Positions::ThumbGridBottomSecond:
				keyReport.AddKey(KEY_MENU);
				break;
			case Positions::ThumbInner:
				if (layer == 3 && !layerUsed) {
					SendSyntheticKey(layerShiftSyntheticKeys[layer]);
					layerUsed = true;
				}
				keyReport.AddKey(KEY_SPACE);
				break;
			case Positions::ThumbOuter:
				// Nothing (layer 1 shift)
				break;
			default:
				if (!deadLayerKeys.IsPressed(key + keyCount)) {
					AddLayerKey(keyReport, KeyReport::GetRightLayerKey(layer, key), wasHeld);
				}
				break;
		}
	}

	void AddLayerKey(KeyReport& keyReport, const LayerKey& layerKey, bool wasHeld) {
		if (keyReport.NeedSyntheticRelease(layerKey)) {
			SendSyntheticRelease(layerKey);
		}
		if (layerKey.IsValid()) {
			if (!wasHeld) {
				for (auto modifierKey : modifierKeys) {
					if (layerKey.GetModifiers() & modifierKey) {
						keyReport.AddModifier(modifierKey);
						SendKeyReport(keyReport);
					}
				}
			}
			keyReport.AddKey(layerKey.GetKeyCode());
			layerUsed = true;
		}
	}

	void SendKeyReport(const KeyReport& keyReport) {
		if (keyReport != previousKeyReport) {
			keyReport.Transmit();
			previousKeyReport = keyReport;
		}
	}

private:
	KeyState previousKeyState;
	StableScanner stableScanner;
	PressedKeys previousPressedKeys;
	PressedKeys deadLayerKeys;
	KeyReport previousKeyReport;
	uint8_t layer = 0;
	unsigned long layerStartMs = 0;
	bool layerUsed = false;
};

constexpr uint8_t fullIntensity = 0b00011111;

class LedColor {
public:
	uint8_t intensity;
	uint8_t red;
	uint8_t blue;
	uint8_t green;

	constexpr static uint8_t HueToRgb(uint8_t hue) {
		return static_cast<uint8_t>(
			hue < 60 ? (6 * static_cast<unsigned int>(hue) * 255 / 360) :
			hue < 180 ? 255 :
			hue < 240 ? ((240 - static_cast<unsigned int>(hue)) * 6 * 255 / 360) :
			0);
	}

	constexpr static LedColor FromHue(uint8_t hue) {
		return FromRgb(
			HueToRgb(static_cast<uint8_t>((static_cast<unsigned int>(hue) + 120) % 360)),
			HueToRgb(hue),
			HueToRgb(static_cast<uint8_t>((static_cast<unsigned int>(hue) + 240) % 360)));
	}

	constexpr static LedColor FromRgb(uint8_t red, uint8_t blue, uint8_t green) {
		return {
			fullIntensity,
			red,
			blue,
			green
		};
	}

	constexpr static LedColor FromHex(unsigned long value) {
		return {
			fullIntensity,
			static_cast<uint8_t>((value & 0xff0000) >> 16),
			static_cast<uint8_t>(value & 0x0000ff),
			static_cast<uint8_t>((value & 0x00ff00) >> 8)
		};
	}

	LedColor TransitionTo(const LedColor& to, uint8_t step, uint8_t count) const {
		return {
			TransitionPart(intensity, to.intensity, step, count),
			TransitionPart(red, to.red, step, count),
			TransitionPart(blue, to.blue, step, count),
			TransitionPart(green, to.green, step, count)
		};
	}

private:
	static uint8_t TransitionPart(uint8_t from, uint8_t to, uint8_t step, uint8_t count) {
		if (from == to || step == count) {
			return to;
		}
		auto goingUp = from < to;
		unsigned int difference = goingUp ? to - from : from - to;
		auto delta = static_cast<uint8_t>((difference * step) / count);
		return goingUp ? from + delta : from - delta;
	}
};

namespace LedColors {
	constexpr auto black = LedColor::FromHex(0x000000ul);
	constexpr auto white = LedColor::FromHex(0xfffffful);
	constexpr auto silver = LedColor::FromHex(0xc0c0c0ul);
	constexpr auto gray = LedColor::FromHex(0x808080ul);

	constexpr auto maroon = LedColor::FromHex(0x800000ul);
	constexpr auto red = LedColor::FromHex(0xff0000ul);
	constexpr auto purple = LedColor::FromHex(0x800080ul);
	constexpr auto fuchsia = LedColor::FromHex(0xff00fful);

	constexpr auto green = LedColor::FromHex(0x008000ul);
	constexpr auto lime = LedColor::FromHex(0x00ff00ul);
	constexpr auto olive = LedColor::FromHex(0x808000ul);
	constexpr auto yellow = LedColor::FromHex(0xffff00ul);

	constexpr auto chartreuse = LedColor::FromHex(0x7fff00ul);
	constexpr auto darkgreen = LedColor::FromHex(0x006400ul);
	constexpr auto darkolivegreen = LedColor::FromHex(0x556b2ful);
	constexpr auto darkseagrean = LedColor::FromHex(0x8fbc8ful);
	constexpr auto lightseagreen = LedColor::FromHex(0x20b2aaul);
	constexpr auto mediumseagreen = LedColor::FromHex(0x3cb371ul);
	constexpr auto seagreen = LedColor::FromHex(0x2e8b57ul);
	constexpr auto darkslategray = LedColor::FromHex(0x2f4f4ful);
	constexpr auto forestgreen = LedColor::FromHex(0x228b22ul);
	constexpr auto greenyellow = LedColor::FromHex(0xadff2ful);
	constexpr auto lawngreen = LedColor::FromHex(0x7cfc00ul);
	constexpr auto lightgreen = LedColor::FromHex(0x90ee90ul);
	constexpr auto limegreen = LedColor::FromHex(0x32cd32ul);
	constexpr auto mediumspringgreen = LedColor::FromHex(0x00fa9aul);
	constexpr auto olivedrab = LedColor::FromHex(0x6b8e23ul);
	constexpr auto palegreen = LedColor::FromHex(0x98fb98ul);
	constexpr auto springgreen = LedColor::FromHex(0x00ff7ful);
	constexpr auto yellowgreen = LedColor::FromHex(0x9acd32ul);

	constexpr auto navy = LedColor::FromHex(0x000080ul);
	constexpr auto blue = LedColor::FromHex(0x0000fful);
	constexpr auto teal = LedColor::FromHex(0x008080ul);
	constexpr auto aqua = LedColor::FromHex(0x00fffful);

	constexpr auto aquamarine = LedColor::FromHex(0x7fffd4ul);
	constexpr auto cadetblue = LedColor::FromHex(0x5f9ea0ul);
	constexpr auto cornflowerblue = LedColor::FromHex(0x6495edul);
	constexpr auto darkblue = LedColor::FromHex(0x00008bul);
	constexpr auto darkcyan = LedColor::FromHex(0x008b8bul);
	constexpr auto darkslateblue = LedColor::FromHex(0x483d8bul);
	constexpr auto lightblue = LedColor::FromHex(0xadd8e6ul);
	constexpr auto lightcyan = LedColor::FromHex(0xe0fffful);
	constexpr auto lightskyblue = LedColor::FromHex(0x87cefaul);
	constexpr auto midnightblue = LedColor::FromHex(0x191970ul);
	constexpr auto powderblue = LedColor::FromHex(0xb0e0e6ul);
	constexpr auto royalblue = LedColor::FromHex(0x4169e1ul);
	constexpr auto steelblue = LedColor::FromHex(0x4682b4ul);
	constexpr auto skyblue = LedColor::FromHex(0x87ceebul);

	constexpr auto crimson = LedColor::FromHex(0xdc143cul);
	constexpr auto gold = LedColor::FromHex(0xffd700ul);
	constexpr auto khaki = LedColor::FromHex(0xf0e68cul);
	constexpr auto goldenrod = LedColor::FromHex(0xdaa520ul);
}

constexpr uint8_t frameSize = 32;

enum class LedPosition {
	PinkyExtraTop = 0,
	PinkyTop,
	RingTop,
	MiddleTop,
	IndexTop,
	IndexExtraTop,
	IndexExtraHome,
	IndexHome,
	MiddleHome,
	RingHome,
	PinkyHome,
	PinkyExtraBottom,
	PinkyBottom,
	RingBottom,
	MiddleBottom,
	IndexBottom,
	IndexExtraBottom,
	ThumbGridTopFirst,
	ThumbGridTopSecond,
	ThumbGridBottomSecond,
	ThumbGridBottomFirst,
	ThumbInner,
	ThumbOuter
};

const LedColor layerColors[layerCount] = {
	LedColors::green,
	LedColors::blue,
	LedColors::crimson,
	LedColors::gold,
	LedColors::aqua,
	LedColors::fuchsia
};

constexpr uint8_t loopIntervalMs = 10;
constexpr uint8_t transitionStepCount = 50;
constexpr uint8_t pulsateStepCount = 200;

const uint8_t distanceFromThumb[keyCount] = { 13, 13, 12, 11, 9, 7, 6, 7, 9, 10, 11, 11, 9, 8, 7, 6, 4, 3, 5, 3, 1, 2, 0 };

enum class LedMode {
	Pulsate,
	Polar
};

class LedDriver {
	LedMode ledMode;
	uint8_t hue;
	uint8_t hueStep;
	uint8_t layer;
	LedColor currentColor;
	LedColor transitionFromColor;
	uint8_t nextLayer;
	uint8_t step;
	uint8_t leftDataPin;
	uint8_t rightDataPin;

public:
	LedDriver() {
		ledMode = LedMode::Pulsate;
		hue = 0;
		hueStep = 0;
		layer = 0;
		currentColor = layerColors[0];
		transitionFromColor = layerColors[0];
		nextLayer = 0;
		step = 0;
		SetAreHandsInverted(false);
	}

	void SetAreHandsInverted(bool areHandsInverted) {
		if (areHandsInverted) {
			leftDataPin = static_cast<uint8_t>(Pins::LedDataRightHand);
			rightDataPin = static_cast<uint8_t>(Pins::LedDataLeftHand);
		} else {
			leftDataPin = static_cast<uint8_t>(Pins::LedDataLeftHand);
			rightDataPin = static_cast<uint8_t>(Pins::LedDataRightHand);
		}
	}

	void TransitionToLayer(uint8_t value) {
		if (nextLayer != value) {
			transitionFromColor = currentColor;
			nextLayer = value;
			step = 0;
		}
	}

	void Step() {
		switch (ledMode) {
			case LedMode::Pulsate:
				StepPulsate();
				break;
			case LedMode::Polar:
				StepPolar();
				break;
		}
	}

private:
	void StepPolar() {
		TransmitPolar();
		++hueStep;
		if (hueStep == 4) {
			hueStep = 0;
			hue = (hue + 1) % 360;
		}
	}

	void StepPulsate() {
		if (IsTransitioningLayers()) {
			TransitionLayerColor();
			++step;
			if (step == transitionStepCount) {
				layer = nextLayer;
				step = 0;
			}
		} else {
			PulsateLayerColor();
			step = (step + 1) % pulsateStepCount;
		}
	}

	bool IsTransitioningLayers() const {
		return nextLayer != layer;
	}

	void TransmitPolar() {
		TransmitStartFrame();
		for (uint8_t key = 0; key < keyCount; ++key) {
			auto color = LedColor::FromHue(static_cast<uint8_t>((static_cast<unsigned int>(hue) + 0 * static_cast<unsigned int>(distanceFromThumb[key])) % 360));
			TransmitLedColor(color, color);
		}
		TransmitEndFrame();
	}

	void TransitionLayerColor() {
		auto color = transitionFromColor.TransitionTo(layerColors[nextLayer], step, transitionStepCount);
		TransmitUniformColor(color);
	}

	void PulsateLayerColor() {
		auto layerColor = layerColors[layer];
		auto increment = step * 32 / pulsateStepCount;
		layerColor.intensity = step < (pulsateStepCount / 2) ?
			(fullIntensity - increment) :
			(fullIntensity + increment - 32);
		TransmitUniformColor(layerColor);
	}

	void TransmitUniformColor(const LedColor& color) {
		currentColor = color;
		TransmitStartFrame();
		for (uint8_t key = 0; key < keyCount; ++key) {
			TransmitLedColor(color, color);
		}
		TransmitEndFrame();
	}

	void PulseLedClock() {
		digitalWrite(static_cast<uint8_t>(Pins::LedClock), HIGH);
		digitalWrite(static_cast<uint8_t>(Pins::LedClock), LOW);
	}

	void TransmitBit(bool left, bool right) {
		// Dual transmit LED data to each hand using a single clock
		digitalWrite(leftDataPin, left ? HIGH : LOW);
		digitalWrite(rightDataPin, right ? HIGH : LOW);
		PulseLedClock();
	}

	void TransmitByte(uint8_t left, uint8_t right) {
		const uint8_t bitsPerByte = 8;
		for (uint8_t index = 0; index < bitsPerByte; ++index) {
			// MSB first, LSB last
			uint8_t mask = 0b10000000 >> index;
			TransmitBit((left & mask) == mask, (right & mask) == mask);
		}
	}

	void TransmitStartFrame() {
		TransmitFrame(false);
	}

	void TransmitEndFrame() {
		TransmitFrame(true);
	}

	void TransmitFrame(bool value) {
		for (uint8_t index = 0; index < frameSize; ++index) {
			TransmitBit(value, value);
		}
	}

	void TransmitLedColor(const LedColor& leftColor, const LedColor& rightColor) {
		TransmitByte(leftColor.intensity | 0b11100000, rightColor.intensity | 0b11100000);
		TransmitByte(leftColor.blue, rightColor.blue);
		TransmitByte(leftColor.green, rightColor.green);
		TransmitByte(leftColor.red, rightColor.red);
	}
};

void loop() {
	static KeyboardDriver keyboardDriver;
	static LedDriver ledDriver;
	keyboardDriver.ScanAndTransmit();
	ledDriver.SetAreHandsInverted(keyboardDriver.AreHandsInverted());
	ledDriver.TransitionToLayer(keyboardDriver.GetLayer());
	ledDriver.Step();
	delay(loopIntervalMs);
}
