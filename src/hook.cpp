#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdinclude.hpp>

bool g_sendserver = true;
int server_port = 80;
char server_ip[256];
DWORD getCurrentDisplayHz();
void DumpHex(const void* data, size_t size);

#pragma comment(lib, "ws2_32")

using namespace std;

namespace
{
	bool isLiveStartFlag = false;
	int count = 0;
	char* readAllFileBytes(const char* name)
	{
		ifstream fl(name);
		fl.seekg(0, ios::end);
		size_t len = fl.tellg();
		char* ret = new char[len];
		fl.seekg(0, ios::beg);
		fl.read(ret, len);
		fl.close();
		return ret;
	}
	bool isSubset(char arr1[], char arr2[],
		int m, int n)
	{
		int i = 0;
		int j = 0;
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				if (arr2[i] == arr1[j])
					break;
			}

			/* If the above inner loop was
			not broken at all then arr2[i]
			is not present in arr1[] */
			if (j == m)
				return 0;
		}

		/* If we reach here then all
		elements of arr2[] are present
		in arr1[] */
		return 1;
	}
	
	void path_game_assembly();
	void bootstrap_carrot_juicer();

	bool mh_inited = false;
	vector<void*> enabled_hooks;

	void deleteDirectoryContents(const std::string& dir_path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(dir_path))
			std::filesystem::remove_all(entry.path());
	}
	void dump_bytes(void* pos)
	{
		printf("Hex dump of %p\n", pos);

		char* memory = reinterpret_cast<char*>(pos);
		
		for (int i = 0; i < 0x20; i++)
		{
			if (i > 0 && i % 16 == 0)
				printf("\n");

			char byte = *(memory++);
			
			printf("%02hhX ", byte);
		}

		printf("\n\n");
	}

	void* load_library_w_orig = nullptr;
	HMODULE __stdcall load_library_w_hook(const wchar_t* path)
	{
		// GameAssembly.dll code must be loaded and decrypted while loading criware library
		if (path == L"cri_ware_unity.dll"s)
		{
			path_game_assembly();
			bootstrap_carrot_juicer();
			MH_DisableHook(LoadLibraryW);
			MH_RemoveHook(LoadLibraryW);

			// use original function beacuse we have unhooked that
			return LoadLibraryW(path);
		}

		return reinterpret_cast<decltype(LoadLibraryW)*>(load_library_w_orig)(path);
	}

	void* populate_with_errors_orig = nullptr;
	bool populate_with_errors_hook(void* _this, Il2CppString* str, TextGenerationSettings_t* settings, void* context)
	{
		return reinterpret_cast<decltype(populate_with_errors_hook)*>(populate_with_errors_orig) (
			_this, local::get_localized_string(str), settings, context
			);
	}

	void* localize_get_orig = nullptr;
	Il2CppString* localize_get_hook(int id)
	{
		auto orig_result = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(id);
		auto result = local::get_localized_string(id);

		return result ? result : orig_result;
	}

	std::unordered_map<void*, bool> text_queries;

	void* query_ctor_orig = nullptr;
	void* query_ctor_hook(void* _this, void* conn, Il2CppString* sql)
	{
		auto ssql = std::wstring(sql->start_char);

		if (ssql.find(L"text_data") != std::string::npos ||
			ssql.find(L"character_system_text") != std::string::npos ||
			ssql.find(L"race_jikkyo_comment") != std::string::npos ||
			ssql.find(L"race_jikkyo_message") != std::string::npos ) 
		{
			text_queries.emplace(_this, true);
		}
		//wprintf(L"SQL_Query[ %s ]\n", ssql.c_str());
		//printf("Text: %s\n", text_queries[0]);
		return reinterpret_cast<decltype(query_ctor_hook)*>(query_ctor_orig)(_this, conn, sql);
	}

	void* query_dispose_orig = nullptr;
	void query_dispose_hook(void* _this)
	{
		if (text_queries.contains(_this))
			text_queries.erase(_this);

		return reinterpret_cast<decltype(query_dispose_hook)*>(query_dispose_orig)(_this);
	}

	void* query_getstr_orig = nullptr;
	Il2CppString*  query_getstr_hook(void* _this, int idx)
	{
		auto result = reinterpret_cast<decltype(query_getstr_hook)*>(query_getstr_orig)(_this, idx);

		if (text_queries.contains(_this))
			return local::get_localized_string(result);
		
		return result;
	}

	void* set_fps_orig = nullptr;
	void set_fps_hook(int value)
	{
		if (g_autofps) {
			int hz = getCurrentDisplayHz();
			printf("Auto fps limit setted : %d fps\n",hz+1);
			return reinterpret_cast<decltype(set_fps_hook)*>(set_fps_orig)((int)hz+1);
		}
		else {
			return reinterpret_cast<decltype(set_fps_hook)*>(set_fps_orig)(g_max_fps);
		}
		
	}

	void* set_animefps_orig = nullptr;
	void set_animefps_hook(float val)
	{
		return reinterpret_cast<decltype(set_animefps_hook)*>(set_animefps_orig)(0.0);
	}

	

	
	void* live_CameraLookAt_UpdateCamera_orig = nullptr;
	void live_CameraLookAt_UpdateCamera_hook() {
		printf("UpdateCamera Called\n");
		//return reinterpret_cast<decltype(live_CameraLookAt_UpdateCamera_hook)*>(live_CameraLookAt_UpdateCamera_orig)();
	}

	void* live_Cutt_LiveTimelineControl_GetCameraPos_orig = nullptr;
	Vector3_t* live_Cutt_LiveTimelineControl_GetCameraPos_hook(int index) {
		auto pos = reinterpret_cast<decltype(live_Cutt_LiveTimelineControl_GetCameraPos_hook)*>(live_Cutt_LiveTimelineControl_GetCameraPos_orig)(index);
		printf("Charaid: %d, Cam: x=%.2f, y=%.2f, z=%.2f\n", index, pos->x, pos->y, pos->z);
		return pos;
	}

	void* live_Cutt_LiveTimelineControl_set_liveStageCenterPos_orig = nullptr;
	void live_Cutt_LiveTimelineControl_set_liveStageCenterPos_hook(Vector3_t* value) {
		printf("set_liveStageCenterPos: x=%.2f, y=%.2f, z=%.2f\n", value->x, value->y, value->z);
		return reinterpret_cast<decltype(live_Cutt_LiveTimelineControl_set_liveStageCenterPos_hook)*>
			(live_Cutt_LiveTimelineControl_set_liveStageCenterPos_orig)(value);
	}

	void* live_cam_GetCullingMask_orig = nullptr;
	unsigned int live_cam_GetCullingMask_hook(unsigned int val) {
		//printf("cam %d\n", val);		
		return reinterpret_cast<decltype(live_cam_GetCullingMask_hook)*>(live_cam_GetCullingMask_orig)(val);
		//return 511;
	}

	void* AssetBundleHelper_GetResourceVer_orig = nullptr;
	Il2CppString* AssetBundleHelper_GetResourceVer_hook() {
		auto str = reinterpret_cast<decltype(AssetBundleHelper_GetResourceVer_hook)*>
			(AssetBundleHelper_GetResourceVer_orig)();
		std::wstring verstr = std::wstring(str->start_char);
		wprintf(L"Version Str:[%s]\n",verstr.c_str());

		char* conv_str = new char[str->length + 2];
		memset(conv_str, 0, str->length + 2);
		wcstombs(conv_str, verstr.c_str(), str->length + 1);
		

		httplib::Client cli("127.0.0.1", 564);
		std::string data(conv_str, str->length + 2);
		
		auto res = cli.Post("/umamusume_test_server/tool/upload_resourcever", data, "application/text");

		delete[] conv_str;
		//res->status;
		//const char* returned = res->body.c_str();
		//size_t clength = res->body.length();

		//wprintf()
		return str;
	}

	bool (*is_virt)() = nullptr;
	int last_height = 0, last_width = 0;

	void* wndproc_orig = nullptr;
	LRESULT wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_SIZING)
		{
			RECT* rect = reinterpret_cast<RECT*>(lParam);

			float ratio = is_virt() ? 1.f / g_aspect_ratio : g_aspect_ratio;
			float height = rect->bottom - rect->top;
			float width = rect->right - rect->left;

			float new_ratio = width / height;

			if (new_ratio > ratio && height >= last_height || width < last_width)
				height = width / ratio;
			else if (new_ratio < ratio && width >= last_width || height < last_height)
				width = height * ratio;

			switch (wParam)
			{
				case WMSZ_TOP:
				case WMSZ_TOPLEFT:
				case WMSZ_TOPRIGHT:
					rect->top = rect->bottom - height;
					break;
				default:
					rect->bottom = rect->top + height;
					break;
			}

			switch (wParam)
			{
				case WMSZ_LEFT:
				case WMSZ_TOPLEFT:
				case WMSZ_BOTTOMLEFT:
					rect->left = rect->right - width;
					break;
				default:
					rect->right = rect->left + width;
					break;
			}

			last_height = height;
			last_width = width;

			return TRUE;
		}

		return reinterpret_cast<decltype(wndproc_hook)*>(wndproc_orig)(hWnd, uMsg, wParam, lParam);
	}

	void* get_virt_size_orig = nullptr;
	Vector3_t* get_virt_size_hook(Vector3_t* pVec3, int width, int height)
	{
		auto size = reinterpret_cast<decltype(get_virt_size_hook)*>(get_virt_size_orig)(pVec3, width, height);

		height = width * g_aspect_ratio;

		size->x = width;
		size->y = height;
		size->z = g_aspect_ratio;

		return size;
	}
	
	void* get_hori_size_orig = nullptr;
	Vector3_t* get_hori_size_hook(Vector3_t* pVec3, int width, int height)
	{
		auto size = reinterpret_cast<decltype(get_hori_size_hook)*>(get_hori_size_orig)(pVec3, width, height);

		width = height * g_aspect_ratio;

		size->x = width;
		size->y = height;
		size->z = g_aspect_ratio;

		return size;
	}

	Resolution_t* (*get_resolution)(Resolution_t* buffer);

	void* gallop_get_screenheight_orig;
	int gallop_get_screenheight_hook()
	{
		Resolution_t res;
		res = *get_resolution(&res);

		int w = max(res.width, res.height), h = min(res.width, res.height);

		return is_virt() ? w : h;
	}

	void* gallop_get_screenwidth_orig;
	int gallop_get_screenwidth_hook()
	{
		Resolution_t res;
		res = *get_resolution(&res);

		int w = max(res.width, res.height), h = min(res.width, res.height);

		return is_virt() ? h : w;
	}

	void (*set_scale_factor)(void*, float);

	void* canvas_scaler_setres_orig;
	void canvas_scaler_setres_hook(void* _this, Vector2_t res)
	{
		Resolution_t r;
		r = *get_resolution(&r);

		// set scale factor to make ui bigger on hi-res screen
		set_scale_factor(_this, max(1.0f, r.width / 1920.f) * g_ui_scale);

		return reinterpret_cast<decltype(canvas_scaler_setres_hook)*>(canvas_scaler_setres_orig)(_this, res);
	}

	void (*text_assign_font)(void*);
	int (*text_get_size)(void*);
	void (*text_set_size)(void*, int);
	float (*text_get_linespacing)(void*);
	void (*text_set_style)(void*, int);
	void (*text_set_linespacing)(void*, float);

	void* on_populate_orig = nullptr;
	void on_populate_hook(void* _this, void* toFill)
	{
		if (text_get_linespacing(_this) != 1.05f)
		{
			text_assign_font(_this);
			text_set_style(_this,1);
			text_set_size(_this, text_get_size(_this) - 5);
			text_set_linespacing(_this, 1.05f);
		}
		
		return reinterpret_cast<decltype(on_populate_hook)*>(on_populate_orig)(_this, toFill);
	}

	void* set_resolution_orig;
	void set_resolution_hook(int width, int height, bool fullscreen)
	{
		Resolution_t r;
		r = *get_resolution(&r);

		bool need_fullscreen = false;

		if (is_virt() && r.width / static_cast<double>(r.height) == (9.0 / 16.0))
			need_fullscreen = true;
		else if (!is_virt() && r.width / static_cast<double>(r.height) == (16.0 / 9.0))
			need_fullscreen = true;

		return reinterpret_cast<decltype(set_resolution_hook)*>(set_resolution_orig)(
			need_fullscreen ? r.width : width, need_fullscreen ? r.height : height, need_fullscreen
		);
	}

	void adjust_size()
	{
		thread([]() {
			auto tr = il2cpp_thread_attach(il2cpp_domain_get());

			Resolution_t r;
			r = *get_resolution(&r);

			auto target_height = r.height - 100;

			set_resolution_hook(target_height * 0.5625f, target_height, false);

			il2cpp_thread_detach(tr);
		}).detach();
	}

	void dump_all_entries()
	{
		// TextId 0 - 0xA55, 0 is None
		for (int i = 1; i <= 0xA55; ++i)
		{
			auto entry = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(i);
			logger::write_entry(i, entry->start_char);
		}
	}

	void path_game_assembly()
	{
		if (!mh_inited)
			return;

		printf("Trying to patch GameAssembly.dll...\n");

		auto il2cpp_module = GetModuleHandle("GameAssembly.dll");

		// load il2cpp exported functions
		il2cpp_symbols::init(il2cpp_module);

#pragma region HOOK_MACRO
#define ADD_HOOK(_name_, _fmt_) \
	auto _name_##_offset = reinterpret_cast<void*>(_name_##_addr); \
	\
	printf(_fmt_, _name_##_offset); \
	dump_bytes(_name_##_offset); \
	\
	MH_CreateHook(_name_##_offset, _name_##_hook, &_name_##_orig); \
	MH_EnableHook(_name_##_offset); \
	\
	enabled_hooks.push_back(_name_##_offset)
#pragma endregion
#pragma region HOOK_ADDRESSES
		auto populate_with_errors_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.TextRenderingModule.dll",
			"UnityEngine", "TextGenerator",
			"PopulateWithErrors", 3
		);

		// have to do this way because there's Get(TextId id) and Get(string id)
		// the string one looks like will not be called by elsewhere
		auto localize_get_addr = il2cpp_symbols::find_method("umamusume.dll", "Gallop", "Localize", [](const MethodInfo* method) {
			return method->name == "Get"s && 
				method->parameters->parameter_type->type == IL2CPP_TYPE_VALUETYPE;
		});

		auto query_ctor_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3", 
			"Query", ".ctor", 2
		);

		auto query_getstr_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "GetText", 1
		);

		auto query_dispose_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "Dispose", 0
		);

		auto set_fps_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Application", "set_targetFrameRate", 1
		);

		auto set_animefps_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.AnimationModule.dll", "UnityEngine",
			"AnimationClip", "set_frameRate", 1
		);

		auto wndproc_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"StandaloneWindowResize", "WndProc", 4
		);

		auto get_virt_size_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"StandaloneWindowResize", "getOptimizedWindowSizeVirt", 2
		);

		auto get_hori_size_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"StandaloneWindowResize", "getOptimizedWindowSizeHori", 2
		);

		is_virt = reinterpret_cast<bool(*)()>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"StandaloneWindowResize", "get_IsVirt", 0
		));

		get_resolution = reinterpret_cast<Resolution_t * (*)(Resolution_t*)>(
			il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Screen", "get_currentResolution", 0
			)
		);

		auto gallop_get_screenheight_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"Screen", "get_Height", 0
		);

		auto gallop_get_screenwidth_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"Screen", "get_Width", 0
		);

		auto change_resize_ui_for_pc_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"UIManager", "ChangeResizeUIForPC", 2
		);

		auto canvas_scaler_setres_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.UI.dll", "UnityEngine.UI",
			"CanvasScaler", "set_referenceResolution", 1
		);

		set_scale_factor = reinterpret_cast<void(*)(void*,float)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"CanvasScaler", "set_scaleFactor", 1
			)
		);

		auto on_populate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"TextCommon", "OnPopulateMesh", 1
		);

		text_assign_font = reinterpret_cast<void(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "AssignDefaultFont", 0
			)
		);

		text_get_size = reinterpret_cast<int(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"TextCommon", "get_FontSize", 0
			)
		);

		text_set_size = reinterpret_cast<void(*)(void*, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"TextCommon", "set_FontSize", 1
			)
		);

		text_get_linespacing = reinterpret_cast<float(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "get_lineSpacing", 0
			)
		);

		text_set_style = reinterpret_cast<void(*)(void*, int)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "set_fontStyle", 1
			)
		);

		text_set_linespacing = reinterpret_cast<void(*)(void*, float)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UI.dll", "UnityEngine.UI",
				"Text", "set_lineSpacing", 1
			)
		);

		auto set_resolution_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Screen", "SetResolution", 3
		);

		auto live_cam_GetCullingMask_addr = reinterpret_cast<unsigned int(*)(unsigned int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live.Cutt",
				"LiveCameraCullingLayer_Helper", "GetCullingMask", 1
			));
		
		auto live_CameraLookAt_UpdateCamera_addr = reinterpret_cast<void(*)(void)>(
			il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live",
			"CameraLookAt", "UpdateCamera", 0
		));

		auto live_Cutt_LiveTimelineControl_GetCameraPos_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "GetCameraPos", 1
		);

		auto live_Cutt_LiveTimelineControl_set_liveStageCenterPos_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "set_liveStageCenterPos", 1
		);

		auto AssetBundleHelper_GetResourceVer_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"AssetBundleHelper", "GetResourceVer", 0
		);

#pragma endregion

		// hook UnityEngine.TextGenerator::PopulateWithErrors to modify text
		ADD_HOOK(populate_with_errors, "UnityEngine.TextGenerator::PopulateWithErrors at %p\n");

		// Looks like they store all localized texts that used by code in a dict
		ADD_HOOK(localize_get, "Gallop.Localize.Get(TextId) at %p\n");

		ADD_HOOK(query_ctor, "Query::ctor at %p\n");
		ADD_HOOK(query_getstr, "Query::GetString at %p\n");
		ADD_HOOK(query_dispose, "Query::Dispose at %p\n");
		ADD_HOOK(set_animefps, "UnityEngine.AnimationClip.set_frameRate at %p \n");
		ADD_HOOK(live_cam_GetCullingMask, "Gallop.Live.Cutt.LiveCameraCullingLayer_Helper.GetCullingMask(uint) at %p\n");
		//ADD_HOOK(live_CameraLookAt_UpdateCamera, "Gallop.Live.CameraLookAt.UpdateCamera(longlong) at %p \n");
		ADD_HOOK(live_Cutt_LiveTimelineControl_GetCameraPos, "Gallop.Live.Cutt.LiveTimelineControl.GetCameraPos(int) at %p\n");
		ADD_HOOK(live_Cutt_LiveTimelineControl_set_liveStageCenterPos, "Gallop.Live.Cutt.LiveTimelineControl.set_liveStageCenterPos(Vector3) at %p\n");
		ADD_HOOK(AssetBundleHelper_GetResourceVer, "Gallop.AssetBundleHelper.GetResourceVer() at %p\n");
		if (g_replace_font)
		{
			ADD_HOOK(on_populate, "Gallop.TextCommon::OnPopulateMesh at %p\n");
		}
		
		if (g_max_fps > -1)
		{
			// break 30-40fps limit
			ADD_HOOK(set_fps, "UnityEngine.Application.set_targetFrameRate at %p \n");
		}
		
		if (g_unlock_size)
		{
			// break 1080p size limit
			ADD_HOOK(get_virt_size, "Gallop.StandaloneWindowResize.getOptimizedWindowSizeVirt at %p \n");
			ADD_HOOK(get_hori_size, "Gallop.StandaloneWindowResize.getOptimizedWindowSizeHori at %p \n");
			ADD_HOOK(wndproc, "Gallop.StandaloneWindowResize.WndProc at %p \n");

			// remove fixed 1080p render resolution
			ADD_HOOK(gallop_get_screenheight, "Gallop.Screen::get_Height at %p\n");
			ADD_HOOK(gallop_get_screenwidth, "Gallop.Screen::get_Width at %p\n");

			ADD_HOOK(canvas_scaler_setres, "UnityEngine.UI.CanvasScaler::set_referenceResolution at %p\n");
		}

		if (g_auto_fullscreen)
		{
			ADD_HOOK(set_resolution, "UnityEngine.Screen.SetResolution(int, int, bool) at %p\n");
			adjust_size();
		}
		
		if (g_dump_entries)
			dump_all_entries();
	}
	std::string current_time()
	{
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch());
		return std::to_string(ms.count());
	}
	void write_file(std::string file_name, char* buffer, int len)
	{
		FILE* fp;
		fopen_s(&fp, file_name.c_str(), "wb");
		if (fp != nullptr) {
			fwrite(buffer, 1, len, fp);
			fclose(fp);
		}
	}
	void* LZ4_decompress_safe_ext_orig = nullptr;

	int LZ4_decompress_safe_ext_hook(
		char* src,
		char* dst,
		int compressedSize,
		int dstCapacity)
	{
		//printf("compressedSize=%d,dstCapacity=%d", compressedSize, dstCapacity);
		char* decrypted = NULL;
		//server_ip;
		int ret = 0;
		
		if (g_lz4Encrypt) {
			decrypted = new char[dstCapacity];
			ret = reinterpret_cast<decltype(LZ4_decompress_safe_ext_hook)*>(LZ4_decompress_safe_ext_orig)(
				src, decrypted, compressedSize, dstCapacity);
		}
		else {
			char* realLZ4buff = new char[compressedSize - (int)4];
			decrypted = new char[dstCapacity];
			memcpy(realLZ4buff, src + 4, compressedSize - (int)4);
			//DumpHex(realLZ4buff, compressedSize - (int)4);
			/*
			z_stream infstream;
			infstream.zalloc = Z_NULL;
			infstream.zfree = Z_NULL;
			infstream.opaque = Z_NULL;
			// setup "b" as the input and "c" as the compressed output
			infstream.avail_in = (uInt)compressedSize; // size of input
			infstream.next_in = (Bytef*)src; // input char array
			infstream.avail_out = (uInt)dstCapacity; // size of output
			infstream.next_out = (Bytef*)decrypted; // output char array

			// the actual DE-compression work.
			inflateInit2(&infstream, 16 + MAX_WBITS);
			inflate(&infstream, Z_NO_FLUSH);
			inflateEnd(&infstream);
			printf("in: %d\n", compressedSize);
			if (infstream.msg != NULL) {
				printf("%s\n", infstream.msg);
			}
			
			printf("Uncompressed size is: %lu\n", infstream.total_out);
			//printf("Uncompressed string is: %s\n", c);*/
			//ret = (int)infstream.total_out;
			ret = LZ4_decompress_safe(realLZ4buff,decrypted,compressedSize-(int)4,dstCapacity);
			//lz4
			printf("compressedSize: %d, dstCapacity:%d\n", compressedSize, dstCapacity);
			delete[] realLZ4buff;
			//ret = compressedSize;
			//memcpy(decrypted,dst,compressedSize);
		}
		
		printf("Server Response: %d Bytes\n",ret);
		
		//auto out_path = std::string("CarrotJuicer\\").append(current_time()).append("R.msgpack");
		//write_file(out_path, decrypted, ret);
		//printf("wrote response to %s\n", out_path.c_str());		
		
		if (g_sendserver) {
			printf("------Real Cygames server -> modding server(local) -> Client------\n");
		
			
			httplib::Client cli(server_ip,server_port);
			std::string data(decrypted, ret);
			auto res = cli.Post("/umamusume_uploadmsgpack/live",data, "application/x-msgpack");
			res->status;
			const char* returned = res->body.c_str();			
			printf("\n");
			size_t clength = res->body.length();
			printf("Original Server length: %d, modified length: %d\n", ret, clength);
			ret = res->body.length();
			
			//printf("Status: %d\nContent-Length:%d\n", res->status, clength);
			
			memcpy(dst, returned, clength);//sss
			delete[] decrypted;

			//delete[] returned;
			//free(src);
			printf("-----------------------------------------------------------------\n");
		}
		

		//dst = {0};
		return ret;
	}
	void* LZ4_compress_default_ext_orig = nullptr;

	int LZ4_compress_default_ext_hook(
		char* src,
		char* dst,
		int srcSize,
		int dstCapacity)
	{
		//char* decrypted = new char[dstCapacity] {};
		char* raw_data = new char[srcSize] {};
		memcpy(raw_data, src, srcSize);
		if (g_sendserver) {
			httplib::Client cli(server_ip, server_port);
			std::string data(raw_data, srcSize);
			printf("------Client -> modding server(local) -> Real Cygames server------\n");
			auto res = cli.Post("/umamusume_uploadmsgpack/uma_client_request_toserver", data, "application/x-msgpack");
			res->status;
			const char* returned = res->body.c_str();			
			size_t clength = res->body.length();
			memcpy(src, returned, clength);
			int ret = 0;
			if (g_lz4Encrypt) {
				ret = reinterpret_cast<decltype(LZ4_compress_default_ext_hook)*>(LZ4_compress_default_ext_orig)(
					src, dst, clength, dstCapacity);
			}
			else {
				ret = LZ4_compress_default(src, dst, clength, dstCapacity);
				//memcpy(dst, src, clength);
				//ret = clength;
			}
			printf("Raw Client data: %d Bytes , Modified Clinet Req data: %d bytes, (Finally Compressed/Encrypted to %d bytes)\n", srcSize, clength ,ret);
			//auto out_path = std::string("CarrotJuicer\\").append(current_time()).append("Q.msgpack");
			//write_file(out_path, src, srcSize);
			//printf("wrote raw clinet request to %s\n", out_path.c_str());
			printf("-------------------------------------------------------------------------\n");
			delete[] raw_data;
			return ret;
		}
		else {
			int ret = reinterpret_cast<decltype(LZ4_compress_default_ext_hook)*>(LZ4_compress_default_ext_orig)(
				src, dst, srcSize, dstCapacity);
			printf("Raw Client data: %d Bytes (Compressed/Encrypted to %d bytes) -> ", srcSize, ret);
			auto out_path = std::string("CarrotJuicer\\").append(current_time()).append("Q.msgpack");
			//write_file(out_path, src, srcSize);
			//printf("wrote clinet request to %s\n", out_path.c_str());
			delete[] raw_data;
			return ret;
		}

		
	}
	void bootstrap_carrot_juicer()
	{
		std::filesystem::create_directory("CarrotJuicer");

		auto libnative_module = GetModuleHandle("libnative.dll");
		printf("libnative.dll at %p\n", libnative_module);
		if (libnative_module == nullptr)
		{
			return;
		}

		auto LZ4_decompress_safe_ext_ptr = GetProcAddress(libnative_module, "LZ4_decompress_safe_ext");
		printf("LZ4_decompress_safe_ext at %p\n", LZ4_decompress_safe_ext_ptr);
		if (LZ4_decompress_safe_ext_ptr == nullptr) {
			return;
		}
		MH_CreateHook(LZ4_decompress_safe_ext_ptr, LZ4_decompress_safe_ext_hook, &LZ4_decompress_safe_ext_orig);
		MH_EnableHook(LZ4_decompress_safe_ext_ptr);

		auto LZ4_compress_default_ext_ptr = GetProcAddress(libnative_module, "LZ4_compress_default_ext");
		printf("LZ4_compress_default_ext at %p\n", LZ4_compress_default_ext_ptr);
		if (LZ4_compress_default_ext_ptr == nullptr) {
			return;
		}
		MH_CreateHook(LZ4_compress_default_ext_ptr, LZ4_compress_default_ext_hook, &LZ4_compress_default_ext_orig);
		MH_EnableHook(LZ4_compress_default_ext_ptr);
	}


	
}

bool init_hook()
{
	std::ifstream config_stream{ "config.json" };
	std::vector<std::string> dicts{};

	if (!config_stream.is_open())
		return false;

	rapidjson::IStreamWrapper wrapper{ config_stream };
	rapidjson::Document document;

	document.ParseStream(wrapper);

	if (!document.HasParseError()) {
		g_sendserver = document["passPacket"].GetBool();
		strcpy(server_ip, document["serverIP"].GetString());
		server_port = document["serverPort"].GetInt();
		//strcpy(anotherprgname, document["runanotherprgname"].GetString());
		if (g_sendserver) {			
			printf("Json Pass to another server enabled: %s:%d\n", server_ip,server_port);
			//printf("excuting %s\n", anotherprgname);
			//ShellExecute(0, "open", "explorer", anotherprgname, 0, SW_HIDE);
		}
	}
	if (mh_inited)
		return false;

	if (MH_Initialize() != MH_OK)
		return false;

	mh_inited = true;

	MH_CreateHook(LoadLibraryW, load_library_w_hook, &load_library_w_orig);
	MH_EnableHook(LoadLibraryW);

	return true;
}

void uninit_hook()
{
	if (!mh_inited)
		return;

	for (auto hook : enabled_hooks)
		MH_DisableHook(hook);

	enabled_hooks.clear();

	MH_Uninitialize();
}

void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		}
		else {
			ascii[i % 16] = '.';
		}
		if ((i + 1) % 8 == 0 || i + 1 == size) {
			printf(" ");
			if ((i + 1) % 16 == 0) {
				printf("|  %s \n", ascii);
			}
			else if (i + 1 == size) {
				ascii[(i + 1) % 16] = '\0';
				if ((i + 1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i + 1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

DWORD getCurrentDisplayHz() {
	DEVMODE devmode;
	memset(&devmode, 0, sizeof(DEVMODE));
	devmode.dmSize = sizeof(DEVMODE);
	devmode.dmFields = DM_DISPLAYFREQUENCY;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);

	//free(&devmode);
	return devmode.dmDisplayFrequency;
}