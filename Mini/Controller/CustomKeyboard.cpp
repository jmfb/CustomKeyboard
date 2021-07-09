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
		if (pressed) {
			auto position = scanPositions[row][column];
			if (position != Positions::None) {
				keys[static_cast<uint8_t>(position)] = true;
			}
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

class KeyState {
public:
	void Scan() {
		for (uint8_t row = 0; row < rowCount; ++row) {
			ScanRow(row);
		}
	}

	KeyState operator&&(const KeyState& other) const {
		KeyState result;
		result.leftHand = leftHand && other.leftHand;
		result.rightHand = rightHand && other.rightHand;
		return result;
	}

	bool operator==(const KeyState& other) const {
		return leftHand == other.leftHand && rightHand == other.rightHand;
	}

	bool operator!=(const KeyState& other) const {
		return !operator==(other);
	}

	bool IsPressed(uint8_t key) const {
		if (key < keyCount) {
			return leftHand.IsPressed(key);
		}
		auto rightHandKey = key - keyCount;
		if (key < keyCount) {
			return rightHand.IsPressed(rightHandKey);
		}
		return false;
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
		delay(1);
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
		return true;
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

	bool IsPressed(uint8_t key) const {
		for (uint8_t index = 0; index < nextKey; ++index) {
			if (keys[index] == key) {
				return true;
			}
		}
		return false;
	}

private:
	void HoldPreviousKeys(const PressedKeys& previousPressedKeys, const KeyState& keyState) {
		for (uint8_t key = 0; key < previousPressedKeys.nextKey; ++key) {
			auto previousKey = previousPressedKeys.keys[key];
			if (keyState.IsPressed(previousKey)) {
				Press(previousKey);
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

	constexpr static LayerKey CA(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_CTRL | KEY_LEFT_ALT); }
	constexpr static LayerKey CS(unsigned int keyCode) { return LayerKey(keyCode, KEY_LEFT_CTRL | KEY_LEFT_SHIFT); }
};

constexpr uint8_t layerRowCount = 3;
constexpr uint8_t layerColumnCount = 5;
constexpr uint8_t layerSize = layerRowCount * layerColumnCount;
constexpr uint8_t layerCount = 5;

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
	// |<[({\|
	// |:^%*,|
	{
		H::LS(KEY_TILDE),     H::LS(KEY_1),         H::RS(KEY_7), H::RS(KEY_BACKSLASH),  H::LS(KEY_3),
		H::RS(KEY_COMMA),     H::K(KEY_LEFT_BRACE), H::RS(KEY_9), H::RS(KEY_LEFT_BRACE), H::K(KEY_BACKSLASH),
		H::RS(KEY_SEMICOLON), H::LS(KEY_6),         H::LS(KEY_5), H::RS(KEY_8),          H::K(KEY_COMMA)
	},
	{
		H::K(),            H::K(),          H::VD(KEY_F4), H::K(),           H::K(),
		H::CA(KEY_DELETE), H::VD(KEY_LEFT), H::VD(KEY_D),  H::VD(KEY_RIGHT), H::CS(KEY_ESC),
		H::K(),            H::K(),          H::K(),        H::K(),           H::K()
	},
	{
		H::K(KEY_BACKSPACE), H::K(KEY_HOME), H::K(KEY_UP),       H::K(KEY_END),       H::K(KEY_INSERT),
		H::K(KEY_ENTER),     H::K(KEY_LEFT), H::K(KEY_DOWN),     H::K(KEY_RIGHT),     H::K(KEY_DELETE),
		H::K(),              H::K(),         H::K(KEY_PAGE_UP),  H::K(KEY_PAGE_DOWN), H::K()
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
	// |/})]>|
	// |.+-=?|
	{
		H::LS(KEY_4),     H::RS(KEY_QUOTE),       H::K(KEY_QUOTE), H::K(KEY_TILDE),       H::LS(KEY_2),
		H::K(KEY_SLASH),  H::RS(KEY_RIGHT_BRACE), H::RS(KEY_0),    H::K(KEY_RIGHT_BRACE), H::RS(KEY_PERIOD),
		H::K(KEY_PERIOD), H::RS(KEY_EQUAL),       H::K(KEY_MINUS), H::K(KEY_EQUAL),       H::RS(KEY_SLASH)
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

	void AddLayerKey(const LayerKey& layerKey) {
		if (!layerKey.IsValid()) {
			return;
		}
		modifiers |= layerKey.GetModifiers();
		AddKey(layerKey.GetKeyCode());
	}

	void AddLeftLayerKey(uint8_t layer, uint8_t key) {
		AddLayerKey(leftHandLayers[layer][GetLeftHandLayerIndex(key)]);
	}

	void AddRightLayerKey(uint8_t layer, uint8_t key) {
		AddLayerKey(rightHandLayers[layer][GetRightHandLayerIndex(key)]);
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
		delay(2);
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

	bool IsAlreadyAdded(uint8_t value) {
		for (uint8_t key = 0; key < nextKey; ++key) {
			if (keys[key] == value) {
				return true;
			}
		}
		return false;
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

constexpr uint8_t layerShifts[layerCount] = {
	0,
	layer1Shift,
	layer2Shift,
	layer3Shift,
	layer4Shift
};

constexpr LayerKey layerShiftSyntheticKeys[layerCount] = {
	H::K(),
	H::RS(KEY_MINUS),
	H::K(KEY_SPACE),
	H::K(KEY_SEMICOLON),
	H::K(KEY_MENU)
};

class KeyboardDriver {
public:
	KeyboardDriver() {
		layer = 0;
		layerKeyCount = 0;
	}

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

private:
	void GenerateKeyReports(const PressedKeys& pressedKeys) {
		DetectLayerUnshift(pressedKeys);
		DetectLayerShift(pressedKeys);
		KeyReport keyReport;
		for (uint8_t index = 0; index < pressedKeys.GetCount(); ++index) {
			AddToReport(keyReport, pressedKeys.Get(index));
		}
		SendKeyReport(keyReport);
	}

	bool IsLayerShifted() const {
		return layer != 0;
	}

	void DetectLayerUnshift(const PressedKeys& pressedKeys) {
		if (IsLayerShifted() && !pressedKeys.IsPressed(layerShifts[layer])) {
			if (layerKeyCount == 0) {
				SendSyntheticKey(layerShiftSyntheticKeys[layer]);
			}
			SwitchLayer(0);
		}
	}

	void DetectLayerShift(const PressedKeys& pressedKeys) {
		if (!IsLayerShifted()) {
			for (uint8_t newLayer = 1; newLayer < layerCount; ++newLayer) {
				if (pressedKeys.IsPressed(layerShifts[newLayer])) {
					SwitchLayer(newLayer);
					break;
				}
			}
		}
	}

	void SwitchLayer(int value) {
		layer = value;
		layerKeyCount = 0;
	}

	void SendSyntheticKey(const LayerKey& layerKey) {
		KeyReport release = previousKeyReport;
		KeyReport press = previousKeyReport;
		press.AddLayerKey(layerKey);
		SendKeyReport(press);
		SendKeyReport(release);
	}

	void AddToReport(KeyReport& keyReport, uint8_t key) {
		if (key < keyCount) {
			AddLeftHandKey(keyReport, key);
		} else {
			auto rightHandKey = key - keyCount;
			if (rightHandKey < keyCount) {
				AddRightHandKey(keyReport, rightHandKey);
			}
		}
	}

	void AddLeftHandKey(KeyReport& keyReport, uint8_t key) {
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
				keyReport.AddKey(KEY_LEFT_SHIFT);
				break;
			case Positions::ThumbOuter:
				// Nothing (layer 2 shift)
				break;
			default:
				keyReport.AddLeftLayerKey(layer, key);
				++layerKeyCount;
				break;
		}
	}

	void AddRightHandKey(KeyReport& keyReport, uint8_t key) {
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
				keyReport.AddKey(KEY_SPACE);
				break;
			case Positions::ThumbOuter:
				// Nothing (layer 1 shift)
				break;
			default:
				keyReport.AddRightLayerKey(layer, key);
				++layerKeyCount;
				break;
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
	KeyReport previousKeyReport;
	uint8_t layer;
	uint8_t layerKeyCount;
};

constexpr uint8_t fullIntensity = 0b00011111;

class LedColor {
public:
	uint8_t intensity;
	uint8_t red;
	uint8_t blue;
	uint8_t green;

	constexpr static LedColor FromHex(unsigned int value) {
		return {
			fullIntensity,
			static_cast<uint8_t>((value & 0xff0000) >> 16),
			static_cast<uint8_t>(value & 0x0000ff),
			static_cast<uint8_t>((value & 0x00ff00) >> 8)
		};
	}
};

namespace LedColors {
	// const auto black = LedColor::FromHex(0x000000ul);
	// const auto white = LedColor::FromHex(0xfffffful);
	// const auto silver = LedColor::FromHex(0xc0c0c0ul);
	// const auto gray = LedColor::FromHex(0x808080ul);

	// const auto maroon = LedColor::FromHex(0x800000ul);
	// const auto red = LedColor::FromHex(0xff0000ul);
	// const auto purple = LedColor::FromHex(0x800080ul);
	// const auto fuchsia = LedColor::FromHex(0xff00fful);

	constexpr auto green = LedColor::FromHex(0x008000ul);
	// const auto lime = LedColor::FromHex(0x00ff00ul);
	// const auto olive = LedColor::FromHex(0x808000ul);
	// const auto yellow = LedColor::FromHex(0xffff00ul);

	constexpr auto navy = LedColor::FromHex(0x000080ul);
	constexpr auto blue = LedColor::FromHex(0x0000fful);
	constexpr auto teal = LedColor::FromHex(0x008080ul);
	// const auto aqua = LedColor::FromHex(0x00fffful);

	// const auto aquamarine = LedColor::FromHex(0x7fffd4ul);
	// const auto cadetblue = LedColor::FromHex(0x5f9ea0ul);
	// const auto cornflowerblue = LedColor::FromHex(0x6495edul);
	// const auto darkblue = LedColor::FromHex(0x00008bul);
	// const auto darkcyan = LedColor::FromHex(0x008b8bul);
	// const auto darkslateblue = LedColor::FromHex(0x483d8bul);
	// const auto lightblue = LedColor::FromHex(0xadd8e6ul);
	// const auto lightcyan = LedColor::FromHex(0xe0fffful);
	// const auto lightseagreen = LedColor::FromHex(0x20b2aaul);
	// const auto lightskyblue = LedColor::FromHex(0x87cefaul);
	// const auto midnightblue = LedColor::FromHex(0x191970ul);
	// const auto powderblue = LedColor::FromHex(0xb0e0e6ul);
	// const auto royalblue = LedColor::FromHex(0x4169e1ul);
	// const auto seagreen = LedColor::FromHex(0x2e8b57ul);
	// const auto steelblue = LedColor::FromHex(0x4682b4ul);
}

const uint8_t frameSize = 32;

// LED Order
// =========
// PinkyExtraTop
// PinkyTop
// RingTop
// MiddleTop
// IndexTop
// IndexExtraTop
// IndexExtraHome
// IndexHome
// MiddleHome
// RingHome
// PinkyHome
// PinkyExtraBottom
// PinkyBottom
// RingBottom
// MiddleBottom
// IndexBottom
// IndexExtraBottom
// ThumbGridTopFirst
// ThumbGridTopSecond
// ThumbGridBottomSecond
// ThumbGridBottomFirst
// ThumbInner
// ThumbOuter

class LedDriver {
public:
	void SetUniformColor(const LedColor& color) {
		TransmitStartFrame();
		for (uint8_t key = 0; key < keyCount; ++key) {
			TransmitLedColor(color, color);
		}
		TransmitEndFrame();
	}

	// TODO: More advanced color algorithms

private:
	void PulseLedClock() {
		digitalWrite(static_cast<uint8_t>(Pins::LedClock), HIGH);
		digitalWrite(static_cast<uint8_t>(Pins::LedClock), LOW);
	}

	void TransmitBit(bool left, bool right) {
		// Dual transmit LED data to each hand using a single clock
		digitalWrite(static_cast<uint8_t>(Pins::LedDataLeftHand), left ? HIGH : LOW);
		digitalWrite(static_cast<uint8_t>(Pins::LedDataRightHand), right ? HIGH : LOW);
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
		TransmitByte(leftColor.red, rightColor.red);
		TransmitByte(leftColor.blue, rightColor.blue);
		TransmitByte(leftColor.green, rightColor.green);
	}
};

const LedColor layerColors[layerCount] = {
	LedColors::green,
	LedColors::blue,
	LedColors::navy,
	LedColors::teal
};

void loop() {
	static KeyboardDriver keyboardDriver;
	static LedDriver ledDriver;
	keyboardDriver.ScanAndTransmit();
	ledDriver.SetUniformColor(layerColors[keyboardDriver.GetLayer()]);
	delay(100);
}
