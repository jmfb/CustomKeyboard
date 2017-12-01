#include "Arduino.h"

int rowPins[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
int columnPins[] = { 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22 };

const int columnCount = sizeof(columnPins) / sizeof(columnPins[0]);
const int rowCount = sizeof(rowPins) / sizeof(rowPins[0]);

//Keyboard layout is split into top 6 and bottom 6 rows (12 total, pins 0-11)
const int topRowCount = 6;
const int bottomRowCount = 6;

//Keyboard layout is split into left and right (top 7/4, bottom 8/3), (11 total, pins 12-22)
const int leftHandColumnCount = 7;
const int numPadColumnCount = 4;
const int rightHandColumnCount = 8;
const int arrowKeyColumnCount = 3;

const int hyperKey = 1000;

//Left Hand: Row pins 0-5, Column pins 12-18
int leftHandKeyMap[topRowCount][leftHandColumnCount] =
{
	{ KEY_ESC,        KEY_F1,       KEY_F2,       KEY_F3,    KEY_F4, KEY_F5, 0 },
	{ KEY_TILDE,      KEY_1,        KEY_2,        KEY_3,     KEY_4,  KEY_5,  KEY_6 },
	{ KEY_TAB,        KEY_Q,        KEY_W,        KEY_E,     KEY_R,  KEY_T,  0 },
	{ 0            ,  KEY_A,        KEY_S,        KEY_D,     KEY_F,  KEY_G,  0 },
	{ KEY_LEFT_SHIFT, KEY_Z,        KEY_X,        KEY_C,     KEY_V,  KEY_B,  0 },
	{ KEY_LEFT_CTRL,  KEY_LEFT_GUI, KEY_LEFT_ALT, hyperKey,  0,      0,      0 }
};

//Numpad: Row pins 0-4, Column pins 19-22
int numPadKeyMap[topRowCount][numPadColumnCount] =
{
	{ KEY_NUM_LOCK, KEYPAD_SLASH, KEYPAD_ASTERIX, KEYPAD_MINUS },
	{ KEYPAD_7,     KEYPAD_8,     KEYPAD_9,       KEYPAD_PLUS },
	{ KEYPAD_4,     KEYPAD_5,     KEYPAD_6,       0 },
	{ KEYPAD_1,     KEYPAD_2,     KEYPAD_3,       KEYPAD_ENTER },
	{ KEYPAD_0,     0,            KEYPAD_PERIOD,  0 },
	{ 0,            0,            0,              0 }
};

//Right Hand: Row pins 6-11, Column pins 12-19
int rightHandKeyMap[bottomRowCount][rightHandColumnCount] =
{
	{ 0,     KEY_F6, KEY_F7, KEY_F8, KEY_F9,    KEY_F10,        KEY_F11,         KEY_F12 },
	{ 0,     KEY_7,  KEY_8,  KEY_9,  KEY_0,     KEY_MINUS,      KEY_EQUAL,       KEY_BACKSPACE },
	{ KEY_Y, KEY_U,  KEY_I,  KEY_O,  KEY_P,     KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_BACKSLASH },
	{ 0,     KEY_H,  KEY_J,  KEY_K,  KEY_L,     KEY_SEMICOLON,  KEY_QUOTE,       KEY_ENTER },
	{ 0,     0,      KEY_N,  KEY_M,  KEY_COMMA, KEY_PERIOD,     KEY_SLASH,       KEY_RIGHT_SHIFT },
	{ 0,     0,      0,      0,      KEY_SPACE, KEY_RIGHT_ALT,  KEY_MENU,        KEY_RIGHT_CTRL }
};

//Arrow Keys: Row pins 6-10, Column pins 20-22
int arrowKeyKeyMap[bottomRowCount][arrowKeyColumnCount] =
{
	{ KEY_PRINTSCREEN, KEY_SCROLL_LOCK, KEY_PAUSE },
	{ KEY_INSERT,      KEY_HOME,        KEY_PAGE_UP },
	{ KEY_DELETE,      KEY_END,         KEY_PAGE_DOWN },
	{ 0,               KEY_UP,          0 },
	{ KEY_LEFT,        KEY_DOWN,        KEY_RIGHT },
	{ 0,               0,               0 }
};

int keyMap[rowCount][columnCount];

void initializeKeyMap()
{
	for (int row = 0; row < topRowCount; ++row)
	{
		for (int column = 0; column < leftHandColumnCount; ++column)
			keyMap[row][column] = leftHandKeyMap[row][column];
		for (int column = 0; column < numPadColumnCount; ++column)
			keyMap[row][column + leftHandColumnCount] = numPadKeyMap[row][column];
	}
	for (int row = 0; row < bottomRowCount; ++row)
	{
		for (int column = 0; column < rightHandColumnCount; ++column)
			keyMap[row + topRowCount][column] = rightHandKeyMap[row][column];
		for (int column = 0; column < arrowKeyColumnCount; ++column)
			keyMap[row + topRowCount][column + rightHandColumnCount] = arrowKeyKeyMap[row][column];
	}
}

void setup()
{
	for (int row = 0; row < rowCount; ++row)
	{
		pinMode(rowPins[row], OUTPUT);
		digitalWrite(rowPins[row], HIGH);
	}

	for (int column = 0; column < columnCount; ++column)
	{
		pinMode(columnPins[column], INPUT);
		digitalWrite(columnPins[column], HIGH);
	}

	initializeKeyMap();

	delay(200);
}

class PressedKey
{
public:
	PressedKey()
		: row(0), column(0)
	{
	}

	PressedKey(int row, int column)
		: row(row), column(column)
	{
	}

	bool operator==(const PressedKey& other) const
	{
		return row == other.row && column == other.column;
	}
	bool operator!=(const PressedKey& other) const
	{
		return row != other.row || column != other.column;
	}

	int row;
	int column;
};

class KeyState
{
public:
	KeyState()
	{
		for (int row = 0; row < rowCount; ++row)
			for (int column = 0; column < columnCount; ++column)
				keys[row][column] = false;
	}

	void Scan()
	{
		for (int row = 0; row < rowCount; ++row)
			ScanRow(row);
	}

	KeyState operator&&(const KeyState& other) const
	{
		KeyState result;
		for (int row = 0; row < rowCount; ++row)
			for (int column = 0; column < columnCount; ++column)
				result.keys[row][column] = keys[row][column] && other.keys[row][column];
		return result;
	}

	bool IsPressed(const PressedKey& pressedKey) const
	{
		return keys[pressedKey.row][pressedKey.column];
	}

private:
	void ScanRow(int row)
	{
		digitalWrite(rowPins[row], LOW);
		for (int column = 0; column < columnCount; ++column)
			ScanKey(row, column);
		digitalWrite(rowPins[row], HIGH);
	}

	void ScanKey(int row, int column)
	{
		int signal = digitalRead(columnPins[column]);
		bool isPressed = signal == LOW;
		keys[row][column] = isPressed;
	}

private:
	bool keys[rowCount][columnCount];
};

const int debounceCount = 3;

class StableScanner
{
public:
	StableScanner()
	{
		nextIndex = 0;
	}

	KeyState Scan()
	{
		int index = nextIndex;
		nextIndex = (nextIndex + 1) % debounceCount;
		keyState[index].Scan();
		return keyState[0] && keyState[1] && keyState[2];
	}

private:
	int nextIndex;
	KeyState keyState[debounceCount];
};

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

	bool operator!=(const PressedKeys& other) const
	{
		if (count != other.count)
			return true;
		for (int index = 0; index < count; ++index)
			if (keys[index] != other.keys[index])
				return true;
		return false;
	}

private:
	void HoldPreviousKeys(const PressedKeys& previousPressedKeys, const KeyState& keyState)
	{
		for (int index = 0; index < previousPressedKeys.count; ++index)
			if (keyState.IsPressed(previousPressedKeys[index]))
				Press(previousPressedKeys[index]);
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

private:
	int count;
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
			int keyCode = pressedKeys.GetKeyCode(index);
			// TODO: Left off here (GetKeyCode doesn't exist, count isn't public)
		}
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
	}

private:
	uint8_t modifiers;
	uint8_t keys[maxKeyCount];
};

void loop()
{
	static StableScanner stableScanner;
	static KeyState previousKeyState;
	static PressedKeys previousPressedKeys;
	KeyState keyState = stableScanner.Scan();
	if (keyState != previousKeyState)
	{
		previousKeyState = keyState;
		PressedKeys pressedKeys;
		pressedKeys.Scan(previousPressedKeys, keyState);
		if (pressedKeys != previousPressedKeys)
		{
			previousPressedKeys = pressedKeys;
			KeyReport keyReport;
			keyReport.Translate(pressedKeys);
			keyReport.Transmit();
			delay(2);
		}
	}
}

#if 0
const int maxKeyCount = 6;

class KeyState
{
public:
	KeyState()
	{
		modifiers = 0;
		for (int key = 0; key < maxKeyCount; ++key)
			keys[key] = 0;
		hyper = false;
	}

	void Scan()
	{
		for (int row = 0; row < rowCount; ++row)
			ScanRow(row);
	}

	bool operator!=(const KeyState& rhs) const
	{
		return hyper != rhs.hyper ||
			modifiers != rhs.modifiers ||
			keys[0] != rhs.keys[0] ||
			keys[1] != rhs.keys[1] ||
			keys[2] != rhs.keys[2] ||
			keys[3] != rhs.keys[3] ||
			keys[4] != rhs.keys[4] ||
			keys[5] != rhs.keys[5];
	}

	void Transmit()
	{
		if (hyper)
		{
			TranslateHyperKeys();
		}
		else
		{
			Keyboard.set_modifier(modifiers);
			Keyboard.set_key1(keys[0]);
			Keyboard.set_key2(keys[1]);
			Keyboard.set_key3(keys[2]);
			Keyboard.set_key4(keys[3]);
			Keyboard.set_key5(keys[4]);
			Keyboard.set_key6(keys[5]);
			Keyboard.send_now();
		}
	}

private:
	void ScanRow(int row)
	{
		digitalWrite(rowPins[row], LOW);
		for (int column = 0; column < columnCount; ++column)
			ScanKey(row, column);
		digitalWrite(rowPins[row], HIGH);
	}

	void ScanKey(int row, int column)
	{
		int signal = digitalRead(columnPins[column]);
		bool isPressed = signal == LOW;
		if (isPressed)
			PressKey(keyMap[row][column]);
	}

	bool PressKey(int keyCode)
	{
		if (keyCode == hyperKey)
		{
			hyper = true;
			return true;
		}

		if (keyCode == KEY_LEFT_GUI ||
			keyCode == KEY_RIGHT_GUI ||
			keyCode == KEY_LEFT_SHIFT ||
			keyCode == KEY_RIGHT_SHIFT ||
			keyCode == KEY_LEFT_ALT ||
			keyCode == KEY_RIGHT_ALT ||
			keyCode == KEY_LEFT_CTRL ||
			keyCode == KEY_RIGHT_CTRL)
		{
			modifiers |= keyCode;
			return true;
		}

		for (int key = 0; key < maxKeyCount; ++key)
		{
			if (keys[key] == 0)
			{
				keys[key] = static_cast<uint8_t>(keyCode);
				return true;
			}
		}
		return false;
	}

	void TranslateHyperKeys()
	{
		for (int key = 0; key < maxKeyCount; ++key)
		{
			uint8_t keyCode = keys[key];
			if (keyCode != 0)
			{
				TranslateHyperKey(keyCode);
			}
		}
	}

	void TranslateHyperKey(uint8_t keyCode)
	{
		#define HYPER(from, to) case static_cast<uint8_t>(from): SendHyperKey(to); break
		switch (keyCode)
		{
		HYPER(KEY_SPACE, KEY_BACKSPACE);
		HYPER(KEY_TAB, KEY_ENTER);
		HYPER(KEY_ESC, KEY_CAPS_LOCK);
		HYPER(KEY_S, '['); HYPER(KEY_L, ']');
		HYPER(KEY_D, '('); HYPER(KEY_K, ')');
		HYPER(KEY_F, '{'); HYPER(KEY_J, '}');
		HYPER(KEY_G, '<'); HYPER(KEY_H, '>');
		}
		#undef HYPER
	}

	void SendHyperKey(int code)
	{
		Keyboard.press(code);
		Keyboard.release(code);
	}

private:
	uint8_t modifiers;
	uint8_t keys[maxKeyCount];
	bool hyper;
};

void loop()
{
	static KeyState keyState;
	KeyState newKeyState;
	newKeyState.Scan();
	if (newKeyState != keyState)
	{
		keyState = newKeyState;
		keyState.Transmit();
		delay(2);
	}
}
#endif
