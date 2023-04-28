#pragma once
#define NOMINMAX

#include "stb_image.h"

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <signal.h>

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

#include <d3d11.h>
#include <tchar.h>

#include <atlconv.h>

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

extern bool g_dump_entries;
extern bool g_enable_logger;
extern bool g_enable_console;
extern int g_max_fps;
extern bool g_unlock_size;
extern float g_ui_scale;
extern float g_aspect_ratio;
extern bool g_replace_font;
extern bool g_auto_fullscreen;
extern bool g_autofps;
extern bool g_lz4Encrypt;
extern bool g_showLiveTitleWindow;
extern bool g_sendHorseMeterData;
extern bool g_saveMsgPack;
extern bool g_dumpGamedll;
extern float g_rankUIShowMeter;
extern float g_rankUIHideoffset;
extern bool g_liveCharaAutoDressReplace;
extern bool g_useExclusiveFullScreen;
extern int g_exclusiveFullScreenWidth;
extern int g_exclusiveFullScreenHeight;
extern char* g_customDataPath;
extern char* g_customHost;
extern bool g_skipResourceDownload;
extern int c_gachaCutinChara;
extern int c_gachaCutinDress;
extern int c_gachaCutinHeadid;
extern bool c_stopLiveCam;
extern int c_raceResultCutinMotionChara;
extern int c_raceResultCutinMotionDress;
extern int c_raceResultCutinMotionGrade;
extern int c_raceResultCutinMotionRank;
extern bool c_changeStoryChar;
extern int c_story3dCharID;
extern int c_story3dClothID;
extern int c_story3dMobid;
extern int c_story3dHeadID;
extern bool g_force_landscape;
extern int g_antialiasing ;
extern int g_graphics_quality;
extern int g_vsync_count;
extern int g_cardid;
extern bool g_highquality;


extern TimelineKeyCharacterType c_gachaCharaType;

void ResetGame();