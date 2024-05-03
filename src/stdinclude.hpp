#pragma once
#define NOMINMAX
#define IMGUI_DEFINE_MATH_OPERATORS

#include "stb_image.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <signal.h>
#include <time.h>

#include <shlobj.h>

#include <cinttypes>

#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <locale>
#include <string>
#include <thread>
#include <vector>
#include <unordered_map>
#include <chrono>

#include <MinHook.h>

#include <rapidjson/document.h>
#include <rapidjson/encodings.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>

#include "il2cpp/il2cpp_symbols.hpp"
#include "local/local.hpp"
#include "logger/logger.hpp"
#include "PEDump.h"
#include "url.hpp"

#include <httplib.h>
#include <psapi.h>

#include "lz4.h"

#include <cstdio>


#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui_internal.h"

#include "imgui/imgui_notify.h"

#include "implot/implot.h"

#include "d3d11.h"
#include <wincodec.h>
//#include "d3dx11tex.h"
#include <tchar.h>

#include <atlconv.h>

#include <deque>

#include "kiero/kiero.h"




#pragma GCC diagnostic ignored "-Wdiv-by-zero"
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma execution_character_set( "utf-8" )



typedef void (*SignalHandlerPointer)(int);
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef HRESULT(__stdcall* ResizeBuffers)(IDXGISwapChain* pSwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;

extern Present oPresent;
extern ID3D11Device* pDevice;
extern ID3D11DeviceContext* pContext;
extern HWND umaWindow;
extern ID3D11RenderTargetView* mainRenderTargetView;
extern IDXGISwapChain* g_pSwapChain;

extern int imguiwindow();
extern bool imgui_settingwnd_open;

#pragma region ImGui_Texture

extern int kimura_image_width;
extern int kimura_image_height;
extern ID3D11ShaderResourceView* texture_kimura;

#pragma endregion


//must be value name = json config member name
extern struct globalUmaSettings {
    bool dumpStaticEntries = false;
    bool enableLogger = false;
    bool enableConsole = false;
    bool autoFpsSet = false;
    int maxFps = -1;
    bool unlockSize = false;
    float uiScale = 1.0f;
    bool replaceFont = true;
    bool autoFullscreen = true;
    bool lz4Encrypt = true;
    bool showLiveTitleWindow = true;
    bool sendHorseMeterData = false;
    bool saveMsgPack = false;
    bool dumpGameassembly = false;
    float rankUIShowMeter = 0.0f;
    float rankUIHideoffset = 0.0f;
    bool liveCharaAutoDressReplace = false;
    bool useExclusiveFullScreen = false;
    int exclusiveFullScreenWidth = 1920;
    int exclusiveFullScreenHeight = 1080;
    char* customHost = (char*)"";
    char* customDataPath = (char*)"";
    bool skipResourceDownload = false;
    bool forceLandscape = false;
    bool highQuality = true;
    bool passPacket = false;
    char* serverIP = (char*)"127.0.0.1";
    int serverPort = 80;
    bool isTapEffectEnabled = true;
    bool isShowLiveFPSGraph = true;
    bool isShowLivePerfInfo = true;
    float virtualResolutionMultiple = 1.0f;
    bool enableVSync = false;

    bool gotoTitleOnError = true; //N
    bool walkMotionAllUrara = false;
    bool homeAllDiamond = false;
    bool winMotion564 = false;

};

extern struct localUmaSettings {
    int gachaCutinChara = -1;
    int gachaCutinDress = -1;
    int gachaCutinHeadid = -1;
    int raceResultCutinMotionChara = -1;
    int raceResultCutinMotionDress = -1;
    int raceResultCutinMotionGrade = -1;
    int raceResultCutinMotionRank = -1;
    bool stopLiveCam = false;
    bool changeStoryChar = false;
    int story3dCharID = -1;
    int story3dClothID = -1;
    int story3dMobid = -1;
    int story3dHeadID = -1;
    int antialiasing = 8;
    int graphics_quality = 2;
    int vsync_count = 0;
    int cardid = -1;
    float aspect_ratio = 16.f / 9.f;
    int gachaCharaType = -1;
    bool isLiveTimeManual = false;
    bool isShowLivePerfInfo = true;
    bool isShowLiveFPSGraph = false;
};

extern globalUmaSettings* g_sett;
extern const globalUmaSettings* g_sett_initial;

extern localUmaSettings* sett;

extern struct hookStr
{
	const char* assemblyName;
	const char* namespaceName;
	const char* className;
	const char* methodName;
	int argsCount;
	LPVOID addr;
};

extern std::vector<hookStr> hooked_addr;

extern bool hook_end;

extern int patchCount;






extern TimelineKeyCharacterType c_gachaCharaType;

void ResetGame();

extern char* (*msgPackToJson)(char* MsgPackBuffer, int size);

extern struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};

extern class discordRpc {
private:
    HMODULE module = NULL;
    void (*dispose)();

public:
    bool (*init)();   
    void (*set)(char* msgpack, int size, const char* url);
    void (*setScene)(int sceneId);
    void (*initDB)(const char* dbPath);
    void (*showCharSelectForm)();
    
    discordRpc() {
        module = LoadLibraryA("plugins/Uma.Helper.DiscordRPC.dll");
        init = (bool (*)())GetProcAddress(module, "init");
        initDB = (void (*)(const char*))GetProcAddress(module, "initDB");
        set = (void (*)(char*, int, const char*))GetProcAddress(module, "processRPC");
        setScene = (void (*)(int))GetProcAddress(module, "setSceneID");
        dispose = (void (*)())GetProcAddress(module, "disposeRPC");
        showCharSelectForm = (void(*)())GetProcAddress(module, "showCharSelectForm");
    }

    ~discordRpc() {
        dispose();
        FreeLibrary(module);
    }
};

extern discordRpc* rpc;