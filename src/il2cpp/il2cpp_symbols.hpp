#pragma once

struct TimelineCharacter;

enum class KeyCode {
	None = 0,
	Backspace = 8,
	Tab = 9,
	Clear = 12, // 0x0000000C
	Return = 13, // 0x0000000D
	Pause = 19, // 0x00000013
	Escape = 27, // 0x0000001B
	Space = 32, // 0x00000020
	Exclaim = 33, // 0x00000021
	DoubleQuote = 34, // 0x00000022
	Hash = 35, // 0x00000023
	Dollar = 36, // 0x00000024
	Ampersand = 38, // 0x00000026
	Quote = 39, // 0x00000027
	LeftParen = 40, // 0x00000028
	RightParen = 41, // 0x00000029
	Asterisk = 42, // 0x0000002A
	Plus = 43, // 0x0000002B
	Comma = 44, // 0x0000002C
	Minus = 45, // 0x0000002D
	Period = 46, // 0x0000002E
	Slash = 47, // 0x0000002F
	Alpha0 = 48, // 0x00000030
	Alpha1 = 49, // 0x00000031
	Alpha2 = 50, // 0x00000032
	Alpha3 = 51, // 0x00000033
	Alpha4 = 52, // 0x00000034
	Alpha5 = 53, // 0x00000035
	Alpha6 = 54, // 0x00000036
	Alpha7 = 55, // 0x00000037
	Alpha8 = 56, // 0x00000038
	Alpha9 = 57, // 0x00000039
	Colon = 58, // 0x0000003A
	Semicolon = 59, // 0x0000003B
	Less = 60, // 0x0000003C
	Equals = 61, // 0x0000003D
	Greater = 62, // 0x0000003E
	Question = 63, // 0x0000003F
	At = 64, // 0x00000040
	LeftBracket = 91, // 0x0000005B
	Backslash = 92, // 0x0000005C
	RightBracket = 93, // 0x0000005D
	Caret = 94, // 0x0000005E
	Underscore = 95, // 0x0000005F
	BackQuote = 96, // 0x00000060
	A = 97, // 0x00000061
	B = 98, // 0x00000062
	C = 99, // 0x00000063
	D = 100, // 0x00000064
	E = 101, // 0x00000065
	F = 102, // 0x00000066
	G = 103, // 0x00000067
	H = 104, // 0x00000068
	I = 105, // 0x00000069
	J = 106, // 0x0000006A
	K = 107, // 0x0000006B
	L = 108, // 0x0000006C
	M = 109, // 0x0000006D
	N = 110, // 0x0000006E
	O = 111, // 0x0000006F
	P = 112, // 0x00000070
	Q = 113, // 0x00000071
	R = 114, // 0x00000072
	S = 115, // 0x00000073
	T = 116, // 0x00000074
	U = 117, // 0x00000075
	V = 118, // 0x00000076
	W = 119, // 0x00000077
	X = 120, // 0x00000078
	Y = 121, // 0x00000079
	Z = 122, // 0x0000007A
	Delete = 127, // 0x0000007F
	Keypad0 = 256, // 0x00000100
	Keypad1 = 257, // 0x00000101
	Keypad2 = 258, // 0x00000102
	Keypad3 = 259, // 0x00000103
	Keypad4 = 260, // 0x00000104
	Keypad5 = 261, // 0x00000105
	Keypad6 = 262, // 0x00000106
	Keypad7 = 263, // 0x00000107
	Keypad8 = 264, // 0x00000108
	Keypad9 = 265, // 0x00000109
	KeypadPeriod = 266, // 0x0000010A
	KeypadDivide = 267, // 0x0000010B
	KeypadMultiply = 268, // 0x0000010C
	KeypadMinus = 269, // 0x0000010D
	KeypadPlus = 270, // 0x0000010E
	KeypadEnter = 271, // 0x0000010F
	KeypadEquals = 272, // 0x00000110
	UpArrow = 273, // 0x00000111
	DownArrow = 274, // 0x00000112
	RightArrow = 275, // 0x00000113
	LeftArrow = 276, // 0x00000114
	Insert = 277, // 0x00000115
	Home = 278, // 0x00000116
	End = 279, // 0x00000117
	PageUp = 280, // 0x00000118
	PageDown = 281, // 0x00000119
	F1 = 282, // 0x0000011A
	F2 = 283, // 0x0000011B
	F3 = 284, // 0x0000011C
	F4 = 285, // 0x0000011D
	F5 = 286, // 0x0000011E
	F6 = 287, // 0x0000011F
	F7 = 288, // 0x00000120
	F8 = 289, // 0x00000121
	F9 = 290, // 0x00000122
	F10 = 291, // 0x00000123
	F11 = 292, // 0x00000124
	F12 = 293, // 0x00000125
	F13 = 294, // 0x00000126
	F14 = 295, // 0x00000127
	F15 = 296, // 0x00000128
	Numlock = 300, // 0x0000012C
	CapsLock = 301, // 0x0000012D
	ScrollLock = 302, // 0x0000012E
	RightShift = 303, // 0x0000012F
	LeftShift = 304, // 0x00000130
	RightControl = 305, // 0x00000131
	LeftControl = 306, // 0x00000132
	RightAlt = 307, // 0x00000133
	LeftAlt = 308, // 0x00000134
	RightApple = 309, // 0x00000135
	RightCommand = 309, // 0x00000135
	LeftApple = 310, // 0x00000136
	LeftCommand = 310, // 0x00000136
	LeftWindows = 311, // 0x00000137
	RightWindows = 312, // 0x00000138
	AltGr = 313, // 0x00000139
	Help = 315, // 0x0000013B
	Print = 316, // 0x0000013C
	SysReq = 317, // 0x0000013D
	Break = 318, // 0x0000013E
	Menu = 319, // 0x0000013F
	Mouse0 = 323, // 0x00000143
	Mouse1 = 324, // 0x00000144
	Mouse2 = 325, // 0x00000145
	Mouse3 = 326, // 0x00000146
	Mouse4 = 327, // 0x00000147
	Mouse5 = 328, // 0x00000148
	Mouse6 = 329, // 0x00000149
	JoystickButton0 = 330, // 0x0000014A
	JoystickButton1 = 331, // 0x0000014B
	JoystickButton2 = 332, // 0x0000014C
	JoystickButton3 = 333, // 0x0000014D
	JoystickButton4 = 334, // 0x0000014E
	JoystickButton5 = 335, // 0x0000014F
	JoystickButton6 = 336, // 0x00000150
	JoystickButton7 = 337, // 0x00000151
	JoystickButton8 = 338, // 0x00000152
	JoystickButton9 = 339, // 0x00000153
	JoystickButton10 = 340, // 0x00000154
	JoystickButton11 = 341, // 0x00000155
	JoystickButton12 = 342, // 0x00000156
	JoystickButton13 = 343, // 0x00000157
	JoystickButton14 = 344, // 0x00000158
	JoystickButton15 = 345, // 0x00000159
	JoystickButton16 = 346, // 0x0000015A
	JoystickButton17 = 347, // 0x0000015B
	JoystickButton18 = 348, // 0x0000015C
	JoystickButton19 = 349, // 0x0000015D
	Joystick1Button0 = 350, // 0x0000015E
	Joystick1Button1 = 351, // 0x0000015F
	Joystick1Button2 = 352, // 0x00000160
	Joystick1Button3 = 353, // 0x00000161
	Joystick1Button4 = 354, // 0x00000162
	Joystick1Button5 = 355, // 0x00000163
	Joystick1Button6 = 356, // 0x00000164
	Joystick1Button7 = 357, // 0x00000165
	Joystick1Button8 = 358, // 0x00000166
	Joystick1Button9 = 359, // 0x00000167
	Joystick1Button10 = 360, // 0x00000168
	Joystick1Button11 = 361, // 0x00000169
	Joystick1Button12 = 362, // 0x0000016A
	Joystick1Button13 = 363, // 0x0000016B
	Joystick1Button14 = 364, // 0x0000016C
	Joystick1Button15 = 365, // 0x0000016D
	Joystick1Button16 = 366, // 0x0000016E
	Joystick1Button17 = 367, // 0x0000016F
	Joystick1Button18 = 368, // 0x00000170
	Joystick1Button19 = 369, // 0x00000171
	Joystick2Button0 = 370, // 0x00000172
	Joystick2Button1 = 371, // 0x00000173
	Joystick2Button2 = 372, // 0x00000174
	Joystick2Button3 = 373, // 0x00000175
	Joystick2Button4 = 374, // 0x00000176
	Joystick2Button5 = 375, // 0x00000177
	Joystick2Button6 = 376, // 0x00000178
	Joystick2Button7 = 377, // 0x00000179
	Joystick2Button8 = 378, // 0x0000017A
	Joystick2Button9 = 379, // 0x0000017B
	Joystick2Button10 = 380, // 0x0000017C
	Joystick2Button11 = 381, // 0x0000017D
	Joystick2Button12 = 382, // 0x0000017E
	Joystick2Button13 = 383, // 0x0000017F
	Joystick2Button14 = 384, // 0x00000180
	Joystick2Button15 = 385, // 0x00000181
	Joystick2Button16 = 386, // 0x00000182
	Joystick2Button17 = 387, // 0x00000183
	Joystick2Button18 = 388, // 0x00000184
	Joystick2Button19 = 389, // 0x00000185
	Joystick3Button0 = 390, // 0x00000186
	Joystick3Button1 = 391, // 0x00000187
	Joystick3Button2 = 392, // 0x00000188
	Joystick3Button3 = 393, // 0x00000189
	Joystick3Button4 = 394, // 0x0000018A
	Joystick3Button5 = 395, // 0x0000018B
	Joystick3Button6 = 396, // 0x0000018C
	Joystick3Button7 = 397, // 0x0000018D
	Joystick3Button8 = 398, // 0x0000018E
	Joystick3Button9 = 399, // 0x0000018F
	Joystick3Button10 = 400, // 0x00000190
	Joystick3Button11 = 401, // 0x00000191
	Joystick3Button12 = 402, // 0x00000192
	Joystick3Button13 = 403, // 0x00000193
	Joystick3Button14 = 404, // 0x00000194
	Joystick3Button15 = 405, // 0x00000195
	Joystick3Button16 = 406, // 0x00000196
	Joystick3Button17 = 407, // 0x00000197
	Joystick3Button18 = 408, // 0x00000198
	Joystick3Button19 = 409, // 0x00000199
	Joystick4Button0 = 410, // 0x0000019A
	Joystick4Button1 = 411, // 0x0000019B
	Joystick4Button2 = 412, // 0x0000019C
	Joystick4Button3 = 413, // 0x0000019D
	Joystick4Button4 = 414, // 0x0000019E
	Joystick4Button5 = 415, // 0x0000019F
	Joystick4Button6 = 416, // 0x000001A0
	Joystick4Button7 = 417, // 0x000001A1
	Joystick4Button8 = 418, // 0x000001A2
	Joystick4Button9 = 419, // 0x000001A3
	Joystick4Button10 = 420, // 0x000001A4
	Joystick4Button11 = 421, // 0x000001A5
	Joystick4Button12 = 422, // 0x000001A6
	Joystick4Button13 = 423, // 0x000001A7
	Joystick4Button14 = 424, // 0x000001A8
	Joystick4Button15 = 425, // 0x000001A9
	Joystick4Button16 = 426, // 0x000001AA
	Joystick4Button17 = 427, // 0x000001AB
	Joystick4Button18 = 428, // 0x000001AC
	Joystick4Button19 = 429, // 0x000001AD
	Joystick5Button0 = 430, // 0x000001AE
	Joystick5Button1 = 431, // 0x000001AF
	Joystick5Button2 = 432, // 0x000001B0
	Joystick5Button3 = 433, // 0x000001B1
	Joystick5Button4 = 434, // 0x000001B2
	Joystick5Button5 = 435, // 0x000001B3
	Joystick5Button6 = 436, // 0x000001B4
	Joystick5Button7 = 437, // 0x000001B5
	Joystick5Button8 = 438, // 0x000001B6
	Joystick5Button9 = 439, // 0x000001B7
	Joystick5Button10 = 440, // 0x000001B8
	Joystick5Button11 = 441, // 0x000001B9
	Joystick5Button12 = 442, // 0x000001BA
	Joystick5Button13 = 443, // 0x000001BB
	Joystick5Button14 = 444, // 0x000001BC
	Joystick5Button15 = 445, // 0x000001BD
	Joystick5Button16 = 446, // 0x000001BE
	Joystick5Button17 = 447, // 0x000001BF
	Joystick5Button18 = 448, // 0x000001C0
	Joystick5Button19 = 449, // 0x000001C1
	Joystick6Button0 = 450, // 0x000001C2
	Joystick6Button1 = 451, // 0x000001C3
	Joystick6Button2 = 452, // 0x000001C4
	Joystick6Button3 = 453, // 0x000001C5
	Joystick6Button4 = 454, // 0x000001C6
	Joystick6Button5 = 455, // 0x000001C7
	Joystick6Button6 = 456, // 0x000001C8
	Joystick6Button7 = 457, // 0x000001C9
	Joystick6Button8 = 458, // 0x000001CA
	Joystick6Button9 = 459, // 0x000001CB
	Joystick6Button10 = 460, // 0x000001CC
	Joystick6Button11 = 461, // 0x000001CD
	Joystick6Button12 = 462, // 0x000001CE
	Joystick6Button13 = 463, // 0x000001CF
	Joystick6Button14 = 464, // 0x000001D0
	Joystick6Button15 = 465, // 0x000001D1
	Joystick6Button16 = 466, // 0x000001D2
	Joystick6Button17 = 467, // 0x000001D3
	Joystick6Button18 = 468, // 0x000001D4
	Joystick6Button19 = 469, // 0x000001D5
	Joystick7Button0 = 470, // 0x000001D6
	Joystick7Button1 = 471, // 0x000001D7
	Joystick7Button2 = 472, // 0x000001D8
	Joystick7Button3 = 473, // 0x000001D9
	Joystick7Button4 = 474, // 0x000001DA
	Joystick7Button5 = 475, // 0x000001DB
	Joystick7Button6 = 476, // 0x000001DC
	Joystick7Button7 = 477, // 0x000001DD
	Joystick7Button8 = 478, // 0x000001DE
	Joystick7Button9 = 479, // 0x000001DF
	Joystick7Button10 = 480, // 0x000001E0
	Joystick7Button11 = 481, // 0x000001E1
	Joystick7Button12 = 482, // 0x000001E2
	Joystick7Button13 = 483, // 0x000001E3
	Joystick7Button14 = 484, // 0x000001E4
	Joystick7Button15 = 485, // 0x000001E5
	Joystick7Button16 = 486, // 0x000001E6
	Joystick7Button17 = 487, // 0x000001E7
	Joystick7Button18 = 488, // 0x000001E8
	Joystick7Button19 = 489, // 0x000001E9
	Joystick8Button0 = 490, // 0x000001EA
	Joystick8Button1 = 491, // 0x000001EB
	Joystick8Button2 = 492, // 0x000001EC
	Joystick8Button3 = 493, // 0x000001ED
	Joystick8Button4 = 494, // 0x000001EE
	Joystick8Button5 = 495, // 0x000001EF
	Joystick8Button6 = 496, // 0x000001F0
	Joystick8Button7 = 497, // 0x000001F1
	Joystick8Button8 = 498, // 0x000001F2
	Joystick8Button9 = 499, // 0x000001F3
	Joystick8Button10 = 500, // 0x000001F4
	Joystick8Button11 = 501, // 0x000001F5
	Joystick8Button12 = 502, // 0x000001F6
	Joystick8Button13 = 503, // 0x000001F7
	Joystick8Button14 = 504, // 0x000001F8
	Joystick8Button15 = 505, // 0x000001F9
	Joystick8Button16 = 506, // 0x000001FA
	Joystick8Button17 = 507, // 0x000001FB
	Joystick8Button18 = 508, // 0x000001FC
	Joystick8Button19 = 509, // 0x000001FD
};

enum FullScreenMode {
	ExclusiveFullScreen,
	FullScreenWindow,
	MaximizedWindow,
	Windowed
};

// UnityEngine.ScreenOrientation
enum class ScreenOrientation {
	Unknown,
	Portrait,
	PortraitUpsideDown,
	LandscapeLeft,
	LandscapeRight,
	AutoRotation,
	Landscape = 3
};

//Gallop.ModelLoader.ZekkenColor
enum class ZekkenColor {
	DeepBlue,
	DeepRed,
	DeepGreen,
	Black,
	White,
	Count,
	Default = 3
};

//Gallop.ModelLoader.ZekkenFontColor
enum class ZekkenFontColor
{
	Black,
	White,
	Yellow,
	Count,
	Default = 1
};

//Gallop.ModelLoader.ControllerType
enum class ControllerType
{
	Default,
	Race,
	Training,
	EventTimeline,
	Live,
	HomeStand,
	HomeTalk,
	HomeWalk,
	CutIn,
	TrainingTop,
	SingleRace,
	Simple,
	Mini,
	Paddock,
	Champions
};

//Gallop.ModelLoader.TrackSuitColor
enum class TrackSuitColor
{
	White,
	Black,
	Red,
	Blue,
	Yellow,
	Green,
	Orange,
	Peach
};

//Gallop.CySpringDataContainer.Category
namespace CySpringDataContainer {
	enum class Category {
		Invalid = -1,
		Live,
		Race,
		Story,
		Home,
		Training
	};
}

// UnityEngine.Color
struct Color_t
{
public:
	// System.Single UnityEngine.Color::r
	float r;
	// System.Single UnityEngine.Color::g
	float g;
	// System.Single UnityEngine.Color::b
	float b;
	// System.Single UnityEngine.Color::a
	float a;
};

// UnityEngine.Vector2
struct Vector2_t
{
public:
	// System.Single UnityEngine.Vector2::x
	float x;
	// System.Single UnityEngine.Vector2::y
	float y;
};

struct Vector2_Int_t {
	int m_X;
	int m_Y;
};


// UnityEngine.Vector3
struct Vector3_t
{
public:
	// System.Single UnityEngine.Vector3::x
	float x;
	// System.Single UnityEngine.Vector3::y
	float y;
	// System.Single UnityEngine.Vector3::z
	float z;
};
enum Il2CppTypeEnum
{
	IL2CPP_TYPE_END = 0x00,       /* End of List */
	IL2CPP_TYPE_VOID = 0x01,
	IL2CPP_TYPE_BOOLEAN = 0x02,
	IL2CPP_TYPE_CHAR = 0x03,
	IL2CPP_TYPE_I1 = 0x04,
	IL2CPP_TYPE_U1 = 0x05,
	IL2CPP_TYPE_I2 = 0x06,
	IL2CPP_TYPE_U2 = 0x07,
	IL2CPP_TYPE_I4 = 0x08,
	IL2CPP_TYPE_U4 = 0x09,
	IL2CPP_TYPE_I8 = 0x0a,
	IL2CPP_TYPE_U8 = 0x0b,
	IL2CPP_TYPE_R4 = 0x0c,
	IL2CPP_TYPE_R8 = 0x0d,
	IL2CPP_TYPE_STRING = 0x0e,
	IL2CPP_TYPE_PTR = 0x0f,
	IL2CPP_TYPE_BYREF = 0x10,
	IL2CPP_TYPE_VALUETYPE = 0x11,
	IL2CPP_TYPE_CLASS = 0x12,
	IL2CPP_TYPE_VAR = 0x13,
	IL2CPP_TYPE_ARRAY = 0x14,
	IL2CPP_TYPE_GENERICINST = 0x15,
	IL2CPP_TYPE_TYPEDBYREF = 0x16,
	IL2CPP_TYPE_I = 0x18,
	IL2CPP_TYPE_U = 0x19,
	IL2CPP_TYPE_FNPTR = 0x1b,
	IL2CPP_TYPE_OBJECT = 0x1c,
	IL2CPP_TYPE_SZARRAY = 0x1d,
	IL2CPP_TYPE_MVAR = 0x1e,
	IL2CPP_TYPE_CMOD_REQD = 0x1f,
	IL2CPP_TYPE_CMOD_OPT = 0x20,
	IL2CPP_TYPE_INTERNAL = 0x21,

	IL2CPP_TYPE_MODIFIER = 0x40,
	IL2CPP_TYPE_SENTINEL = 0x41,
	IL2CPP_TYPE_PINNED = 0x45,

	IL2CPP_TYPE_ENUM = 0x55
};

typedef struct Il2CppType
{
	void* dummy;
	unsigned int attrs : 16;
	Il2CppTypeEnum type : 8;
	unsigned int num_mods : 6;
	unsigned int byref : 1;
	unsigned int pinned : 1;
} Il2CppType;

struct Il2CppClassHead
{
	const void* image;
	void* gc_desc;
	const char* name;
	const char* namespaze;
};

struct ParameterInfo
{
	const char* name;
	int32_t position;
	uint32_t token;
	const Il2CppType* parameter_type;
};



struct Il2CppObject
{
	union
	{
		void* klass;
		void* vtable;
	};
	void* monitor;
};

typedef struct Il2CppArraySize
{
	Il2CppObject obj;
	void* bounds;
	uintptr_t max_length;
	alignas(8)
		void* vector[0];
} Il2CppArraySize;


struct Resolution_t
{
public:
	int width;
	int height;
	int herz;
};

struct LoadSceneParameters {
public:
	int LoadSceneMode;
	int LocalPhysicsMode;
};

// UnityEngine.TextGenerationSettings
struct TextGenerationSettings_t
{
public:
	// UnityEngine.Font UnityEngine.TextGenerationSettings::font
	void* font;
	// UnityEngine.Color UnityEngine.TextGenerationSettings::color
	Color_t color;
	// System.Int32 UnityEngine.TextGenerationSettings::fontSize
	int32_t fontSize;
	// System.Single UnityEngine.TextGenerationSettings::lineSpacing
	float lineSpacing;
	// System.Boolean UnityEngine.TextGenerationSettings::richText
	bool richText;
	// System.Single UnityEngine.TextGenerationSettings::scaleFactor
	float scaleFactor;
	// UnityEngine.FontStyle UnityEngine.TextGenerationSettings::fontStyle
	int32_t fontStyle;
	// UnityEngine.TextAnchor UnityEngine.TextGenerationSettings::textAnchor
	int32_t textAnchor;
	// System.Boolean UnityEngine.TextGenerationSettings::alignByGeometry
	bool alignByGeometry;
	// System.Boolean UnityEngine.TextGenerationSettings::resizeTextForBestFit
	bool resizeTextForBestFit;
	// System.Int32 UnityEngine.TextGenerationSettings::resizeTextMinSize
	int32_t resizeTextMinSize;
	// System.Int32 UnityEngine.TextGenerationSettings::resizeTextMaxSize
	int32_t resizeTextMaxSize;
	// System.Boolean UnityEngine.TextGenerationSettings::updateBounds
	bool updateBounds;
	// UnityEngine.VerticalWrapMode UnityEngine.TextGenerationSettings::verticalOverflow
	int32_t verticalOverflow;
	// UnityEngine.HorizontalWrapMode UnityEngine.TextGenerationSettings::horizontalOverflow
	int32_t horizontalOverflow;
	// UnityEngine.Vector2 UnityEngine.TextGenerationSettings::generationExtents
	Vector2_t  generationExtents;
	// UnityEngine.Vector2 UnityEngine.TextGenerationSettings::pivot
	Vector2_t  pivot;
	// System.Boolean UnityEngine.TextGenerationSettings::generateOutOfBounds
	bool generateOutOfBounds;
};

struct MethodInfo
{
	uintptr_t methodPointer;
	uintptr_t invoker_method;
	const char* name;
	uintptr_t klass;
	uintptr_t return_type;
	const ParameterInfo* parameters;
	uintptr_t methodDefinition;
	uintptr_t genericContainer;
	uint32_t token;
	uint16_t flags;
	uint16_t iflags;
	uint16_t slot;
	uint8_t parameters_count;
	uint8_t is_generic : 1;
	uint8_t is_inflated : 1;
	uint8_t wrapper_type : 1;
	uint8_t is_marshaled_from_native : 1;
};

// not real Il2CppString class
struct Il2CppString
{
	void* Empty;
	void* WhiteChars;
	int32_t length;
	wchar_t start_char[1];
};

struct Il2CppString_unk
{
	LPVOID Empty;
	LPVOID WhiteChars;
	LPVOID length;
	char start_char;
};

struct Il2CppChar
{
	LPVOID Empty;
	LPVOID WhiteChars;
	LPVOID unk1;
	long long length;
	short start_char;
};

typedef struct FieldInfo
{
	const char* name;
	const Il2CppType* type;
	void* parent;
	int32_t offset; // If offset is -1, then it's thread static
	uint32_t token;
} FieldInfo;

typedef struct Il2CppClass
{
	// The following fields are always valid for a Il2CppClass structure
	const void* image;
	void* gc_desc;
	const char* name;
	const char* namespaze;
	Il2CppType byval_arg;
	Il2CppType this_arg;
	Il2CppClass* element_class;
	Il2CppClass* castClass;
	Il2CppClass* declaringType;
	Il2CppClass* parent;
	void* generic_class;
	void* typeMetadataHandle; // non-NULL for Il2CppClass's constructed from type defintions
	const void* interopData;
	Il2CppClass* klass; // hack to pretend we are a MonoVTable. Points to ourself
	// End always valid fields

	// The following fields need initialized before access. This can be done per field or as an aggregate via a call to Class::Init
	FieldInfo* fields; // Initialized in SetupFields
	const void* events; // Initialized in SetupEvents
	const void* properties; // Initialized in SetupProperties
	const MethodInfo** methods; // Initialized in SetupMethods
	Il2CppClass** nestedTypes; // Initialized in SetupNestedTypes
	Il2CppClass** implementedInterfaces; // Initialized in SetupInterfaces
	void* interfaceOffsets; // Initialized in Init
	void* static_fields; // Initialized in Init
	const void* rgctx_data; // Initialized in Init
	// used for fast parent checks
	Il2CppClass** typeHierarchy; // Initialized in SetupTypeHierachy
	// End initialization required fields

	void* unity_user_data;

	uint32_t initializationExceptionGCHandle;

	uint32_t cctor_started;
	uint32_t cctor_finished;
	size_t cctor_thread;

	// Remaining fields are always valid except where noted
	void* genericContainerHandle;
	uint32_t instance_size; // valid when size_inited is true
	uint32_t actualSize;
	uint32_t element_size;
	int32_t native_size;
	uint32_t static_fields_size;
	uint32_t thread_static_fields_size;
	int32_t thread_static_fields_offset;
	uint32_t flags;
	uint32_t token;

	uint16_t method_count; // lazily calculated for arrays, i.e. when rank > 0
	uint16_t property_count;
	uint16_t field_count;
	uint16_t event_count;
	uint16_t nested_type_count;
	uint16_t vtable_count; // lazily calculated for arrays, i.e. when rank > 0
	uint16_t interfaces_count;
	uint16_t interface_offsets_count; // lazily calculated for arrays, i.e. when rank > 0

	uint8_t typeHierarchyDepth; // Initialized in SetupTypeHierachy
	uint8_t genericRecursionDepth;
	uint8_t rank;
	uint8_t minimumAlignment; // Alignment of this type
	uint8_t naturalAligment; // Alignment of this type without accounting for packing
	uint8_t packingSize;

	// this is critical for performance of Class::InitFromCodegen. Equals to initialized && !has_initialization_error at all times.
	// Use Class::UpdateInitializedAndNoError to update
	uint8_t initialized_and_no_error : 1;

	uint8_t valuetype : 1;
	uint8_t initialized : 1;
	uint8_t enumtype : 1;
	uint8_t is_generic : 1;
	uint8_t has_references : 1; // valid when size_inited is true
	uint8_t init_pending : 1;
	uint8_t size_init_pending : 1;
	uint8_t size_inited : 1;
	uint8_t has_finalize : 1;
	uint8_t has_cctor : 1;
	uint8_t is_blittable : 1;
	uint8_t is_import_or_windows_runtime : 1;
	uint8_t is_vtable_initialized : 1;
	uint8_t has_initialization_error : 1;
	void* vtable[0];
} Il2CppClass;



typedef struct PropertyInfo {
	Il2CppClass* parent;
	const char* name;
	const MethodInfo* get;
	const MethodInfo* set;
	uint32_t attrs;
	uint32_t token;
} PropertyInfo;



static const size_t kIl2CppSizeOfArray = (offsetof(Il2CppArraySize, vector));

//CodeStage.AntiCheat.ObscuredTypes.ObscuredInt
struct ObscuredInt {
	int32_t currentCryptoKey;
	int32_t hiddenValue;
	bool inited;
	int32_t fakeValue;
	bool fakeValueActive;
};

//Gallop.MasterLiveData.LiveData
struct __declspec(align(8)) LiveData {
public:
	char dummy[0x10];
	int32_t MusicId;
	int32_t Sort;
	int32_t MusicType;
	Il2CppString* TitleColorTop;
	Il2CppString* TitleColorBottom;
	int32_t ConditionType;
	int32_t SongCharaType;
	int32_t LiveMemberNumber;
	int32_t DefaultMainDress;
	int32_t DefaultMainDressColor;
	int32_t DefaultMobDress;
	int32_t DefaultMobDressColor;
	int32_t BackdancerOrder;
	int32_t BackdancerDress;
	int32_t BackdancerDressColor;
	int32_t HasLive;
	int64_t StartDate;
	int64_t EndDate;
	int32_t _eConditionType;
	int32_t _eSongCharaType;
	int32_t _eMusicType;
};

//Gallop.LiveTheaterMemberInfo
struct __declspec(align(8)) LiveTheaterMemberInfo {
public:
	long dummy1;
	long dummy2;
	int32_t chara_id;
	int32_t mob_id;
	int32_t dress_id;
	int32_t color_id;
	int32_t dress_id2;
	int32_t color_id2;
};

//Gallop.CharaDressIdSet
struct __declspec(align(8)) CharaDressIdSet {
public:
	long dummy1;
	long dummy2;
	bool _IsMob_k__BackingField;
	int32_t _DressColorId_k__BackingField;
	int32_t _DressColorId2_k__BackingField;
	//struct System_Func_int__o* _getRandomMob;
	int dummy3;
	int32_t _charaId;
	int32_t _dressId;
	int32_t _dressId2;
};

//Gallop.MobBuildInfo
struct MobBuildInfo {
public:
	int32_t faceKindId;
	int32_t hairKindId;
	int32_t hairColorId;
	int32_t DressColorId;
	float HairCutoff;
	int32_t AttachmentModelId;
};

//Gallop.MasterDressData
struct MasterDressData {
public:
	//long dummy1;
	//long dummy2;
	char dummy[0x10];
	int32_t Id;
	int32_t ConditionType;
	bool HaveMini;
	int32_t GeneralPurpose;
	int32_t CharaId;
	int32_t UseGender;
	int32_t BodyType;
	int32_t BodyTypeSub;
	int32_t BodySetting;
	int32_t UseRace;
	int32_t UseLive;
	int32_t UseLiveTheater;
	int32_t UseHome;
	int32_t IsWet;
	int32_t IsDirt;
	int32_t HeadSubId;
	int32_t UseSeason;
	char dummy1[4];
	Il2CppString* DressColorMain;
	Il2CppString* DressColorSub;
	int32_t ColorNum;
	int32_t DispOrder;
	int32_t TailModelId;
	int32_t TailModelSubId;
	int64_t StartTime;
	int64_t EndTime;
	int32_t _getCondition;
};


//Gallop.CharacterBuildInfo
struct CharacterBuildInfo {
public:
	char dummy[0x10];
	int32_t _cardId;
	int32_t _charaId;
	int32_t _mobId;
	int32_t _headModelSubId;
	bool _isUseDressDataHeadModelSubId;
	int32_t _bodyModelSubId;
	int32_t _dressId;
	int32_t _controllerType;
	int32_t _zekken;
	int dummy5;
	Il2CppString* _name;
	int32_t _genderType;
	int32_t _heightType;
	int32_t _bodySize;
	int32_t _bustType;
	float _height;
	int32_t _skinType;
	int32_t _socksType;
	int32_t _defaultPersonalityType;
	int32_t _raceGateInPersonalityType;
	int32_t _raceRunPersonalityType;
	int32_t _raceOverRunPersonalityType;
	int32_t _raceRunningType;
	int32_t _zekkenColor;
	int32_t _zekkenFontColor;
	int32_t _zekkenFontStyle;
	void* _wetTextureArray;
	void* _dirtTextureArray;
	void* _sweatLocator;
	void* _sweatObject;

	int32_t _frameColor;
	int32_t _popularity;
	int32_t _npcType;
	void* _charaBuildPathInfo;
	void* _clothBuildPathInfo;
	MasterDressData* _dressElement;
	int32_t _backDancerColorId;
	struct MobBuildInfo _mobInfo;
	bool _isPersonalDress;
	int32_t _miniMobTailId;
	int32_t _miniMobParentCharaId;
	int32_t _overrideClothCategory;
	int32_t _loadHashKey;
	bool _isPreCreatedLoadHashKey;
	bool _initialized;


};

//Gallop.CutIn.Cutt.TimelineKeyCharacterType
enum TimelineKeyCharacterType
{
	User,
	Target,
	FixRank,
	FixCharaId,
	CardId,
	Mob
};

//Gallop.CutIn.CutInCharacterCreateInfo
struct CutInCharacterCreateInfo
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	int _characterType; // 0x10
	int _charaIndex; // 0x14
	LPVOID _parent; // 0x18
	int _charaId; // 0x20
	int _clothId; // 0x24
	int _headId; // 0x28
	int IsUseDressDataHeadModelSubId; // 0x2C
	int _zekkenNo; // 0x30
	bool _isWet; // 0x34
	bool _isDirt; // 0x35
	short unknown3; // 0x36
	int OverrideClothCategory; // 0x38
};


//libnative.sqlite3.dbconnection
struct Connection {
	char dummy[0x10];
	uintptr_t Handle;
	Il2CppString* dbPath;
};

//int charaS[] = {1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015};

template<typename T>
struct Nullable
{
	T value;
	bool has_value;

	bool HasValue() const { return has_value; }
	const T& GetValue() const { return value; }
	void SetValue(T& newValue) { value = newValue; }
};

//LibNative.Sqlite3
struct DBConnection_t
{
public:
	void* DBHandle;
	Il2CppString* dbPath;
};

struct LiveQualitySettings {
	// Token: 0x0400879B RID: 34715
	bool IsMonitor;
	// Token: 0x0400879C RID: 34716
	bool IsRealShadow;
	// Token: 0x0400879D RID: 34717
	bool IsProjector;
	// Token: 0x0400879E RID: 34718
	bool IsReflection;
	// Token: 0x0400879F RID: 34719
	bool IsMonitorCamera;
	// Token: 0x040087A0 RID: 34720
	bool IsLensflare;
	// Token: 0x040087A1 RID: 34721
	bool IsMirrorScan;
	// Token: 0x040087A2 RID: 34722
	bool IsCameraShake;
	// Token: 0x040087A3 RID: 34723
	bool IsMob2D;
};

struct CutInCharacter
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	CutInCharacterCreateInfo* _createInfo; // 0x10
	LPVOID _createType; // 0x18
	LPVOID _model; // 0x20
	LPVOID _ikController; // 0x28
	LPVOID _isAddIKControllerFromCutIn; // 0x30
	LPVOID  _sweatController; // 0x38
	LPVOID _animator; // 0x40
	LPVOID _isCreateAnimator; // 0x48
	LPVOID _modelRendererHolder; // 0x50
	LPVOID _footShadowController; // 0x58
	LPVOID __isAddFootShadowController; // 0x60
	LPVOID _eyeTraceController; // 0x68
	LPVOID _emissiveScrollList; // 0x70
	LPVOID _cutInMode; // 0x78
	LPVOID _cachedPos; // 0x80
	int unknown3; //0x88
	LPVOID _cachedRot; // 0x8C
	LPVOID unknown4; // 0x94
	LPVOID _cachedScale; // 0x9C
	int unknown5; //0xA4
	int k__BackingField; // 0xA8
	int _originClothCategory; // 0xAC
};

struct GachaMainViewController
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	LPVOID unknown3; //0x10
	LPVOID unknown4; //0x18
	LPVOID unknown5; //0x20
	LPVOID unknown6; //0x28
	LPVOID _viewInfo; // 0x30
	bool _isSkipAct; // 0x38
	bool _isNextCut; // 0x39
	bool _isRequestedChangeView; // 0x3A
	bool unknown7; //0x3B
	int unknown8; //0x3C
	LPVOID _rootObj; // 0x40
	LPVOID _context; // 0x48
	LPVOID _cutList; // 0x50
	LPVOID _playingIndex; // 0x58
	LPVOID _soundLoadCharaIdsGacha; // 0x60
	LPVOID _soundLoadSeCueSheetList; // 0x68
	LPVOID _cardBGCamera; // 0x70
	LPVOID _cardCamera; // 0x78
	LPVOID _cardCameraTrs; // 0x80
	LPVOID _isLockGameCanvas; // 0x88
};
struct MoveNext
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	LPVOID state; // 0x10
	LPVOID current; // 0x18
	GachaMainViewController* _this; // 0x20
	int cardCount; // 0x28
	int cutCount; // 0x2C
	int index; // 0x30
};

struct TimelineKeyCharacterDataList
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	LPVOID unknown3; //0x10
	LPVOID unknown4; //0x18
	LPVOID unknown5; //0x20
	LPVOID unknown6; //0x28
	LPVOID unknown7; //0x30
	LPVOID unknown8; //0x38
	LPVOID unknown9; //0x40
	int _selectCharaId; //0x48
	int _selectClothId; // 0x4C
	int _selectHeadId; // 0x50
};

struct TimelineCharacterGroupData
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	int* _types; // 0x10
	TimelineCharacter* _chara; // 0x18
	int _characterColorNum; // 0x20
	int CharacterWindNum; // 0x24
	int _characterShadowNum; // 0x28
	int _characterFootSmokeNum; // 0x2C
	int _characterSweatNum; // 0x30
	int _characterCollisionNum; // 0x34
	int CharacterSpringCollisionNum; // 0x38
	int _characterIKNum; // 0x3C
	int CharacterPartsNum; // 0x40
	int unknown3; //0x44
	TimelineKeyCharacterDataList* _characterKeys; // 0x48
};

struct TimelineCharacter
{
	LPVOID unknown1; //0x00
	LPVOID unknown2; //0x08
	LPVOID _controller; // 0x10
	TimelineCharacterGroupData* _data; // 0x18
	LPVOID _transformObject; // 0x20
	LPVOID _transform; // 0x28
	LPVOID _animation; // 0x30
	int _motionTriggeredKeyIndex; // 0x38
	int _selectCharaId; // 0x3C
	int _selectClothId; // 0x40
	int _selectHeadId; // 0x44
	int _isUseDressDataHeadModelSubId; // 0x48
	int _charaIndex; // 0x4C
	LPVOID _animator; // 0x50
	CutInCharacter* _cutInCharacter; // 0x58
};

struct EyeReflectionController {
	char dummy[0x28];
    void* _cameraObject;
    void* _transform;
    void* _camera;
    void* _eyeReflection;
    bool _isEyeReflection;
    bool _isHighReso;
    void* _context;
};

struct EyeHighlightController_HighlightKeyLocator {
	char dummy[10];
	int _highlightType;
	char dummy2[8];
	LPVOID _locator;
	bool IsEnable;
};

struct EyeHighlightController_context {
	LPVOID EyeMaterial;
	EyeHighlightController_HighlightKeyLocator* KeyLocator;
};

struct LiveModelController_EyeReflectionContext {
	char dummy[10];
	bool IsEnabled;
	float Roll;
	float FieldOfView;
	float NearClipPlane;
	float FarClipPlane;
	int CullingMask;
	float Power;
	void* MaskTexture;
};

struct CutInModelController_Context {
	int CardId;
	int CharaId;
	int DressId;
	int HeadId;
	int ZekkenNo;
	ZekkenColor ZekkenColor;
	ZekkenFontColor ZekkenFontColor;
	TrackSuitColor FrameColor;
	ControllerType ControllerType;
	bool IsWet;
	bool IsDirt;
	CySpringDataContainer::Category  _overrideClothCategory;
	bool IsUseDressDataHeadModelSubId;
};

struct LiveTimelineWorkSheet {
	Il2CppString** sVersionList;
	Il2CppString* version;
	int targetCameraIndex;
	bool enableAtRuntime;
	bool enableAtEdit;
	float TotalTimeLength;
	bool Lyrics;
	int SheetType;

};

//Cute.Cri.Audio.RequestCueInfo
struct RequestCueInfo {
	Il2CppString* CueSheetName;
	Il2CppString* CueName;
	int CueId;
};

//Cyan.Loader.AssetLoader+AssetRequest
struct AssetRequest {
	char dummy[0xF];
	unsigned char kind;
	unsigned char state;
	char dummy1[0x7];
	Il2CppString* hname;
	Il2CppString* path;
};

// function types
typedef Il2CppString* (*il2cpp_string_new_utf16_t)(const wchar_t* str, unsigned int len);
typedef Il2CppString* (*il2cpp_string_new_t)(const char* str);
typedef void* (*il2cpp_domain_get_t)();
typedef void* (*il2cpp_domain_assembly_open_t)(void* domain, const char* name);
typedef void* (*il2cpp_assembly_get_image_t)(void* assembly);
typedef Il2CppClass* (*il2cpp_class_from_name_t)(void* image, const char* namespaze, const char* name);
typedef MethodInfo* (*il2cpp_class_get_methods_t)(Il2CppClass* klass, void** iter);
typedef MethodInfo* (*il2cpp_class_get_method_from_name_t)(void* klass, const char* name, int argsCount);
typedef MethodInfo* (*il2cpp_property_get_get_method_t)(void* prop);
typedef MethodInfo* (*il2cpp_property_get_set_method_t)(void* prop);
typedef Il2CppType* (*il2cpp_method_get_param_t)(const MethodInfo* method, uint32_t index);
typedef Il2CppObject* (*il2cpp_object_new_t)(Il2CppClass* klass);
typedef void* (*il2cpp_resolve_icall_t)(const char* name);
typedef void* (*il2cpp_array_new_t)(Il2CppClass* klass, uintptr_t count);
typedef void* (*il2cpp_thread_attach_t)(void* domain);
typedef void (*il2cpp_thread_detach_t)(void* thread);
typedef const Il2CppType* (*il2cpp_class_get_type_t)(Il2CppClass* klass);
typedef uint32_t(*il2cpp_class_get_type_token_t)(Il2CppClass* klass);
typedef FieldInfo* (*il2cpp_class_get_field_from_name_t)(Il2CppClass* klass, const char* name);
typedef void (*il2cpp_field_get_value_t)(Il2CppObject* obj, FieldInfo* field, void* value);
typedef void (*il2cpp_field_set_value_t)(Il2CppObject* obj, FieldInfo* field, void* value);
typedef void (*il2cpp_field_static_get_value_t)(FieldInfo* field, void* value);
typedef void (*il2cpp_field_static_set_value_t)(FieldInfo* field, void* value);
typedef const Il2CppType* (*il2cpp_field_get_type_t)(FieldInfo* field);
typedef void* (*il2cpp_class_get_nested_types_t)(void* klass, void** iter);
typedef Il2CppObject* (*il2cpp_type_get_object_t)(const Il2CppType* type);
typedef FieldInfo* (*il2cpp_class_get_fields_t)(Il2CppClass* klass, void** iter);
typedef void (*il2cpp_runtime_object_init_t)(Il2CppObject* obj);

typedef void* (*il2cpp_image_get_class_t)(void* image, size_t index);

typedef const char* (*il2cpp_image_get_name_t)(void* image);
typedef size_t(*il2cpp_image_get_class_count_t)(void* image);
typedef bool (*il2cpp_type_is_byref_t)(const Il2CppType* type);
typedef uint32_t(*il2cpp_method_get_flags_t)(const MethodInfo* mehod, uint32_t* iflags);
typedef const Il2CppType* (*il2cpp_method_get_return_type_t)(const MethodInfo* method);
typedef Il2CppClass* (*il2cpp_class_from_type_t)(const Il2CppType* type);
typedef const char* (*il2cpp_class_get_name_t)(Il2CppClass* klass);
typedef const PropertyInfo* (*il2cpp_class_get_properties_t)(Il2CppClass* klass, void** iter);
typedef bool (*il2cpp_class_is_enum_t)(const Il2CppClass* klass);
typedef FieldInfo* (*il2cpp_class_get_fields_t)(Il2CppClass* klass, void** iter);
typedef const char* (*il2cpp_method_get_name_t)(const MethodInfo* method);
typedef uint32_t(*il2cpp_method_get_param_count_t)(const MethodInfo* method);
typedef const char* (*il2cpp_method_get_param_name_t)(const MethodInfo* method, uint32_t index);
typedef Il2CppClass* (*il2cpp_class_get_parent_t)(Il2CppClass* klass);
typedef Il2CppClass* (*il2cpp_class_get_interfaces_t)(Il2CppClass* klass, void** iter);
typedef const char* (*il2cpp_class_get_namespace_t)(Il2CppClass* klass);
typedef int (*il2cpp_class_get_flags_t)(const Il2CppClass* klass);
typedef bool (*il2cpp_class_is_valuetype_t)(const Il2CppClass* klass);
typedef uint32_t(*il2cpp_property_get_flags_t) (PropertyInfo* prop);
typedef const char* (*il2cpp_property_get_name_t) (PropertyInfo* prop);
typedef Il2CppClass* (*il2cpp_property_get_parent_t) (PropertyInfo* prop);
typedef int (*il2cpp_field_get_flags_t)(FieldInfo* field);
typedef const char* (*il2cpp_field_get_name_t)(FieldInfo* field);
typedef Il2CppClass* (*il2cpp_field_get_parent_t)(FieldInfo* field);
typedef size_t(*il2cpp_field_get_offset_t)(FieldInfo* field);
typedef void** (*il2cpp_domain_get_assemblies_t)(void* domain, std::size_t* size);

// function defines
extern il2cpp_string_new_utf16_t il2cpp_string_new_utf16;
extern il2cpp_string_new_t il2cpp_string_new;
extern il2cpp_domain_get_t il2cpp_domain_get;
extern il2cpp_domain_assembly_open_t il2cpp_domain_assembly_open;
extern il2cpp_assembly_get_image_t il2cpp_assembly_get_image;
extern il2cpp_class_from_name_t il2cpp_class_from_name;
extern il2cpp_class_get_methods_t il2cpp_class_get_methods;
extern il2cpp_class_get_method_from_name_t il2cpp_class_get_method_from_name;
extern il2cpp_method_get_param_t il2cpp_method_get_param;
extern il2cpp_object_new_t il2cpp_object_new;
extern il2cpp_resolve_icall_t il2cpp_resolve_icall;
extern il2cpp_array_new_t il2cpp_array_new;
extern il2cpp_thread_attach_t il2cpp_thread_attach;
extern il2cpp_thread_detach_t il2cpp_thread_detach;
extern il2cpp_class_get_type_t il2cpp_class_get_type;
extern il2cpp_class_get_type_token_t il2cpp_class_get_type_token;
extern il2cpp_class_get_field_from_name_t il2cpp_class_get_field_from_name;
extern il2cpp_field_get_value_t il2cpp_field_get_value;
extern il2cpp_field_set_value_t il2cpp_field_set_value;
extern il2cpp_field_static_get_value_t il2cpp_field_static_get_value;
extern il2cpp_field_static_set_value_t il2cpp_field_static_set_value;
extern il2cpp_field_get_type_t il2cpp_field_get_type;
extern il2cpp_type_get_object_t il2cpp_type_get_object;
extern il2cpp_property_get_get_method_t il2cpp_property_get_get_method;
extern il2cpp_property_get_set_method_t il2cpp_property_get_set_method;
extern il2cpp_class_get_fields_t il2cpp_class_get_fields;
extern il2cpp_class_get_nested_types_t il2cpp_class_get_nested_types;

extern il2cpp_image_get_class_t il2cpp_image_get_class;

extern il2cpp_runtime_object_init_t il2cpp_runtime_object_init;

extern il2cpp_image_get_name_t il2cpp_image_get_name;
extern il2cpp_image_get_class_count_t il2cpp_image_get_class_count;
extern il2cpp_type_is_byref_t il2cpp_type_is_byref;
extern il2cpp_method_get_flags_t il2cpp_method_get_flags;
extern il2cpp_method_get_return_type_t il2cpp_method_get_return_type;
extern il2cpp_class_from_type_t il2cpp_class_from_type;
extern il2cpp_class_get_name_t il2cpp_class_get_name;
extern il2cpp_class_get_properties_t il2cpp_class_get_properties;
extern il2cpp_class_is_enum_t il2cpp_class_is_enum;
extern il2cpp_class_get_fields_t il2cpp_class_get_fields;
extern il2cpp_method_get_name_t il2cpp_method_get_name;
extern il2cpp_method_get_param_count_t il2cpp_method_get_param_count;
extern il2cpp_method_get_param_name_t il2cpp_method_get_param_name;
extern il2cpp_class_get_parent_t il2cpp_class_get_parent;
extern il2cpp_class_get_interfaces_t il2cpp_class_get_interfaces;
extern il2cpp_class_get_namespace_t il2cpp_class_get_namespace;
extern il2cpp_class_get_flags_t il2cpp_class_get_flags;
extern il2cpp_class_is_valuetype_t il2cpp_class_is_valuetype;
extern il2cpp_property_get_flags_t il2cpp_property_get_flags;
extern il2cpp_property_get_get_method_t il2cpp_property_get_get_method;
extern il2cpp_property_get_set_method_t il2cpp_property_get_set_method;
extern il2cpp_property_get_name_t il2cpp_property_get_name;
extern il2cpp_property_get_parent_t il2cpp_property_get_parent;
extern il2cpp_field_get_flags_t il2cpp_field_get_flags;
extern il2cpp_field_get_name_t il2cpp_field_get_name;
extern il2cpp_field_get_parent_t il2cpp_field_get_parent;
extern il2cpp_field_get_offset_t il2cpp_field_get_offset;
extern il2cpp_domain_get_assemblies_t il2cpp_domain_get_assemblies;

char* il2cpp_array_addr_with_size(void* arr, int32_t size, uintptr_t idx);

// array macro
#define il2cpp_array_addr(array, type, index) ((type*)(void*) il2cpp_array_addr_with_size (array, sizeof (type), index))

#define il2cpp_array_setref(array, index, value)  \
    do {    \
        void* *__p = (void* *) il2cpp_array_addr ((array), void*, (index)); \
         *__p = (value);    \
    } while (0)

namespace il2cpp_symbols
{
	void init(HMODULE game_module);
	uintptr_t get_method_pointer(const char* assemblyName, const char* namespaze,
	const char* klassName, const char* name, int argsCount, int index = -1);
	void* find_nested_class(void* klass, std::predicate<void*> auto&& predicate)
	{
		void* iter{};
		while (const auto curNestedClass = il2cpp_class_get_nested_types(klass, &iter))
		{
			if (static_cast<decltype(predicate)>(predicate)(curNestedClass))
			{
				return curNestedClass;
			}
		}

		return nullptr;
	}

	void* get_class_from_instance(const void* instance);

	void* get_class(const char* assemblyName, const char* namespaze, const char* klassName);

	void* find_nested_class_from_name(void* klass, const char* name);

	MethodInfo* get_method(const char* assemblyName, const char* namespaze,
						   const char* klassName, const char* name, int argsCount);

	uintptr_t find_method(const char* assemblyName, const char* namespaze,
						  const char* klassName, std::function<bool(const MethodInfo*)> predict);

	void* GetStaticFieldValue(FieldInfo* field);
	void SetStaticFieldValue(FieldInfo* field, void* value);

	template <typename T = void*>
	void iterate_IEnumerable(const void* obj, std::invocable<T> auto&& receiver)
	{
		const auto klass = get_class_from_instance(obj);
		const auto getEnumeratorMethod = reinterpret_cast<void* (*)(const void*)>(il2cpp_class_get_method_from_name(klass, "GetEnumerator", 0)->methodPointer);
		const auto enumerator = getEnumeratorMethod(obj);
		const auto enumeratorClass = get_class_from_instance(enumerator);
		const auto getCurrentMethod = reinterpret_cast<T(*)(void*)>(il2cpp_class_get_method_from_name(enumeratorClass, "get_Current", 0)->methodPointer);
		const auto moveNextMethod = reinterpret_cast<bool(*)(void*)>(il2cpp_class_get_method_from_name(enumeratorClass, "MoveNext", 0)->methodPointer);

		while (moveNextMethod(enumerator))
		{
			static_cast<decltype(receiver)>(receiver)(getCurrentMethod(enumerator));
		}
	}
}
