#pragma once


enum FullScreenMode {
	ExclusiveFullScreen,
	FullScreenWindow,
	MaximizedWindow,
	Windowed
};

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



struct Resolution_t
{
public:
	int width;
	int height;
	int herz;
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

// not real Il2CppString class
struct Il2CppString
{
	void* Empty;
	void* WhiteChars;
	int32_t length;
	wchar_t start_char[1];
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

struct ParameterInfo
{
	const char* name;
	int32_t position;
	uint32_t token;
	const Il2CppType* parameter_type;
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
struct CutInCharacterCreateInfo {
	char dummy[0x10];
	TimelineKeyCharacterType _characterType;
	int _charaIndex;
	void* _parent;
	int _charaId;
	int _clothId;
	int _headId;
	bool IsUseDressDataHeadModelSubId;
	int _zekkenNo;
	bool _isWet;
	bool _isDirt;
	int OverrideClothCategory;
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

// function types
typedef Il2CppString* (*il2cpp_string_new_utf16_t)(const wchar_t* str, unsigned int len);
typedef Il2CppString* (*il2cpp_string_new_t)(const char* str);
typedef void* (*il2cpp_domain_get_t)();
typedef void* (*il2cpp_domain_assembly_open_t)(void* domain, const char* name);
typedef void* (*il2cpp_assembly_get_image_t)(void* assembly);
typedef void* (*il2cpp_class_from_name_t)(void* image, const char* namespaze, const char* name);
typedef MethodInfo* (*il2cpp_class_get_methods_t)(void* klass, void** iter);
typedef MethodInfo* (*il2cpp_class_get_method_from_name_t)(void* klass, const char* name, int argsCount);
typedef void* (*il2cpp_method_get_param_t)(const MethodInfo* method, uint32_t index);
typedef void* (*il2cpp_object_new_t)(void* klass);
typedef void* (*il2cpp_resolve_icall_t)(const char* name);
typedef void* (*il2cpp_array_new_t)(void* klass, uintptr_t count);
typedef void* (*il2cpp_thread_attach_t)(void* domain);
typedef void (*il2cpp_thread_detach_t)(void* thread);

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
								 const char* klassName, const char* name, int argsCount);

	void* get_class(const char* assemblyName, const char* namespaze, const char* klassName);

	MethodInfo* get_method(const char* assemblyName, const char* namespaze,
						   const char* klassName, const char* name, int argsCount);

	uintptr_t find_method(const char* assemblyName, const char* namespaze,
						  const char* klassName, std::function<bool(const MethodInfo*)> predict);
}
