#pragma once
// This is a mock file for test hooks for CustomKeyboard.cpp, not the real Arduino.h used by the teensyduino loader.

//Teensy API Types
using uint8_t = unsigned char;

//Teensy API Constants
const int OUTPUT = 0;
const int INPUT = 1;
const int INPUT_PULLUP = 2;
const int LOW = 0;
const int HIGH = 1;

//Keyboard Constants - from C:\Program Files (x86)\Arduino\hardware\teensy\cores\teensy\keylayouts.h
#define MODIFIERKEY_CTRL        ( 0x01 | 0x8000 )
#define MODIFIERKEY_SHIFT       ( 0x02 | 0x8000 )
#define MODIFIERKEY_ALT         ( 0x04 | 0x8000 )
#define MODIFIERKEY_GUI         ( 0x08 | 0x8000 )
#define MODIFIERKEY_LEFT_CTRL   ( 0x01 | 0x8000 )
#define MODIFIERKEY_LEFT_SHIFT  ( 0x02 | 0x8000 )
#define MODIFIERKEY_LEFT_ALT    ( 0x04 | 0x8000 )
#define MODIFIERKEY_LEFT_GUI    ( 0x08 | 0x8000 )
#define MODIFIERKEY_RIGHT_CTRL  ( 0x10 | 0x8000 )
#define MODIFIERKEY_RIGHT_SHIFT ( 0x20 | 0x8000 )
#define MODIFIERKEY_RIGHT_ALT   ( 0x40 | 0x8000 )
#define MODIFIERKEY_RIGHT_GUI   ( 0x80 | 0x8000 )
#define KEY_MEDIA_VOLUME_INC    0x01
#define KEY_MEDIA_VOLUME_DEC    0x02
#define KEY_MEDIA_MUTE          0x04
#define KEY_MEDIA_PLAY_PAUSE    0x08
#define KEY_MEDIA_NEXT_TRACK    0x10
#define KEY_MEDIA_PREV_TRACK    0x20
#define KEY_MEDIA_STOP          0x40
#define KEY_MEDIA_EJECT         0x80
#define KEY_A           ( 4   | 0x4000 )
#define KEY_B           ( 5   | 0x4000 )
#define KEY_C           ( 6   | 0x4000 )
#define KEY_D           ( 7   | 0x4000 )
#define KEY_E           ( 8   | 0x4000 )
#define KEY_F           ( 9   | 0x4000 )
#define KEY_G           ( 10  | 0x4000 )
#define KEY_H           ( 11  | 0x4000 )
#define KEY_I           ( 12  | 0x4000 )
#define KEY_J           ( 13  | 0x4000 )
#define KEY_K           ( 14  | 0x4000 )
#define KEY_L           ( 15  | 0x4000 )
#define KEY_M           ( 16  | 0x4000 )
#define KEY_N           ( 17  | 0x4000 )
#define KEY_O           ( 18  | 0x4000 )
#define KEY_P           ( 19  | 0x4000 )
#define KEY_Q           ( 20  | 0x4000 )
#define KEY_R           ( 21  | 0x4000 )
#define KEY_S           ( 22  | 0x4000 )
#define KEY_T           ( 23  | 0x4000 )
#define KEY_U           ( 24  | 0x4000 )
#define KEY_V           ( 25  | 0x4000 )
#define KEY_W           ( 26  | 0x4000 )
#define KEY_X           ( 27  | 0x4000 )
#define KEY_Y           ( 28  | 0x4000 )
#define KEY_Z           ( 29  | 0x4000 )
#define KEY_1           ( 30  | 0x4000 )
#define KEY_2           ( 31  | 0x4000 )
#define KEY_3           ( 32  | 0x4000 )
#define KEY_4           ( 33  | 0x4000 )
#define KEY_5           ( 34  | 0x4000 )
#define KEY_6           ( 35  | 0x4000 )
#define KEY_7           ( 36  | 0x4000 )
#define KEY_8           ( 37  | 0x4000 )
#define KEY_9           ( 38  | 0x4000 )
#define KEY_0           ( 39  | 0x4000 )
#define KEY_ENTER       ( 40  | 0x4000 )
#define KEY_ESC         ( 41  | 0x4000 )
#define KEY_BACKSPACE   ( 42  | 0x4000 )
#define KEY_TAB         ( 43  | 0x4000 )
#define KEY_SPACE       ( 44  | 0x4000 )
#define KEY_MINUS       ( 45  | 0x4000 )
#define KEY_EQUAL       ( 46  | 0x4000 )
#define KEY_LEFT_BRACE  ( 47  | 0x4000 )
#define KEY_RIGHT_BRACE ( 48  | 0x4000 )
#define KEY_BACKSLASH   ( 49  | 0x4000 )
#define KEY_NON_US_NUM  ( 50  | 0x4000 )
#define KEY_SEMICOLON   ( 51  | 0x4000 )
#define KEY_QUOTE       ( 52  | 0x4000 )
#define KEY_TILDE       ( 53  | 0x4000 )
#define KEY_COMMA       ( 54  | 0x4000 )
#define KEY_PERIOD      ( 55  | 0x4000 )
#define KEY_SLASH       ( 56  | 0x4000 )
#define KEY_CAPS_LOCK   ( 57  | 0x4000 )
#define KEY_F1          ( 58  | 0x4000 )
#define KEY_F2          ( 59  | 0x4000 )
#define KEY_F3          ( 60  | 0x4000 )
#define KEY_F4          ( 61  | 0x4000 )
#define KEY_F5          ( 62  | 0x4000 )
#define KEY_F6          ( 63  | 0x4000 )
#define KEY_F7          ( 64  | 0x4000 )
#define KEY_F8          ( 65  | 0x4000 )
#define KEY_F9          ( 66  | 0x4000 )
#define KEY_F10         ( 67  | 0x4000 )
#define KEY_F11         ( 68  | 0x4000 )
#define KEY_F12         ( 69  | 0x4000 )
#define KEY_PRINTSCREEN ( 70  | 0x4000 )
#define KEY_SCROLL_LOCK ( 71  | 0x4000 )
#define KEY_PAUSE       ( 72  | 0x4000 )
#define KEY_INSERT      ( 73  | 0x4000 )
#define KEY_HOME        ( 74  | 0x4000 )
#define KEY_PAGE_UP     ( 75  | 0x4000 )
#define KEY_DELETE      ( 76  | 0x4000 )
#define KEY_END         ( 77  | 0x4000 )
#define KEY_PAGE_DOWN   ( 78  | 0x4000 )
#define KEY_RIGHT       ( 79  | 0x4000 )
#define KEY_LEFT        ( 80  | 0x4000 )
#define KEY_DOWN        ( 81  | 0x4000 )
#define KEY_UP          ( 82  | 0x4000 )
#define KEY_NUM_LOCK    ( 83  | 0x4000 )
#define KEYPAD_SLASH    ( 84  | 0x4000 )
#define KEYPAD_ASTERIX  ( 85  | 0x4000 )
#define KEYPAD_MINUS    ( 86  | 0x4000 )
#define KEYPAD_PLUS     ( 87  | 0x4000 )
#define KEYPAD_ENTER    ( 88  | 0x4000 )
#define KEYPAD_1        ( 89  | 0x4000 )
#define KEYPAD_2        ( 90  | 0x4000 )
#define KEYPAD_3        ( 91  | 0x4000 )
#define KEYPAD_4        ( 92  | 0x4000 )
#define KEYPAD_5        ( 93  | 0x4000 )
#define KEYPAD_6        ( 94  | 0x4000 )
#define KEYPAD_7        ( 95  | 0x4000 )
#define KEYPAD_8        ( 96  | 0x4000 )
#define KEYPAD_9        ( 97  | 0x4000 )
#define KEYPAD_0        ( 98  | 0x4000 )
#define KEYPAD_PERIOD   ( 99  | 0x4000 )
#define KEY_MENU        ( 101 | 0x4000 )
#define KEY_F13         ( 104 | 0x4000 )
#define KEY_F14         ( 105 | 0x4000 )
#define KEY_F15         ( 106 | 0x4000 )
#define KEY_F16         ( 107 | 0x4000 )
#define KEY_F17         ( 108 | 0x4000 )
#define KEY_F18         ( 109 | 0x4000 )
#define KEY_F19         ( 110 | 0x4000 )
#define KEY_F20         ( 111 | 0x4000 )
#define KEY_F21         ( 112 | 0x4000 )
#define KEY_F22         ( 113 | 0x4000 )
#define KEY_F23         ( 114 | 0x4000 )
#define KEY_F24         ( 115 | 0x4000 )
#define KEY_UP_ARROW    KEY_UP
#define KEY_DOWN_ARROW  KEY_DOWN
#define KEY_LEFT_ARROW  KEY_LEFT
#define KEY_RIGHT_ARROW KEY_RIGHT
#define KEY_RETURN      KEY_ENTER
#define KEY_LEFT_CTRL   MODIFIERKEY_LEFT_CTRL
#define KEY_LEFT_SHIFT  MODIFIERKEY_LEFT_SHIFT
#define KEY_LEFT_ALT    MODIFIERKEY_LEFT_ALT
#define KEY_LEFT_GUI    MODIFIERKEY_LEFT_GUI
#define KEY_RIGHT_CTRL  MODIFIERKEY_RIGHT_CTRL
#define KEY_RIGHT_SHIFT MODIFIERKEY_RIGHT_SHIFT
#define KEY_RIGHT_ALT   MODIFIERKEY_RIGHT_ALT
#define KEY_RIGHT_GUI   MODIFIERKEY_RIGHT_GUI

//Teensy API
void delay(int milliseconds);
void delayMicroseconds(int microseconds);
unsigned long millis();
void pinMode(int pin, int mode);
void digitalWrite(int pin, int signal);
int digitalRead(int pin);

//Program Hooks
void setup();
void loop();

//Keyboard API (singleton w/ mock callback)
void keyboardCallback(uint8_t modifiers, uint8_t key1, uint8_t key2, uint8_t key3, uint8_t key4, uint8_t key5, uint8_t key6);

class KeyboardSingleton
{
public:
	KeyboardSingleton()
		: modifiers(0), key1(0), key2(0), key3(0), key4(0), key5(0), key6(0)
	{
	}

	void set_modifier(uint8_t value)
	{
		modifiers = value;
	}
	void set_key1(uint8_t value)
	{
		key1 = value;
	}
	void set_key2(uint8_t value)
	{
		key2 = value;
	}
	void set_key3(uint8_t value)
	{
		key3 = value;
	}
	void set_key4(uint8_t value)
	{
		key4 = value;
	}
	void set_key5(uint8_t value)
	{
		key5 = value;
	}
	void set_key6(uint8_t value)
	{
		key6 = value;
	}
	void send_now()
	{
		keyboardCallback(modifiers, key1, key2, key3, key4, key5, key6);
	}

private:
	uint8_t modifiers;
	uint8_t key1;
	uint8_t key2;
	uint8_t key3;
	uint8_t key4;
	uint8_t key5;
	uint8_t key6;
};

extern KeyboardSingleton Keyboard;
