#define NOMINMAX
#include <stdinclude.hpp>

extern bool init_hook();
extern void uninit_hook();
extern void start_console();

bool g_dump_entries = false;
bool g_enable_logger = false;
bool g_enable_console = false;
bool g_autofps = false;
int g_max_fps = -1;
bool g_unlock_size = false;
float g_ui_scale = 1.0f;
float g_aspect_ratio = 16.f / 9.f;
bool g_replace_font = true;
bool g_auto_fullscreen = true;
bool g_lz4Encrypt = true;
bool g_showLiveTitleWindow = true;
bool g_sendHorseMeterData = false;
bool g_saveMsgPack = false;
bool g_dumpGamedll = false;
float g_rankUIShowMeter = 0.0f;
float g_rankUIHideoffset = 0.0f;
bool g_liveCharaAutoDressReplace = false;
bool g_useExclusiveFullScreen = false;
int g_exclusiveFullScreenWidth = 1920;
int g_exclusiveFullScreenHeight = 1080;
char* g_customHost;

namespace
{
	void create_debug_console()
	{
		AllocConsole();

		// open stdout stream
		auto _ = freopen("CONOUT$", "w", stdout);
		_ = freopen("CONOUT$", "w", stderr);
		_ = freopen("CONIN$", "r", stdin);

		SetConsoleTitle("Umamusume - Debug Console");

		// set this to avoid turn japanese texts into question mark
		SetConsoleOutputCP(65001);
		std::locale::global(std::locale(""));

		wprintf(L"\u30a6\u30de\u5a18 Localify Patch Loaded! - By GEEKiDoS / Edited by runner38\n");
		std::ifstream file("ascii-art.txt");
		std::string str;
		
		while (std::getline(file, str))
		{
			std::cout << str << "\n";
		}


	}

	std::vector<std::string> read_config()
	{
		std::ifstream config_stream { "config.json" };
		std::vector<std::string> dicts {};

		if (!config_stream.is_open())
			return dicts;

		rapidjson::IStreamWrapper wrapper {config_stream};
		rapidjson::Document document;

		document.ParseStream(wrapper);

		if (!document.HasParseError())
		{
			g_enable_console = document["enableConsole"].GetBool();
			g_enable_logger = document["enableLogger"].GetBool();
			g_dump_entries = document["dumpStaticEntries"].GetBool();
			g_max_fps = document["maxFps"].GetInt();
			g_unlock_size = document["unlockSize"].GetBool();
			g_ui_scale = document["uiScale"].GetFloat();
			g_replace_font = document["replaceFont"].GetBool();
			g_auto_fullscreen = document["autoFullscreen"].GetBool();
			g_autofps = document["autoFpsSet"].GetBool();
			g_lz4Encrypt = document["lz4Encrypt"].GetBool();
			g_dumpGamedll = document["dumpGameassembly"].GetBool();
			g_saveMsgPack = document["saveMsgPack"].GetBool();
			g_showLiveTitleWindow = document["showLiveTitleWindow"].GetBool();
			g_sendHorseMeterData = document["sendHorseMeterData"].GetBool();
			g_rankUIShowMeter = document["rankUIShowMeter"].GetFloat();
			g_rankUIHideoffset = document["rankUIHideoffset"].GetFloat();
			g_liveCharaAutoDressReplace = document["liveCharaAutoDressReplace"].GetBool();
			g_useExclusiveFullScreen = document["useExclusiveFullScreen"].GetBool();
			g_exclusiveFullScreenWidth = document["exclusiveFullScreenWidth"].GetInt();
			g_exclusiveFullScreenHeight = document["exclusiveFullScreenHeight"].GetInt();
			const char* d = document["customHost"].GetString();
			g_customHost = new char[strlen(d)+1];
			strcpy(g_customHost, d);
			
			// Looks like not working for now
			// g_aspect_ratio = document["customAspectRatio"].GetFloat();

			auto& dicts_arr = document["dicts"];
			auto len = dicts_arr.Size();

			for (size_t i = 0; i < len; ++i)
			{
				auto dict = dicts_arr[i].GetString();

				dicts.push_back(dict);
			}
		}
		else {
			MessageBox(NULL, "Config.json parse error", "Error", MB_OK | MB_ICONERROR);
		}

		config_stream.close();
		return dicts;
	}
}

int __stdcall DllMain(HINSTANCE, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		// the DMM Launcher set start path to system32 wtf????
		std::string module_name;
		module_name.resize(MAX_PATH);
		module_name.resize(GetModuleFileName(nullptr, module_name.data(), MAX_PATH));

		std::filesystem::path module_path(module_name);

		// check name
		if (module_path.filename() != "umamusume.exe")
			return 1;

		std::filesystem::current_path(
			module_path.parent_path()
		);

		auto dicts = read_config();

		
		create_debug_console();
	
			

		std::thread init_thread([dicts]() {
			logger::init_logger();
			local::load_textdb(&dicts);
			init_hook();

			if (g_enable_console)
				start_console();
			});
		init_thread.detach();
	}
	else if (reason == DLL_PROCESS_DETACH)
	{
		uninit_hook();
		logger::close_logger();
	}

	return 1;
}
