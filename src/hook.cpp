#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS 1
#define MAX_HORSE_NUM 18
#define IMGUI_DEFINE_MATH_OPERATORS
#define stringify( name ) #name

#include <stdinclude.hpp>

static ImVector<ImRect> s_GroupPanelLabelStack;

HWND currenthWnd = NULL;
HWND imguiWnd = NULL;
bool hook_end = false;
bool imguiShow = false;
bool beforePatched = false;

bool imgui_settingwnd_open = false;

DWORD getCurrentDisplayHz();
void DumpHex(const void* data, size_t size);
void (*setExclusiveFullScreen)(int, int, int, int) = nullptr;
void adjust_size(int w = 0, int h=0);
void set_resolution_hook(int, int, bool);
void (*sql_query)(Connection* connection, Il2CppString* sql) = nullptr;
bool (*sql_exec)() = nullptr;
Il2CppString* (*GetCharaNameByCharaId)(int) = nullptr;
Il2CppObject* masterDB_instance = nullptr;
Connection* masterDBconnection = nullptr;
int live_currentFrame = 0;
Vector3_t liveCam_Lookat{};
Vector3_t liveCam_pos1{};
Vector3_t liveCam_pos2{};
Url lastUrl;
Url currentUrl;
float liveTimeSec = 0.0;
float liveTotalTimeSec = 0.0;

float liveTimelineManualScale = 0.1;
bool toastImGui = false;

int liveTimeLineFPS = 0;
char* toastMessage;
void startThread();
void hook_beforeboot();
int currSceneID = -1;

const char* GPUName;
//void setCustomFont();


WSADATA wsa;
SOCKET s;
struct sockaddr_in server;
Il2CppObject* ChangeScreenOrientation_hook(ScreenOrientation targetOrientation, bool isForce);

#pragma comment(lib, "ws2_32")

using namespace std;



	
int race_Currentrank = 0;
int race_MaxRank = 0;
bool isLiveStartFlag = false;
int count = 0;
float horseMeters[MAX_HORSE_NUM] = {0.1};
float lastMeter = 0.0;
bool (*GetKeyDown)(KeyCode) = nullptr;
bool (*GetKey)(KeyCode) = nullptr;
void (*TapEffect_Disable)(void*);
bool isKimuraChallenge = false;

void (*SoftwareReset)(void*);
void (*LoadScene)(Il2CppString*, LoadSceneParameters*);
void (*set_TimeScale)(float);
float (*get_TimeScale)();
void* (*UIManager_GetCanvasScalerList)(void*);
Il2CppString* (*GetGraphicsDeviceName)();
bool showFinishOrderFlash = true;

void* (*GameObject_Find)(Il2CppString*);

void SignalHandler(int signal)
{
	printf("Signal %d", signal);
	throw "!Access Violation!";
}


BOOL EnableCloseButton(const HWND hwnd, const BOOL bState)
{
	HMENU   hMenu;
	UINT    dwExtra;

	if (hwnd == NULL) return FALSE;
	if ((hMenu = GetSystemMenu(hwnd, FALSE)) == NULL) return FALSE;
	dwExtra = bState ? MF_ENABLED : (MF_DISABLED | MF_GRAYED);
	return EnableMenuItem(hMenu, SC_CLOSE, MF_BYCOMMAND | dwExtra) != -1;
}

bool hasEnding(std::string const& fullString, std::string const& ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
	return str;
}

void GameObject_SetActive(const char* path, bool enable) {
	Il2CppObject* gobj = (Il2CppObject*)GameObject_Find(il2cpp_string_new(string(path).data()));
	auto gobj_setActive = reinterpret_cast<void (*)
		(Il2CppObject * _instance, bool value)>(il2cpp_class_get_method_from_name(gobj->klass, "SetActive", 1)->methodPointer);
	if (gobj != nullptr) {
		printf("Get GameObj path=%s, res=0x%p, SetActive %d\n", path, &gobj, enable);
		gobj_setActive(gobj, enable);
	}
	else {
		printf("Error: Get GameObj path=%s is nullptr!!\n", path);
	}

}

void ResetGame() {
	LoadSceneParameters p = { 0 };
	p.LoadSceneMode = 0; //Single
	p.LocalPhysicsMode = 0;
	LoadScene(il2cpp_string_new(string("_Boot").data()), &p);
}



void* s2c_orig = nullptr;
Il2CppChar* s2c_hook(void* _this)
{

	Il2CppChar* ret = reinterpret_cast<decltype(s2c_hook)*>(s2c_orig)(_this);

	return ret;

}

string UmaGetString(Il2CppString_unk* in, int debug = 1) {
	string out = "";
	long long length = s2c_hook(in)->length;
	//printf("length is %d\n", length);
	short* start = &(s2c_hook(in)->start_char);
	//printf("start is %p\n", start);
	for (int i = 0; i <= length - 1; i++) {
		if (debug == 1) {
			printf("Text is %p\n", *start);
		}
		out += *(start++);
	}
	return out;
}

std::vector < std::string > explode(const std::string& str,
	const char& ch) {
	std::string next;
	std::vector < std::string > result;

	// For each character in the string
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		// If we've hit the terminal character
		if (*it == ch) {
			// If we have some characters accumulated
			if (!next.empty()) {
				// Add them to the result vector
				result.push_back(next);
				next.clear();
			}
		}
		else {
			// Accumulate the next character into the sequence
			next += *it;
		}
	}
	if (!next.empty())
		result.push_back(next);
	return result;
}

void meterDataSendThread() {
	printf("meterDataSendThread IN!!!\n");
	while (true)
	{
		//printf("Thread Alive!!\n");
		if (horseMeters[0] != lastMeter) {
			char* databyte = new char[sizeof(float) * MAX_HORSE_NUM];
			int byteAdded = 0;
			for (int i = 0; i < MAX_HORSE_NUM; i++) {
				char* p = reinterpret_cast<char*>(&horseMeters[i]);
				for (int k = 0; k < sizeof(float); k++)
				{
					databyte[byteAdded + k] = p[k];
					//printf("p[%d]=0x%02X\n", k, p[k]);
				}
				byteAdded += sizeof(float);
			}
			httplib::Client cli(g_sett->serverIP, g_sett->serverPort);
			std::string data(databyte, sizeof(float) * MAX_HORSE_NUM);
			auto res = cli.Post("/Mdataup", data, "application/binary");
			delete[] databyte;
			lastMeter = horseMeters[0];
			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	
	}
	
	
}

void keyDownCheckThread() {
	printf("KeyDownCheckThread IN!!!\n");
	
	while (true) {
		bool ret = GetKeyDown(KeyCode::S); //S
		bool ctrl = GetKey(KeyCode::LeftControl); //LCtrl
		bool alt = GetKey(KeyCode::LeftAlt);
		bool f12 = GetKeyDown(KeyCode::F12);
		bool f10 = GetKeyDown(KeyCode::F10);
		bool f11 = GetKeyDown(KeyCode::F11);
		if (ret) {
			printf("S is Pressed!!!!\n");
			while (GetKeyDown(KeyCode::S)) { }

			sett->stopLiveCam = !sett->stopLiveCam;
			printf("LiveCam Stop %s.\n",sett->stopLiveCam ? "Enabled" : "Disabled");
			if (sett->stopLiveCam) {
				toastImGui = true;
				toastMessage = (char*)"라이브 카메라를 정지했습니다.";
			}
			else {
				toastImGui = true;
				toastMessage = (char*)"라이브 카메라를 다시 움직입니다.";
			}

		}
		if (ctrl) {
			printf("LCtrl is pressed!!!!!\n");
			while (ctrl) {	
				//printf("check R\n");
				if (GetKeyDown(KeyCode::R)) {
					printf("Lctrl + R pressed, reset game\n");
					ResetGame();
					while (GetKey(KeyCode::R));
					break;
				} //R
				else if (GetKeyDown(KeyCode::P)) { //P
					if (get_TimeScale() <= 0.0) {
						set_TimeScale(1.0);
					}
					else {
						set_TimeScale(0.0);
					}
					printf("Lctrl + P pressed, Set game TimeScale to %.2f\n",get_TimeScale());
					while (GetKey(KeyCode::P));
					break;
				}
				else if (GetKey(KeyCode::PageUp)) {
					set_TimeScale(get_TimeScale() + 0.05);
					printf("Set Timescale %.2f\n", get_TimeScale());
					Sleep(100);
					//while (GetKey(KeyCode::PageUp));
					//break;
				}
				else if (GetKey(KeyCode::PageDown)) {
					set_TimeScale(get_TimeScale() - 0.05);
					printf("Set Timescale %.2f\n", get_TimeScale());
					Sleep(100);
					//while (GetKey(KeyCode::PageDown));
					//break;
				}
				else if (GetKeyDown(KeyCode::End)) {
					set_TimeScale(1.0);
					printf("Reset Timescale to %.2f\n", get_TimeScale());
					toastImGui = true;
					toastMessage = (char*)"배속을 1.0으로 초기화했습니다.";
					while (GetKey(KeyCode::PageDown));
					break;
				}
				else if (GetKey(KeyCode::LeftArrow)) {
					if (!sett->isLiveTimeManual) {
						sett->isLiveTimeManual = true;
						toastImGui = true;
						toastMessage = (char*)"라이브 타임라인 수동 조작이 활성화 되었습니다.";
					}
					
					if (liveTimeSec <= 0.0) 
					{ 
						liveTimeSec = 0.0; 
					}
					else {
						liveTimeSec = liveTimeSec - liveTimelineManualScale;
					}
					
					printf("set liveTime Second to %.4f\n", liveTimeSec);
					Sleep(1);
				}
				else if (GetKey(KeyCode::RightArrow)) {
					if (!sett->isLiveTimeManual) {
						sett->isLiveTimeManual = true;
						toastImGui = true;
						toastMessage = (char*)"라이브 타임라인 수동 조작이 활성화 되었습니다.";
					}
					liveTimeSec = liveTimeSec + liveTimelineManualScale;
					printf("set liveTime Second to %.4f\n", liveTimeSec);
					Sleep(1);
				}
				ctrl = GetKey(KeyCode::LeftControl);
			}
		}
		if (f12) {
			printf("F12 is Pressed!!!!\n");
			while (GetKeyDown(KeyCode::F12)) {}

			//bool b = ShowWindow(imguiWnd, imguiShow ? SW_HIDE : SW_SHOW);
			//imguiShow = !imguiShow;
			imgui_settingwnd_open = !imgui_settingwnd_open;
			printf("Show setting screen \n");
		}
		if (f10) {
			printf("F10 is Pressed!!!!\n");
			while (GetKeyDown(KeyCode::F10)) {}
			g_sett->isShowLivePerfInfo = !g_sett->isShowLivePerfInfo;
			//printf("%d\n", g_sett->isShowLivePerfInfo);
		}
		if (f11) {
			printf("F11 is Pressed!!!!\n");
			while (GetKeyDown(KeyCode::F11)) {}
			g_sett->isShowLiveFPSGraph = !g_sett->isShowLiveFPSGraph;
		}
		/*else if (alt) {
			printf("LAlt is pressed!!!!!\n");
			while (alt) {
				
				alt = GetKey(KeyCode::LeftAlt);
			}
							
		}*/
		//std::this_thread::sleep_for(std::chrono::milliseconds(5));
	}
}
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
wstring readFileIntoString(const string& path) {
		ifstream input_file(path);
		if (!input_file.is_open()) {
			cerr << "Could not open the file - '"
				<< path << "'" << endl;
			exit(EXIT_FAILURE);
		}
		return wstring((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
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

bool SaveFrameBufferToFile(ID3D11Device* pDevice, const std::string& filePath)
	{
		// 프레임 버퍼의 크기와 형식을 가져옵니다.
		ID3D11DeviceContext* pDeviceContext;
		pDevice->GetImmediateContext(&pDeviceContext);

		ID3D11RenderTargetView* pRenderTargetView;
		pDeviceContext->OMGetRenderTargets(1, &pRenderTargetView, nullptr);

		ID3D11Resource* pResource;
		pRenderTargetView->GetResource(&pResource);

		ID3D11Texture2D* pFrameBuffer;
		HRESULT hr = pResource->QueryInterface<ID3D11Texture2D>(&pFrameBuffer);
		pResource->Release();
		pRenderTargetView->Release();
		pDeviceContext->Release();

		if (FAILED(hr))
		{
			std::cerr << "Failed to access frame buffer" << std::endl;
			return false;
		}

		D3D11_TEXTURE2D_DESC frameBufferDesc;
		pFrameBuffer->GetDesc(&frameBufferDesc);

		// CPU용 텍스처를 생성합니다.
		D3D11_TEXTURE2D_DESC cpuTextureDesc;
		cpuTextureDesc.Width = frameBufferDesc.Width;
		cpuTextureDesc.Height = frameBufferDesc.Height;
		cpuTextureDesc.MipLevels = 1;
		cpuTextureDesc.ArraySize = 1;
		cpuTextureDesc.Format = frameBufferDesc.Format;
		cpuTextureDesc.SampleDesc.Count = 1;
		cpuTextureDesc.SampleDesc.Quality = 0;
		cpuTextureDesc.Usage = D3D11_USAGE_STAGING;
		cpuTextureDesc.BindFlags = 0;
		cpuTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		cpuTextureDesc.MiscFlags = 0;

		ID3D11Texture2D* pCPUTexture;
		hr = pDevice->CreateTexture2D(&cpuTextureDesc, nullptr, &pCPUTexture);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create CPU texture" << std::endl;
			pFrameBuffer->Release();
			return false;
		}

		// 프레임 버퍼의 내용을 CPU용 텍스처로 복사합니다.
		pDeviceContext->CopyResource(pCPUTexture, pFrameBuffer);

		// WIC 이미지 라이브러리를 초기화합니다.
		CoInitialize(nullptr);
		IWICImagingFactory* pWICFactory;
		hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
		if (FAILED(hr))
		{
			std::cerr << "Failed to create WIC imaging factory" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			return false;
		}

		// WIC 비트맵을 생성합니다.
		IWICBitmap* pWICBitmap;
		hr = pWICFactory->CreateBitmapFromMemory(
			frameBufferDesc.Width, frameBufferDesc.Height, GUID_WICPixelFormat32bppBGRA,
			frameBufferDesc.Width * 4, frameBufferDesc.Width * frameBufferDesc.Height * 4,
			reinterpret_cast<BYTE*>(pCPUTexture), &pWICBitmap
		);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create WIC bitmap" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICFactory->Release();
			return false;
		}

		// 이미지를 BMP 형식으로 저장합니다.
		IWICBitmapEncoder* pEncoder;
		hr = pWICFactory->CreateEncoder(GUID_ContainerFormatBmp, nullptr, &pEncoder);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create WIC encoder" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pWICFactory->Release();
			return false;
		}

		std::wstring wideFilePath(filePath.begin(), filePath.end());
		hr = pEncoder->Initialize(
			static_cast<IStream*>(nullptr),
			WICBitmapEncoderNoCache
		);
		if (FAILED(hr))
		{
			std::cerr << "Failed to initialize WIC encoder" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		IWICBitmapFrameEncode* pFrameEncode;
		hr = pEncoder->CreateNewFrame(&pFrameEncode, nullptr);
		if (FAILED(hr))
		{
			std::cerr << "Failed to create WIC frame encode" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		hr = pFrameEncode->Initialize(nullptr);
		if (FAILED(hr))
		{
			std::cerr << "Failed to initialize WIC frame encode" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pFrameEncode->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		hr = pFrameEncode->SetSize(frameBufferDesc.Width, frameBufferDesc.Height);
		if (FAILED(hr))
		{
			std::cerr << "Failed to set WIC frame size" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pFrameEncode->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		WICPixelFormatGUID formatGUID = GUID_WICPixelFormat32bppBGRA;
		hr = pFrameEncode->SetPixelFormat(&formatGUID);
		if (FAILED(hr))
		{
			std::cerr << "Failed to set WIC frame pixel format" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pFrameEncode->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		hr = pFrameEncode->WriteSource(pWICBitmap, nullptr);
		if (FAILED(hr))
		{
			std::cerr << "Failed to write WIC frame source" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pFrameEncode->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		hr = pFrameEncode->Commit();
		if (FAILED(hr))
		{
			std::cerr << "Failed to commit WIC frame encode" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pFrameEncode->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		hr = pEncoder->Commit();
		if (FAILED(hr))
		{
			std::cerr << "Failed to commit WIC encoder" << std::endl;
			pFrameBuffer->Release();
			pCPUTexture->Release();
			pWICBitmap->Release();
			pFrameEncode->Release();
			pEncoder->Release();
			pWICFactory->Release();
			return false;
		}

		// 사용한 리소스를 해제합니다.
		pFrameBuffer->Release();
		pCPUTexture->Release();
		pWICBitmap->Release();
		pFrameEncode->Release();
		pEncoder->Release();
		pWICFactory->Release();

		CoUninitialize();

		return true;
	}

int (*ObscuredInt_Decrypted)(ObscuredInt*) = nullptr;
void* load_library_w_orig = nullptr;
HMODULE __stdcall load_library_w_hook(const wchar_t* path)
	{
		wprintf(L"loaded %s\n", path);
		// GameAssembly.dll code must be loaded and decrypted while loading criware library
		if (path == L"advapi32"s) {
			hook_beforeboot();
			//imguiwindow();
		}
		else if (path == L"cri_ware_unity.dll"s)
		{
			printf("criwareunity\n");
			currenthWnd = GetActiveWindow();
			wchar_t buf[256];
			swprintf_s(buf, L"umamusume_L (%d개 패치 적용됨)", patchCount);


			

			/*RECT rect;
			HDC wdc = GetWindowDC(currenthWnd);
			GetClientRect(currenthWnd, &rect);
			SetTextColor(wdc, 0x00000000);
			SetBkMode(wdc, TRANSPARENT);
			rect.left = 0;
			rect.top = 0;
			DrawText(wdc, "message", -1, &rect, DT_SINGLELINE | DT_NOCLIP);
			DeleteDC(wdc);*/

			path_game_assembly();
			SetWindowText(currenthWnd, buf);
			hook_end = true;
			//GameAssembly ´ýÇÁ
			if (g_sett->dumpGameassembly) {
				HMODULE ga = GetModuleHandle(L"GameAssembly.dll");
				if (ga != nullptr) {
					//std::string exe_name = module_filename(NULL);
					printf("Trying to dump GameAssembly.dll...\n");
					pedump(ga, "dumped_GameAssembly.dll");
				}
				ga = nullptr;
			}

			


			//SaveFrameBufferToFile(pDevice, "c:\\screenshot.bmp");
			//HMODULE sq = GetModuleHandle("baselib.dll");
			//if (sq != nullptr) {
			//	//std::string exe_name = module_filename(NULL);
			//	printf("Trying to dump baselib.dll...\n");
			//	pedump(sq, "dumped_baselib.dll");
			//}
			//sq = nullptr;
			startThread();
			bootstrap_carrot_juicer();
			MH_DisableHook(LoadLibraryW);
			MH_RemoveHook(LoadLibraryW);


			//Il2CppObject* dbConn = il2cpp_object_new((Il2CppClass*)il2cpp_symbols::get_class("LibNative.Runtime.dll", "LibNative.Sqlite3", "Connection"));

			//printf("il2cppobject_new %p\n",dbConn->klass);

			//auto open = reinterpret_cast<bool (*)(Il2CppObject * _dbConnInstance, Il2CppString* filename, Il2CppString* vfsname, char* key, int cipherType)>(il2cpp_class_get_method_from_name((Il2CppClass*)dbConn->klass, "Open", 4)->methodPointer);
			//
			//printf("il2cppobject_new open = %p\n", &open);

			//printf("open=%d\n", open(dbConn, il2cpp_string_new("D:/code/Visual Studio/repos/MalEditor/MalEditor/bin/Debug/data/translate/translate_ko.db"), nullptr, nullptr, 1));

			//auto get_handle = reinterpret_cast<uintptr_t (*)(Il2CppObject * _dbConnInstance)>(il2cpp_class_get_method_from_name((Il2CppClass*)dbConn->klass, "get_DBHandle", 0)->methodPointer);
			//auto get_path = reinterpret_cast<Il2CppString* (*)(Il2CppObject * _dbConnInstance)>(il2cpp_class_get_method_from_name((Il2CppClass*)dbConn->klass, "get_dbPath", 0)->methodPointer);

			////FieldInfo* handleField = { il2cpp_class_get_field_from_name((Il2CppClass*)dbConn->klass, "DBHandle") };
			////FieldInfo* pathField = { il2cpp_class_get_field_from_name((Il2CppClass*)dbConn->klass, "dbPath") };
			//
			//Connection translateDB = {};
			//translateDB.dbPath = get_path(dbConn);
			//translateDB.Handle = get_handle(dbConn);

			////il2cpp_field_get_value(dbConn, handleField, (void*)translateDB.Handle);
			////il2cpp_field_get_value(dbConn, pathField, (void*)translateDB.dbPath);

			//wprintf(L"translatedbhandle = %p, path = %s\n", translateDB.Handle, translateDB.dbPath->start_char);

			//Il2CppObject* dbQuery = il2cpp_object_new((Il2CppClass*)il2cpp_symbols::get_class("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query"));
			//auto query = reinterpret_cast<void (*)(Il2CppObject* _instance, Connection* conn, Il2CppString* querystr)>(il2cpp_symbols::get_method_pointer("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query",".ctor",2));
			//auto query_exec = reinterpret_cast<bool (*)(Il2CppObject * _instance)>(il2cpp_symbols::get_method_pointer("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query", "Exec", 0));
			//auto query_GetText = reinterpret_cast<Il2CppString* (*)(Il2CppObject * _instance,int idx)>(il2cpp_symbols::get_method_pointer("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query", "GetText", 1));
			//query(dbQuery,&translateDB, il2cpp_string_new("select * from text_ko_kr;"));
			//bool r = query_exec(dbQuery);
			//printf("Exec = %d\n", r);
			//Il2CppString* q_res = query_GetText(dbQuery, 2);
			//wprintf(L"GetText = %s\n", q_res->start_char);

			
			// use original function beacuse we have unhooked that
			return LoadLibraryW(path);
			//GetModuleHandle()
			
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
		
		if (isKimuraChallenge) {
			string kim = "키무라";
			return il2cpp_string_new(kim.data());
		}

		//wprintf(L"Get textid : %d, return %s\n", id,orig_result->start_char);

		return result ? result : orig_result;
	}

	std::unordered_map<void*, bool> text_queries;

	void* query_ctor_orig = nullptr;
	void* query_ctor_hook(Il2CppObject* _this, Connection* conn, Il2CppString* sql)
	{
		auto ssql = std::wstring(sql->start_char);


		if (ssql.find(L"text_data") != std::string::npos ||
			ssql.find(L"character_system_text") != std::string::npos ||
			ssql.find(L"race_jikkyo_comment") != std::string::npos ||
			ssql.find(L"race_jikkyo_message") != std::string::npos ) 
		{
			text_queries.emplace(_this, true);
		}


		//wprintf(L"DBConnection info: Handle=%p dbPath=%s\n",conn->Handle,conn->dbPath->start_char);
		wstring path = conn->dbPath->start_char;
		//bool end = hasEnding(string(path.begin(), path.end()),"master.mdb");
		if (masterDBconnection == nullptr && hasEnding(string(path.begin(), path.end()), "master.mdb")) {
			masterDBconnection = conn;
			masterDB_instance = _this;
			wprintf(L"Set masterDBConnection Handle=%p dbPath=%s\n", conn->Handle, conn->dbPath->start_char);

			

			


			//if (std::filesystem::exists("firstrun.sql")) {
			//	wstring sqlQuery = readFileIntoString("firstrun.sql");
			//	//const wchar_t *f_sql = sqlQuery.append(ssql.c_str()).c_str();
			//	const wchar_t* f_sql = ssql.append(sqlQuery.c_str()).c_str();
			//	Il2CppString* str = il2cpp_string_new_utf16(f_sql,wcslen(f_sql));
			//	sql = str;
			//}
			//wprintf(L"Excute first sql [%s]\n", wstring(sql->start_char).c_str());
		}
		
		//Il2CppString* handle = (Il2CppString*)conn + 0xcc0;
		//wprintf(L"SQL Connection[ %s ]", wstring(conn.dbPath->start_char).c_str());
		//wprintf(L"SQL_Query[ %s ]\n", ssql.c_str());

		
		//const wchar_t* editsql = L"update text_data set `text`=\"Test01\" where `id`=16 and `category`=16 and `index`=1001;";
		
		//reinterpret_cast<decltype(query_ctor_hook)*>(query_ctor_orig)(_this, conn, il2cpp_string_new_utf16(editsql,wcslen(editsql)));
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
		if (g_sett->autoFpsSet) {
			g_sett->maxFps = getCurrentDisplayHz();
			printf("Auto fps limit setted : %d fps\n", g_sett->maxFps);
		}
		else {
			printf("fps limit setted : %d fps\n", g_sett->maxFps);
		}
		value = g_sett->maxFps;
		return reinterpret_cast<decltype(set_fps_hook)*>(set_fps_orig)(value);
		
	}

	
	/*void* live_CameraLookAt_UpdateCamera_orig = nullptr;
	void live_CameraLookAt_UpdateCamera_hook(void* _this) {
		printf("UpdateCamera Called\n");
		return reinterpret_cast<decltype(live_CameraLookAt_UpdateCamera_hook)*>
			(live_CameraLookAt_UpdateCamera_orig)(_this);
	}*/

	/*void* live_Cutt_LiveTimelineControl_GetCameraPos_orig = nullptr;
	Vector3_t* live_Cutt_LiveTimelineControl_GetCameraPos_hook(int index) {
		auto pos = reinterpret_cast<decltype(live_Cutt_LiveTimelineControl_GetCameraPos_hook)*>(live_Cutt_LiveTimelineControl_GetCameraPos_orig)(index);
		printf("Charaid: %d, Cam: x=%.2f, y=%.2f, z=%.2f\n", index, pos->x, pos->y, pos->z);
		return pos;
	}*/

	/*void* live_Cutt_LiveTimelineControl_set_liveStageCenterPos_orig = nullptr;
	void live_Cutt_LiveTimelineControl_set_liveStageCenterPos_hook(Vector3_t* value) {
		printf("set_liveStageCenterPos: x=%.2f, y=%.2f, z=%.2f\n", value->x, value->y, value->z);
		return reinterpret_cast<decltype(live_Cutt_LiveTimelineControl_set_liveStageCenterPos_hook)*>
			(live_Cutt_LiveTimelineControl_set_liveStageCenterPos_orig)(value);
	}*/

	//void* live_cam_GetCullingMask_orig = nullptr;
	//unsigned int live_cam_GetCullingMask_hook(unsigned int val) {
	//	//printf("cam %d\n", val);		
	//	return reinterpret_cast<decltype(live_cam_GetCullingMask_hook)*>(live_cam_GetCullingMask_orig)(val);
	//	//return 511;
	//}

	void* AssetBundleHelper_GetResourceVer_orig = nullptr;
	Il2CppString* AssetBundleHelper_GetResourceVer_hook() {
		auto str = reinterpret_cast<decltype(AssetBundleHelper_GetResourceVer_hook)*>
			(AssetBundleHelper_GetResourceVer_orig)();
		std::wstring verstr = std::wstring(str->start_char);
		wprintf(L"Version Str:[%s]\n",verstr.c_str());

		char* conv_str = new char[str->length + 2];
		memset(conv_str, 0, str->length + 2);
		wcstombs(conv_str, verstr.c_str(), str->length + 1);
		

		//httplib::Client cli("127.0.0.1", 564);
		//std::string data(conv_str, str->length + 2);
		
		//auto res = cli.Post("/umamusume_test_server/tool/upload_resourcever", data, "application/text");

		delete[] conv_str;
		//res->status;
		//const char* returned = res->body.c_str();
		//size_t clength = res->body.length();

		//wprintf()
		return str;
	}

	void* GachaBGController_OverrideRarity_orig = nullptr;
	void GachaBGController_OverrideRarity_hook(void* _this, int doorindex, int type) {
		printf("[GachaBGController.OverrideRarity] Doorindex:=%d, GachaDefine.GachaRarityType_Door(enum)= %d\n", doorindex,type);
		return reinterpret_cast<decltype(GachaBGController_OverrideRarity_hook)*>
			(GachaBGController_OverrideRarity_orig)(_this,doorindex,type);
	}

	void* GachaBGController_SetGateDoorRarity_orig = nullptr;
	void GachaBGController_SetGateDoorRarity_hook(void* _this, int doorindex) {
		printf("[GachaBGController.SetGateDoorRarity] Doorindex=%d\n",doorindex);
		return reinterpret_cast<decltype(GachaBGController_SetGateDoorRarity_hook)*>
			(GachaBGController_SetGateDoorRarity_orig)(_this,doorindex);
	}

	void* FadeGui_SetAlpha_orig = nullptr;
	void FadeGui_SetAlpha_hook(Il2CppObject* _this, float alpha) {
		
		if (!g_sett->showLiveTitleWindow) {
			alpha = 0.0f;
		}

		return reinterpret_cast<decltype(FadeGui_SetAlpha_hook)*>
			(FadeGui_SetAlpha_orig)(_this, alpha);
	}

	//void* LiveTitleController_Setup_orig = nullptr;
	//void LiveTitleController_Setup_hook(void* _this, LiveData* livedata, bool isAdjustForDialog = false){
	//	//wprintf(L"%s", std::wstring(title->start_char).c_str());
	//	printf("LiveTitle_Setup: [MusicID=%d, LiveMemberNumber=%d]\n", livedata->MusicId,livedata->LiveMemberNumber);
	//	return reinterpret_cast<decltype(LiveTitleController_Setup_hook)*>
	//		(LiveTitleController_Setup_orig)(_this, livedata,isAdjustForDialog);
	//}
	//void* LiveTitleController_FadeIn_orig = nullptr;
	//void LiveTitleController_FadeIn_hook(void* _this, float duration, float currentTime) {
	//	printf("Title_FadeIn %.5f, %.5f\n", duration, currentTime);
	//	if (g_showLiveTitleWindow) {
	//		return reinterpret_cast<decltype(LiveTitleController_FadeIn_hook)*>
	//			(LiveTitleController_FadeIn_orig)(_this, duration, currentTime);
	//	}
	//	return;
	//}

	//void* LiveTitleController_FadeOut_orig = nullptr;
	//void LiveTitleController_FadeOut_hook(void* _this, float duration, float currentTime) {
	//	//printf("Title_FadeOut %.5f, %.5f\n", duration, currentTime);
	//	if (g_showLiveTitleWindow) {
	//		return reinterpret_cast<decltype(LiveTitleController_FadeOut_hook)*>
	//			(LiveTitleController_FadeOut_orig)(_this, duration, currentTime);
	//	}		
	//	return;
	//}

	//void* LiveTitleController_AlterUpdate_orig = nullptr;
	//void LiveTitleController_AlterUpdate_hook(Il2CppObject* _this) {
	//	printf("Called LiveTitleController_AlterUpdate\n");
	//	return;
	//}

	//void* Unity_Post_orig = nullptr;
	//void* Unity_Post_hook(Il2CppString* Uri, void* stuff) {
	//	wprintf(L"%s\n", std::wstring(Uri->start_char).c_str());
	//	return reinterpret_cast<decltype(Unity_Post_hook)*>
	//		(Unity_Post_orig)(Uri, stuff);
	//}



	void* RaceUI_SetRaceUIActive_orig = nullptr;
	void RaceUI_SetRaceUIActive_hook(void* _this, bool active) {
		printf("RaceUI.SetRaceUIActive = %d\n", active);
		return reinterpret_cast<decltype(RaceUI_SetRaceUIActive_hook)*>
			(RaceUI_SetRaceUIActive_orig)(_this,active);
	}

	void* RaceUI_SetVisibleRank_orig = nullptr;
	void RaceUI_SetVisibleRank_hook(void* _this, bool active) {
		printf("RaceUI.SetVisibleRank = %d\n", active);
		return reinterpret_cast<decltype(RaceUI_SetVisibleRank_hook)*>
			(RaceUI_SetVisibleRank_orig)(_this, active);
	}

	void* RaceUIRank_Setup_orig = nullptr;
	void RaceUIRank_Setup_hook(void* _this,int index, int indexMax, void* displayTarget, void* distanceCheckTarget, int horseNum, float showDistance, float hideDistance) {
		printf("RaceUIRank_Setup index=%d, indexMax=%d, horseNum=%d, showDist=%5f, hideDist=%5f\n"
			, index, indexMax, horseNum, showDistance, hideDistance);
		race_Currentrank = horseNum - 5;
		race_MaxRank = horseNum;
		return reinterpret_cast<decltype(RaceUIRank_Setup_hook)*>
			(RaceUIRank_Setup_orig)(_this, index, indexMax, displayTarget, distanceCheckTarget, horseNum, ((g_sett->rankUIShowMeter < 0) ? 0.0f : g_sett->rankUIShowMeter), (hideDistance + g_sett->rankUIHideoffset));
	}

	void* RaceUIRank_PlayPlayerRankUp_orig = nullptr;
	void RaceUIRank_PlayPlayerRankUp_hook(void* _this) {
		race_Currentrank--;
		printf("PlayPlayerRankUp() Called -> Current Rank: %d/%d\n",race_Currentrank,race_MaxRank);
		return reinterpret_cast<decltype(RaceUIRank_PlayPlayerRankUp_hook)*>
			(RaceUIRank_PlayPlayerRankUp_orig)(_this);
	}

	void* RaceUIRank_PlayPlayerRankDown_orig = nullptr;
	void RaceUIRank_PlayPlayerRankDown_hook(void* _this) {
		race_Currentrank++;
		printf("PlayPlayerRankDown() Called -> Current Rank: %d/%d\n",race_Currentrank, race_MaxRank);
		return reinterpret_cast<decltype(RaceUIRank_PlayPlayerRankDown_hook)*>
			(RaceUIRank_PlayPlayerRankDown_orig)(_this);
	}

	void* RaceManager_GetHorseDistanceByIndex_orig = nullptr;
	float RaceManager_GetHorseDistanceByIndex_hook(void* _this, int index) {
		float dist = reinterpret_cast<decltype(RaceManager_GetHorseDistanceByIndex_hook)*>
			(RaceManager_GetHorseDistanceByIndex_orig)(_this,index);
		horseMeters[index] = dist;
		printf("Horse=%d, CurrentDist=%.1f m\n",index+1, dist);
		//sendDataToSock(p, 4);
		return dist;
	}

	void* LiveTheaterInfo_GetDefaultDressid_orig = nullptr;
	void LiveTheaterInfo_GetDefaultDressid_hook(void* _this,int index, int charaId, bool isMob, int* dressId, int* dressColor, int* dressId2, int* dressColor2) {
		printf("LiveTheaterInfo_GetDefaultDressid index=%d,charaid=%d,ismob=%d,*dressId=%p,*dressColor=%p,*dressId2=%p,dressColor2=%p\n",
			index, charaId, isMob, dressId, dressColor, dressId2, dressColor2);
		//*dressId = *dressId;
		return reinterpret_cast<decltype(LiveTheaterInfo_GetDefaultDressid_hook)*>
			(LiveTheaterInfo_GetDefaultDressid_orig)(_this,index,charaId,isMob,dressId,dressColor,dressId2,dressColor2);
	}

	void* LiveTheaterInfo_UpdateCharaDressIds_orig = nullptr;
	void LiveTheaterInfo_UpdateCharaDressIds_hook(void* _this, void* LiveTheaterMemberInfo) {
		printf("LiveTheaterInfo_UpdateCharaDressIds called\n");
		reinterpret_cast<decltype(LiveTheaterInfo_UpdateCharaDressIds_hook)*>
			(LiveTheaterInfo_UpdateCharaDressIds_orig)(_this, LiveTheaterMemberInfo);
	}

	void* LiveTheaterInfo_CheckDress_orig = nullptr;
	void LiveTheaterInfo_CheckDress_hook(void* _this, int index, CharaDressIdSet* idset) {
		printf("LiveTheaterInfo_CheckDress called index=%d, charaid=%d\n",index,idset->_charaId);
		if (g_sett->liveCharaAutoDressReplace) {
			return reinterpret_cast<decltype(LiveTheaterInfo_CheckDress_hook)*>
				(LiveTheaterInfo_CheckDress_orig)(_this, index, idset);
		}
		return;
	}

	void* (*ModelLoader_CreateMiniModel)(void*,CharacterBuildInfo*);

	void* ModelLoader_CreateNormalModel_orig = nullptr;
	void* ModelLoader_CreateNormalModel_hook(void* _this, CharacterBuildInfo* charinfo) {
		wprintf(L"CreateNormalModel charinfo->charaid=%d, Dressdata_HeadSubid=%d ,HeadModelSubid=%d, dressid=%d ,DressElement.charaid=%d,DressElement.id=%d,DressElement.UseLiveTheater=%d, Name=%s, bust=%d , isPd=%d\n",
			charinfo->_charaId, charinfo->_dressElement->HeadSubId,charinfo->_headModelSubId, charinfo->_dressId, charinfo->_dressElement->CharaId, charinfo->_dressElement->Id, charinfo->_dressElement->UseLiveTheater,
			std::wstring(charinfo->_name->start_char).c_str(), charinfo->_bustType, charinfo->_isPersonalDress);
		//charinfo->_charaId = 1002;
		//charinfo->_dressId = 9;
		//charinfo->_dressElement->CharaId = 1002;
		//charinfo->_dressElement->Id = 9;
		//charinfo->_dressElement->CharaId = 9;
		return reinterpret_cast<decltype(ModelLoader_CreateNormalModel_hook)*>
			(ModelLoader_CreateNormalModel_orig)(_this,charinfo);
	}

	void* ModelLoader_CreateModel_orig = nullptr;
	void* ModelLoader_CreateModel_hook(void* _this, CharacterBuildInfo* charinfo) {
		printf("CreateModel Called \n");
		/*wprintf(L"CreateModel charinfo->charaid=%d, cardid=%d, HeadModelSubid=%d, dressid=%d ,DressElement.charaid=%d,DressElement.id=%d,DressElement.UseLiveTheater=%d, Name=%s, bust=%d , isPd=%d\n",
			charinfo->_charaId, charinfo->_cardId,charinfo->_headModelSubId,charinfo->_dressId,charinfo->_dressElement->CharaId, charinfo->_dressElement->Id, charinfo->_dressElement->UseLiveTheater,
			std::wstring(charinfo->_name->start_char).c_str(),charinfo->_bustType,charinfo->_isPersonalDress);*/
		//charinfo->_charaId = 1007;
		//charinfo->_dressId = 9;
		//charinfo->_dressElement->CharaId = 9;*/
		/*if (charinfo->_charaId == 9005) {
			charinfo->_headModelSubId = -1;
		}*/
		void* GameObj = ModelLoader_CreateMiniModel(_this, charinfo);
		return GameObj;
		/*return reinterpret_cast<decltype(ModelLoader_CreateModel_hook)*>
			(ModelLoader_CreateModel_orig)(charinfo);*/
	}

	void* CharacterBuildInfo_ctor_orig = nullptr;
	void CharacterBuildInfo_ctor_hook(CharacterBuildInfo* _this, 
		int charaId, int dressId, int controllerType, int headId = 0, int zekken = 0, 
		int mobId = 0, int backDancerColorId = -1, bool isUseDressDataHeadModelSubId = true, int audienceId = 0,
		int motionDressId = -1, bool isEnableModelCache = true) {
		printf("CharacterBuildInfo_ctor called origcharaid=%d, origdressid=%d, mini=%d\n", charaId, dressId,_this->_miniMobParentCharaId);
		
		//if(_this.)
		if (g_sett->homeAllDiamond) {
			charaId = 1067;
			dressId = 106701;
			headId = 0;
		}
		else {
			if (sett->gachaCutinChara > -1) {
				/*if (charaId == 9001)
					charaId = 9002;*/
					//else
				_this->_charaId = sett->gachaCutinChara;
				charaId = sett->gachaCutinChara;
			}
			if (sett->gachaCutinDress > -1) {
				/*if (dressId == 900101)
					dressId = 900201;*/
					//else

				_this->_dressId = sett->gachaCutinDress;
				dressId = sett->gachaCutinDress;
			}
			if (sett->gachaCutinHeadid > -1) {
				_this->_headModelSubId = sett->gachaCutinHeadid;
				headId = sett->gachaCutinHeadid;
			}
		}
		//controllerType = 0x0c;

		printf("CharacterBuildInfo_ctor called ccharaid=%d, cdressid=%d\n", charaId, dressId);
		//charaId = 1070;
		//dressId = 2;
		return reinterpret_cast<decltype(CharacterBuildInfo_ctor_hook)*>
			(CharacterBuildInfo_ctor_orig)(_this,charaId,dressId,controllerType,headId,
				zekken,mobId,backDancerColorId,isUseDressDataHeadModelSubId, audienceId,
				motionDressId,isEnableModelCache);
	}

	void* CharacterBuildInfo_ctor_overload1_orig = nullptr;
	void CharacterBuildInfo_ctor_overload1_hook(CharacterBuildInfo* src) {
		printf("CharacterBuildInfo_ctor_overload1 called charaid=%d, dressid=%d\n", src->_charaId, src->_dressId);

		if (sett->gachaCutinChara > -1) {
			src->_charaId = sett->gachaCutinChara;
		}
		if (sett->gachaCutinDress > -1) {
			src->_dressId = sett->gachaCutinDress;
		}
		if (sett->gachaCutinHeadid > -1) {
			src->_headModelSubId = sett->gachaCutinHeadid;
		}
		printf("CharacterBuildInfo_ctor_overload1 called ccharaid=%d, cdressid=%d\n", src->_charaId, src->_dressId);
		return reinterpret_cast<decltype(CharacterBuildInfo_ctor_overload1_hook)*>
			(CharacterBuildInfo_ctor_overload1_orig)(src);
	}


	void* CharacterBuildInfo_ctor_overload2_orig = nullptr;
	void CharacterBuildInfo_ctor_overload2_hook(void* _instance, 
		int cardId, int charaId, int dressId, int controllerType, 
		int headId = 0, int zekken = 0, int mobId = 0, int backDancerColorId = -1, 
		int overrideClothCategory = -1, bool isUseDressDataHeadModelSubId = true,
		int audienceId = 0, int motionDressId = -1, bool isEnableModelCache = true) {
		printf("CharacterBuildInfo_ctor2 called origcharaid=%d, origdressid=%d\n", charaId, dressId);

		if (sett->gachaCutinChara > -1) {
			charaId = sett->gachaCutinChara;
		}
		if (sett->gachaCutinDress > -1) {
			dressId = sett->gachaCutinDress;
		}
		if (sett->gachaCutinHeadid > -1) {
			headId = sett->gachaCutinHeadid;
		}

		printf("CharacterBuildInfo_ctor2 called ccharaid=%d, cdressid=%d\n", charaId, dressId);
		return reinterpret_cast<decltype(CharacterBuildInfo_ctor_overload2_hook)*>
			(CharacterBuildInfo_ctor_overload2_orig)(_instance,cardId, charaId, dressId, controllerType, 
				headId, zekken, mobId, backDancerColorId,overrideClothCategory, 
				isUseDressDataHeadModelSubId, audienceId,motionDressId,isEnableModelCache);

	}


	int chars[] = {1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023,1024,1025,1026,1027};
	int dress[] = { 100101,100202,100303,100404,100505,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020,1021,1022,1023,1024,1025,1026,1027 };
	void* StoryCharacter3D_LoadModel_orig = nullptr;
	void* StoryCharacter3D_LoadModel_hook(int charaId, int cardId, int clothId, int zekkenNumber, int headId, bool isWet, bool isDirt, int mobId, int dressColorId, Il2CppString* zekkenName, int zekkenFontStyle = 1, int color = 3, int fontColor = 1, int suitColor = 0,bool isUseDressDataHeadModelSubId = true, bool useCircleShadow = false) {
		printf("StoryCharacter3D_LoadModel called charaid=%d, cardid=%d, clothid=%d, headid=%d mobid=%d\n", charaId, cardId,clothId,headId,mobId);
		/*std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(0, 26);
		charaId = chars[dis(gen)];
		clothId = 5;
		headId = 0;
		printf("%d\n", charaId);*/
		


		if (sett->changeStoryChar) 
		{
			if ((sett->story3dCharID < 0) || (sett->story3dClothID < 0) || (sett->story3dHeadID < 0) || (sett->story3dMobid < 0))
			{
				std::string line;
				while (true) {
					try {
						

						std::cout << "Enter charaid, clothid, headid, mobid: " << line << "\n";

						std::getline(std::cin, line);

						//std::cout <<  "Entered: " << line;
						std::vector < std::string > arg = explode(line, ' ');

						
						int _charaId = std::stoi(arg.at(0).c_str());
						int _dressId = std::stoi(arg.at(1).c_str());
						int _mobid = std::stoi(arg.at(2).c_str());
						int _headid = std::stoi(arg.at(3).c_str());
						
						charaId = _charaId;
						clothId = _dressId;
						mobId = _mobid;
						headId = _headid;

						printf("story3d set manual %d %d %d %d\n", charaId, clothId, mobId, headId);
						break;
					}
					catch (std::invalid_argument&) {
						printf("Value Error: please enter number only\n");
					}
					catch (std::out_of_range&) {
						std::cout << "You entered " << line << "\n";
						printf_s("Argument Error: please enter {CharID} {Dress id} {Mod ID} {Head id}\n");
					}
				}
				


				//printf("Enter charaid, clothid, headid, mobid: ");

				//scanf_s("%d %d %d %d", &charaId, &clothId, &headId, &mobId);
				printf("\n");
			}
			else 
			{
				charaId = sett->story3dCharID;
				clothId = sett->story3dClothID;
				mobId = sett->story3dMobid;
				headId = sett->story3dHeadID;
				printf("story3d set %d %d %d %d\n", charaId, clothId, mobId, headId);
			}
		}
		

		
		/*static bool flag = false;
		charaId = 9004;
		clothId = 900401;
		headId = 0;
		flag = !flag;*/
		return reinterpret_cast<decltype(StoryCharacter3D_LoadModel_hook)*>
			(StoryCharacter3D_LoadModel_orig)(charaId, cardId, clothId, zekkenNumber, headId, isWet, isDirt, mobId,dressColorId, zekkenName, zekkenFontStyle, color, fontColor , suitColor ,isUseDressDataHeadModelSubId,useCircleShadow);
	}


	void* Cute_Http_WWWRequest_Post_orig = nullptr;
	void Cute_Http_WWWRequest_Post_hook(void* _this,Il2CppString* url, char* postdata, void* headers) {
		wprintf(L"Original URL: %s\n", url->start_char);
		wstring url_raw = wstring(url->start_char);
		string url_conv;
		url_conv.assign(url_raw.begin(), url_raw.end());
		Url posturl(url_conv);
		currentUrl = posturl;
		if (strlen(g_sett->customHost) > 0) {
			Url replaceUrl(g_sett->customHost);
			posturl.scheme(replaceUrl.scheme());
			posturl.host(replaceUrl.host());
			posturl.port(replaceUrl.port());
			printf("Replaced URL: %s\n", posturl.str().c_str());
		}										
		
		return reinterpret_cast<decltype(Cute_Http_WWWRequest_Post_hook)*>
			(Cute_Http_WWWRequest_Post_orig)(_this, il2cpp_string_new(posturl.str().c_str()), postdata, headers);
							
	}

	void* apply_graphics_quality_orig = nullptr;
	void apply_graphics_quality_hook(Il2CppObject* _this, int quality, bool force)
	{
		reinterpret_cast<decltype(apply_graphics_quality_hook)*>(apply_graphics_quality_orig)(_this, 0x04, true);
	}
	
	/*void* BitmapTextCommon_GetFontPath_orig = nullptr;
	Il2CppString* BitmapTextCommon_GetFontPath_hook(void* _this,void* fontType) {
		auto path = reinterpret_cast<decltype(BitmapTextCommon_GetFontPath_hook)*>
			(BitmapTextCommon_GetFontPath_orig)(_this,fontType);
		wprintf(L"FontPath=%s\n", std::wstring(path->start_char).c_str());
		return path;
	}*/

	


	/*void* Race_GetRankNumSmallPath_orig = nullptr;
	Il2CppString* Race_GetRankNumSmallPath_hook(int num) {

		auto str = reinterpret_cast<decltype(Race_GetRankNumSmallPath_hook)*>
			(Race_GetRankNumSmallPath_orig)(num);

		wprintf(L"GetRankNumSmallPath = %s\n", std::wstring(str->start_char).c_str());

		return str;
	}*/

	void* GachaBGController_GateDoor_SetRarity_orig = nullptr;
	void GachaBGController_GateDoor_SetRarity_hook(void* _this, int e) {
		printf("[GachaBGController.GateDoor.SetRarity] Raritydoor_Type=%d\n", e);
		return reinterpret_cast<decltype(GachaBGController_GateDoor_SetRarity_hook)*>
			(GachaBGController_GateDoor_SetRarity_orig)(_this, e);
	}

	void* RaceResultScene_GetMotionVariationId_orig = nullptr;
	int RaceResultScene_GetMotionVariationId_hook(int charaId) {
		printf("GetMotionVariationId : charaId=%d\n", charaId);
		if (sett->raceResultCutinMotionChara > -1) {
			charaId = sett->raceResultCutinMotionChara;
			//dress->CharaId = c_raceResultCutinMotionChara;
		}
		return reinterpret_cast<decltype(RaceResultScene_GetMotionVariationId_hook)*>
			(RaceResultScene_GetMotionVariationId_orig)(charaId);
	}

	void* RaceResultScene_PlayFinishOrderAnim_orig = nullptr;
	void RaceResultScene_PlayFinishOrderAnim_hook(void* _this, void* onPlayed) {
		printf("RaceResultScene_PlayFinishOrderAnim called\n");
		return reinterpret_cast<decltype(RaceResultScene_PlayFinishOrderAnim_hook)*>
			(RaceResultScene_PlayFinishOrderAnim_orig)(_this,onPlayed);
	}

	void* RaceResultCutInHelper_LoadBodyMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadBodyMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType, void* resultSwapData, void* resultSwapMotionData) {
		
		printf("RaceResultCutInHelper_LoadBodyMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n",characterId, dress->Id,rank,grade);
		if (g_sett->winMotion564) {
			characterId = 1007;
			dress->CharaId = 1007;
			dress->Id = 100701;
			grade = 200;
			//c_raceResultCutinMotionRank = rank;
		}
		else {
			if (sett->raceResultCutinMotionChara > -1) {
				characterId = sett->raceResultCutinMotionChara;
				dress->CharaId = sett->raceResultCutinMotionChara;
			}
			if (sett->raceResultCutinMotionDress > -1) {
				dress->Id = sett->raceResultCutinMotionDress;
			}
			if (sett->raceResultCutinMotionGrade > -1) {
				grade = sett->raceResultCutinMotionGrade;
			}
			if (sett->raceResultCutinMotionRank > -1) {
				rank = sett->raceResultCutinMotionRank;
			}
		}
		
		
		
		//return nullptr;
		return reinterpret_cast<decltype(RaceResultCutInHelper_LoadBodyMotion_hook)*>
			(RaceResultCutInHelper_LoadBodyMotion_orig)(characterId, dress, personalityType, rank, grade, raceType, resultSwapData, resultSwapMotionData);
	}

	void* RaceResultCutInHelper_LoadCameraMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadCameraMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType, void* resultSwapData, void* resultSwapMotionData) {
		//printf("RaceResultCutInHelper_LoadCameraMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		printf("characterId: %d\n", characterId);
		printf("dress:\n");
		printf("\tId: %d\n", dress->Id);
		printf("\tConditionType: %d\n", dress->ConditionType);
		printf("\tHaveMini: %d\n", dress->HaveMini);
		printf("\tGeneralPurpose: %d\n", dress->GeneralPurpose);
		printf("\tCharaId: %d\n", dress->CharaId);
		printf("\tUseGender: %d\n", dress->UseGender);
		printf("\tBodyType: %d\n", dress->BodyType);
		printf("\tBodyTypeSub: %d\n", dress->BodyTypeSub);
		printf("\tBodySetting: %d\n", dress->BodySetting);
		printf("\tUseRace: %d\n", dress->UseRace);
		printf("\tUseLive: %d\n", dress->UseLive);
		printf("\tUseLiveTheater: %d\n", dress->UseLiveTheater);
		printf("\tUseHome: %d\n", dress->UseHome);
		printf("\tIsWet: %d\n", dress->IsWet);
		printf("\tIsDirt: %d\n", dress->IsDirt);
		printf("\tHeadSubId: %d\n", dress->HeadSubId);
		printf("\tUseSeason: %d\n", dress->UseSeason);
		printf("\tDressColorMain: %p\n", (void*)dress->DressColorMain);
		printf("\tDressColorSub: %p\n", (void*)dress->DressColorSub);
		printf("\tColorNum: %d\n", dress->ColorNum);
		printf("\tDispOrder: %d\n", dress->DispOrder);
		printf("\tTailModelId: %d\n", dress->TailModelId);
		printf("\tTailModelSubId: %d\n", dress->TailModelSubId);
		printf("\tStartTime: %lld\n", dress->StartTime);
		printf("\tEndTime: %lld\n", dress->EndTime);
		printf("\t_getCondition: %d\n", dress->_getCondition);
		printf("personalityType: %d\n", personalityType);
		printf("rank: %d\n", rank);
		printf("grade: %d\n", grade);
		printf("raceType: %d\n", raceType);
		//return nullptr;
		if (g_sett->winMotion564) {
			characterId = 1007;
			dress->CharaId = 1007;
			dress->Id = 100701;
			grade = 200;
			//c_raceResultCutinMotionRank = rank;
		}
		else {
			if (sett->raceResultCutinMotionChara > -1) {
				characterId = sett->raceResultCutinMotionChara;
				dress->CharaId = sett->raceResultCutinMotionChara;
			}
			if (sett->raceResultCutinMotionDress > -1) {
				dress->Id = sett->raceResultCutinMotionDress;
			}
			if (sett->raceResultCutinMotionGrade > -1) {
				grade = sett->raceResultCutinMotionGrade;
			}
			if (sett->raceResultCutinMotionRank > -1) {
				rank = sett->raceResultCutinMotionRank;
			}
		}

		void* ret = reinterpret_cast<decltype(RaceResultCutInHelper_LoadCameraMotion_hook)*>
			(RaceResultCutInHelper_LoadCameraMotion_orig)(
					characterId, dress, personalityType, rank, grade, raceType, resultSwapData, resultSwapMotionData
				);
		printf("LoadCameraMotion ret=%p\n", ret);
		return ret;
	}

	void* RaceResultCutInHelper_LoadEarMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadEarMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {
		printf("RaceResultCutInHelper_LoadEarMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		//return nullptr;
		if (g_sett->winMotion564) {
			characterId = 1007;
			dress->CharaId = 1007;
			dress->Id = 100701;
			grade = 200;
			//c_raceResultCutinMotionRank = rank;
		}
		else {
			if (sett->raceResultCutinMotionChara > -1) {
				characterId = sett->raceResultCutinMotionChara;
				dress->CharaId = sett->raceResultCutinMotionChara;
			}
			if (sett->raceResultCutinMotionDress > -1) {
				dress->Id = sett->raceResultCutinMotionDress;
			}
			if (sett->raceResultCutinMotionGrade > -1) {
				grade = sett->raceResultCutinMotionGrade;
			}
			if (sett->raceResultCutinMotionRank > -1) {
				rank = sett->raceResultCutinMotionRank;
			}
		}
		
		return reinterpret_cast<decltype(RaceResultCutInHelper_LoadEarMotion_hook)*>
			(RaceResultCutInHelper_LoadEarMotion_orig)(characterId, dress, personalityType, rank, grade, raceType);
	}

	void* RaceResultCutInHelper_LoadFacialMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadFacialMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {
		printf("RaceResultCutInHelper_LoadFacialMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		//return nullptr;
		if (g_sett->winMotion564) {
			characterId = 1007;
			dress->CharaId = 1007;
			dress->Id = 100701;
			grade = 200;
			//c_raceResultCutinMotionRank = rank;
		}
		else {
			if (sett->raceResultCutinMotionChara > -1) {
				characterId = sett->raceResultCutinMotionChara;
				dress->CharaId = sett->raceResultCutinMotionChara;
			}
			if (sett->raceResultCutinMotionDress > -1) {
				dress->Id = sett->raceResultCutinMotionDress;
			}
			if (sett->raceResultCutinMotionGrade > -1) {
				grade = sett->raceResultCutinMotionGrade;
			}
			if (sett->raceResultCutinMotionRank > -1) {
				rank = sett->raceResultCutinMotionRank;
			}
		}
	
		return reinterpret_cast<decltype(RaceResultCutInHelper_LoadFacialMotion_hook)*>
			(RaceResultCutInHelper_LoadFacialMotion_orig)(characterId, dress, personalityType, rank, grade, raceType);
	}

	void* ResourcePath_GetCharacterRaceResultMotionPath_orig = nullptr;
	Il2CppString* ResourcePath_GetCharacterRaceResultMotionPath_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {				
		printf("ResourcePath_GetCharacterRaceResultMotionPath: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		
		Il2CppString* ret = reinterpret_cast<decltype(ResourcePath_GetCharacterRaceResultMotionPath_hook)*>
			(ResourcePath_GetCharacterRaceResultMotionPath_orig)(characterId, dress, personalityType, rank, grade, raceType);
		
		wprintf(L"ResourcePath_GetCharacterRaceResultMotionPath return=%s", wstring(ret->start_char).c_str());
		return ret;
	}

	void* RaceSkillCutInHelper_PreInstantiateCharaUser_orig = nullptr;
	void RaceSkillCutInHelper_PreInstantiateCharaUser_hook(int charaId, int dressId, int charaIndex, int headId = 0, bool isUseDressDataHeadModelSubId = true) {
		printf("RaceSkillCutInHelper_PreInstantiateCharaUser called charaid=%d\n", charaId);
		return reinterpret_cast<decltype(RaceSkillCutInHelper_PreInstantiateCharaUser_hook)*>
			(RaceSkillCutInHelper_PreInstantiateCharaUser_orig)(charaId, dressId, charaIndex, headId, isUseDressDataHeadModelSubId);
	}

	/*void* RaceSkillCutInHelper_InitForGacha_orig = nullptr;
	void RaceSkillCutInHelper_InitForGacha_hook(void* _this,void* owner) {
		printf("RaceSkillCutInHelper_InitForGacha called\n");

		return reinterpret_cast<decltype(RaceSkillCutInHelper_InitForGacha_hook)*>
			(RaceSkillCutInHelper_InitForGacha_orig)(_this,owner);

	}*/

	void* Gallop_Cutin_CutinCharacter_ctor_orig = nullptr;
	void* Gallop_Cutin_CutinCharacter_ctor_hook(void*_this, CutInCharacterCreateInfo* createInfo) {
		printf("CutinCharacter type=%d charaid=%d dressid=%d, headid=%d, is어쩌구=%d, index=%d\n",
			createInfo->_characterType,createInfo->_charaId,createInfo->_clothId,createInfo->_headId, createInfo->IsUseDressDataHeadModelSubId,createInfo->_charaIndex);
		
		if (g_sett->homeAllDiamond) {
			createInfo->_characterType = (TimelineKeyCharacterType)-1;
			createInfo->_charaId = 1067;
			createInfo->_clothId = 106701;
			createInfo->_headId = 0;
		}
		else {
			if (sett->gachaCharaType > -1) {
				createInfo->_characterType = sett->gachaCharaType;
			}
			if (sett->gachaCutinChara > -1) {
				createInfo->_charaId = sett->gachaCutinChara;
			}
			if (sett->gachaCutinDress > -1) {
				createInfo->_clothId = sett->gachaCutinDress;
			}
			if (sett->gachaCutinHeadid > -1) {
				createInfo->_headId = sett->gachaCutinHeadid;
			}
		}
		
		

		/*if (createInfo->_charaId == 1001) {
			createInfo->_charaId = 1026;
			if (createInfo->_clothId == 901001) {
				createInfo->_clothId = 901026;
			}
			else {
				createInfo->_clothId = 102601;
			}
		}
		else {*/
			//createInfo->_characterType = FixCharaId;
			/*createInfo->_charaId = 1065;
			createInfo->_clothId = 2;
			createInfo->_headId = 0;*/
		//}
		
		//createInfo->IsUseDressDataHeadModelSubId = 1;
		return reinterpret_cast<decltype(Gallop_Cutin_CutinCharacter_ctor_hook)*>
			(Gallop_Cutin_CutinCharacter_ctor_orig)(_this,createInfo);
	}

	

	/*void* CutIn_CutInCharacter_EntryModel_orig = nullptr;
	bool CutIn_CutInCharacter_EntryModel_hook(void* _this, void* model, int type) {
		printf("CutIn_CutInCharacter_EntryModel called type=%d\n", type);
		return reinterpret_cast<decltype(CutIn_CutInCharacter_EntryModel_hook)*>
			(CutIn_CutInCharacter_EntryModel_orig)(_this, model, 3);
	}*/

	void* CutInHelper_OnCreateCharacterModel_orig = nullptr;
	void* CutInHelper_OnCreateCharacterModel_hook(void* _this, CutInCharacterCreateInfo* info) {
		printf("CutInHelper_OnCreateCharacterModel called origType=%d, origcharaid=%d\n",info->_characterType, info->_charaId );

		if (g_sett->homeAllDiamond) {
			info->_characterType = (TimelineKeyCharacterType)-1;
			info->_charaId = 1067;
			info->_clothId = 106701;
			info->_headId = 0;
		}
		else {
			if (sett->gachaCharaType > -1) {
				info->_characterType = sett->gachaCharaType;
			}
			if (sett->gachaCutinChara > -1) {
				info->_charaId = sett->gachaCutinChara;
			}
			if (sett->gachaCutinDress > -1) {
				info->_clothId = sett->gachaCutinDress;
			}
			if (sett->gachaCutinHeadid > -1) {
				info->_headId = sett->gachaCutinHeadid;
			}
		}
		
		printf("CutInHelper_OnCreateCharacterModel called cType=%d, ccharaid=%d\n",  info->_characterType,info->_charaId );
		return reinterpret_cast<decltype(CutInHelper_OnCreateCharacterModel_hook)*>
			(CutInHelper_OnCreateCharacterModel_orig)(_this, info);
	}

	void* CutInHelper_ApplyCharacterMotion_Facial_orig = nullptr;
	void CutInHelper_ApplyCharacterMotion_Facial_hook(void* _this, void* character, void* key, int charaId, int personalityId, int charaTypeRule) {
		printf("CutInHelper_ApplyCharacterMotion_Facial called charaid=%d\n",charaId);
		return reinterpret_cast<decltype(CutInHelper_ApplyCharacterMotion_Facial_hook)*>
			(CutInHelper_ApplyCharacterMotion_Facial_orig)(_this, character,key,charaId,personalityId,charaTypeRule);
	}

	void* CutInHelper_ApplyCharacterMotion_Ear_orig = nullptr;
	void CutInHelper_ApplyCharacterMotion_Ear_hook(void* _this, void* character, void* key, int charaId, int personalityId, int charaTypeRule) {
		printf("CutInHelper_ApplyCharacterMotion_ear called charaid=%d\n", charaId);
		return reinterpret_cast<decltype(CutInHelper_ApplyCharacterMotion_Ear_hook)*>
			(CutInHelper_ApplyCharacterMotion_Ear_orig)(_this, character, key, charaId, personalityId, charaTypeRule);
	}

	void* CutInHelper_GetCharaId_orig = nullptr;
	int CutInHelper_GetCharaId_hook(void* _this, int type, int id) {
		printf("CutInHelper_GetCharaId called id=%d\n", id);
		int res = reinterpret_cast<decltype(CutInHelper_GetCharaId_hook)*>
			(CutInHelper_GetCharaId_orig)(_this, type,id);
		return res;
	}

	void* DialogManager_PushErrorCommon_orig = nullptr;
	void DialogManager_PushErrorCommon_hook(void* _this, Il2CppString* message, Il2CppString* headerMessage, void* onCloseButtonPushed, int popupType = 0) {
		wprintf(L"ErrorDialog Message=%s, popupType=%d\n", wstring(message->start_char).c_str(), popupType);
		return reinterpret_cast<decltype(DialogManager_PushErrorCommon_hook)*>
			(DialogManager_PushErrorCommon_orig)(_this, message, headerMessage, onCloseButtonPushed, popupType);
	}

	void* Live_LiveTimelineCamera_AlterUpdate_orig = nullptr;
	void Live_LiveTimelineCamera_AlterUpdate_hook(void* _this, float liveTime) {
		//printf("Livecam AlterUpdate time=%.5f\n",liveTime);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_LiveTimelineCamera_AlterUpdate_hook)*>
				(Live_LiveTimelineCamera_AlterUpdate_orig)(_this, liveTime);
		}
		
	}

	void* Live_Cutt_AlterUpdate_CameraLookAt_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraLookAt_hook(void* _this, void* sheet, int currentFrame, float currentTime, Vector3_t* outLookAt) {
		if (!sett->stopLiveCam) {
			reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraLookAt_hook)*>
				(Live_Cutt_AlterUpdate_CameraLookAt_orig)(_this, sheet, currentFrame, currentTime,outLookAt);
		}
		liveCam_Lookat = *outLookAt;
		//printf("AlterUpdate_CameraLookAt x=%.2f y=%.2f z=%.2f CurrentFrame=%d\n", outLookAt->x, outLookAt->y, outLookAt->z, currentFrame);
		
	}

	void* Live_Cutt_AlterUpdate_CameraPos_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraPos_hook(void* _this, void* sheet, int currentFrame, float currentTime, int sheetIndex, bool isUseCameraMotion) {
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraPos_hook)*>
				(Live_Cutt_AlterUpdate_CameraPos_orig)(_this, sheet, currentFrame, currentTime, sheetIndex, isUseCameraMotion);
		}
		//printf("AlterUpdate_CameraPos sheetIndex=%d, CurrentFrame=%d\n",sheetIndex, currentFrame);
		
	}

	void* Live_Cutt_AlterUpdate_CameraSwitcher_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraSwitcher_hook(void* _this, void* sheet, int currentFrame) {
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraSwitcher_hook)*>
				(Live_Cutt_AlterUpdate_CameraSwitcher_orig)(_this, sheet, currentFrame);
		}
		//printf("AlterUpdate_CameraSwitcher CurrentFrame=%d\n", currentFrame);
		
	}

	void* Live_Cutt_AlterUpdate_CameraLayer_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraLayer_hook(void* _this, void* sheet, int currentFrame, Vector3_t* offsetMaxPosition, Vector3_t* offsetMinPosition) {
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraLayer_hook)*>
				(Live_Cutt_AlterUpdate_CameraLayer_orig)(_this, sheet, currentFrame,offsetMaxPosition,offsetMinPosition);
		}
		/*printf("AlterUpdate_CameraLayer CurrentFrame=%d minPos x=%.2f y=%.2f z=%.2f maxPos x=%.2f y=%.2f z=%.2f\n", 
			currentFrame,
		offsetMinPosition->x,offsetMinPosition->y,offsetMinPosition->z,
			offsetMaxPosition->x, offsetMaxPosition->y, offsetMaxPosition->z );*/
		
	}

	void* Live_Cutt_AlterUpdate_CameraMotion_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraMotion_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_CameraMotion CurrentFrame=%d\n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraMotion_hook)*>
				(Live_Cutt_AlterUpdate_CameraMotion_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterUpdate_CameraRoll_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraRoll_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_CameraRoll CurrentFrame=%d\n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraRoll_hook)*>
				(Live_Cutt_AlterUpdate_CameraRoll_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterUpdate_MultiCameraSwitcher_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraSwitcher_hook(void* _this, void* sheet, void* curData, void* nextData, int currentFrame, int keyIndex, bool isFading) {
		printf("AlterUpdate_MultiCameraSwitcher CurrentFrame=%d, keyindex=%d\n", currentFrame,keyIndex);
		return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraRoll_hook)*>
			(Live_Cutt_AlterUpdate_CameraRoll_orig)(_this, sheet,currentFrame);
	}

	/*void* Live_Cutt_AlterUpdate_MultiCamera_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCamera_hook(void* _this, void* sheet, int currentFrame) {
		printf("AlterUpdate_MultiCamera CurrentFrame=%d \n", currentFrame);
		return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCamera_hook)*>
			(Live_Cutt_AlterUpdate_MultiCamera_orig)(_this, sheet,currentFrame);
	}*/

	void* Live_Cutt_AlterUpdate_CameraFov_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraFov_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_CameraFov CurrentFrame=%d \n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraFov_hook)*>
				(Live_Cutt_AlterUpdate_CameraFov_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterLateUpdate_CameraMotion_orig = nullptr;
	bool Live_Cutt_AlterLateUpdate_CameraMotion_hook(void* _this, void* sheet, int currentFrame) {
		/*bool ret = reinterpret_cast<decltype(Live_Cutt_AlterLateUpdate_CameraMotion_hook)*>
			(Live_Cutt_AlterLateUpdate_CameraMotion_orig)(_this, sheet, currentFrame);*/

		//printf("AlterLateUpdate_CameraMotion CurrentFrame=%d, ret=%d\n", currentFrame,ret);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterLateUpdate_CameraMotion_hook)*>
				(Live_Cutt_AlterLateUpdate_CameraMotion_orig)(_this, sheet, currentFrame);
		}
		
			return true;
		

	}

	void* Live_Cutt_AlterUpdate_EyeCameraLookAt_orig = nullptr;
	void Live_Cutt_AlterUpdate_EyeCameraLookAt_hook(void* _this, void* sheet, int currentFrame, float currentTime) {
		//printf("AlterUpdate_EyeCameraLookAt CurrentFrame=%d \n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_EyeCameraLookAt_hook)*>
				(Live_Cutt_AlterUpdate_EyeCameraLookAt_orig)(_this, sheet, currentFrame, currentTime);
		}
		
	}

	void* Live_Cutt_AlterUpdate_EyeCameraPosition_orig = nullptr;
	void Live_Cutt_AlterUpdate_EyeCameraPosition_hook(void* _this, void* sheet, int currentFrame, float currentTime) {
		//printf("AlterUpdate_EyeCameraLookAt CurrentFrame=%d \n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_EyeCameraPosition_hook)*>
				(Live_Cutt_AlterUpdate_EyeCameraPosition_orig)(_this, sheet, currentFrame,currentTime);
		}
		
	}

	void* Live_Cutt_AlterUpdate_orig = nullptr;
	void Live_Cutt_AlterUpdate_hook(void* _this, float liveTime) {
		//printf("AlterUpdate time=%.2f \n", liveTime);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_hook)*>
				(Live_Cutt_AlterUpdate_orig)(_this, liveTime);
		}
	}

	void* Live_Cutt_AlterUpdate_MultiCameraLookAt_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraLookAt_hook(void* _this, void* sheet, int currentFrame, float currentTime) {
		//printf("AlterUpdate_MultiCameraLookAt currentFrame=%d \n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCameraLookAt_hook)*>
				(Live_Cutt_AlterUpdate_MultiCameraLookAt_orig)(_this, sheet,currentFrame, currentTime);
		}
	}

	void* Live_Cutt_AlterUpdate_MultiCameraPosition_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraPosition_hook(void* _this, void* sheet, int currentFrame, float currentTime) {
		//printf("AlterUpdate_MultiCameraPosition currentFrame=%d \n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCameraPosition_hook)*>
				(Live_Cutt_AlterUpdate_MultiCameraPosition_orig)(_this, sheet, currentFrame, currentTime);
		}
	}

	void* Live_Cutt_AlterUpdate_MultiCameraTiltShift_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraTiltShift_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_MultiCameraTiltShift currentFrame=%d \n", currentFrame);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCameraTiltShift_hook)*>
				(Live_Cutt_AlterUpdate_MultiCameraTiltShift_orig)(_this, sheet, currentFrame);
		}
	}

	void* Live_Cutt_AlterUpdate_PostEffect_DOF_orig = nullptr;
	void Live_Cutt_AlterUpdate_PostEffect_DOF_hook(void* _this, void* sheet, int currentFrame, Vector3_t* cameraLookAt) {
		
		//printf("AlterUpdate_PostEffect_DOF currentFrame=%d pos x=%.2f y=%.2f z=%.2f \n", currentFrame,
			//cameraLookAt->x,cameraLookAt->y,cameraLookAt->z);
		if (!sett->stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_PostEffect_DOF_hook)*>
				(Live_Cutt_AlterUpdate_PostEffect_DOF_orig)(_this, sheet, currentFrame,cameraLookAt);
		}
	}

	void* Unity_KeyboardEvent_orig = nullptr;
	void* Unity_KeyboardEvent_hook(Il2CppString* key) {
		wprintf(L"KeyEvent %s\n", wstring(key->start_char).c_str());

		return reinterpret_cast<decltype(Unity_KeyboardEvent_hook)*>
			(Unity_KeyboardEvent_orig)(key);
	}

	void* GameSystem_SoftwareReset_orig = nullptr;
	void GameSystem_SoftwareReset_hook(void* _this) {

		printf("Called GameSystem_SoftwareReset\nContinew?: (y, n)");
		char ans = 0;
		scanf_s("%c", &ans,1);
		if (ans == 'y') {
			return reinterpret_cast<decltype(GameSystem_SoftwareReset_hook)*>
				(GameSystem_SoftwareReset_orig)(_this);
		}

	}

	void* GallopUtil_GotoTitleOnError_orig = nullptr;
	void GallopUtil_GotoTitleOnError_hook(Il2CppString* text) {
		wprintf(L"GallopUtil_GotoTitleOnError text=%s\n", wstring(text->start_char).c_str());
		if (g_sett->gotoTitleOnError) {
			return reinterpret_cast<decltype(GallopUtil_GotoTitleOnError_hook)*>
				(GallopUtil_GotoTitleOnError_orig)(text);
		}		
	}

	void* GetKeyDown_orig = nullptr;
	bool GetKeyDown_hook(int key) {
		bool ret = reinterpret_cast<decltype(GetKeyDown_hook)*>
			(GetKeyDown_orig)(key);
		bool ret2 = reinterpret_cast<decltype(GetKeyDown_hook)*>
			(GetKeyDown_orig)(0x73);
		if(ret)
			printf("KeyDown %d=%s\n", key, ret?"true":"false");
		if(ret2)
			printf("KeyDown2 %d=%s\n", 0x73, ret2 ? "true" : "false");
		return ret;
	}

	
	void* UnityEngine_GameObject_ctor_orig = nullptr;
	void UnityEngine_GameObject_ctor_hook(void* _this, Il2CppString* name) {
		reinterpret_cast<decltype(UnityEngine_GameObject_ctor_hook)*>
			(UnityEngine_GameObject_ctor_orig)(_this, name);
		auto getobjectname = reinterpret_cast<Il2CppString * (*)(void*)>(il2cpp_symbols::get_method_pointer("UnityEngine.CoreModule.dll", "UnityEngine", "Object", "ToString", 0));

		wprintf(L"new GameObject name=%s\n", getobjectname(_this)->start_char);
		
		return;
	}

	void* UnityEngine_GameObject_ctor1_orig = nullptr;
	void UnityEngine_GameObject_ctor1_hook(void* _this) {
		reinterpret_cast<decltype(UnityEngine_GameObject_ctor1_hook)*>
			(UnityEngine_GameObject_ctor1_orig)(_this);
		auto getobjectname = reinterpret_cast<Il2CppString * (*)(void*)>(il2cpp_symbols::get_method_pointer("UnityEngine.CoreModule.dll", "UnityEngine", "Object", "ToString", 0));

		wprintf(L"new GameObject name=%s\n", getobjectname(_this)->start_char);

		return;
	}

	void* UnityEngine_GameObject_ctor2_orig = nullptr;
	void UnityEngine_GameObject_ctor2_hook(void* _this, Il2CppString* name,void* components) {
		reinterpret_cast<decltype(UnityEngine_GameObject_ctor2_hook)*>
			(UnityEngine_GameObject_ctor2_orig)(_this, name,components);
		auto getobjectname = reinterpret_cast<Il2CppString * (*)(void*)>(il2cpp_symbols::get_method_pointer("UnityEngine.CoreModule.dll", "UnityEngine", "Object", "ToString", 0));

		wprintf(L"new GameObject name=%s\n", getobjectname(_this)->start_char);

		return;
	}
	void* TapEffectController_ctor_orig = nullptr;
	void TapEffectController_ctor_hook(void* _this) {
		reinterpret_cast<decltype(TapEffectController_ctor_hook)*>
			(TapEffectController_ctor_orig)(_this);
		printf("disabled tapeffect\n");
		TapEffect_Disable(_this);
	}

	void* Cute_Core_Device_GetPersistentDataPath_orig = nullptr;
	Il2CppString* Cute_Core_Device_GetPersistentDataPath_hook() {
		
		Il2CppString* ret = reinterpret_cast<decltype(Cute_Core_Device_GetPersistentDataPath_hook)*>
			(Cute_Core_Device_GetPersistentDataPath_orig)();

		if (strlen(g_sett->customDataPath) > 0) {
			Il2CppString* custompath = il2cpp_string_new(g_sett->customDataPath);
			wprintf(L"originalpersistentpath=%s, replacedpersistentpath=%s\n", ret->start_char, custompath->start_char);
			return custompath;
		}		
		
		return ret;
	}

	void* unityengine_get_persistentDataPath_orig = nullptr;
	Il2CppString* unityengine_get_persistentDataPath_hook() {
		Il2CppString* ret = reinterpret_cast<decltype(unityengine_get_persistentDataPath_hook)*>
			(unityengine_get_persistentDataPath_orig)();

		if (strlen(g_sett->customDataPath) > 0) {
			Il2CppString* custompath = il2cpp_string_new(g_sett->customDataPath);
			wprintf(L"[unityengine_get_persistentDataPath] originalpersistentpath=%s, replacedpersistentpath=%s\n", ret->start_char, custompath->start_char);
			return custompath;
		}

		return ret;
	}

	/*void* assetbundleHelper_IsNeedManifestSetup_orig = nullptr;
	bool assetbundleHelper_IsNeedManifestSetup_hook() {
		bool orig = reinterpret_cast<decltype(assetbundleHelper_IsNeedManifestSetup_hook)*>
			(assetbundleHelper_IsNeedManifestSetup_orig)();

		if (g_skipResourceDownload) {
			printf("SkipResourceDL(IsNeedManifestSetup) ret false, orig=%d\n",orig);
			return false;
		}
		else {
			return orig;
		}

	}

	void* assetbundleHelper_IsExistGallopResources_orig = nullptr;
	bool assetbundleHelper_IsExistGallopResources_hook() {
		bool orig = reinterpret_cast<decltype(assetbundleHelper_IsExistGallopResources_hook)*>
			(assetbundleHelper_IsExistGallopResources_orig)();
		static bool isFirst = true;

		if (g_skipResourceDownload) {
			printf("SkipResourceDL(IsExistGallopResources) ret true, orig=%d\n", orig);
			if (isFirst) {
				isFirst = false;
			}
			else {
				orig = true;
			}
				
		}

		return orig;
	}*/

	//void* Cutin_GenerateCutInModelControllerContext_orig = nullptr;
	//void* Cutin_GenerateCutInModelControllerContext_hook(void* _this, int) {

	//}

	/*void* RaceResultCutInHelper_GetModelController_orig = nullptr;
	void* RaceResultCutInHelper_GetModelController_hook(CutInCharacterCreateInfo* info) {
		info->_characterType = FixCharaId;
		info->_charaId = 1001;
		info->_isWet = 1;
		info->_isDirt = 1;
		printf("GetModelController:type=%d, charaid=%d\n", info->_characterType, info->_charaId);
		return reinterpret_cast<decltype(RaceResultCutInHelper_GetModelController_hook)*>
			(RaceResultCutInHelper_GetModelController_orig)(info);
	}*/

	/*void* RaceResultCutInHelper_GetResultCuttCueId_orig = nullptr;
	bool RaceResultCutInHelper_GetResultCuttCueId_hook(int charaId, int cardId, int* cueId) {
		int cue = 0;
		bool ret = reinterpret_cast<decltype(RaceResultCutInHelper_GetResultCuttCueId_hook)*>
			(RaceResultCutInHelper_GetResultCuttCueId_orig)(charaId,cardId,&cue);
		printf("GetResultCuttCueId:charaId=%d,cardId=%d,cueId=%d\n", charaId, cardId, cue);

		*cueId = cue;
		return ret;
	}*/

	bool (*is_virt)() = nullptr;

	int last_height = 0, last_width = 0;

	void* wndproc_orig = nullptr;
	LRESULT wndproc_hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//printf("wndproc_hook\n");
		//SetWindowText(hWnd,"test");

		if (uMsg == WM_SIZING)
		{
			//printf("WM_SIZING\n");
			RECT* rect = reinterpret_cast<RECT*>(lParam);

			float ratio = is_virt() ? 1.f / sett->aspect_ratio : sett->aspect_ratio;
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

		height = width * sett->aspect_ratio;

		size->x = width;
		size->y = height;
		size->z = sett->aspect_ratio;

		return size;
	}
	
	void* get_hori_size_orig = nullptr;
	Vector3_t* get_hori_size_hook(Vector3_t* pVec3, int width, int height)
	{
		auto size = reinterpret_cast<decltype(get_hori_size_hook)*>(get_hori_size_orig)(pVec3, width, height);

		width = height * sett->aspect_ratio;

		size->x = width;
		size->y = height;
		size->z = sett->aspect_ratio;

		return size;
	}

	Resolution_t* (*get_resolution)(Resolution_t* buffer);

	void get_resolution_stub(Resolution_t* r)
	{
		*r = *get_resolution(r);

		int width = min(r->height, r->width) * sett->aspect_ratio;
		if (r->width > r->height)
			r->width = width;
		else
			r->height = width;
	}

	void* gallop_get_screenheight_orig;
	int gallop_get_screenheight_hook()
	{
		Resolution_t res;
		get_resolution_stub(&res);

		int w = max(res.width, res.height), h = min(res.width, res.height);

		return is_virt() ? w : h;
	}

	void* gallop_get_screenwidth_orig;
	int gallop_get_screenwidth_hook()
	{
		Resolution_t res;
		get_resolution_stub(&res);

		int w = max(res.width, res.height), h = min(res.width, res.height);

		return is_virt() ? h : w;
	}

	void (*set_scale_factor)(void*, float);

	void* canvas_scaler_setres_orig;
	void canvas_scaler_setres_hook(void* _this, Vector2_t res)
	{
		Resolution_t r;
		get_resolution_stub(&r);

		// set scale factor to make ui bigger on hi-res screen
		set_scale_factor(_this, max(1.0f, r.width / 1920.f) * g_sett->uiScale);

		return reinterpret_cast<decltype(canvas_scaler_setres_hook)*>(canvas_scaler_setres_orig)(_this, res);
	}

	void* change_resize_ui_for_pc_orig;
	void change_resize_ui_for_pc_hook(void* _this, int width, int height)
	{
		float scale = g_sett->uiScale;
		Resolution_t r;
		get_resolution_stub(&r);

		if (g_sett->forceLandscape && (g_sett->uiScale > 0.5))
			scale = g_sett->uiScale - 0.5;

		

		reinterpret_cast<decltype(change_resize_ui_for_pc_hook)*>(change_resize_ui_for_pc_orig)(_this, width, height);

		const auto canvasScalerList = UIManager_GetCanvasScalerList(_this);
		il2cpp_symbols::iterate_IEnumerable(canvasScalerList, [&](void* canvasScaler)
			{
				set_scale_factor(canvasScaler, max(1.0f, r.width / 1920.f) * scale);
			});


		
	}

	void (*text_assign_font)(void*);
	int (*text_get_size)(void*);
	void (*text_set_size)(void*, int);
	float (*text_get_linespacing)(void*);
	void (*text_set_style)(void*, int);
	void (*text_set_linespacing)(void*, float);
	void (*gameObject_setActive)(void*,bool);

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
		if (g_sett->autoFullscreen) {
			Resolution_t r;
			r = *get_resolution(&r);

			bool need_fullscreen = false;

			if (is_virt() && r.width / static_cast<double>(r.height) == (9.0 / 16.0))
				need_fullscreen = true;
			else if (!is_virt() && r.width / static_cast<double>(r.height) == (16.0 / 9.0))
				need_fullscreen = true;



			if (need_fullscreen) {
				if (g_sett->useExclusiveFullScreen) {
					if (g_sett->exclusiveFullScreenWidth > 0 && g_sett->exclusiveFullScreenHeight > 0) {

						setExclusiveFullScreen(g_sett->exclusiveFullScreenWidth, g_sett->exclusiveFullScreenHeight, FullScreenMode::ExclusiveFullScreen, g_sett->maxFps);
						Sleep(1000);
					}
					else {

						setExclusiveFullScreen(r.width, r.height, FullScreenMode::ExclusiveFullScreen, g_sett->maxFps);
						Sleep(1000);
					}

				}
				else {

					return reinterpret_cast<decltype(set_resolution_hook)*>(set_resolution_orig)(
						r.width, r.height, true
						);


				}
			}
			else {

				return reinterpret_cast<decltype(set_resolution_hook)*>(set_resolution_orig)(
					width, height, false
					);


			}
		}
		else {
			return reinterpret_cast<decltype(set_resolution_hook)*>(set_resolution_orig)(
				width, height, false
				);
		}
		
		
		
		//setExclusiveFullScreen(need_fullscreen ? 3840 : width, need_fullscreen ? 2160 : height, screenmode, currentRefreshRate);
		
	}

	//void set_resolution_hook() {

	//}

	void adjust_size(int w, int h)
	{
		thread([&]() {
			auto tr = il2cpp_thread_attach(il2cpp_domain_get());

			Resolution_t r;
			r = *get_resolution(&r);

			auto target_height = r.height - 100;
			if (w > 0 && h > 0) {
				set_resolution_hook(h * 0.5625f, h, false);
			}
			else {
				set_resolution_hook(target_height * 0.5625f, target_height, false);
			}
			

			il2cpp_thread_detach(tr);
		}).detach();
	}

	void* load_scene_internal_orig = nullptr;
	void* load_scene_internal_hook(Il2CppString* sceneName, int sceneBuildIndex, void* parameters, bool mustCompleteNextFrame)
	{
		wprintf(L"%s\n", sceneName->start_char);
		return reinterpret_cast<decltype(load_scene_internal_hook)*>(load_scene_internal_orig)(sceneName, sceneBuildIndex, parameters, mustCompleteNextFrame);
	}

	void* set_virt_orig = nullptr;
	void* set_virt_hook(bool val) {
		printf("set_virt %d\n", val);
		return reinterpret_cast<decltype(set_virt_hook)*>(set_virt_orig)(true);
	}

	void* textutil_getmastertext_orig = nullptr;
	Il2CppString* textutil_getmastertext_hook(int category, int index) {
		auto ret_orig = reinterpret_cast<decltype(textutil_getmastertext_hook)*>(textutil_getmastertext_orig)(category, index);
		if (isKimuraChallenge) {
			string test = "키무라";
			return il2cpp_string_new(test.data());
			//Il2CppString* t = GetCharaNameByCharaId(1002);
			//return il2cpp_string_new_utf16(t->start_char,t->length);
			//return GetCharaNameByCharaId(1002);
		}		
		return ret_orig;
	}

	void* textutil_getstatictext_orig = nullptr;
	Il2CppString* textutil_getstatictext_hook(int textid) {
		auto ret_orig = reinterpret_cast<decltype(textutil_getstatictext_hook)*>(textutil_getstatictext_orig)(textid);
		if (isKimuraChallenge) {
			string test = "키무라";
			return il2cpp_string_new(test.data());
		}
		
		//wprintf(L"GetStaticText Textid%d, str=%s\n", textid, ret_orig->start_char);
		return ret_orig;
	}

	void* cyan_localfile_pathresolver_getlocalpath_orig = nullptr;
	Il2CppString* cyan_localfile_pathresolver_getlocalpath_hook(void* _this, int type, Il2CppString* hname) {
		Il2CppString* ret = reinterpret_cast<decltype(cyan_localfile_pathresolver_getlocalpath_hook)*>(cyan_localfile_pathresolver_getlocalpath_orig)(_this, type, hname);

		wprintf(L"GetLocalPath hname=%s, ret=%s\n", hname->start_char, ret->start_char);
		return ret;
	}

	void* MasterEventMotionData_GetAnimCommand_orig = nullptr;
	Il2CppObject* MasterEventMotionData_GetAnimCommand_hook(Il2CppObject* _this, Il2CppString* commandName) {
		wstring orig = commandName->start_char;
		Il2CppObject* ret = nullptr;
		if (g_sett->walkMotionAllUrara) {
			if (orig.starts_with(L"homewalk")) {
				if (orig.contains(L"_U")) {
					orig = L"homewalk03_U";
				}
				else if (orig.contains(L"_D")) {
					orig = L"homewalk03_D";
				}
				else {
					orig = L"homewalk03";
				}
				printf("Replaced walk motion to urara\n");
			}
			
			ret = reinterpret_cast<decltype(MasterEventMotionData_GetAnimCommand_hook)*>
				(MasterEventMotionData_GetAnimCommand_orig)(_this, il2cpp_string_new_utf16(orig.data(), orig.length()));
		}
		else {
			ret = reinterpret_cast<decltype(MasterEventMotionData_GetAnimCommand_hook)*>
				(MasterEventMotionData_GetAnimCommand_orig)(_this, commandName);
		}
		return ret;
	}

	void* ModelController_GetCardId_orig = nullptr;
	int ModelController_GetCardId_hook(Il2CppObject* _this) {
		int ret = reinterpret_cast<decltype(ModelController_GetCardId_hook)*>
			(ModelController_GetCardId_orig)(_this);
		auto gen = std::bind(std::uniform_int_distribution<>(0, 1), std::default_random_engine());
		if (g_sett->winMotion564) {
			bool b = gen();
			int ret_edit = b ? 100701 : 100702;
			printf("GetCardid ret=%d, replace=%d\n", ret, ret_edit);
			ret = ret_edit;
		}
		else if(sett->cardid > -1) {
			ret = sett->cardid;
			printf("GetCardid ret=%d, replace=%d\n", ret,sett->cardid);
		}
		return ret;
	}

	//void* CutInHelper_InstantiateTimeline_orig = nullptr;
	//bool CutInHelper_InstantiateTimeline_hook(Il2CppObject* _instance, Il2CppString* path, void* parent, void* camera,
	//	Il2CppString* preInstantiateKey, int requireType = 0, int cuttIndex = 0) {
	//	wprintf(L"called InstantiateTimeline path=%s, key=%s, requireType=%d, cuttindex=%d\n", path->start_char, preInstantiateKey->start_char, requireType, cuttIndex);

	//	return reinterpret_cast<decltype(CutInHelper_InstantiateTimeline_hook)*>
	//		(CutInHelper_InstantiateTimeline_orig)(_instance, path, parent, camera, preInstantiateKey, requireType, cuttIndex);
	//}


	void* user_orig = nullptr;

	void* user_hook(void* _this, CutInCharacterCreateInfo* info)
	{
		//printf("Something Pass User! \n");

		/*
		if (info->_characterType == 3 and info->_charaIndex == 0) {
			info->_charaId = info->_charaId;
		}
		else {
			info->_charaId = 1056;
			info->_clothId = 105601;
			info->_headId = 0;
		}
		*/

		info->_charaId = 1019;
		info->_clothId = 901019;
		info->_headId = 0;


		/*printf("Data is %p \n", info->_characterType);
		printf("Data is %p \n", info->_charaIndex);
		printf("Data is %p \n", info->_charaId);
		printf("Data is %p \n", info->_clothId);
		printf("Data is %p \n", info->_headId);
		printf("Data is %p \n", info->_zekkenNo);
		printf("Data is %p \n", info->OverrideClothCategory);
		printf("Data is %p \n", info->IsUseDressDataHeadModelSubId);*/




		void* ret = reinterpret_cast<decltype(user_hook)*>(user_orig)(_this, info);

		//spring_hook(ret, 100);

		//printf("User End! \n");

		return ret;

	}

	void* chara_orig = nullptr;

	void chara_hook(TimelineCharacter* _this, int currentFrame)
	{
		//printf("Something Pass Chara! \n");

		////_this->_data->_characterKeys->_selectCharaId = -1;

		//printf("Data is %p \n", currentFrame);

		//printf("Data is %p \n", _this->_data);
		//printf("Data is %p \n", _this->_data->_characterKeys);
		//printf("Data is %p \n", _this->_data->_characterKeys->_selectCharaId);
		//printf("Data is %p \n", _this->_data->_characterKeys->_selectClothId);
		//printf("Data is %p \n", _this->_data->_characterKeys->_selectHeadId);

		//printf("Data is %p \n", _this->_selectCharaId);
		//printf("Data is %p \n", _this->_selectClothId);
		//printf("Data is %p \n", _this->_selectHeadId);
		printf("_this->_data->_chara->_selectCharaId=%d\n", _this->_data->_chara->_selectCharaId);
		printf("_charaIndex %d\n", _this->_charaIndex);
		printf("CharacterPartsNum %d\n", _this->_data->CharacterPartsNum);
		printf("CharacterSpringCollisionNum %d\n", _this->_data->CharacterSpringCollisionNum);
		printf("CharacterWindNum %d\n", _this->_data->CharacterWindNum);
		printf("_characterCollisionNum %d\n", _this->_data->_characterCollisionNum);
		printf("_characterIKNum %d\n", _this->_data->_characterIKNum);

		_this->_data->_chara->_selectCharaId = sett->gachaCutinChara;
		_this->_data->_chara->_selectClothId = sett->gachaCutinDress;
		_this->_data->_chara->_selectHeadId = sett->gachaCutinHeadid;
		_this->_data->_characterKeys->_selectCharaId = sett->gachaCutinChara;
		_this->_data->_characterKeys->_selectClothId = sett->gachaCutinDress;
		_this->_data->_characterKeys->_selectHeadId = sett->gachaCutinHeadid;
		_this->_selectCharaId = sett->gachaCutinChara;
		_this->_selectClothId = sett->gachaCutinDress;
		_this->_selectHeadId = sett->gachaCutinHeadid;
		//_this->_data->CharacterWindNum = 0;
		

		reinterpret_cast<decltype(chara_hook)*>(chara_orig)(_this, currentFrame);

		//printf("Work Completed!\n");

		return;

	}

	void* CutInCharacter_GenerateCutInModelControllerContext_orig = nullptr;
	void* CutInCharacter_GenerateCutInModelControllerContext_hook(Il2CppObject* _this, int cardId, int charaId, int clothId, int headId, 
		bool isUseDressDataHeadModelSubId, void* controller, bool isDirt, bool isWet, 
		int overrideClothCategory = 0) {
		printf("\n!====Called GenerateCutInModelControllerContext==\n");
		printf("cardid=%d\n", cardId);
		printf("charaId=%d\n", charaId);
		printf("clothId=%d\n", clothId);
		printf("headId=%d\n", headId);
		printf("isUseDressDataHeadModelSubId=%d\n", isUseDressDataHeadModelSubId);
		printf("isDirt=%d\n", isDirt);
		printf("isWet=%d\n", isWet);
		printf("overrideClothCategory=%d\n", overrideClothCategory);

		charaId = sett->gachaCutinChara;
		clothId = sett->gachaCutinDress;
		headId = sett->gachaCutinHeadid;
		isUseDressDataHeadModelSubId = false;
		

		printf("\n====GenerateCutInModelControllerContext End==\n");
		return reinterpret_cast<decltype(CutInCharacter_GenerateCutInModelControllerContext_hook)*>(CutInCharacter_GenerateCutInModelControllerContext_orig)
			(_this,cardId,charaId,clothId,headId,isUseDressDataHeadModelSubId,controller,isDirt,isWet,overrideClothCategory);
	}

	void* CutInCharacter_CreateModel_orig = nullptr;
	bool CutInCharacter_CreateModel_hook(Il2CppObject* _this, CutInCharacterCreateInfo* info, int controllerType) {
		printf("\n!!$$%% ====Called CutInCharacter_CreateModel==\n");
		printf(" info->_characterType=%d\n", info->_characterType);
		printf("info->_charaId=%d\n", info->_charaId);
		printf("info->_clothId=%d\n", info->_clothId);
		printf("info->_headId=%d\n", info->_headId);
		printf("info->isUseDressDataHeadModelSubId=%d\n", info->IsUseDressDataHeadModelSubId);
		printf("controllerType=%d\n", controllerType);
		printf("info->=%d\n",info->_isWet);
		printf("info->overrideClothCategory=%d\n", info->OverrideClothCategory);
		printf("\n!!====Called CutInCharacter_CreateModel end==\n");
		info->OverrideClothCategory = 2;
		//controllerType = 0;
		return reinterpret_cast<decltype(CutInCharacter_CreateModel_hook)*>(CutInCharacter_CreateModel_orig)
			(_this, info,controllerType);
	}

	void* RaceSkillCutInHelper_Owner_CreateAddChara_orig = nullptr;
	int RaceSkillCutInHelper_Owner_CreateAddChara_hook(Il2CppObject* _this) {
		printf("\n!====Called RaceSkillCutInHelper+Owner.get_CharaId==\n");
		int ret = reinterpret_cast<decltype(RaceSkillCutInHelper_Owner_CreateAddChara_hook)*>(RaceSkillCutInHelper_Owner_CreateAddChara_orig)
			(_this);
		printf("ret=%d\,", ret);
		//printf("cardid=%d\n", cardId);
		//printf("charaId=%d\n", charaId);
		//printf("dressId=%d\n", dressId);
		//printf("headId=%d\n", headId);
		//printf("isUseDressDataHeadModelSubId=%d\n", isUseDressDataHeadModelSubId);
		//charaId = 1007;
		//dressId = 100701;
		//headId = 0;
		return ret;
	}

	void* get_camera_pos_orig;
	Vector3_t* get_camera_pos_hook(void* _this, Il2CppObject* timelineControl) {
		auto pos = reinterpret_cast<decltype(get_camera_pos_hook)*>(get_camera_pos_orig)(_this, timelineControl);
		liveCam_pos1.x = pos->x;
		liveCam_pos1.y = pos->y;
		liveCam_pos1.z = pos->z;
		return pos;
	}

	void* get_camera_pos2_orig;  
	Vector3_t* get_camera_pos2_hook(void* _this, Il2CppObject* timelineControl, void* type) {
		auto pos = reinterpret_cast<decltype(get_camera_pos2_hook)*>(get_camera_pos2_orig)(_this, timelineControl, type);
		liveCam_pos2 = *pos;
		return pos;
	}

	void* ChangeScreenOrientation_orig = nullptr;

	Il2CppObject* ChangeScreenOrientation_hook(ScreenOrientation targetOrientation, bool isForce) {
		return reinterpret_cast<decltype(ChangeScreenOrientation_hook)*>(ChangeScreenOrientation_orig)(
			g_sett->forceLandscape ? ScreenOrientation::Landscape : targetOrientation, isForce);
	}

	void* Screen_set_orientation_orig = nullptr;

	void Screen_set_orientation_hook(ScreenOrientation orientation) {
		if ((orientation == ScreenOrientation::Portrait ||
			orientation == ScreenOrientation::PortraitUpsideDown) && g_sett->forceLandscape) {
			orientation = ScreenOrientation::Landscape;
		}
		reinterpret_cast<decltype(Screen_set_orientation_hook)*>(Screen_set_orientation_orig)(
			orientation);
	}

	void* EyeHighlightController_ctor_orig = nullptr;
	void EyeHighlightController_ctor_hook(Il2CppObject* _this, EyeHighlightController_context* context) {
		//context->KeyLocator->_highlightType = -1;
		//context = nullptr;
		
		//Il2CppClass* klass = (Il2CppClass*)_this->klass;
		//auto RemoveHighlightLocator = reinterpret_cast<void (*)(Il2CppObject * _Instance)>(il2cpp_class_get_method_from_name((Il2CppClass*)klass, "RemoveHighlightLocator", 0)->methodPointer);

		//RemoveHighlightLocator(_this);
		reinterpret_cast<decltype(EyeHighlightController_ctor_hook)*>(EyeHighlightController_ctor_orig)(
			_this, context);
		//context->KeyLocator->_highlightType = -1;
		//context->KeyLocator->IsEnable = false;
		//printf("_highlightType=%d, IsEnable=%d\n", context->KeyLocator->_highlightType, context->KeyLocator->IsEnable);
		return;
	}

	void* EyeHighlightController_AfterUpdate_orig = nullptr;
	void EyeHighlightController_AfterUpdate_hook(Il2CppObject* _this) {
		printf("Called EyeHighlightController.AfterUpdate()\n");
	}

	void* EyeHighlightController_set_PupliScale_orig = nullptr;
	void EyeHighlightController_set_PupliScale_hook(Il2CppObject* _this, float value) {
		printf("Called EyeHighlightController.set_PupliScale value=%.5f\n",value);
		reinterpret_cast<decltype(EyeHighlightController_set_PupliScale_hook)*>(EyeHighlightController_set_PupliScale_orig)(
			_this, value);
	}

	void* LiveModelController_UpdateEyeReflectionController_orig = nullptr;
	void LiveModelController_UpdateEyeReflectionController_hook(LiveModelController_EyeReflectionContext* context) {
		printf("Called UpdateEyeReflectionController \n");
		printf("CullingMask=%d\n", context->CullingMask);
		printf("FarClipPlane=%.3f\n", context->FarClipPlane);
		printf("FieldOfView=%.3f\n", context->FieldOfView);
		printf("IsEnabled=%d\n", context->IsEnabled);
		printf("MaskTexture=%p\n", context->MaskTexture);
		printf("CullingMask=%.3f\n", context->NearClipPlane);
		printf("Power=%.3f\n", context->Power);
		printf("Roll=%.3f\n", context->Roll);
		//context->IsEnabled = true;
		//context.
		//context->Power = 1.0;
		return reinterpret_cast<decltype(LiveModelController_UpdateEyeReflectionController_hook)*>
			(LiveModelController_UpdateEyeReflectionController_orig)( context);
	}

	void* EyeReflectionController_set_ReflectionPower_orig = nullptr;
	void EyeReflectionController_set_ReflectionPower_hook(Il2CppObject* _this, float value) {
		printf("Called EyeReflectionController.set_ReflectionPower value=%.5f\n", value);
		return reinterpret_cast<decltype(EyeReflectionController_set_ReflectionPower_hook)*>(EyeReflectionController_set_ReflectionPower_orig)(
			_this, value);
	}

	void* EyeReflectionController_ctor_orig = nullptr;
	void EyeReflectionController_ctor_hook(Il2CppObject* _this, void* context) {
		printf("Called EyeReflectionController_ctor \n");
		
		reinterpret_cast<decltype(EyeReflectionController_ctor_hook)*>(EyeReflectionController_ctor_orig)(
			_this,context);
		EyeReflectionController_set_ReflectionPower_hook(_this, 0.0);
		return;
	}

	void* ModelController_ctor_orig = nullptr;
	void ModelController_ctor_hook(Il2CppObject* _Instance) {
		printf("Called ModelController_ctor\n");
		reinterpret_cast<decltype(ModelController_ctor_hook)*>(ModelController_ctor_orig)(
			_Instance);
		auto AddEyeReflection = reinterpret_cast<EyeReflectionController* (*)(Il2CppObject * _Instance)>(il2cpp_class_get_method_from_name(_Instance->klass, "AddEyeReflectionComponent", 0)->methodPointer);
		EyeReflectionController* eye = AddEyeReflection(_Instance);
		eye->_isEyeReflection = true;
		return;
	}

	//Graphics

	/*void* set_vsync_count_orig = nullptr;
	void set_vsync_count_hook(int value) {
		printf("setVsyncCount: %d -> %d\n", value, g_vsync_count);
		return reinterpret_cast<decltype(set_vsync_count_hook)*>(set_vsync_count_orig)(g_vsync_count == -1 ? value : g_vsync_count);
	}*/

	void* set_antialiasing_orig = nullptr;
	void set_antialiasing_hook(int value) {
		printf("setAntialiasing: %d -> %d\n", value, sett->antialiasing);
		//set_vsync_count_hook(1);
		return reinterpret_cast<decltype(set_antialiasing_hook)*>(set_antialiasing_orig)(!g_sett->highQuality ? value : sett->antialiasing);
	}

	void* graphics_quality_orig = nullptr;
	void graphics_quality_hook(Il2CppObject* thisObj, int quality, bool force) {
		printf("setGraphicsQuality: %d -> %d\n", quality, sett->graphics_quality);
		return reinterpret_cast<decltype(graphics_quality_hook)*>(graphics_quality_orig)(thisObj,
			sett->graphics_quality == -1 ? quality : sett->graphics_quality,
			true);
	}

	void* set_RenderTextureAntiAliasing_orig;
	void set_RenderTextureAntiAliasing_hook(void* _this, int value) {
		return reinterpret_cast<decltype(set_RenderTextureAntiAliasing_hook)*>(set_RenderTextureAntiAliasing_orig)(_this,
			!g_sett->highQuality ? value : sett->antialiasing);
	}

	void* Get3DAntiAliasingLevel_orig;
	int Get3DAntiAliasingLevel_hook(void* _this, bool allowMSAA) {
		if (g_sett->highQuality) allowMSAA = true;
		auto data = reinterpret_cast<decltype(Get3DAntiAliasingLevel_hook)*>(Get3DAntiAliasingLevel_orig)(_this, allowMSAA);
		printf("Get3DAntiAliasingLevel: %d %d\n", allowMSAA, data);
		return data;
	}

	void* ProcessMouseEvent_orig;
	void ProcessMouseEvent_hook(void* _this, int id) {
		if(!imgui_settingwnd_open)
			return reinterpret_cast<decltype(ProcessMouseEvent_hook)*>(ProcessMouseEvent_orig)(_this, id);;
	}

	//void* BootSystem_Awake_orig = nullptr;

	//void BootSystem_Awake_hook(Il2CppObject* _this) {
	//	printf("Boot_Awake runned\n");
	//	if (g_force_landscape) {
	//		ChangeScreenOrientation_hook(ScreenOrientation::Landscape, true);
	//		auto enumerator1 = reinterpret_cast<Il2CppObject * (*)()>(il2cpp_symbols::get_method_pointer(
	//			"umamusume.dll",
	//			"Gallop",
	//			"Screen", "ChangeScreenOrientationLandscapeAsync", -1))();
	//		auto move_next1 = reinterpret_cast<void* (*)(
	//			Il2CppObject * _this)>(il2cpp_class_get_method_from_name(enumerator1->klass,
	//				"MoveNext",
	//				0)->methodPointer);
	//		move_next1(enumerator1);
	//		//printf("Boot_Awake runned\n");
	//		//landscape();
	//	}
	//	
	//	reinterpret_cast<decltype(BootSystem_Awake_hook)*>(BootSystem_Awake_orig)(_this);
	//}

	void* Gallop_SceneManager_LoadScene_orig = nullptr;
	void* Gallop_SceneManager_LoadScene_hook(void* _this, int sceneId) {
		currSceneID = sceneId;
		printf("LoadScene id=%d\n",sceneId);
		//
		if (g_sett->forceLandscape && (sceneId == 1) ) {
			
			auto enumerator1 = reinterpret_cast<Il2CppObject * (*)()>(il2cpp_symbols::get_method_pointer(
				"umamusume.dll",
				"Gallop",
				"Screen", "ChangeScreenOrientationLandscapeAsync", -1))();
			auto move_next1 = reinterpret_cast<void* (*)(
				Il2CppObject * _this)>(il2cpp_class_get_method_from_name(enumerator1->klass,
					"MoveNext",
					0)->methodPointer);
			move_next1(enumerator1);

			Screen_set_orientation_hook(ScreenOrientation::Landscape);
			ChangeScreenOrientation_hook(ScreenOrientation::Landscape, true);
			//printf("Boot_Awake runned\n");
			//landscape();
		}
		return reinterpret_cast<decltype(Gallop_SceneManager_LoadScene_hook)*>(Gallop_SceneManager_LoadScene_orig)(_this, sceneId);
	}
	
	void* ResourcePath_GetRaceResultCuttPath_orig = nullptr;
	Il2CppString* ResourcePath_GetRaceResultCuttPath_hook(int charaId, int subId, int cardId, int rank, int grade, int raceType) {
		if (sett->raceResultCutinMotionChara > -1) {
			charaId = sett->raceResultCutinMotionChara;
			// dress->CharaId = sett->raceResultCutinMotionChara;
		}
		if (sett->cardid > -1) {
			cardId = sett->cardid;
		}
		if (sett->raceResultCutinMotionGrade > -1) {
			grade = sett->raceResultCutinMotionGrade;
		}
		if (sett->raceResultCutinMotionRank > -1) {
			rank = sett->raceResultCutinMotionRank;
		}
		printf("GetRaceResultCuttPath charaid=%d, subid=%d, cardid=%d,rank=%d, grade=%d, raceType=%d\n",charaId,subId,cardId,rank,grade,raceType);
		return reinterpret_cast<decltype(ResourcePath_GetRaceResultCuttPath_hook)*>(ResourcePath_GetRaceResultCuttPath_orig)(charaId,subId,cardId,rank,grade,raceType);
	}

	void* RaceUIFinishOrderFlash_Play_orig = nullptr;
	void RaceUIFinishOrderFlash_Play_hook(Il2CppObject* _instance, int finishOrder) {
		
		if (showFinishOrderFlash) {
			printf("RaceUIFinishOrderFlash_Play finishOrder=%d\n", finishOrder);
			return reinterpret_cast<decltype(RaceUIFinishOrderFlash_Play_hook)*>(RaceUIFinishOrderFlash_Play_orig)(_instance, finishOrder);
		}
		else {
			printf("RaceUIFinishOrderFlash_Play ignored\n");
		}
			
	}

	void* Gallop_CutInModelController_CreateModel_orig = nullptr;
	void* Gallop_CutInModelController_CreateModel_hook(CutInModelController_Context* context) {
		printf("Called Gallop_CutInModelController_CreateModel\n");
		printf("charid=%d\n", context->CharaId);
		printf("cardid=%d\n", context->CardId);
		printf("DressID=%d\n", context->DressId);
		printf("Controllertype=%d\n", context->ControllerType);
		printf("overrideClothCategory=%d\n", context->_overrideClothCategory);

		/*Il2CppObject* EditableCharinfo = il2cpp_object_new((Il2CppClass*)il2cpp_symbols::get_class("umamusume.dll", "Gallop", "EditableCharacterBuildInfo"));
		
		printf("il2cppobject_new %p\n", EditableCharinfo->klass);

		auto ctor_2 = reinterpret_cast<void (*)
			(Il2CppObject * _instance, int cardId, int charaId, int dressId, int controllerType, int zekken, int mobId, int backDancerColorId, int headId, bool isUseDressDataHeadModelSubId, bool isEnableModelCache)>(il2cpp_class_get_method_from_name((Il2CppClass*)EditableCharinfo->klass, ".ctor",10)->methodPointer);

		printf("il2cppobject_new ctor_2 = %p\n", &ctor_2);

		ctor_2(EditableCharinfo, 100101, 1001, 100101, (int)context->ControllerType , 0, 0, -1, 0, true, true);

		printf("%p\n", EditableCharinfo);

		return EditableCharinfo->klass;*/
		
		//context->_overrideClothCategory = CySpringDataContainer::Category::Training;
		if (sett->changeStoryChar) {
			if ((sett->story3dCharID < 0) || (sett->story3dClothID < 0) || (sett->story3dHeadID < 0))
			{
				std::string line;
				while (true) {
					try {


						std::cout << "Enter charaid, clothid, headid, cardid" << line << "\n";

						std::getline(std::cin, line);

						//std::cout <<  "Entered: " << line;
						std::vector < std::string > arg = explode(line, ' ');


						int _charaId = std::stoi(arg.at(0).c_str());
						int _dressId = std::stoi(arg.at(1).c_str());
						int _headid = std::stoi(arg.at(2).c_str());
						int _cardid = std::stoi(arg.at(3).c_str());

						context->CharaId = _charaId;
						context->DressId = _dressId;
						context->HeadId = _headid;
						context->CardId = _cardid;

						printf("CutInModelController.Context set manual %d %d %d %d\n", context->CharaId, context->DressId, context->HeadId,context->CardId);
						break;
					}
					catch (std::invalid_argument&) {
						printf("Value Error: please enter number only\n");
					}
					catch (std::out_of_range&) {
						std::cout << "You entered " << line << "\n";
						printf_s("Argument Error: please enter {CharID} {Dress id} {Head ID} {Card id}\n");
					}
				}



				//printf("Enter charaid, clothid, headid, mobid: ");

				//scanf_s("%d %d %d %d", &charaId, &clothId, &headId, &mobId);
				printf("\n");
			}
			else
			{
				context->CharaId = sett->story3dCharID;
				context->DressId = sett->story3dClothID;
				context->HeadId = sett->story3dHeadID;
				printf("CutInModelController.Context set %d %d %d\n", context->CharaId, context->DressId, context->HeadId);
			}
		}
		//context->CharaId = 9004;
		return reinterpret_cast<decltype(Gallop_CutInModelController_CreateModel_hook)*>(Gallop_CutInModelController_CreateModel_orig)(context);
	}
	float oldLiveTime = 0.0;
	void* Live_Cutt_LiveTimelineControl_AlterUpdate_orig = nullptr;
	void Live_Cutt_LiveTimelineControl_AlterUpdate_hook(void* _instance, float liveTime) {
		
		float target_frame_time = 1000.0f / static_cast<float>(liveTimeLineFPS);
		float remaining_time = target_frame_time - liveTime;

		if (sett->isLiveTimeManual) {
			liveTime = liveTimeSec;
		}
		else {
			liveTimeSec = liveTime;
		}
		if (liveTimeLineFPS > 0) {
			
		/*	if (remaining_time > 0) {
				return reinterpret_cast<decltype(Live_Cutt_LiveTimelineControl_AlterUpdate_hook)*>(Live_Cutt_LiveTimelineControl_AlterUpdate_orig)(_instance, liveTime);

			}*/
			if ((liveTime - oldLiveTime) >= (1.0f / static_cast<float>(liveTimeLineFPS))) {
				oldLiveTime = liveTime;
			}
			liveTime = oldLiveTime;
		}
		
		oldLiveTime = liveTime;
		return reinterpret_cast<decltype(Live_Cutt_LiveTimelineControl_AlterUpdate_hook)*>(Live_Cutt_LiveTimelineControl_AlterUpdate_orig)(_instance, liveTime);
	}

	void* Live_Director_get_LiveTotalTime_orig = nullptr;
	float Live_Director_get_LiveTotalTime_hook(Il2CppObject* _instance) {
		float ret = reinterpret_cast<decltype(Live_Director_get_LiveTotalTime_hook)*>(Live_Director_get_LiveTotalTime_orig)(_instance);
		liveTotalTimeSec = ret;
		if (sett->isLiveTimeManual) {
			ret = 9999.9;
		}
		return ret;
	}



	/*void* Gallop_EditableCharacterBuildInfo_ctor_overload2_orig = nullptr;
	void* Gallop_EditableCharacterBuildInfo_ctor_overload2_hook(Il2CppObject* _instance,
		int cardId, int charaId, int dressId, int controllerType, int zekken = 0, int mobId = 0,
		int backDancerColorId = -1, int headId = 0, bool isUseDressDataHeadModelSubId = true, bool isEnableModelCache = true) {

		printf("************************************\n");
		printf("*        Function Parameters        *\n");
		printf("************************************\n");
		printf("cardId: %d\n", cardId);
		printf("charaId: %d\n", charaId);
		printf("dressId: %d\n", dressId);
		printf("controllerType: %d\n", controllerType);
		printf("zekken: %d\n", zekken);
		printf("mobId: %d\n", mobId);
		printf("backDancerColorId: %d\n", backDancerColorId);
		printf("headId: %d\n", headId);
		printf("isUseDressDataHeadModelSubId: %d\n", isUseDressDataHeadModelSubId);
		printf("isEnableModelCache: %d\n", isEnableModelCache);
		printf("************************************\n");

		return reinterpret_cast<decltype(Gallop_EditableCharacterBuildInfo_ctor_overload2_hook)*>(Gallop_EditableCharacterBuildInfo_ctor_overload2_orig)
			(_instance, cardId, charaId ,dressId, controllerType, zekken, mobId, backDancerColorId, headId, isUseDressDataHeadModelSubId, isEnableModelCache);
	}*/



	/*void* Gallop_CharacterBuildPathInfo_ctor_orig = nullptr;
	void Gallop_CharacterBuildPathInfo_ctor_hook(Il2CppObject* _instance, 
		CharacterBuildInfo* charBuildInfo, MasterDressData* dress) {

		printf("----Gallop_CharacterBuildPathInfo_ctor_hook start----\n");
		
		printf("_cardId: %d\n", charBuildInfo->_cardId);
		printf("_charaId: %d\n", charBuildInfo->_charaId);
		printf("_mobId: %d\n", charBuildInfo->_mobId);
		printf("_headModelSubId: %d\n", charBuildInfo->_headModelSubId);
		printf("_isUseDressDataHeadModelSubId: %d\n", charBuildInfo->_isUseDressDataHeadModelSubId);
		printf("_bodyModelSubId: %d\n", charBuildInfo->_bodyModelSubId);
		printf("_dressId: %d\n", charBuildInfo->_dressId);
		printf("_controllerType: %d\n", charBuildInfo->_controllerType);
		printf("_zekken: %d\n", charBuildInfo->_zekken);
		wprintf(L"_name: %p\n", charBuildInfo->_name->start_char);
		printf("_genderType: %d\n", charBuildInfo->_genderType);
		printf("_heightType: %d\n", charBuildInfo->_heightType);
		printf("_bodySize: %d\n", charBuildInfo->_bodySize);
		printf("_bustType: %d\n", charBuildInfo->_bustType);
		printf("_height: %f\n", charBuildInfo->_height);
		printf("_skinType: %d\n", charBuildInfo->_skinType);
		printf("_socksType: %d\n", charBuildInfo->_socksType);
		printf("_defaultPersonalityType: %d\n", charBuildInfo->_defaultPersonalityType);
		printf("_raceGateInPersonalityType: %d\n", charBuildInfo->_raceGateInPersonalityType);
		printf("_raceRunPersonalityType: %d\n", charBuildInfo->_raceRunPersonalityType);
		printf("_raceOverRunPersonalityType: %d\n", charBuildInfo->_raceOverRunPersonalityType);
		printf("_raceRunningType: %d\n", charBuildInfo->_raceRunningType);
		printf("_zekkenColor: %d\n", charBuildInfo->_zekkenColor);
		printf("_zekkenFontColor: %d\n", charBuildInfo->_zekkenFontColor);
		printf("_zekkenFontStyle: %d\n", charBuildInfo->_zekkenFontStyle);
		printf("_wetTextureArray: %p\n", charBuildInfo->_wetTextureArray);
		printf("_dirtTextureArray: %p\n", charBuildInfo->_dirtTextureArray);
		printf("_sweatLocator: %p\n", charBuildInfo->_sweatLocator);
		printf("_sweatObject: %p\n", charBuildInfo->_sweatObject);
		printf("_frameColor: %d\n", charBuildInfo->_frameColor);
		printf("_popularity: %d\n", charBuildInfo->_popularity);
		printf("_npcType: %d\n", charBuildInfo->_npcType);
		printf("_charaBuildPathInfo: %p\n", charBuildInfo->_charaBuildPathInfo);
		printf("_clothBuildPathInfo: %p\n", charBuildInfo->_clothBuildPathInfo);
		printf("_dressElement: %p\n", charBuildInfo->_dressElement);
		printf("_backDancerColorId: %d\n", charBuildInfo->_backDancerColorId);
		printf("_mobInfo: %p\n", &charBuildInfo->_mobInfo);
		printf("_isPersonalDress: %d\n", charBuildInfo->_isPersonalDress);
		printf("_miniMobTailId: %d\n", charBuildInfo->_miniMobTailId);
		printf("_miniMobParentCharaId: %d\n", charBuildInfo->_miniMobParentCharaId);
		printf("_overrideClothCategory: %d\n", charBuildInfo->_overrideClothCategory);
		printf("_loadHashKey: %d\n", charBuildInfo->_loadHashKey);
		printf("_isPreCreatedLoadHashKey: %d\n", charBuildInfo->_isPreCreatedLoadHashKey);
		printf("_initialized: %d\n", charBuildInfo->_initialized);

		printf("----MasterDressData dress----\n");
		printf("Id: %d\n", dress->Id);
		printf("ConditionType: %d\n", dress->ConditionType);
		printf("HaveMini: %d\n", dress->HaveMini);
		printf("GeneralPurpose: %d\n", dress->GeneralPurpose);
		printf("CharaId: %d\n", dress->CharaId);
		printf("UseGender: %d\n", dress->UseGender);
		printf("BodyType: %d\n", dress->BodyType);
		printf("BodyTypeSub: %d\n", dress->BodyTypeSub);
		printf("BodySetting: %d\n", dress->BodySetting);
		printf("UseRace: %d\n", dress->UseRace);
		printf("UseLive: %d\n", dress->UseLive);
		printf("UseLiveTheater: %d\n", dress->UseLiveTheater);
		printf("UseHome: %d\n", dress->UseHome);
		printf("IsWet: %d\n", dress->IsWet);
		printf("IsDirt: %d\n", dress->IsDirt);
		printf("HeadSubId: %d\n", dress->HeadSubId);
		printf("UseSeason: %d\n", dress->UseSeason);
		printf("DressColorMain: %p\n", dress->DressColorMain);
		printf("DressColorSub: %p\n", dress->DressColorSub);
		printf("ColorNum: %d\n", dress->ColorNum);
		printf("DispOrder: %d\n", dress->DispOrder);
		printf("TailModelId: %d\n", dress->TailModelId);
		printf("TailModelSubId: %d\n", dress->TailModelSubId);
		printf("StartTime: %lld\n", dress->StartTime);
		printf("EndTime: %lld\n", dress->EndTime);
		printf("_getCondition: %d\n", dress->_getCondition);

		printf("----Gallop_CharacterBuildPathInfo_ctor_hook End----\n");

		charBuildInfo->_isUseDressDataHeadModelSubId = 0;
		charBuildInfo->_cardId = 100101;
		charBuildInfo->_charaId = 1001;
		charBuildInfo->_headModelSubId = 0;
		charBuildInfo->_dressId = 100101;
		charBuildInfo->_zekken = 100101;
		dress->Id = 100101;
		dress->CharaId = 1001;
		dress->UseGender = 1001;
		return reinterpret_cast<decltype(Gallop_CharacterBuildPathInfo_ctor_hook)*>(Gallop_CharacterBuildPathInfo_ctor_orig)
			(_instance, charBuildInfo, dress);
	}*/



	//void* Live_Cutt_LiveTimelineWorkSheet_cctor_orig = nullptr;
	//void Live_Cutt_LiveTimelineWorkSheet_cctor_hook(LiveTimelineWorkSheet* _instance) {
	//	//Il2CppClass* klass = (Il2CppClass*)_instance->klass;
	//	//printf("name %s\n", klass->name);
	//	
	//	//FieldInfo* field = il2cpp_class_get_field_from_name((Il2CppClass*)_instance->klass , "TotalTimeLength");
	//	//field
	//	
	//	printf("TotalTime=%.4f\n", _instance->TotalTimeLength);
	//	
	//	return reinterpret_cast<decltype(Live_Cutt_LiveTimelineWorkSheet_cctor_hook)*>(Live_Cutt_LiveTimelineWorkSheet_cctor_orig)(_instance);
	//}

	/*void* unity_font_ctor_orig = nullptr;
	void* unity_font_ctor_hook(void* _this, Il2CppString* name) {
		wprintf(L"Font Load: %s\n", name->start_char);
		return reinterpret_cast<decltype(unity_font_ctor_hook)*>(unity_font_ctor_orig)(_this, name);
	}*/

	void dump_all_entries()
	{
		// 0 is None
		for (int i = 1;; i++)
		{
			auto* str = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(i);

			if (str && *str->start_char)
			{
				logger::write_entry(i, str->start_char);
			}
			else
			{
				// check next string, if it's still empty, then we are done!
				auto* nextStr = reinterpret_cast<decltype(localize_get_hook)*>(localize_get_orig)(i + 1);
				if (!(nextStr && *nextStr->start_char))
					break;
			}
		}
	}

	void path_game_assembly()
	{
		if (!mh_inited)
			return;

		printf("Trying to patch GameAssembly.dll...\n");

		auto il2cpp_module = GetModuleHandle(L"GameAssembly.dll");

		// load il2cpp exported functions
		printf("Il2cpp Init\n");
		il2cpp_symbols::init(il2cpp_module);

#pragma region HOOK_MACRO
#define ADD_HOOK(_name_, _fmt_) \
	auto _name_##_offset = reinterpret_cast<void*>(_name_##_addr); \
	\
	printf(_fmt_, _name_##_offset); \
	dump_bytes(_name_##_offset); \
	\
	MH_CreateHook(_name_##_offset, _name_##_hook, &_name_##_orig); \
	MH_EnableHook(_name_##_offset); 
#pragma endregion
#pragma region HOOK_ADDRESSES

		auto assetbundleHelper_IsExistGallopResources_addr = reinterpret_cast<bool(*)()> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"AssetBundleHelper", "IsExistGallopResources", 0
			)
		);

		auto assetbundleHelper_IsNeedManifestSetup_addr = reinterpret_cast<bool(*)()> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"AssetBundleHelper", "IsNeedManifestSetup", 0
			)
		);

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
			"Query", "_Setup", 2
		);

		sql_query = reinterpret_cast<void(*)(Connection*,Il2CppString*)>(query_ctor_addr);

		sql_exec = reinterpret_cast<bool(*)()>(il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "Exec", 0
		));


		auto query_getstr_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "GetText", 1
		);

		auto query_dispose_addr = il2cpp_symbols::get_method_pointer(
			"LibNative.Runtime.dll", "LibNative.Sqlite3",
			"Query", "Dispose", 0
		);

		auto set_fps_addr =il2cpp_resolve_icall("UnityEngine.Application::set_targetFrameRate(System.Int32)");
			/* il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Application", "set_targetFrameRate", 1
		);*/

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

		auto set_virt_addr = reinterpret_cast<void(*)(bool)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"StandaloneWindowResize", "set_IsVirt", 1
			));

		auto textutil_getmastertext_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"TextUtil", "GetMasterText", 2
		);

		auto textutil_getstatictext_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"TextUtil", "GetStaticText", 1
		);

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

		UIManager_GetCanvasScalerList = reinterpret_cast<decltype(UIManager_GetCanvasScalerList)>(il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"UIManager", "GetCanvasScalerList", -1
		));

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

		setExclusiveFullScreen = reinterpret_cast<void(*)(int,int,int,int)>(
				il2cpp_symbols::get_method_pointer(
					"UnityEngine.CoreModule.dll", "UnityEngine",
					"Screen", "SetResolution", 4
				)
			);

		/*auto live_cam_GetCullingMask_addr = reinterpret_cast<unsigned int(*)(unsigned int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live.Cutt",
				"LiveCameraCullingLayer_Helper", "GetCullingMask", 1
			));
		
		auto live_CameraLookAt_UpdateCamera_addr = reinterpret_cast<void(*)(void*)>(
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
		);*/

		auto AssetBundleHelper_GetResourceVer_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"AssetBundleHelper", "GetResourceVer", 0
		);
		
		auto GachaBGController_OverrideRarity_addr = reinterpret_cast<void(*)(void*, int, int)>(
			il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GachaBGController", "OverrideRarity", 2
		));

		
		auto GachaBGController_SetGateDoorRarity_addr = reinterpret_cast<void(*)(void*,int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"GachaBGController", "SetGateDoorRarity", 1
			));

	/*	auto LiveTitleController_Setup_addr = reinterpret_cast<void(*)(void*, LiveData*,bool)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live",
				"LiveTitleController", "Setup", 2
			));
		auto LiveTitleController_FadeIn_addr = reinterpret_cast<void(*)(void*,float,float)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live",
				"LiveTitleController", "FadeIn", 2
			)
			);

		auto LiveTitleController_FadeOut_addr = reinterpret_cast<void(*)(void*, float, float)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live",
				"LiveTitleController", "FadeOut", 2
			));

		auto LiveTitleController_AlterUpdate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live",
			"LiveTitleController", "AlterUpdate", 0
		);*/

		auto FadeGui_SetAlpha_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live",
			"FadeGui", "SetAlpha", 1
		);

	/*	auto Unity_Post_addr = reinterpret_cast<void(*)(Il2CppString*,void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.UnityWebRequestModule.dll", "UnityEngine.Networking",
				"UnityWebRequest", "Post", 2
			));*/

		auto RaceUI_SetRaceUIActive_addr = reinterpret_cast<void(*)(void*,bool)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceUI", "SetRaceUIActive", 1
			));

		auto RaceUI_SetVisibleRank_addr = reinterpret_cast<void(*)(void*, bool)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceUI", "SetVisibleRank", 1
			));

		/*auto RaceUIRank_Setup_addr = reinterpret_cast<void(*)(void*,int,int,void*,void*,int,float,float)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceUIRank", "Setup", 7
			));*/

		auto RaceUIRank_PlayPlayerRankDown_addr = reinterpret_cast<void(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceUIRank", "PlayPlayerRankDown", 0
			));

		auto RaceUIRank_PlayPlayerRankUp_addr = reinterpret_cast<void(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceUIRank", "PlayPlayerRankUp", 0
			));

		auto RaceManager_GetHorseDistanceByIndex_addr = reinterpret_cast<void(*)(void*,int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceManager", "GetHorseDistanceByIndex", 1
			));

		auto LiveTheaterInfo_GetDefaultDressid_addr = reinterpret_cast<void(*)(void*,int,int,bool,int*,int*,int*,int*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"LiveTheaterInfo", "GetDefaultDressId", 7
			));

		auto LiveTheaterInfo_UpdateCharaDressIds_addr = reinterpret_cast<void(*)(void*, void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"LiveTheaterInfo", "UpdateCharaDressIds", 1
			));

		auto LiveTheaterInfo_CheckDress_addr = reinterpret_cast<void(*)(void*,int ,CharaDressIdSet*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"LiveTheaterInfo", "CheckDress", 2
			));

		auto BitmapTextCommon_GetFontPath_addr = reinterpret_cast<void(*)(void*,void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"BitmapTextCommon", "GetFontPath", 1
			));

		auto ModelLoader_CreateNormalModel_addr = reinterpret_cast<void*(*)(void*,CharacterBuildInfo* )>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"ModelLoader", "CreateNormalModel", 1
			));

		ModelLoader_CreateMiniModel = reinterpret_cast<void* (*)(void*, CharacterBuildInfo*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"ModelLoader", "CreateMiniModel", 1
			));

		auto ModelLoader_CreateModel_addr = reinterpret_cast<void*(*)(void*, CharacterBuildInfo*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"ModelLoader", "CreateModel", 1
			));

		ObscuredInt_Decrypted = reinterpret_cast<int(*)(ObscuredInt*)>(
			il2cpp_symbols::get_method_pointer(
				"Plugins.dll", "CodeStage.AntiCheat.ObscuredTypes",
				"ObscuredInt", "op_Implicit", 1
			));

		//auto load_scene_internal_addr = il2cpp_resolve_icall("UnityEngine.SceneManagement.SceneManager::LoadSceneAsyncNameIndexInternal_Injected(System.String,System.Int32,UnityEngine.SceneManagement.LoadSceneParameters&,System.Boolean)");

		//printf("icall %p\n", load_scene_internal_addr);

		auto Cute_Http_WWWRequest_Post_addr = il2cpp_symbols::get_method_pointer(
			"Cute.Http.Assembly.dll", "Cute.Http",
			"WWWRequest", "Post", 3
		);

		auto RaceResultScene_GetMotionVariationId_addr = reinterpret_cast<int(*)(int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultScene", "GetMotionVariationId", 1
			)
			);
		auto RaceResultScene_PlayFinishOrderAnim_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"RaceResultScene", "PlayFinishOrderAnim", 1
		);

		auto RaceResultCutInHelper_LoadBodyMotion_addr = reinterpret_cast<void*(*)(int,MasterDressData*,int,int,int,int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "LoadBodyMotion", 8
			)
		);


		auto RaceResultCutInHelper_LoadCameraMotion_addr = reinterpret_cast<void* (*)(int, MasterDressData*, int, int, int, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "LoadCameraMotion", 8
			)
		);

		auto RaceResultCutInHelper_LoadEarMotion_addr = reinterpret_cast<void* (*)(int, MasterDressData*, int, int, int, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "LoadEarMotion", 6
			)
			);

		auto RaceResultCutInHelper_LoadFacialMotion_addr = reinterpret_cast<void* (*)(int, MasterDressData*, int, int, int, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "LoadFacialMotion", 6
			)
			);

		auto ResourcePath_GetCharacterRaceResultMotionPath_addr = reinterpret_cast<Il2CppString* (*)(int, MasterDressData*, int, int, int, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "LoadFacialMotion", 6
			)
			);

		auto RaceSkillCutInHelper_PreInstantiateCharaUser_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"RaceSkillCutInHelper", "PreInstantiateCharaUser", 6
		);

		/*auto RaceSkillCutInHelper_InitForGacha_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"RaceSkillCutInHelper", "InitForGacha", 1
		);*/

		auto Gallop_Cutin_CutinCharacter_ctor_addr = reinterpret_cast<void(*)(void*, CutInCharacterCreateInfo*) > (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.CutIn",
				"CutInCharacter", ".ctor", 1
			)
		);

	/*	auto CutIn_CutInCharacter_EntryModel_addr = reinterpret_cast<bool(*)(void*, void*, int)> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.CutIn",
				"CutInCharacter", "EntryModel", 2
			)
		);*/

		auto CutInHelper_OnCreateCharacterModel_addr = reinterpret_cast<void(*)(void*, CutInCharacterCreateInfo*)> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"CutInHelper", "OnCreateCharacterModel", 1
			)
		);

		auto CutInHelper_ApplyCharacterMotion_Facial_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CutInHelper", "ApplyCharacterMotion_Facial", 5
		);

		auto CutInHelper_ApplyCharacterMotion_Ear_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CutInHelper", "ApplyCharacterMotion_Ear", 5
		);

		auto CutInHelper_GetCharaId_addr = reinterpret_cast<int(*)(void*, int,int)> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"CutInHelper", "GetCharaId", 2
			)
		);

		auto CharacterBuildInfo_ctor_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CharacterBuildInfo", ".ctor", 9+2
		);

		auto CharacterBuildInfo_ctor_overload1_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CharacterBuildInfo", ".ctor", 0
		);

		auto CharacterBuildInfo_ctor_overload2_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CharacterBuildInfo", ".ctor", 11+2
		);

		auto StoryCharacter3D_LoadModel_addr = reinterpret_cast<void(*)(int, int,int,int,int,bool,bool,int,int,Il2CppString*,int,int,int,int,bool,bool)> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"StoryCharacter3D", "LoadModel", 16
			)
		);

		auto DialogManager_PushErrorCommon_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll","Gallop",
			"DialogManager","PushErrorCommon",4
		);

		auto Live_LiveTimelineCamera_AlterUpdate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live",
			"LiveTimelineCamera", "AlterUpdate", 1
		);

		auto Live_Cutt_AlterUpdate_CameraLookAt_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraLookAt", 4
		);

		auto Live_Cutt_AlterUpdate_CameraPos_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraPos", 5
		);

		auto Live_Cutt_AlterUpdate_CameraSwitcher_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraSwitcher", 2
		);

		auto Live_Cutt_AlterUpdate_CameraLayer_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraLayer", 4
		);

		auto Live_Cutt_AlterUpdate_CameraMotion_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraMotion", 2
		);

		auto Live_Cutt_AlterUpdate_CameraRoll_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraRoll", 2
		);

		auto Live_Cutt_AlterUpdate_MultiCameraSwitcher_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCameraSwitcher", 6
		);

		/*auto Live_Cutt_AlterUpdate_MultiCamera_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCamera", 2
		);*/

		auto Live_Cutt_AlterUpdate_EyeCameraPosition_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_EyeCameraPosition", 3
		);

		auto Live_Cutt_AlterUpdate_EyeCameraLookAt_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_EyeCameraLookAt", 3
		);

		auto Live_Cutt_AlterLateUpdate_CameraMotion_addr = reinterpret_cast<bool(*)(void*, void*, int)> (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live.Cutt",
				"LiveTimelineControl", "AlterLateUpdate_CameraMotion", 2
			)
		);

		auto Live_Cutt_AlterUpdate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate", 1
		);
		auto Live_Cutt_AlterUpdate_MultiCameraLookAt_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCameraLookAt", 3
		);

		auto Live_Cutt_AlterUpdate_MultiCameraPosition_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCameraPosition", 3
		);

		auto Live_Cutt_AlterUpdate_MultiCameraTiltShift_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCameraTiltShift", 2
		);

		auto Live_Cutt_AlterUpdate_PostEffect_DOF_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_PostEffect_DOF", 3
		);

		auto Live_Cutt_AlterUpdate_CameraFov_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraFov", 2
		); 

		auto  GameSystem_SoftwareReset_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GameSystem", "SoftwareReset", 0
		);


		auto GallopUtil_GotoTitleOnError_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GallopUtil", "GotoTitleOnError", 1
		);

		auto UnityEngine_GameObject_ctor_addr = reinterpret_cast<void(*)(void*, Il2CppString*)>(
			il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"GameObject", ".ctor", 1
			)
		);

		auto UnityEngine_GameObject_ctor1_addr = reinterpret_cast<void(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"GameObject", ".ctor", 0
			)
		);

		auto UnityEngine_GameObject_ctor2_addr = reinterpret_cast<void(*)(void*, Il2CppString*, void*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"GameObject", ".ctor", 2
			)
		);

		gameObject_setActive = reinterpret_cast<void(*)(void*, bool)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"GameObject", "SetActive", 1
				)
			);




		GetKeyDown = reinterpret_cast<bool(*)(KeyCode)> (
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.InputLegacyModule.dll", "UnityEngine",
				"Input", "GetKeyDown", 1
			)
		);

		GetKey = reinterpret_cast<bool(*)(KeyCode)> (
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.InputLegacyModule.dll", "UnityEngine",
				"Input", "GetKey", 1
			)
		);

		TapEffect_Disable = reinterpret_cast<void(*)(void*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"TapEffectController", "Disable", 0
			)
			);

		auto TapEffectController_ctor_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"TapEffectController", ".ctor", 0
		);

		auto Cute_Core_Device_GetPersistentDataPath_addr = reinterpret_cast<Il2CppString* (*)()>(
			il2cpp_symbols::get_method_pointer(
			"Cute.Core.Assembly.dll", "Cute.Core",
			"Device", "GetPersistentDataPath", 0
		));

		auto apply_graphics_quality_addr = reinterpret_cast<void (*)(
			Il2CppObject*, int, bool)>(il2cpp_symbols::get_method_pointer(
				"umamusume.dll",
				"Gallop",
				"GraphicSettings", "ApplyGraphicsQuality", 2)
				);
		
		auto unityengine_get_persistentDataPath_addr = reinterpret_cast<Il2CppString*(*)()>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"Application", "get_persistentDataPath", 0
			));

		LoadScene = reinterpret_cast<void(*)(Il2CppString*, LoadSceneParameters*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine.SceneManagement",
				"SceneManager", "LoadScene", 2
			));

		set_TimeScale = reinterpret_cast<void(*)(float)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"Time", "set_timeScale", 1
			));

		get_TimeScale = reinterpret_cast<float(*)()>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"Time", "get_timeScale", 0
			));

		auto MasterEventMotionData_GetAnimCommand_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"MasterEventMotionData", "GetAnimCommand", 1
		);

		auto ModelController_GetCardId_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"ModelController", "GetCardId", 0
		);

		GetCharaNameByCharaId = reinterpret_cast<Il2CppString * (*)(int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"MasterDataUtil", "GetCharaNameByCharaId", 1
			)
		);

		auto s2c_addr = il2cpp_symbols::get_method_pointer(
			"mscorlib.dll", "System",
			"String", "ToCharArray", 0
		);

		MH_CreateHook((LPVOID)s2c_addr, s2c_hook, &s2c_orig);
		MH_EnableHook((LPVOID)s2c_addr);


		/*auto CutInHelper_InstantiateTimeline_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CutInHelper", "InstantiateTimeline", 6
		);

		printf("onplayer_addr at %p\n", CutInHelper_InstantiateTimeline_addr);


		MH_CreateHook((LPVOID)CutInHelper_InstantiateTimeline_addr, CutInHelper_InstantiateTimeline_hook, &CutInHelper_InstantiateTimeline_orig);
		MH_EnableHook((LPVOID)CutInHelper_InstantiateTimeline_addr);*/

		/*auto user_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CutInOwner", "GetUserModel", 1, 3646
		);

		printf("user_addr at %p\n", user_addr);

		MH_CreateHook((LPVOID)user_addr, user_hook, &user_orig);
		MH_EnableHook((LPVOID)user_addr);*/

		/*auto chara_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.CutIn.Cutt",
			"TimelineCharacter", "CreateCharacterModel", 1
		);*/

		//printf("chara_addr at %p\n", chara_addr);

		/*MH_CreateHook((LPVOID)chara_addr, chara_hook, &chara_orig);
		MH_EnableHook((LPVOID)chara_addr);*/

		/*auto CutInCharacter_GenerateCutInModelControllerContext_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.CutIn",
			"CutInCharacter", "GenerateCutInModelControllerContext", 9 
		); 

		MH_CreateHook((LPVOID)CutInCharacter_GenerateCutInModelControllerContext_addr, CutInCharacter_GenerateCutInModelControllerContext_hook, &CutInCharacter_GenerateCutInModelControllerContext_orig);
		MH_EnableHook((LPVOID)CutInCharacter_GenerateCutInModelControllerContext_addr);*/

		auto CutInCharacter_CreateModel_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.CutIn",
			"CutInCharacter", "CreateModel", 2
		);
		MH_CreateHook((LPVOID)CutInCharacter_CreateModel_addr, CutInCharacter_CreateModel_hook, &CutInCharacter_CreateModel_orig);
		MH_EnableHook((LPVOID)CutInCharacter_CreateModel_addr);

		auto Gallop_CutInModelController_CreateModel_addr = reinterpret_cast<void*(*)(CutInModelController_Context*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"CutInModelController", "CreateModel", 1
			)
			);
		MH_CreateHook((LPVOID)Gallop_CutInModelController_CreateModel_addr, Gallop_CutInModelController_CreateModel_hook, &Gallop_CutInModelController_CreateModel_orig);
		MH_EnableHook((LPVOID)Gallop_CutInModelController_CreateModel_addr);


		auto RaceSkillCutInHelper_class = il2cpp_symbols::get_class("umamusume.dll", "Gallop", "RaceSkillCutInHelper");
		auto RaceSkillCutInHelper_Owner_class = il2cpp_symbols::find_nested_class_from_name(RaceSkillCutInHelper_class, "Owner");
		auto RaceSkillCutInHelper_Owner_CreateAddChara_addr = reinterpret_cast<int * (*)()>(
			il2cpp_class_get_method_from_name(
			RaceSkillCutInHelper_Owner_class,
			"set_CharaId", 1
				)->methodPointer
			);


		auto get_camera_pos_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineKeyCameraPositionData", "GetValue", 1
		);

		auto get_camera_pos2_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineKeyCameraPositionData", "GetValue", 2
		);

		/*auto alterupdate_camera_lookat_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraLookAt", 3
		);*/

		auto ChangeScreenOrientation_addr = reinterpret_cast<void (*)(
			ScreenOrientation, bool)>(il2cpp_symbols::get_method_pointer(
				"umamusume.dll",
				"Gallop",
				"Screen", "ChangeScreenOrientation", 2));

		auto Screen_set_orientation_addr = reinterpret_cast<void (*)(
			ScreenOrientation)>(il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll",
				"UnityEngine",
				"Screen", "set_orientation", 1));

		auto EyeHighlightController_ctor_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop.Model.Component",
			"EyeHighlightController", ".ctor", 1);

		/*MH_CreateHook((LPVOID)EyeHighlightController_ctor_addr, EyeHighlightController_ctor_hook, &EyeHighlightController_ctor_orig);
		MH_EnableHook((LPVOID)EyeHighlightController_ctor_addr);*/


		auto EyeHighlightController_AfterUpdate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop.Model.Component",
			"EyeHighlightController", "AfterUpdate", 0);

		//MH_CreateHook((LPVOID)EyeHighlightController_AfterUpdate_addr, EyeHighlightController_AfterUpdate_hook, &EyeHighlightController_AfterUpdate_orig);
		//MH_EnableHook((LPVOID)EyeHighlightController_AfterUpdate_addr);

		auto EyeHighlightController_set_PupliScale_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop.Model.Component",
			"EyeHighlightController", "set_PupliScale", 1);
		/*MH_CreateHook((LPVOID)EyeHighlightController_set_PupliScale_addr, EyeHighlightController_set_PupliScale_hook, &EyeHighlightController_set_PupliScale_orig);
		MH_EnableHook((LPVOID)EyeHighlightController_set_PupliScale_addr);*/

		auto EyeReflectionController_set_ReflectionPower_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop.Model.Component",
			"EyeReflectionController", "set_ReflectionPower", 1);

		/*MH_CreateHook((LPVOID)EyeReflectionController_set_ReflectionPower_addr,
			EyeReflectionController_set_ReflectionPower_hook, &EyeReflectionController_set_ReflectionPower_orig);
		MH_EnableHook((LPVOID)EyeReflectionController_set_ReflectionPower_addr);*/

		auto EyeReflectionController_ctor_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop.Model.Component",
			"EyeReflectionController", ".ctor", 1);

		MH_CreateHook((LPVOID)EyeReflectionController_ctor_addr,
			EyeReflectionController_ctor_hook, &EyeReflectionController_ctor_orig);
		MH_EnableHook((LPVOID)EyeReflectionController_ctor_addr);


		auto ModelController_ctor_addr =  il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop",
			"ModelController", ".ctor", 0);

		MH_CreateHook((LPVOID)ModelController_ctor_addr,
			ModelController_ctor_hook, &ModelController_ctor_orig);
		MH_EnableHook((LPVOID)ModelController_ctor_addr);

		auto set_antialiasing_addr = il2cpp_resolve_icall("UnityEngine.QualitySettings::set_antiAliasing(System.Int32)");
		/*il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"QualitySettings", "set_antiAliasing", 1
		);*/

		auto graphics_quality_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GraphicSettings", "ApplyGraphicsQuality", 2);

		/*auto set_vsync_count_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"QualitySettings", "set_vSyncCount", 1
		);*/
		auto set_RenderTextureAntiAliasing_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"RenderTexture", "set_antiAliasing", 1
		);

		auto Get3DAntiAliasingLevel_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"GraphicSettings", "Get3DAntiAliasingLevel", 1
		);

		auto ProcessMouseEvent_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "UnityEngine.EventSystems",
			"GallopStandaloneInputModule", "ProcessMouseEvent", 1
		);

		auto Gallop_SceneManager_LoadScene_addr = reinterpret_cast<void (*)(int)>(il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"SceneManager", "LoadScene", 1
		));

		auto ResourcePath_GetRaceResultCuttPath_addr = reinterpret_cast<Il2CppString* (*)(int,int,int,int,int,int)>(il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"ResourcePath", "GetRaceResultCuttPath", 6
		));	

		MH_CreateHook((LPVOID)ResourcePath_GetRaceResultCuttPath_addr,
			ResourcePath_GetRaceResultCuttPath_hook, &ResourcePath_GetRaceResultCuttPath_orig);
		MH_EnableHook((LPVOID)ResourcePath_GetRaceResultCuttPath_addr);

		

		auto Live_Cutt_LiveTimelineControl_AlterUpdate_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate", 1
		);

		MH_CreateHook((LPVOID)Live_Cutt_LiveTimelineControl_AlterUpdate_addr,
			Live_Cutt_LiveTimelineControl_AlterUpdate_hook, &Live_Cutt_LiveTimelineControl_AlterUpdate_orig);
		MH_EnableHook((LPVOID)Live_Cutt_LiveTimelineControl_AlterUpdate_addr);


		auto Live_Director_get_LiveTotalTime_addr = reinterpret_cast<float(*)()>(il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live",
			"Director", "get_LiveTotalTime", 0
		));

		MH_CreateHook((LPVOID)Live_Director_get_LiveTotalTime_addr,
			Live_Director_get_LiveTotalTime_hook, &Live_Director_get_LiveTotalTime_orig);
		MH_EnableHook((LPVOID)Live_Director_get_LiveTotalTime_addr);

		GameObject_Find = reinterpret_cast<void*(*)(Il2CppString*)>(il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll","UnityEngine",
			"GameObject","Find",1
		));

		


		/*auto Gallop_EditableCharacterBuildInfo_ctor_overload2_addr = reinterpret_cast<float(*)()>(il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"EditableCharacterBuildInfo", ".ctor", 10
		));*/

		/*MH_CreateHook((LPVOID)Gallop_EditableCharacterBuildInfo_ctor_overload2_addr,
			Gallop_EditableCharacterBuildInfo_ctor_overload2_hook, &Gallop_EditableCharacterBuildInfo_ctor_overload2_orig);
		MH_EnableHook((LPVOID)Gallop_EditableCharacterBuildInfo_ctor_overload2_addr);*/

		/*auto Gallop_CharacterBuildPathInfo_ctor_addr = reinterpret_cast<float(*)()>(il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"CharacterBuildPathInfo", ".ctor", 2
		));

		MH_CreateHook((LPVOID)Gallop_CharacterBuildPathInfo_ctor_addr,
			Gallop_CharacterBuildPathInfo_ctor_hook, &Gallop_CharacterBuildPathInfo_ctor_orig);
		MH_EnableHook((LPVOID)Gallop_CharacterBuildPathInfo_ctor_addr);*/

		/*auto  Live_Cutt_LiveTimelineWorkSheet_cctor_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineWorkSheet", ".cctor", 0
		);

		MH_CreateHook((LPVOID)Live_Cutt_LiveTimelineWorkSheet_cctor_addr,
			Live_Cutt_LiveTimelineWorkSheet_cctor_hook, &Live_Cutt_LiveTimelineWorkSheet_cctor_orig);
		MH_EnableHook((LPVOID)Live_Cutt_LiveTimelineWorkSheet_cctor_addr);*/


		/*auto RaceUIFinishOrderFlash_Play_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"RaceUIFinishOrderFlash", "Play", 1
		);
		
		MH_CreateHook((LPVOID)RaceUIFinishOrderFlash_Play_addr,
			RaceUIFinishOrderFlash_Play_hook, &RaceUIFinishOrderFlash_Play_orig);
		MH_EnableHook((LPVOID)RaceUIFinishOrderFlash_Play_addr);*/

		/*auto LiveModelController_UpdateEyeReflectionController_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll",
			"Gallop",
			"LiveModelController", "UpdateEyeReflectionController", 1);

		MH_CreateHook((LPVOID)LiveModelController_UpdateEyeReflectionController_addr, 
			LiveModelController_UpdateEyeReflectionController_hook, &LiveModelController_UpdateEyeReflectionController_orig);
		MH_EnableHook((LPVOID)LiveModelController_UpdateEyeReflectionController_addr);*/

		//MH_CreateHook((LPVOID)RaceSkillCutInHelper_Owner_CreateAddChara_addr, RaceSkillCutInHelper_Owner_CreateAddChara_hook, &RaceSkillCutInHelper_Owner_CreateAddChara_orig);
		//MH_EnableHook((LPVOID)RaceSkillCutInHelper_Owner_CreateAddChara_addr);
		
		/*auto cyan_localfile_pathresolver_getlocalpath_addr = il2cpp_symbols::get_method_pointer(
			"_Cyan.dll", "Cyan.LocalFile",
			"PathResolver", "GetLocalPath", 2
		);*/

		/*auto unity_font_ctor_addr = reinterpret_cast<void *(*)(void* , Il2CppString*)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.TextRenderingModule.dll", "UnityEngine",
				"Font", ".ctor", 1
			));*/

		/*auto Cutin_GenerateCutInModelControllerContext_addr = reinterpret_cast<void(*)(int, int, int, int, bool, int, bool,bool,int) > (
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "CutIn.CutIn",
				"CutInCharacter", "GenerateCutInModelControllerContext", 9
				)
			);*/

		/*auto Unity_KeyboardEvent_addr = reinterpret_cast<bool(*)(void*, void*, int)> (
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.IMGUIModule.dll", "UnityEngine",
				"Event", "KeyboardEvent", 1
			)
			);*/
		/*auto Live_Cutt_AlterUpdate_MultiCameraTiltShift_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCameraTiltShift", 2
		);*/

		/*auto RaceResultCutInHelper_GetModelController_addr = reinterpret_cast<void*(*)(CutInCharacterCreateInfo*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "GetModelController", 1
			)
			);*/

		/*auto RaceResultCutInHelper_GetResultCuttCueId_addr = reinterpret_cast<bool(*)(int, int,int*)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "GetResultCuttCueId", 3
			)
			);*/

	//auto GachaBGController_GateDoor_SetRarity_addr = il2cpp_resolve_icall("Gallop.GachaBGController$GateDoor::SetRarity(Gallop.GachaDefine.GachaRarityType_Door)");

		/*auto Race_GetRankNumSmallPath_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.AtlasSpritePath",
			"Race", "GetRankNumSmallPath", 1
		);*/


	/*	auto GachaBGController_class = il2cpp_symbols::get_class("umamusume.dll", "Gallop", "GachaBGController");
		auto GachaBGController_GateDoor_class = il2cpp_symbols::find_nested_class_from_name(GachaBGController_class, "GateDoor");
		auto GachaBGController_GateDoor_SetRarity_addr =
			il2cpp_class_get_method_from_name(
				GachaBGController_GateDoor_class,
				"SetRarity",1
			)->methodPointer;
		MH_CreateHook((LPVOID)GachaBGController_GateDoor_SetRarity_addr, GachaBGController_GateDoor_SetRarity_hook, &GachaBGController_GateDoor_SetRarity_orig);
		MH_EnableHook((LPVOID)GachaBGController_GateDoor_SetRarity_addr);*/

		//printf("\033[33mGachaBGController_GateDoor_class %p\033[0m\n", GachaBGController_GateDoor_SetRarity_addr);
#pragma endregion

		// hook UnityEngine.TextGenerator::PopulateWithErrors to modify text
		ADD_HOOK(populate_with_errors, "UnityEngine.TextGenerator::PopulateWithErrors at %p\n");

		// Looks like they store all localized texts that used by code in a dict
		ADD_HOOK(localize_get, "Gallop.Localize.Get(TextId) at %p\n");

		ADD_HOOK(query_ctor, "Query::ctor at %p\n");
		ADD_HOOK(query_getstr, "Query::GetString at %p\n");
		ADD_HOOK(query_dispose, "Query::Dispose at %p\n");
		//ADD_HOOK(live_cam_GetCullingMask, "Gallop.Live.Cutt.LiveCameraCullingLayer_Helper.GetCullingMask(uint) at %p\n");
		//ADD_HOOK(live_CameraLookAt_UpdateCamera, "Gallop.Live.CameraLookAt.UpdateCamera(longlong) at %p \n");
		//ADD_HOOK(live_Cutt_LiveTimelineControl_GetCameraPos, "Gallop.Live.Cutt.LiveTimelineControl.GetCameraPos(int) at %p\n");
		//ADD_HOOK(live_Cutt_LiveTimelineControl_set_liveStageCenterPos, "Gallop.Live.Cutt.LiveTimelineControl.set_liveStageCenterPos(Vector3) at %p\n");
		ADD_HOOK(AssetBundleHelper_GetResourceVer, "Gallop.AssetBundleHelper.GetResourceVer() at %p\n");
		//ADD_HOOK(GachaBGController_OverrideRarity, "Gallop.GachaBGController.OverrideRarity(int, int(enum)) at %p\n");
		//ADD_HOOK(GachaBGController_SetGateDoorRarity, "Gallop.GachaBGController.SetRarity(int) at %p\n");
		/*ADD_HOOK(LiveTitleController_Setup, "Gallop.Live.LiveTitleController.Setup at %p\n");
		ADD_HOOK(LiveTitleController_FadeIn, "Gallop.Live.LiveTitleController.FadeIn(float,float) at %p\n");
		ADD_HOOK(LiveTitleController_FadeOut, "Gallop.Live.LiveTitleController.FadeOut(float,float) at %p\n");
		ADD_HOOK(LiveTitleController_AlterUpdate, "LiveTitleController_AlterUpdate at %p\n");*/
		ADD_HOOK(FadeGui_SetAlpha, "FadeGui_SetAlpha at %p\n");
		//ADD_HOOK(Unity_Post, "Unity_Post at %p\n");
		ADD_HOOK(RaceUI_SetRaceUIActive, "RaceUI.SetRaceUIActive(bool) at %p\n");
		ADD_HOOK(RaceUI_SetVisibleRank, "RaceUI.SetVisibleRank(bool) at %p\n");
		//ADD_HOOK(RaceUIRank_Setup, "RaceUIRank.Setup(7) at %p\n");
		ADD_HOOK(RaceUIRank_PlayPlayerRankUp, "RaceUIRank.PlayPlayerRankUp() at %p\n");
		ADD_HOOK(RaceUIRank_PlayPlayerRankDown, "RaceUIRank.PlayPlayerRankDown() at %p\n");
		ADD_HOOK(RaceManager_GetHorseDistanceByIndex, "RaceManager.GetHorseDistanceByIndex(int) at %p\n");
		ADD_HOOK(LiveTheaterInfo_GetDefaultDressid, "LiveTheaterInfo_GetDefaultDressid(...) at %p\n");
		ADD_HOOK(LiveTheaterInfo_UpdateCharaDressIds, "LiveTheaterInfo_UpdateCharaDressIds(LiveTheaterMemberInfo[]) at %p\n");
		ADD_HOOK(LiveTheaterInfo_CheckDress, "LiveTheaterInfo_CheckDress(int,CharaDressIdSet) at %p\n");
		ADD_HOOK(RaceResultScene_GetMotionVariationId, "RaceResultScene_GetMotionVariationId(int) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadBodyMotion, "RaceResultCutInHelper_LoadBodyMotion(...7) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadCameraMotion, "RaceResultCutInHelper_LoadCameraMotion(...5) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadEarMotion, "RaceResultCutInHelper_LoadEarMotion(...5) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadFacialMotion, "RaceResultCutInHelper_LoadFacialMotion(...5) at %p\n");
		ADD_HOOK(ResourcePath_GetCharacterRaceResultMotionPath,"ResourcePath_GetCharacterRaceResultMotionPath at %p\n")
		ADD_HOOK(RaceResultScene_PlayFinishOrderAnim, "RaceResultScene_PlayFinishOrderAnim(Action) at %p\n");
		//ADD_HOOK(RaceResultCutInHelper_GetModelController, "RaceResultCutInHelper_GetModelController(..) at %p");
		//ADD_HOOK(RaceResultCutInHelper_GetResultCuttCueId, "RaceResultCutInHelper_GetResultCuttCueId(int,int,int*) at %p");
		ADD_HOOK(RaceSkillCutInHelper_PreInstantiateCharaUser, "RaceSkillCutInHelper_PreInstantiateCharaFixed at %p\n");
		//ADD_HOOK(RaceSkillCutInHelper_InitForGacha, "RaceSkillCutInHelper_InitForGacha at %p\n");
		ADD_HOOK(Gallop_Cutin_CutinCharacter_ctor, "Gallop_Cutin_CutinCharacter_.ctor at %p\n");
		ADD_HOOK(set_virt, "set_virt at %p\n");
		printf("CustomHost:%s\n", g_sett->customHost);
		ADD_HOOK(Cute_Http_WWWRequest_Post, "Cute_Http_WWWRequest_Post(...) at %p\n");
		ADD_HOOK(CutInHelper_OnCreateCharacterModel, "CutInHelper_OnCreateCharacterModel at %p\n");

		//ADD_HOOK(CutInHelper_ApplyCharacterMotion_Facial, "CutInHelper_ApplyCharacterMotion_Facial at %p\n");
		//ADD_HOOK(CutInHelper_ApplyCharacterMotion_Ear, "CutInHelper_ApplyCharacterMotion_Ear at %p\n");
		//ADD_HOOK(CutInHelper_GetCharaId, "CutInHelper_GetCharaId at %p\n");
		//ADD_HOOK(CutIn_CutInCharacter_EntryModel, "CutIn_CutInCharacter_EntryModel at %p\n");
		
		//ADD_HOOK(BitmapTextCommon_GetFontPath, "BitmapTextCommon_GetFontPath(TextFormat.BitmapFont) at %p");
		//ADD_HOOK(ModelLoader_CreateNormalModel, "ModelLoader_CreateNormalModel(CharacterBuildInfo) at %p\n");
		//ADD_HOOK(ModelLoader_CreateModel, "ModelLoader_CreateModel(CharacterBuildInfo) at %p\n");
		ADD_HOOK(CharacterBuildInfo_ctor, "CharacterBuildInfo_ctor at %p\n");
		ADD_HOOK(CharacterBuildInfo_ctor_overload1, "CharacterBuildInfo_ctor_overload1 at %p\n");
		ADD_HOOK(CharacterBuildInfo_ctor_overload2, "CharacterBuildInfo_ctor_overload2 at %p\n");
		ADD_HOOK(StoryCharacter3D_LoadModel, "StoryCharacter3D_LoadModel at %p\n");
		ADD_HOOK(DialogManager_PushErrorCommon, "DialogManager_PushErrorCommon at %p\n");
		ADD_HOOK(Live_LiveTimelineCamera_AlterUpdate, "Live_LiveTimelineCamera_AlterUpdate at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraLookAt, "Live_Cutt_AlterUpdate_CameraLookAt at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraFov, "AlterUpdate_CameraFov at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraPos, "Live_Cutt_AlterUpdate_CameraPos at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraSwitcher, "Live_Cutt_AlterUpdate_CameraSwitcher at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraLayer, "Live_Cutt_AlterUpdate_CameraLayer at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraMotion, "Live_Cutt_AlterUpdate_CameraMotion at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_CameraRoll, "Live_Cutt_AlterUpdate_CameraRoll at %p\n");
		//ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraSwitcher, "Live_Cutt_AlterUpdate_MultiCameraSwitcher at %p\n");
		//ADD_HOOK(Live_Cutt_AlterUpdate_MultiCamera, "Live_Cutt_AlterUpdate_MultiCamera at %p\n");
		//ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraLookAt, "Live_Cutt_AlterUpdate_MultiCameraLookAt at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraPosition, "Live_Cutt_AlterUpdate_MultiCameraPosition at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraTiltShift, "Live_Cutt_AlterUpdate_MultiCameraTiltShift at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_PostEffect_DOF, "Live_Cutt_AlterUpdate_PostEffect_DOF at %p\n");
		ADD_HOOK(Live_Cutt_AlterLateUpdate_CameraMotion, "Live_Cutt_AlterLateUpdate_CameraMotion at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_EyeCameraPosition, "Live_Cutt_AlterUpdate_EyeCameraPosition at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_EyeCameraLookAt, "Live_Cutt_AlterUpdate_EyeCameraLookAt at %p\n");
		//ADD_HOOK(GameSystem_SoftwareReset, " GameSystem_SoftwareReset at %p\n");
		ADD_HOOK(GallopUtil_GotoTitleOnError, "GallopUtil_GotoTitleOnError at %p\n");
		ADD_HOOK(TapEffectController_ctor, "TapEffectController_ctor at %p\n");
		ADD_HOOK(Cute_Core_Device_GetPersistentDataPath, "Cute_Core_Device_GetPersistentDataPath at %p\n");
		ADD_HOOK(textutil_getmastertext, "textutil_getmastertext at %p\n");
		ADD_HOOK(textutil_getstatictext, "textutil_getstatictext at %p\n");
		ADD_HOOK(apply_graphics_quality, "Gallop.GraphicSettings.ApplyGraphicsQuality at %p\n");
		ADD_HOOK(unityengine_get_persistentDataPath, "unityengine_get_persistentDataPath at %p\n");
		ADD_HOOK(MasterEventMotionData_GetAnimCommand, "MasterEventMotionData_GetAnimCommand at %p\n");
		ADD_HOOK(ModelController_GetCardId, "ModelController_GetCardId at %p\n");

		ADD_HOOK(get_camera_pos, "get_camera_pos at %p\n");
		ADD_HOOK(get_camera_pos2, "get_camera_pos2 at %p\n");

		ADD_HOOK(Screen_set_orientation, "Gallop.NowLoading::Show at %p\n");
		ADD_HOOK(ChangeScreenOrientation, "Gallop.NowLoading::Show at %p\n");

		if (g_sett->forceLandscape) {
			auto enumerator1 = reinterpret_cast<Il2CppObject * (*)()>(il2cpp_symbols::get_method_pointer(
				"umamusume.dll",
				"Gallop",
				"Screen", "ChangeScreenOrientationLandscapeAsync", -1))();
			auto move_next1 = reinterpret_cast<void* (*)(
				Il2CppObject * _this)>(il2cpp_class_get_method_from_name(enumerator1->klass,
					"MoveNext",
					0)->methodPointer);
			move_next1(enumerator1);
		}

		ADD_HOOK(set_antialiasing, "UnityEngine.CoreModule.QualitySettings.set_antiAliasing at %p\n");
		ADD_HOOK(graphics_quality, "Gallop.GraphicSettings.ApplyGraphicsQuality at %p\n");
		//ADD_HOOK(set_vsync_count, "UnityEngine.CoreModule.QualitySettings.set_vSyncCount at %p\n");
		ADD_HOOK(set_RenderTextureAntiAliasing, "set_RenderTextureAntiAliasing at %p\n");
		ADD_HOOK(Get3DAntiAliasingLevel, "Get3DAntiAliasingLevel at %p\n");
		ADD_HOOK(change_resize_ui_for_pc, "change_resize_ui_for_pc at %p\n");
		ADD_HOOK(ProcessMouseEvent, "ProcessMouseEvent at %p\n");
		//ADD_HOOK(BootSystem_Awake, "BootSystem_Awake at %p\n");
		ADD_HOOK(Gallop_SceneManager_LoadScene, "Gallop_SceneManager_LoadScene at %p\n");
		//set_vsync_count_hook(1);



		//ADD_HOOK(RaceSkillCutInHelper_Owner_CreateAddChara, "RaceSkillCutInHelper$$Owner_CreateAddChara at %p\n");
		//ADD_HOOK(cyan_localfile_pathresolver_getlocalpath, "_Cyan_GetLocalPath at %p\n");
		//ADD_HOOK(unity_font_ctor, "unity_font_ctor(string) at %p\n");
		//ADD_HOOK(assetbundleHelper_IsNeedManifestSetup, "assetbundleHelper_IsNeedManifestSetup at %p\n");
		//ADD_HOOK(assetbundleHelper_IsExistGallopResources, "assetbundleHelper_IsExistGallopResources at %p\n");
		//ADD_HOOK(UnityEngine_GameObject_ctor, "UnityEngine_GameObject_ctor(string) at %p\n");
		//ADD_HOOK(UnityEngine_GameObject_ctor1, "UnityEngine_GameObject_ctor1() at %p\n");
		//ADD_HOOK(UnityEngine_GameObject_ctor2, "UnityEngine_GameObject_ctor2 at %p\n");
		//ADD_HOOK(GetKeyDown, "UnityEngine.Input.GetKeyDown at %p\n");
		//ADD_HOOK(Unity_KeyboardEvent, "Unity_KeyboardEvent at %p\n");
		//ADD_HOOK(Live_Cutt_AlterUpdate, "Live_Cutt_AlterUpdate at %p\n");
		//ADD_HOOK(Race_GetRankNumSmallPath, "AtlasSpritePath.Race.GetRankNumSmallPath(int) at %p");
		//ADD_HOOK(GachaBGController_GateDoor_SetRarity, "Gallop.GachaBGController.GateDoor.SetRarity(int(enum)) at %p\n");
		if (g_sett->replaceFont)
		{
			ADD_HOOK(on_populate, "Gallop.TextCommon::OnPopulateMesh at %p\n");
		}
		
		if (g_sett->maxFps > -1)
		{
			// break 30-40fps limit
			//ADD_HOOK(set_fps, "UnityEngine.Application.set_targetFrameRate at %p \n");
		}
		ADD_HOOK(set_fps, "UnityEngine.Application.set_targetFrameRate at %p \n");

		if (g_sett->unlockSize)
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

		ADD_HOOK(set_resolution, "UnityEngine.Screen.SetResolution(int, int, bool) at %p\n");
		if (g_sett->autoFullscreen)
		{			
			//ADD_HOOK(setExclusiveFullScreen, "UnityEngine.Screen.SetResolution(int, int, int(enum),int) at %p\n");
			adjust_size();
		}
		
		if (g_sett->dumpStaticEntries)
			dump_all_entries();

		
	}

	void hook_beforeboot() {
		if (!beforePatched) {
			printf("Patch before game boot...\n");

			auto il2cpp_module = GetModuleHandle(L"GameAssembly.dll");

			// load il2cpp exported functions
			il2cpp_symbols::init(il2cpp_module);


			auto unityengine_get_persistentDataPath_addr = il2cpp_resolve_icall("UnityEngine.Application::get_persistentDataPath()");/*reinterpret_cast<Il2CppString * (*)()>(
				il2cpp_symbols::get_method_pointer(
					"UnityEngine.CoreModule.dll", "UnityEngine",
					"Application", "get_persistentDataPath", 0
				));*/
			MH_CreateHook((LPVOID)unityengine_get_persistentDataPath_addr, unityengine_get_persistentDataPath_hook, &unityengine_get_persistentDataPath_orig);
			MH_EnableHook((LPVOID)unityengine_get_persistentDataPath_addr);

			/*auto BootSystem_Awake_addr = reinterpret_cast<void (*)(Il2CppObject*)>(il2cpp_symbols::get_method_pointer(
				"umamusume.dll",
				"Gallop", "BootSystem", "Awake", 0));

			printf("bootsystemptr = %p\n", BootSystem_Awake_addr);

			MH_CreateHook((LPVOID)BootSystem_Awake_addr, BootSystem_Awake_hook, &BootSystem_Awake_orig);
			MH_EnableHook((LPVOID)BootSystem_Awake_addr);*/


			/*MH_CreateHook((LPVOID)cyan_localfile_pathresolver_getlocalpath_addr, unityengine_get_persistentDataPath_hook, &cyan_localfile_pathresolver_getlocalpath_orig);
			MH_EnableHook((LPVOID)cyan_localfile_pathresolver_getlocalpath_addr);*/
			GetGraphicsDeviceName = reinterpret_cast<Il2CppString * (*)()>(il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine", "SystemInfo", "GetGraphicsDeviceName", 0
			));

			wprintf(L"GPUName = %s\n", GetGraphicsDeviceName()->start_char);

			char* conv_str = new char[GetGraphicsDeviceName()->length + 2];
			memset(conv_str, 0, GetGraphicsDeviceName()->length + 2);
			
			wcstombs(conv_str, GetGraphicsDeviceName()->start_char, GetGraphicsDeviceName()->length + 1);

			GPUName = (const char*)conv_str;

			beforePatched = true;
		}
		
	}

	void startThread() {
		//std::thread t1(meterDataSendThread);
		//t1.detach();
		//printf("Horse data send Thread started\n");
		thread([]() {
			auto tr = il2cpp_thread_attach(il2cpp_domain_get());
			keyDownCheckThread();
			il2cpp_thread_detach(tr);
			}).detach();

		/*std::thread t2(keyDownCheckThread);
		t2.detach();*/

		/*std::thread t3(imguiwindow);
		t3.detach();*/

		//thread([]() {
		//	auto tr = il2cpp_thread_attach(il2cpp_domain_get());

		//	//printf("I am Running In Here!!!\n");
		//	imguiwindow();

		//	il2cpp_thread_detach(tr);
		//}).detach();
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
		lastUrl = currentUrl;
		//printf("compressedSize=%d,dstCapacity=%d", compressedSize, dstCapacity);
		char* decrypted = NULL;
		//server_ip;
		int ret = 0;
		
		wchar_t buf[256];
		//wprintf(L"%s\n", currentUrl.path().c_str());
		swprintf_s(buf, L"umamusume_L [%s %d]", wstring(currentUrl.path().begin(),currentUrl.path().end()).c_str(), compressedSize);
		SetWindowText(currenthWnd, buf);


		if (g_sett->lz4Encrypt) {
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
		
		if (g_sett->saveMsgPack) {

			auto out_path = std::string("CarrotJuicer\\").append(current_time()).append(ReplaceAll(currentUrl.path(),"/","_")).append("R.msgpack");
			write_file(out_path, decrypted, ret);
			printf("wrote response to %s\n", out_path.c_str());
		}
			
		
		if (g_sett->passPacket) {
			printf("------Real Cygames server -> modding server(local) -> Client------\n");
			httplib::Headers headers = {
				{ "Url", lastUrl.str() }
			};
			
			httplib::Client cli(g_sett->serverIP, g_sett->serverPort);
			std::string data(decrypted, ret);
			auto res = cli.Post("/umamusume_uploadmsgpack/live",headers, data, "application/x-msgpack");
			res->status;
			const char* returned = res->body.c_str();			
			printf("\n");
			size_t clength = res->body.length();
			printf("Original Server length: %d, modified length: %d\n", ret, clength);
			ret = res->body.length();
			
			//printf("Status: %d\nContent-Length:%d\n", res->status, clength);
			
			memcpy(dst, returned, clength);//sss
			

			//delete[] returned;
			//free(src);
			printf("-----------------------------------------------------------------\n");
		}
		else {
			memcpy(dst, decrypted, ret);
		}
		
		delete[] decrypted;
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
		if (g_sett->passPacket) {
			httplib::Client cli(g_sett->serverIP, g_sett->serverPort);
			std::string data(raw_data, srcSize);
			printf("------Client -> modding server(local) -> Real Cygames server------\n");
			httplib::Headers headers = {
				{ "Url", lastUrl.str() }
			};
		
			auto res = cli.Post("/umamusume_uploadmsgpack/uma_client_request_toserver", headers, data, "application/x-msgpack");
			res->status;
			const char* returned = res->body.c_str();			
			size_t clength = res->body.length();
			memcpy(src, returned, clength);
			int ret = 0;
			if (g_sett->lz4Encrypt) {
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
			if (g_sett->saveMsgPack) {
				auto out_path = std::string("CarrotJuicer\\").append(current_time()).append(ReplaceAll(lastUrl.path(), "/", "_")).append("Q.msgpack");
				write_file(out_path, src, srcSize);
				printf("wrote clinet request to %s\n", out_path.c_str());
			}
			printf("-------------------------------------------------------------------------\n");
			delete[] raw_data;
			
			return ret;
		}
		else {
			int ret = 0;
			if (g_sett->lz4Encrypt) {
				ret = reinterpret_cast<decltype(LZ4_compress_default_ext_hook)*>(LZ4_compress_default_ext_orig)(
					src, dst, srcSize, dstCapacity);
			}
			else {
				ret = LZ4_compress_default(src, dst, srcSize, dstCapacity);
				//memcpy(dst, src, clength);
				//ret = clength;
			}
			//int ret = reinterpret_cast<decltype(LZ4_compress_default_ext_hook)*>(LZ4_compress_default_ext_orig)(
			//	src, dst, srcSize, dstCapacity);
			printf("Raw Client data: %d Bytes (Compressed/Encrypted to %d bytes) -> \n", srcSize, ret);
			if (g_sett->saveMsgPack) {
				auto out_path = std::string("CarrotJuicer\\").append(current_time()).append(ReplaceAll(lastUrl.path(), "/", "_")).append("Q.msgpack");
				write_file(out_path, src, srcSize);
				printf("wrote clinet request to %s\n", out_path.c_str());
			}
			
			delete[] raw_data;
			return ret;
		}

		
	}
	void bootstrap_carrot_juicer()
	{
		if (g_sett->saveMsgPack) {
			std::filesystem::create_directory("CarrotJuicer");
		}

		

		auto libnative_module = GetModuleHandle(L"libnative.dll");
		printf("libnative.dll at %p\n", libnative_module);
		if (libnative_module == nullptr)
		{
			printf("[libnativeHook] Error: libnative.dll is nullptr\n");
			return;
		}

		auto LZ4_decompress_safe_ext_ptr = GetProcAddress(libnative_module, "LZ4_decompress_safe_ext");
		printf("LZ4_decompress_safe_ext at %p\n", LZ4_decompress_safe_ext_ptr);
		if (LZ4_decompress_safe_ext_ptr == nullptr) {
			printf("[libnativeHook] Error: LZ4_decompress_safe_ext is nullptr\n");
			return;
		}
		MH_CreateHook(LZ4_decompress_safe_ext_ptr, LZ4_decompress_safe_ext_hook, &LZ4_decompress_safe_ext_orig);
		MH_EnableHook(LZ4_decompress_safe_ext_ptr);

		auto LZ4_compress_default_ext_ptr = GetProcAddress(libnative_module, "LZ4_compress_default_ext");
		printf("LZ4_compress_default_ext at %p\n", LZ4_compress_default_ext_ptr);
		if (LZ4_compress_default_ext_ptr == nullptr) {
			printf("[libnativeHook] Error: LZ4_compress_default_ext is nullptr\n");
			return;
		}
		MH_CreateHook(LZ4_compress_default_ext_ptr, LZ4_compress_default_ext_hook, &LZ4_compress_default_ext_orig);
		MH_EnableHook(LZ4_compress_default_ext_ptr);
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
		
		//strcpy(anotherprgname, document["runanotherprgname"].GetString());
		if (g_sett->passPacket) {
			printf("Json Pass to another server enabled: %s:%d\n", g_sett->serverIP, g_sett->serverPort);
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

#pragma region GuiFunc

//static ID3D11Device* pDevice = NULL;
//static ID3D11DeviceContext* pContext = NULL;
//static IDXGISwapChain* g_pSwapChain = NULL;
//static ID3D11RenderTargetView* mainRenderTargetView = NULL;

static vector<void*> rootObjList;
static bool show_info_window = false;
static void* selected_obj = 0;
static bool show_active_box = false;

const int MAX_FPS_VALUES = 100;               // 최근 100개의 FPS 값을 저장
static ScrollingBuffer fpsValues;                  // FPS 값들을 저장할 큐


int CALLBACK EnumFontFamExProc(
	const LOGFONT* lpelfe, const TEXTMETRIC* lpntme,
	DWORD FontType, LPARAM lparam)
{
	std::wstring* ptrTargetFaceName = reinterpret_cast<std::wstring*>(lparam);
	if (*ptrTargetFaceName == lpelfe->lfFaceName) { return 0; }
	else { return 1; }
}
bool IsFontExistOnSystem(const std::string& iFontKey)
{
	HDC pDC = GetDC(NULL);
	LOGFONT logFont;

	// Set font information structure.
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfPitchAndFamily = 0;

	// @reference https://www.gpgstudy.com/forum/viewtopic.php?t=7047
	std::wstring fontKey;
	{
		USES_CONVERSION;
		fontKey = A2W(iFontKey.c_str());
	}
	wcscpy(logFont.lfFaceName, fontKey.c_str());

	// EnumFontFamiliesEXW (Multi-byte dirty function) returns `0` when found given font face-name.
	const auto flag = EnumFontFamiliesExW(pDC, &logFont, EnumFontFamExProc, reinterpret_cast<LPARAM>(&fontKey), 0);
	return flag == 0;
}
std::optional<std::string> GetFontPathOnSystem(const std::string& iFontKey)
{
	// Check validity.
	if (IsFontExistOnSystem(iFontKey) == false) { return std::nullopt; }

	// @reference https://www.gpgstudy.com/forum/viewtopic.php?t=7047
	std::wstring fontFacename;
	{
		USES_CONVERSION;
		fontFacename = A2W(iFontKey.c_str());
	}

	// Find path using registry.
	static const auto fontRegistryPath = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
	HKEY hKey;

	{ // Open Windows font registry key
		const auto result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey);
		if (result != ERROR_SUCCESS) { return std::nullopt; }
	}

	DWORD maxValueNameSize, maxValueDataSize;
	{
		const auto result = RegQueryInfoKey(hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0);
		if (result != ERROR_SUCCESS) { return std::nullopt; }
	}

	DWORD valueIndex = 0;
	LPWSTR valueName = new WCHAR[maxValueNameSize];
	LPBYTE valueData = new BYTE[maxValueDataSize];
	DWORD valueNameSize, valueDataSize, valueType;
	std::wstring wsFontFile;

	{ // Look for a matching font name
		LONG result;
		do
		{
			wsFontFile.clear();
			valueDataSize = maxValueDataSize;
			valueNameSize = maxValueNameSize;

			result = RegEnumValue(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);
			valueIndex++;

			if (result != ERROR_SUCCESS || valueType != REG_SZ) { continue; }
			// Find a match. 
			std::wstring wsValueName(valueName, valueNameSize);
			if (wsValueName.find(fontFacename) != std::wstring::npos)
			{
				wsFontFile.assign((LPWSTR)valueData, valueDataSize); break;
			}
		} while (result != ERROR_NO_MORE_ITEMS);
	}

	// Release temporary chunk.
	delete[] valueName; delete[] valueData;

	// Close registry.
	RegCloseKey(hKey);
	if (wsFontFile.empty()) { return std::nullopt; }

	// Build full font file path
	WCHAR winDir[MAX_PATH];
	GetWindowsDirectory(winDir, MAX_PATH);

	std::wstringstream ss; ss << winDir << "\\Fonts\\" << wsFontFile;
	wsFontFile = ss.str();

	return std::string(wsFontFile.begin(), wsFontFile.end());
}





//닒츰냔櫓삿혤잚謹츰
string getTypeName(string name) {
	int left = name.rfind('(') + 1;
	int right = name.rfind(')');

	return name.substr(left, right - left);
}

string delArrayName(string name) {
	int left = name.rfind('[');
	return name.substr(0, left);
}



//寧몸鬼鬼狂뵀
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

static void HelpMarker(void* image,int width, int height, const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::Image((void*)image, ImVec2(width, height));
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}
void BeginGroupPanel(const char* name, const ImVec2& size = ImVec2(0.0f, 0.0f));
void EndGroupPanel();

void BeginGroupPanel(const char* name, const ImVec2& size)
{
	ImGui::BeginGroup();

	auto cursorPos = ImGui::GetCursorScreenPos();
	auto itemSpacing = ImGui::GetStyle().ItemSpacing;
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	auto frameHeight = ImGui::GetFrameHeight();
	ImGui::BeginGroup();

	ImVec2 effectiveSize = size;
	//if (size.x < 0.0f)
	//	effectiveSize.x = ImGui::GetContentRegionAvailWidth();
	//else
		effectiveSize.x = size.x;
	ImGui::Dummy(ImVec2(effectiveSize.x, 0.0f));

	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::TextUnformatted(name);
	auto labelMin = ImGui::GetItemRectMin();
	auto labelMax = ImGui::GetItemRectMax();
	ImGui::SameLine(0.0f, 0.0f);
	ImGui::Dummy(ImVec2(0.0, frameHeight + itemSpacing.y));
	ImGui::BeginGroup();

	//ImGui::GetWindowDrawList()->AddRect(labelMin, labelMax, IM_COL32(255, 0, 255, 255));

	ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
	ImGui::GetCurrentWindow()->ContentRegionRect.Max.x -= frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->WorkRect.Max.x -= frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->InnerRect.Max.x -= frameHeight * 0.5f;
#else
	ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x -= frameHeight * 0.5f;
#endif
	ImGui::GetCurrentWindow()->Size.x -= frameHeight;

	auto itemWidth = ImGui::CalcItemWidth();
	ImGui::PushItemWidth(ImMax(0.0f, itemWidth - frameHeight));

	s_GroupPanelLabelStack.push_back(ImRect(labelMin, labelMax));
}

void EndGroupPanel()
{
	ImGui::PopItemWidth();

	auto itemSpacing = ImGui::GetStyle().ItemSpacing;

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));

	auto frameHeight = ImGui::GetFrameHeight();

	ImGui::EndGroup();

	//ImGui::GetWindowDrawList()->AddRectFilled(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(0, 255, 0, 64), 4.0f);

	ImGui::EndGroup();

	ImGui::SameLine(0.0f, 0.0f);
	ImGui::Dummy(ImVec2(frameHeight * 0.5f, 0.0f));
	ImGui::Dummy(ImVec2(0.0, frameHeight - frameHeight * 0.5f - itemSpacing.y));

	ImGui::EndGroup();

	auto itemMin = ImGui::GetItemRectMin();
	auto itemMax = ImGui::GetItemRectMax();
	//ImGui::GetWindowDrawList()->AddRectFilled(itemMin, itemMax, IM_COL32(255, 0, 0, 64), 4.0f);

	auto labelRect = s_GroupPanelLabelStack.back();
	s_GroupPanelLabelStack.pop_back();

	ImVec2 halfFrame = ImVec2(frameHeight * 0.25f, frameHeight) * 0.5f;
	ImRect frameRect = ImRect(itemMin + halfFrame, itemMax - ImVec2(halfFrame.x, 0.0f));
	labelRect.Min.x -= itemSpacing.x;
	labelRect.Max.x += itemSpacing.x;
	for (int i = 0; i < 4; ++i)
	{
		switch (i)
		{
			// left half-plane
		case 0: ImGui::PushClipRect(ImVec2(-FLT_MAX, -FLT_MAX), ImVec2(labelRect.Min.x, FLT_MAX), true); break;
			// right half-plane
		case 1: ImGui::PushClipRect(ImVec2(labelRect.Max.x, -FLT_MAX), ImVec2(FLT_MAX, FLT_MAX), true); break;
			// top
		case 2: ImGui::PushClipRect(ImVec2(labelRect.Min.x, -FLT_MAX), ImVec2(labelRect.Max.x, labelRect.Min.y), true); break;
			// bottom
		case 3: ImGui::PushClipRect(ImVec2(labelRect.Min.x, labelRect.Max.y), ImVec2(labelRect.Max.x, FLT_MAX), true); break;
		}

		ImGui::GetWindowDrawList()->AddRect(
			frameRect.Min, frameRect.Max,
			ImColor(ImGui::GetStyleColorVec4(ImGuiCol_Border)),
			halfFrame.x);

		ImGui::PopClipRect();
	}

	ImGui::PopStyleVar(2);

#if IMGUI_VERSION_NUM >= 17301
	ImGui::GetCurrentWindow()->ContentRegionRect.Max.x += frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->WorkRect.Max.x += frameHeight * 0.5f;
	ImGui::GetCurrentWindow()->InnerRect.Max.x += frameHeight * 0.5f;
#else
	ImGui::GetCurrentWindow()->ContentsRegionRect.Max.x += frameHeight * 0.5f;
#endif
	ImGui::GetCurrentWindow()->Size.x += frameHeight;

	ImGui::Dummy(ImVec2(0.0f, 0.0f));

	ImGui::EndGroup();
}

//wchar_t* GetCharaNameByCharaId(int charaId) {
//	auto getname = reinterpret_cast<Il2CppString* (*)(int)>(
//		il2cpp_symbols::get_method_pointer(
//			"umamusume.dll", "Gallop",
//			"MasterDataUtil", "GetCharaNameByCharaId", 1
//		)
//	);	
//	Il2CppString* ret = getname(charaId);
//	wprintf(L"%s\n", ret->start_char);
//	return ret->start_char;
//}

//bool get_IsCameraShake() {
//	bool isCameraShake = false;
//	Il2CppClass* klass = (Il2CppClass*)il2cpp_symbols::get_class("umamusume.dll", "Gallop", "LiveQualitySettings");
//	//LiveQualitySettings* sett = { (LiveQualitySettings*)il2cpp_symbols::get_class("umamusume.dll", "Gallop", "LiveQualitySettings") };
//	FieldInfo* field = il2cpp_class_get_field_from_name(klass, "IsCameraShake");
//	isCameraShake = il2cpp_symbols::GetStaticFieldValue(field);
//	printf("%d\n",isCameraShake );
//	return isCameraShake;
//		//il2cpp_field_static_get_value();
//}


#pragma endregion


#pragma region GuiMain
//************************************************************************************************************


// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code loop
int imguiwindow()
{
	//printf("imguiwindow()\n");
	/*ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(umaWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);
	static const ImWchar ranges[] = { 0x0001, 0xffff, 0 };
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 25.0f, NULL, ranges);*/

	// Create application window
	//ImGui_ImplWin32_EnableDpiAwareness();

	//WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, _T("UmamusumeSettings"), NULL };
	//::RegisterClassEx(&wc);
	//HWND hwnd = ::CreateWindow(wc.lpszClassName, L"설정", WS_OVERLAPPEDWINDOW, 100, 100, 800,1000, NULL, NULL, wc.hInstance, NULL);
	//imguiWnd = hwnd;

	//// Initialize Direct3D
	//if (!CreateDeviceD3D(hwnd))
	//{
	//	CleanupDeviceD3D();
	//	::UnregisterClass(wc.lpszClassName, wc.hInstance);
	//	return 1;
	//}

	//// Show the window
	//::ShowWindow(hwnd, SW_HIDE);
	//::UpdateWindow(hwnd);

	//EnableCloseButton(hwnd, false);

	// Setup Dear ImGui context
	/*IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;*/
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	//ImGui_ImplWin32_Init(hwnd);
	//ImGui_ImplDX11_Init(pDevice, pContext);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ARIALUNI.TTF", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	// Check japanese (meiryo UI) font is exist on system. 
	//{
	//	std::string japaneseFontName = "";
	//	if (IsFontExistOnSystem("Malgun Gothic (TrueType)") == true) { japaneseFontName = "Malgun Gothic (TrueType)"; }

	//	// If font name is found, add font file to imgui. Otherwise, just pass it.
	//	if (japaneseFontName.empty() == false)
	//	{
	//		// Get system font path from manager.
	//		const auto optPath = GetFontPathOnSystem(japaneseFontName);
	//		//MDY_ASSERT_FORCE(optPath.has_value() == true, "Unexpected error occurred.");
	//		// Create glyphs.
	//		io.Fonts->AddFontFromFileTTF(optPath.value().c_str(), 18.0f, nullptr, io.Fonts->GetGlyphRangesKorean());
	//	}
	//}

	/*static const ImWchar ranges[] = { 0x0001, 0xffff, 0 };
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 25.0f, NULL, ranges);*/



	//IM_ASSERT(font != NULL);

	// Our state
	bool show_demo_window = false;
	bool show_tool_window = true;
	bool show_obj_window = false;

	bool enable_edit = false;
	bool camera_base = false;


	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



	// Main loop
	bool done = false;
	//while (!done)
	{
		//printf("imgui_loop in\n");
		// Poll and handle messages (inputs, window resize, etc.)
		// See the WndProc() function below for our to dispatch events to the Win32 backend.
		//MSG msg;
		/*while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
				done = true;
		}
		if (done)
			break;*/

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuiContext& g = *GImGui;

	
		ImGui::SetNextWindowPos({ 50, 50 }, ImGuiCond_Once);
		ImGui::SetNextWindowSize({ 550, 550 }, ImGuiCond_Once);

		

		/*ImGui::Begin("Hello World!", (bool*)0, ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 0,0 });
		ImGui::SetWindowPos({99999,99999});	
		ImGui::End();*/

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1); // Don't forget to Pop()
		ImGui::PopStyleColor(1);

		if (toastImGui) {
			ImGui::InsertNotification({ ImGuiToastType_Success, 3000, toastMessage });
			toastImGui = false;
		}


		bool isCamStopped = sett->stopLiveCam;

		if (currSceneID == 4) {
			int width, height;
			float currentFps = ImGui::GetIO().Framerate;
			static float t = 0;
			t += ImGui::GetIO().DeltaTime;
			RECT rect;
			if (GetWindowRect(currenthWnd, &rect))
			{
				width = rect.right - rect.left;
				height = rect.bottom - rect.top;
			}

			if (g_sett->isShowLivePerfInfo) {
				ImGui::Begin("Status Bar", NULL,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoScrollbar |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_AlwaysAutoResize);

				


				fpsValues.AddPoint(t, currentFps);
				/*if (fpsValues.size() > MAX_FPS_VALUES) {
					fpsValues.pop_front();
				}*/

				

				//glfwGetWindowSize(window, &width, &height);
				ImGui::SetWindowPos(ImVec2(0, 0));
				ImGui::SetWindowSize(ImVec2(static_cast<float>(width), 0));


				ImGui::Text("%02d:%02d / %02d:%02d | Current: %.3f sec |", static_cast<int>(liveTimeSec) / 60, static_cast<int>(liveTimeSec) % 60,
					static_cast<int>(liveTotalTimeSec) / 60, static_cast<int>(liveTotalTimeSec) % 60, liveTimeSec);
				ImGui::SameLine();

				//ImGui::SameLine(width - 150); // Adjust this value accordingly
				ImGui::Text("출력: %s (%d hz) | 게임: %dx%d %.1f fps | 제한: %d fps | 카메라 %s | 타임라인 %s",
					GPUName,
					getCurrentDisplayHz(), width, height, currentFps, g_sett->maxFps, sett->stopLiveCam ? "수동" : "자동", sett->isLiveTimeManual ? "수동" : "자동");

				ImGui::End();
			}
			


			//Fps graph

			if (g_sett->isShowLiveFPSGraph) {
				ImGui::SetNextWindowPos(ImVec2(width - 600, 0), ImGuiCond_Always); // 오른쪽 상단에 위치
				ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_Always);      // 윈도우 크기 설정



				if (ImGui::Begin("FPS Graph", NULL,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoResize))
				{
					if (ImPlot::BeginPlot("Frame per second")) {
						//ImPlot::SetupAxes(NULL, NULL, flags, flags);
						ImPlot::SetupAxisLimits(ImAxis_X1, t - 10.0f, t, ImGuiCond_Always);
						ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 240.0);
						ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
						ImPlot::PlotLine("FPS", &fpsValues.Data[0].x, &fpsValues.Data[0].y, fpsValues.Data.size(), 0, fpsValues.Offset, 2 * sizeof(float));
						//ImPlot::PlotLine("My Line Plot", x_data, y_data, fpsValues.size());
						ImPlot::EndPlot();
					}

					ImGui::End();
				}
			}

			

		}


		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		if(imgui_settingwnd_open) 
		{
			static float f = 0.0f;
			static int counter = 0;
			

			ImGui::Begin("설정 메뉴 (토글 키: F12)", &imgui_settingwnd_open);                  
			
			
			
			//ImGui::SetNextWindowPos();
			ImGui::Text("Host: %s", g_sett->customHost);

			if (ImGui::CollapsingHeader("게임 시스템", ImGuiTreeNodeFlags_DefaultOpen)) {

				BeginGroupPanel("그래픽");
				ImGui::Checkbox("자동 fps 설정", &g_sett->autoFpsSet); ImGui::SameLine(); HelpMarker("모니터 Hz 수에 맞춰서 fps를 자동 설정합니다.");
				ImGui::SliderInt("FPS 제한", &g_sett->maxFps, 0, 240); ImGui::SameLine(); HelpMarker("제한할 fps를 설정합니다. \"자동 fps 설정\" 이 비활성화 된 경우에만 작동합니다.\n(0인 경우 제한없음)");
				if (ImGui::Button("적용")) {
					set_fps_hook(g_sett->maxFps);
				} ImGui::SameLine(); HelpMarker("fps 제한을 적용합니다.");

				ImGui::Checkbox("강제 가로모드", &g_sett->forceLandscape); ImGui::SameLine(); HelpMarker("게임을 가로 모드로 고정합니다.\n변경 내용은 게임 리셋 시 적용됩니다.");
				ImGui::Checkbox("자동 전체화면", &g_sett->autoFullscreen); ImGui::SameLine(); HelpMarker("가로모드 시 자동으로 전체화면이 됩니다.");
				ImGui::Checkbox("고품질 그래픽", &g_sett->highQuality); ImGui::SameLine(); HelpMarker("안티 에일리어싱, MSAA 허용 등 전반적인 3D 그래픽 품질을 높입니다.\n변경 내용은 게임 리셋 시 적용됩니다.");
				ImGui::SliderFloat("UI 배율", &g_sett->uiScale, 0.1, 10.0, "%.1f"); ImGui::SameLine(); HelpMarker("GUI의 배율을 설정합니다.\n변경 내용은 게임 리셋 시 적용됩니다.");
				
				EndGroupPanel();

				BeginGroupPanel("배속");
				float timescale = get_TimeScale();
				if (ImGui::SliderFloat("게임 배속", &timescale, 0.0, 10.0, "%.4f")) {
					set_TimeScale(timescale);
				}ImGui::SameLine(); HelpMarker("게임의 배속을 설정합니다.\n단축키: LCtrl+PGUP, LCtrl+PGDN");
				if (ImGui::Button("정지"))
					set_TimeScale(0.0);
				ImGui::SameLine();
				HelpMarker("배속을 0.0으로 설정합니다.\n단축키: LCtrl+P");
				ImGui::SameLine();
				if (ImGui::Button("초기화"))
					set_TimeScale(1.0);
				ImGui::SameLine(); HelpMarker("배속을 기본값인 1.0으로 설정합니다.\n단축키: LCtrl+END");
				EndGroupPanel();


				BeginGroupPanel("시스템");
				ImGui::Checkbox("강제 리셋 허용", &g_sett->gotoTitleOnError); ImGui::SameLine(); HelpMarker("게임 내에서 강제적으로 타이틀 화면으로 돌아가는 동작을 허용합니다.");
				if (ImGui::Checkbox("클릭 이펙트 켜기", &g_sett->isTapEffectEnabled))
				{
					if (g_sett->isTapEffectEnabled) {
						GameObject_SetActive("Gallop.GameSystem/SystemManagerRoot/SystemSingleton/UIManager/SystemCanvas/TapEffectCanvas", true);
						
						printf("tapeffect on\n");
					}
					else {
						GameObject_SetActive("Gallop.GameSystem/SystemManagerRoot/SystemSingleton/UIManager/SystemCanvas/TapEffectCanvas", false);
						printf("tapeffect off\n");
					}

				}
				EndGroupPanel();


				
			}
			
			if (ImGui::CollapsingHeader("레이스", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::SliderFloat("등수 표시기 출력 위치", &g_sett->rankUIShowMeter,0.0,3000.0,"%.2f M"); ImGui::SameLine(); HelpMarker("레이스 시작 후 등수 표시기를 출력할 위치를 정합니다.\n(LCtrl+슬라이더 클릭으로 직접 입력 가능)");
				ImGui::SliderFloat("등수 표시기 숨김 위치", &g_sett->rankUIHideoffset,0.0,9999.0,"%.2f"); ImGui::SameLine(); HelpMarker("등수 표시기를 숨길 타이밍을 지정합니다.\n(LCtrl+슬라이더 클릭으로 직접 입력 가능)");
				ImGui::Checkbox("착순 마크 표시", &showFinishOrderFlash); ImGui::SameLine(); HelpMarker("레이스 결과 화면에서 착순 애니메이션을 표시합니다.");
			}

			if (ImGui::CollapsingHeader("라이브", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("라이브 카메라 멈추기", &sett->stopLiveCam); ImGui::SameLine(); HelpMarker("라이브에서 카메라 워크를 정지합니다.\n단축키: S");
				ImGui::Checkbox("라이브 제목 창 출력", &g_sett->showLiveTitleWindow); ImGui::SameLine(); HelpMarker("라이브 시작 시 제목 창을 표시합니다.");
				
				BeginGroupPanel("타임라인");

				//int minutes = static_cast<int>(liveTimeSec / 60); // 분 계산
				//int remainingSeconds = static_cast<int>(seconds) % 60; // 초 계산

				//printf("%02d:%02d", minutes, remainingSeconds);

				ImGui::Text("%02d:%02d / %02d:%02d", static_cast<int>(liveTimeSec) / 60, static_cast<int>(liveTimeSec) % 60,
					static_cast<int>(liveTotalTimeSec) / 60, static_cast<int>(liveTotalTimeSec) % 60);
				
				if (ImGui::SliderFloat("시간 조정", &liveTimeSec, 0.0, liveTotalTimeSec, "%.3f Sec" )) {
					sett->isLiveTimeManual = true;
				}ImGui::SameLine(); HelpMarker("라이브 타임라인을 설정합니다.\n단축키: Ctrl+LeftArrow, Ctrl+RightArrow");
				
				ImGui::PushMultiItemsWidths(1, ImGui::CalcItemWidth());
				ImGui::Indent(5.0f);
				ImGui::PushID(0);
				if (ImGui::Button("+")) {
					sett->isLiveTimeManual = true;
					liveTimeSec = liveTimeSec + liveTimelineManualScale;
				}ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
				if (ImGui::InputScalar("##scale", ImGuiDataType_Float, &liveTimelineManualScale, NULL, NULL, "%.3f")) {}; ImGui::SameLine();
				if (ImGui::Button("-")) {
					sett->isLiveTimeManual = true;
					liveTimeSec = liveTimeSec - liveTimelineManualScale;
				}ImGui::SameLine();
				ImGui::PopID();
				ImGui::PopItemWidth();
				if (ImGui::Checkbox("수동 조정", &sett->isLiveTimeManual)) {
					//ImGui::InsertNotification({ ImGuiToastType_Success, 3000, "Hello World! This is a success! %s", "수동 조정이 활성화되었습니다." });
					//showAlertMessage(5.0, "수동 조정이 활성화되었습니다.");
				} ImGui::SameLine(); HelpMarker("타임라인 수동 조정을 활성화합니다.");
				ImGui::SliderInt("타겟 타임라인 갱신 fps 설정", &liveTimeLineFPS, 0, g_sett->maxFps);
				EndGroupPanel();
				//ImGui::Checkbox("isCameraShake", &IsCamShake);
				
			}
			
			if (ImGui::CollapsingHeader("???", ImGuiTreeNodeFlags_DefaultOpen)) {
				ImGui::Checkbox("키무라 챌린지", &isKimuraChallenge); ImGui::SameLine(); HelpMarker(texture_kimura, kimura_image_width, kimura_image_height, "\"야떼미로-\"");
				ImGui::Checkbox("Urara", &g_sett->walkMotionAllUrara); ImGui::SameLine(); HelpMarker("홈화면 캐릭터들의 도보 모션을 전부 우라라 전용 모션으로 설정합니다.");
				ImGui::Checkbox("◆", &g_sett->homeAllDiamond); ImGui::SameLine(); HelpMarker("Just Diamond.");
				ImGui::Checkbox("Win.564", &g_sett->winMotion564); ImGui::SameLine(); HelpMarker("1착 모션이 캐릭터에 관계없이 전부 골드쉽 모션으로 바뀝니다.\n어이어이, 헛소리 하지 마! 이번에야말로 고루시짱의 시대잖냐아아앗!");
				//int id = 1001;
				//if (ImGui::InputInt("GetCharaNameByCharaId", &id, 1, 9999)) {
				//	//_setmode(_fileno(stdout), _O_U16TEXT); // <=== Windows madness
				//	 
				//	 Il2CppString_unk* ret = (Il2CppString_unk*)GetCharaNameByCharaId(id);
				//	 std::cout << UmaGetString(ret)  << std::endl;
				//	//ImGui::SameLine();
				//	ImGui::Text("%s",  UmaGetString(ret));
				//}
			}
			if (ImGui::CollapsingHeader("디버그 정보")) {
				if (ImGui::TreeNode("Hooked List")) {
					ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
					ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x, 360), false, window_flags);

					
					for_each(hooked_addr.begin(), hooked_addr.end(), [&](hookStr& item) {
						ImGui::Text("[%s]", item.assemblyName);
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s.", item.namespaceName);
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "%s.", item.className);
						ImGui::SameLine();
						ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.0f, 1.0f), "%s(%d)", item.methodName, item.argsCount);
						ImGui::SameLine();
						ImGui::Text(" = 0x%p ", item.addr);
						});
					ImGui::EndChild();
					ImGui::TreePop();
				}
				
				
				

				if (masterDBconnection != nullptr) {
					//std::wstring path = masterDBconnection->dbPath->start_char;
					ImGui::Text("MasterDBConnection ptr: 0x%p", masterDBconnection->Handle);
					//ImGui::Text("MasterDBPath: %s", path);
				}
				else {
					ImGui::Text("MasterDBConnection ptr: nullptr");
					//ImGui::Text("MasterDBPath: nullptr");
				}
				//if (ImGui::Button("SQLTest")) {
				//	Il2CppObject* dbQuery = il2cpp_object_new((Il2CppClass*)il2cpp_symbols::get_class("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query"));
				//	auto query = reinterpret_cast<void (*)(Il2CppObject * _instance, Connection * conn, Il2CppString * querystr)>(il2cpp_symbols::get_method_pointer("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query", "_Setup", 2));
				//	auto query_exec = reinterpret_cast<bool (*)(Il2CppObject * _instance)>(il2cpp_symbols::get_method_pointer("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query", "Exec", 0));
				//	printf("query_exec ok\n");
				//	//auto query_GetText = reinterpret_cast<Il2CppString * (*)(Il2CppObject * _instance, int idx)>(il2cpp_symbols::get_method_pointer("LibNative.Runtime.dll", "LibNative.Sqlite3", "Query", "GetText", 1));
				//	query(dbQuery, masterDBconnection, il2cpp_string_new("update text_data set `text`=\"EditTest\" where id=6 and category=6 and `index`=1001;"));
				//	bool r = query_exec(dbQuery);
				//	//printf("Excuting %s\n", );
				//	printf("Exec = %d\n", r);
				//	//Il2CppString* q_res = query_GetText(dbQuery, 2);
				//	//wprintf(L"GetText = %s\n", q_res->start_char);
				//}
				ImGui::Text("Livecam1 x=% 04.3f, y=% 04.3f, z=% 04.3f", liveCam_pos1.x, liveCam_pos1.y, liveCam_pos1.z);

				//if (liveCam_Lookat != NULL) {
				//	ImGui::Text("liveCam_Lookat x=% 04.3f, y=% 04.3f, z=% 04.3f", liveCam_Lookat.x, liveCam_Lookat.y, liveCam_Lookat.z);
				//	//printf("liveCam_Lookat x=% 04.3f, y=% 04.3f, z=% 04.3f\n", liveCam_Lookat->x, liveCam_Lookat->y, liveCam_Lookat->z);
				//}
				//else {
				//	ImGui::Text("liveCam_Lookat is nullptr");
				//}
				//if (liveCam_pos1 != NULL) {
				//	ImGui::Text("Livecam1 x=% 04.3f, y=% 04.3f, z=% 04.3f", liveCam_pos1.x, liveCam_pos1.y, liveCam_pos1.z);
				//}
				//else {
				//	ImGui::Text("Livecam1 is nullptr");
				//}
				//if (liveCam_pos2 != NULL) {
				//	ImGui::Text("Livecam2 x=% 04.3f, y=% 04.3f, z=% 04.3f", liveCam_pos2.x, liveCam_pos2.y, liveCam_pos2.z);
				//}
				//else {
				//	ImGui::Text("Livecam2 is nullptr");
				//}

				

			}

			
			
			

			
			ImGui::Separator();
			if (ImGui::Button("타이틀로 돌아가기 (게임 리셋)")) {
				ResetGame();
			} ImGui::SameLine(); HelpMarker("단축키: LCtrl+R");

			if (ImGui::Button("게임 종료"))
				ImGui::OpenPopup("확인");
			

			bool open = true;
			if (ImGui::BeginPopupModal("확인", &open))
			{
				ImGui::Text("정말 게임을 종료할까요?");
				if (ImGui::Button("아니오"))
					ImGui::CloseCurrentPopup();
				ImGui::SameLine();
				if (ImGui::Button("예")) {
					exit(0);
				}
					
				ImGui::EndPopup();
			}
			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();


			// Render toasts on top of everything, at the end of your code!
			// You should push style vars here
			

		}

		// 3. Show another simple window.

		if (show_obj_window)
		{
			ImGui::Begin("Obj Window", &show_obj_window);

			//쉔접疳榴써뭐
			static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

			for (int i = 0; i < rootObjList.size(); i++)
			{

				void* currentObj = rootObjList[i];

				//零寧硅땜鮫
				ImGuiTreeNodeFlags node_flags = base_flags;

				//objRecursion(currentObj, base_flags);
				/*
				if (ObjDic[currentObj].children.size() != 0) {
					bool node_open = ImGui::TreeNodeEx(currentObj, node_flags, ObjDic[currentObj].name.c_str());
					if (node_open)
					{
						ImGui::BulletText(to_string(ObjDic[currentObj].children.size()).c_str());
						ImGui::TreePop();
					}
				}
				else {
					node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
					ImGui::TreeNodeEx(currentObj, node_flags, ObjDic[currentObj].name.c_str());
				}
				*/
			}


			ImGui::End();
		}
		// Rendering
		ImGui::Render();
		//const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
		pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		//pContext->ClearRenderTargetView(mainRenderTargetView, clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


		//oPresent(g_pSwapChain, SyncInterval, Flags);
		//g_pSwapChain->Present(1, 0); // Present with vsync
		//g_pSwapChain->Present(0, 0); // Present without vsync
	}

	// Cleanup
	/*ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CleanupDeviceD3D();*/
	/*::DestroyWindow(hwnd);
	::UnregisterClass(wc.lpszClassName, wc.hInstance);*/

	return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &pDevice, &featureLevel, &pContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (pContext) { pContext->Release(); pContext = NULL; }
	if (pDevice) { pDevice->Release(); pDevice = NULL; }
}

void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
	pBackBuffer->Release();
}

void CleanupRenderTarget()
{
	if (mainRenderTargetView) { mainRenderTargetView->Release(); mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (pDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
#pragma endregion



