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
