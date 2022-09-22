#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#define _CRT_SECURE_NO_WARNINGS 1
#define MAX_HORSE_NUM 18

#include <stdinclude.hpp>

HWND currenthWnd = NULL;
bool g_sendserver = true;
int server_port = 80;
char server_ip[256];
DWORD getCurrentDisplayHz();
void DumpHex(const void* data, size_t size);
void (*setExclusiveFullScreen)(int, int, int, int) = nullptr;
void adjust_size(int w = 0, int h=0);
void set_resolution_hook(int, int, bool);
void (*sql_query)(Connection* connection, Il2CppString* sql) = nullptr;
bool (*sql_exec)() = nullptr;
Connection* masterDBconnection = nullptr;
Url lastUrl;
Url currentUrl;
void startThread();


WSADATA wsa;
SOCKET s;
struct sockaddr_in server;

#pragma comment(lib, "ws2_32")

using namespace std;



	
	int race_Currentrank = 0;
	int race_MaxRank = 0;
	bool isLiveStartFlag = false;
	int count = 0;
	float horseMeters[MAX_HORSE_NUM] = {0.1};
	float lastMeter = 0.0;
	bool (*GetKeyDown)(int) = nullptr;
	void (*TapEffect_Disable)(void*);

	string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
		}
		return str;
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
				httplib::Client cli(server_ip, server_port);
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
			bool ret = GetKeyDown(0x73); //S
			if (ret) {
				printf("S is Pressed!!!!\n");
				while (GetKeyDown(0x73)) { }

				c_stopLiveCam = !c_stopLiveCam;
				printf("LiveCam Stop %s.\n",c_stopLiveCam ? "Enabled" : "Disabled");
			}				
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
	int (*ObscuredInt_Decrypted)(ObscuredInt*) = nullptr;
	void* load_library_w_orig = nullptr;
	HMODULE __stdcall load_library_w_hook(const wchar_t* path)
	{
		wprintf(L"loaded %s\n", path);
		// GameAssembly.dll code must be loaded and decrypted while loading criware library
		if (path == L"advapi32"s) {
			path_game_assembly();
		}
		else if (path == L"cri_ware_unity.dll"s)
		{
			currenthWnd = GetActiveWindow();
			char buf[100];
			sprintf_s(buf, "umamusume_cu (%d patch applied)", patchCount);

			SetWindowText(currenthWnd, buf);
			//GameAssembly ´ýÇÁ
			if (g_dumpGamedll) {
				HMODULE ga = GetModuleHandle("GameAssembly.dll");
				if (ga != nullptr) {
					//std::string exe_name = module_filename(NULL);
					printf("Trying to dump GameAssembly.dll...\n");
					pedump(ga, "dumped_GameAssembly.dll");
				}
				ga = nullptr;
			}
			
			HMODULE sq = GetModuleHandle("baselib.dll");
			if (sq != nullptr) {
				//std::string exe_name = module_filename(NULL);
				printf("Trying to dump baselib.dll...\n");
				pedump(sq, "dumped_baselib.dll");
			}
			//sq = nullptr;
			startThread();
			bootstrap_carrot_juicer();
			MH_DisableHook(LoadLibraryW);
			MH_RemoveHook(LoadLibraryW);

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

		return result ? result : orig_result;
	}

	std::unordered_map<void*, bool> text_queries;

	void* query_ctor_orig = nullptr;
	void* query_ctor_hook(void* _this, Connection* conn, Il2CppString* sql)
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

		if (masterDBconnection == nullptr && std::wstring(conn->dbPath->start_char).find(L"master.db")) {
			masterDBconnection = conn;
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
		if (g_autofps) {
			g_max_fps = getCurrentDisplayHz();
			printf("Auto fps limit setted : %d fps\n", g_max_fps);		
		}
		else {
			printf("fps limit setted : %d fps\n", g_max_fps);
		}
		value = g_max_fps;
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

	void* LiveTitleController_Setup_orig = nullptr;
	void LiveTitleController_Setup_hook(void* _this, LiveData* livedata, bool isAdjustForDialog = false){
		//wprintf(L"%s", std::wstring(title->start_char).c_str());
		printf("LiveTitle_Setup: [MusicID=%d, LiveMemberNumber=%d]\n", livedata->MusicId,livedata->LiveMemberNumber);
		return reinterpret_cast<decltype(LiveTitleController_Setup_hook)*>
			(LiveTitleController_Setup_orig)(_this, livedata,isAdjustForDialog);
	}
	void* LiveTitleController_FadeIn_orig = nullptr;
	void LiveTitleController_FadeIn_hook(void* _this, float duration, float currentTime) {
		printf("Title_FadeIn %.5f, %.5f\n", duration, currentTime);
		if (g_showLiveTitleWindow) {
			return reinterpret_cast<decltype(LiveTitleController_FadeIn_hook)*>
				(LiveTitleController_FadeIn_orig)(_this, duration, currentTime);
		}
		return;
	}

	void* LiveTitleController_FadeOut_orig = nullptr;
	void LiveTitleController_FadeOut_hook(void* _this, float duration, float currentTime) {
		//printf("Title_FadeOut %.5f, %.5f\n", duration, currentTime);
		if (g_showLiveTitleWindow) {
			return reinterpret_cast<decltype(LiveTitleController_FadeOut_hook)*>
				(LiveTitleController_FadeOut_orig)(_this, duration, currentTime);
		}		
		return;
	}

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
		race_Currentrank = horseNum-5;
		race_MaxRank = horseNum;
		return reinterpret_cast<decltype(RaceUIRank_Setup_hook)*>
			(RaceUIRank_Setup_orig)(_this, index, indexMax, displayTarget, distanceCheckTarget, horseNum, 
				g_rankUIShowMeter < 0? 0.0f: g_rankUIShowMeter, hideDistance+g_rankUIHideoffset);
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
		if (g_liveCharaAutoDressReplace) {
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
		if (c_gachaCutinChara > -1) {
			/*if (charaId == 9001)
				charaId = 9002;*/
			//else
				charaId = c_gachaCutinChara;
		}
		if (c_gachaCutinDress > -1) {
			/*if (dressId == 900101)
				dressId = 900201;*/
			//else
				dressId = c_gachaCutinDress;
		}
		if (c_gachaCutinHeadid > -1) {
			headId = c_gachaCutinHeadid;
		}

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

		if (c_gachaCutinChara > -1) {
			src->_charaId = c_gachaCutinChara;
		}
		if (c_gachaCutinDress > -1) {
			src->_dressId = c_gachaCutinDress;
		}
		if (c_gachaCutinHeadid > -1) {
			src->_headModelSubId = c_gachaCutinHeadid;
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

		if (c_gachaCutinChara > -1) {
			charaId = c_gachaCutinChara;
		}
		if (c_gachaCutinDress > -1) {
			dressId = c_gachaCutinDress;
		}
		if (c_gachaCutinHeadid > -1) {
			headId = c_gachaCutinHeadid;
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

		if (c_changeStoryChar) 
		{
			if ((c_story3dCharID < 0) || (c_story3dClothID < 0) || (c_story3dHeadID < 0) || (c_story3dMobid < 0))
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
				charaId = c_story3dCharID;
				clothId = c_story3dClothID;
				mobId = c_story3dMobid;
				headId = c_story3dHeadID;
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
		if (strlen(g_customHost) > 0) {
			Url replaceUrl(g_customHost);
			posturl.scheme(replaceUrl.scheme());
			posturl.host(replaceUrl.host());
			posturl.port(replaceUrl.port());
			printf("Replaced URL: %s\n", posturl.str().c_str());
		}										
		
		return reinterpret_cast<decltype(Cute_Http_WWWRequest_Post_hook)*>
			(Cute_Http_WWWRequest_Post_orig)(_this, il2cpp_string_new(posturl.str().c_str()), postdata, headers);
							
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
		if (c_raceResultCutinMotionChara > -1) {
			charaId = c_raceResultCutinMotionChara;
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
	void* RaceResultCutInHelper_LoadBodyMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {
		
		printf("RaceResultCutInHelper_LoadBodyMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n",characterId, dress->Id,rank,grade);
		if (c_raceResultCutinMotionChara > -1) {
			characterId = c_raceResultCutinMotionChara;
			dress->CharaId = c_raceResultCutinMotionChara;
		}
		if (c_raceResultCutinMotionDress > -1) {
			dress->Id = c_raceResultCutinMotionDress;
		}
		if (c_raceResultCutinMotionGrade > -1) {
			grade = c_raceResultCutinMotionGrade;
		}
		if (c_raceResultCutinMotionRank > -1) {
			rank = c_raceResultCutinMotionRank;
		}
		
		//return nullptr;
		return reinterpret_cast<decltype(RaceResultCutInHelper_LoadBodyMotion_hook)*>
			(RaceResultCutInHelper_LoadBodyMotion_orig)(characterId, dress, personalityType, rank, grade, raceType);
	}

	void* RaceResultCutInHelper_LoadCameraMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadCameraMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {
		printf("RaceResultCutInHelper_LoadCameraMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		//return nullptr;
		if (c_raceResultCutinMotionChara > -1) {
			characterId = c_raceResultCutinMotionChara;
			dress->CharaId = c_raceResultCutinMotionChara;
		}
		if (c_raceResultCutinMotionDress > -1) {
			dress->Id = c_raceResultCutinMotionDress;
		}
		if (c_raceResultCutinMotionGrade > -1) {
			grade = c_raceResultCutinMotionGrade;
		}
		if (c_raceResultCutinMotionRank > -1) {
			rank = c_raceResultCutinMotionRank;
		}
		return reinterpret_cast<decltype(RaceResultCutInHelper_LoadCameraMotion_hook)*>
			(RaceResultCutInHelper_LoadCameraMotion_orig)(characterId, dress, personalityType, rank, grade, raceType);
	}

	void* RaceResultCutInHelper_LoadEarMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadEarMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {
		printf("RaceResultCutInHelper_LoadEarMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		//return nullptr;
		if (c_raceResultCutinMotionChara > -1) {
			characterId = c_raceResultCutinMotionChara;
			dress->CharaId = c_raceResultCutinMotionChara;
		}
		if (c_raceResultCutinMotionDress > -1) {
			dress->Id = c_raceResultCutinMotionDress;
		}
		if (c_raceResultCutinMotionGrade > -1) {
			grade = c_raceResultCutinMotionGrade;
		}
		if (c_raceResultCutinMotionRank > -1) {
			rank = c_raceResultCutinMotionRank;
		}
		return reinterpret_cast<decltype(RaceResultCutInHelper_LoadEarMotion_hook)*>
			(RaceResultCutInHelper_LoadEarMotion_orig)(characterId, dress, personalityType, rank, grade, raceType);
	}

	void* RaceResultCutInHelper_LoadFacialMotion_orig = nullptr;
	void* RaceResultCutInHelper_LoadFacialMotion_hook(int characterId, MasterDressData* dress, int personalityType, int rank, int grade, int raceType) {
		printf("RaceResultCutInHelper_LoadFacialMotion: charaId=%d,dressid=%d,rank=%d,grade=%d\n", characterId, dress->Id, rank, grade);
		//return nullptr;
		if (c_raceResultCutinMotionChara > -1) {
			characterId = c_raceResultCutinMotionChara;
			dress->CharaId = c_raceResultCutinMotionChara;
		}
		if (c_raceResultCutinMotionDress > -1) {
			dress->Id = c_raceResultCutinMotionDress;
		}
		if (c_raceResultCutinMotionGrade > -1) {
			grade = c_raceResultCutinMotionGrade;
		}
		if (c_raceResultCutinMotionRank > -1) {
			rank = c_raceResultCutinMotionRank;
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

	void* RaceSkillCutInHelper_InitForGacha_orig = nullptr;
	void RaceSkillCutInHelper_InitForGacha_hook(void* _this,void* owner) {
		printf("RaceSkillCutInHelper_InitForGacha called\n");

		return reinterpret_cast<decltype(RaceSkillCutInHelper_InitForGacha_hook)*>
			(RaceSkillCutInHelper_InitForGacha_orig)(_this,owner);

	}

	void* Gallop_Cutin_CutinCharacter_ctor_orig = nullptr;
	void* Gallop_Cutin_CutinCharacter_ctor_hook(void*_this, CutInCharacterCreateInfo* createInfo) {
		printf("CutinCharacter type=%d charaid=%d dressid=%d, headid=%d, is어쩌구=%d, index=%d\n",
			createInfo->_characterType,createInfo->_charaId,createInfo->_clothId,createInfo->_headId, createInfo->IsUseDressDataHeadModelSubId,createInfo->_charaIndex);
		if (c_gachaCharaType > -1) {
			createInfo->_characterType = c_gachaCharaType;
		}
		if (c_gachaCutinChara > -1) {
			createInfo->_charaId = c_gachaCutinChara;			
		}
		if (c_gachaCutinDress > -1) {
			createInfo->_clothId = c_gachaCutinDress;
		}
		if (c_gachaCutinHeadid > -1) {
			createInfo->_headId = c_gachaCutinHeadid;
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

		if (c_gachaCharaType > -1) {
			info->_characterType = c_gachaCharaType;
		}
		if (c_gachaCutinChara > -1) {
			info->_charaId = c_gachaCutinChara;
		}
		if (c_gachaCutinDress > -1) {
			info->_clothId = c_gachaCutinDress;
		}
		if (c_gachaCutinHeadid > -1) {
			info->_headId = c_gachaCutinHeadid;
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
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_LiveTimelineCamera_AlterUpdate_hook)*>
				(Live_LiveTimelineCamera_AlterUpdate_orig)(_this, liveTime);
		}
		
	}

	void* Live_Cutt_AlterUpdate_CameraLookAt_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraLookAt_hook(void* _this, void* sheet, int currentFrame, Vector3_t* outLookAt) {
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraLookAt_hook)*>
				(Live_Cutt_AlterUpdate_CameraLookAt_orig)(_this, sheet, currentFrame, outLookAt);
		}
		//printf("AlterUpdate_CameraLookAt x=%.2f y=%.2f z=%.2f CurrentFrame=%d\n", outLookAt->x, outLookAt->y, outLookAt->z, currentFrame);
		
	}

	void* Live_Cutt_AlterUpdate_CameraPos_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraPos_hook(void* _this, void* sheet, int currentFrame, int sheetIndex, bool isUseCameraMotion) {
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraPos_hook)*>
				(Live_Cutt_AlterUpdate_CameraPos_orig)(_this, sheet, currentFrame, sheetIndex, isUseCameraMotion);
		}
		//printf("AlterUpdate_CameraPos sheetIndex=%d, CurrentFrame=%d\n",sheetIndex, currentFrame);
		
	}

	void* Live_Cutt_AlterUpdate_CameraSwitcher_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraSwitcher_hook(void* _this, void* sheet, int currentFrame) {
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraSwitcher_hook)*>
				(Live_Cutt_AlterUpdate_CameraSwitcher_orig)(_this, sheet, currentFrame);
		}
		//printf("AlterUpdate_CameraSwitcher CurrentFrame=%d\n", currentFrame);
		
	}

	void* Live_Cutt_AlterUpdate_CameraLayer_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraLayer_hook(void* _this, void* sheet, int currentFrame, Vector3_t* offsetMaxPosition, Vector3_t* offsetMinPosition) {
		if (!c_stopLiveCam) {
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
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraMotion_hook)*>
				(Live_Cutt_AlterUpdate_CameraMotion_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterUpdate_CameraRoll_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraRoll_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_CameraRoll CurrentFrame=%d\n", currentFrame);
		if (!c_stopLiveCam) {
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

	void* Live_Cutt_AlterUpdate_MultiCamera_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCamera_hook(void* _this, void* sheet, int currentFrame) {
		printf("AlterUpdate_MultiCamera CurrentFrame=%d \n", currentFrame);
		return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCamera_hook)*>
			(Live_Cutt_AlterUpdate_MultiCamera_orig)(_this, sheet,currentFrame);
	}

	void* Live_Cutt_AlterUpdate_CameraFov_orig = nullptr;
	void Live_Cutt_AlterUpdate_CameraFov_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_CameraFov CurrentFrame=%d \n", currentFrame);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_CameraFov_hook)*>
				(Live_Cutt_AlterUpdate_CameraFov_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterLateUpdate_CameraMotion_orig = nullptr;
	bool Live_Cutt_AlterLateUpdate_CameraMotion_hook(void* _this, void* sheet, int currentFrame) {
		/*bool ret = reinterpret_cast<decltype(Live_Cutt_AlterLateUpdate_CameraMotion_hook)*>
			(Live_Cutt_AlterLateUpdate_CameraMotion_orig)(_this, sheet, currentFrame);*/

		//printf("AlterLateUpdate_CameraMotion CurrentFrame=%d, ret=%d\n", currentFrame,ret);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterLateUpdate_CameraMotion_hook)*>
				(Live_Cutt_AlterLateUpdate_CameraMotion_orig)(_this, sheet, currentFrame);
		}
		
			return true;
		

	}

	void* Live_Cutt_AlterUpdate_EyeCameraLookAt_orig = nullptr;
	void Live_Cutt_AlterUpdate_EyeCameraLookAt_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_EyeCameraLookAt CurrentFrame=%d \n", currentFrame);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_EyeCameraLookAt_hook)*>
				(Live_Cutt_AlterUpdate_EyeCameraLookAt_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterUpdate_EyeCameraPosition_orig = nullptr;
	void Live_Cutt_AlterUpdate_EyeCameraPosition_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_EyeCameraLookAt CurrentFrame=%d \n", currentFrame);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_EyeCameraPosition_hook)*>
				(Live_Cutt_AlterUpdate_EyeCameraPosition_orig)(_this, sheet, currentFrame);
		}
		
	}

	void* Live_Cutt_AlterUpdate_orig = nullptr;
	void Live_Cutt_AlterUpdate_hook(void* _this, float liveTime) {
		//printf("AlterUpdate time=%.2f \n", liveTime);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_hook)*>
				(Live_Cutt_AlterUpdate_orig)(_this, liveTime);
		}
	}

	void* Live_Cutt_AlterUpdate_MultiCameraLookAt_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraLookAt_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_MultiCameraLookAt currentFrame=%d \n", currentFrame);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCameraLookAt_hook)*>
				(Live_Cutt_AlterUpdate_MultiCameraLookAt_orig)(_this, sheet,currentFrame);
		}
	}

	void* Live_Cutt_AlterUpdate_MultiCameraPosition_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraPosition_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_MultiCameraPosition currentFrame=%d \n", currentFrame);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCameraPosition_hook)*>
				(Live_Cutt_AlterUpdate_MultiCameraPosition_orig)(_this, sheet, currentFrame);
		}
	}

	void* Live_Cutt_AlterUpdate_MultiCameraTiltShift_orig = nullptr;
	void Live_Cutt_AlterUpdate_MultiCameraTiltShift_hook(void* _this, void* sheet, int currentFrame) {
		//printf("AlterUpdate_MultiCameraTiltShift currentFrame=%d \n", currentFrame);
		if (!c_stopLiveCam) {
			return reinterpret_cast<decltype(Live_Cutt_AlterUpdate_MultiCameraTiltShift_hook)*>
				(Live_Cutt_AlterUpdate_MultiCameraTiltShift_orig)(_this, sheet, currentFrame);
		}
	}

	void* Live_Cutt_AlterUpdate_PostEffect_DOF_orig = nullptr;
	void Live_Cutt_AlterUpdate_PostEffect_DOF_hook(void* _this, void* sheet, int currentFrame, Vector3_t* cameraLookAt) {
		
		//printf("AlterUpdate_PostEffect_DOF currentFrame=%d pos x=%.2f y=%.2f z=%.2f \n", currentFrame,
			//cameraLookAt->x,cameraLookAt->y,cameraLookAt->z);
		if (!c_stopLiveCam) {
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
		//return reinterpret_cast<decltype(GallopUtil_GotoTitleOnError_hook)*>
		//	(GallopUtil_GotoTitleOnError_orig)(text);
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
		printf("%p\n", _this);
		auto ssql = std::wstring(name->start_char);

		wprintf(L"new GameObject name=%s\n", ssql.c_str());
		return reinterpret_cast<decltype(UnityEngine_GameObject_ctor_hook)*>
			(UnityEngine_GameObject_ctor_orig)(_this,name);
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

		if (strlen(g_customDataPath) > 0) {
			Il2CppString* custompath = il2cpp_string_new(g_customDataPath);
			wprintf(L"originalpersistentpath=%s, replacedpersistentpath=%s\n", ret->start_char, custompath->start_char);
			return custompath;
		}		
		
		return ret;
	}

	void* unityengine_get_persistentDataPath_orig = nullptr;
	Il2CppString* unityengine_get_persistentDataPath_hook() {
		Il2CppString* ret = reinterpret_cast<decltype(unityengine_get_persistentDataPath_hook)*>
			(unityengine_get_persistentDataPath_orig)();

		if (strlen(g_customDataPath) > 0) {
			Il2CppString* custompath = il2cpp_string_new(g_customDataPath);
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

	void get_resolution_stub(Resolution_t* r)
	{
		*r = *get_resolution(r);

		int width = min(r->height, r->width) * g_aspect_ratio;
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
		set_scale_factor(_this, max(1.0f, r.width / 1920.f) * g_ui_scale);

		return reinterpret_cast<decltype(canvas_scaler_setres_hook)*>(canvas_scaler_setres_orig)(_this, res);
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
		Resolution_t r;
		r = *get_resolution(&r);
		
		bool need_fullscreen = false;

		if (is_virt() && r.width / static_cast<double>(r.height) == (9.0 / 16.0))
			need_fullscreen = true;
		else if (!is_virt() && r.width / static_cast<double>(r.height) == (16.0 / 9.0))
			need_fullscreen = true;

		
		
		if (need_fullscreen) {
			if (g_useExclusiveFullScreen) {
				if (g_exclusiveFullScreenWidth > 0 && g_exclusiveFullScreenHeight > 0) {
					
					setExclusiveFullScreen(g_exclusiveFullScreenWidth, g_exclusiveFullScreenHeight, FullScreenMode::ExclusiveFullScreen, g_max_fps);
					Sleep(1000); 
				}
				else {
					
					setExclusiveFullScreen(r.width, r.height, FullScreenMode::ExclusiveFullScreen, g_max_fps);
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
			"Query", ".ctor", 2
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

		auto set_fps_addr = il2cpp_symbols::get_method_pointer(
			"UnityEngine.CoreModule.dll", "UnityEngine",
			"Application", "set_targetFrameRate", 1
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

		auto set_virt_addr = reinterpret_cast<void(*)(bool)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"StandaloneWindowResize", "set_IsVirt", 1
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

		auto LiveTitleController_Setup_addr = reinterpret_cast<void(*)(void*, LiveData*,bool)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live",
				"LiveTitleController", "Setup", 2
			));
		auto LiveTitleController_FadeIn_addr = reinterpret_cast<void(*)(void*,float,float)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live",
				"LiveTitleController", "FadeIn", 2
			));

		auto LiveTitleController_FadeOut_addr = reinterpret_cast<void(*)(void*, float, float)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop.Live",
				"LiveTitleController", "FadeOut", 2
			));

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

		auto RaceUIRank_Setup_addr = reinterpret_cast<void(*)(void*,int,int,void*,void*,int,float,float)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceUIRank", "Setup", 7
			));

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

		auto load_scene_internal_addr = il2cpp_resolve_icall("UnityEngine.SceneManagement.SceneManager::LoadSceneAsyncNameIndexInternal_Injected(System.String,System.Int32,UnityEngine.SceneManagement.LoadSceneParameters&,System.Boolean)");

		printf("icall %p\n", load_scene_internal_addr);

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
				"RaceResultCutInHelper", "LoadBodyMotion", 6
			)
		);


		auto RaceResultCutInHelper_LoadCameraMotion_addr = reinterpret_cast<void* (*)(int, MasterDressData*, int, int, int, int)>(
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"RaceResultCutInHelper", "LoadCameraMotion", 6
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

		auto RaceSkillCutInHelper_InitForGacha_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop",
			"RaceSkillCutInHelper", "InitForGacha", 1
		);

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
			"LiveTimelineControl", "AlterUpdate_CameraLookAt", 3
		);

		auto Live_Cutt_AlterUpdate_CameraPos_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_CameraPos", 4
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

		auto Live_Cutt_AlterUpdate_MultiCamera_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCamera", 2
		);

		auto Live_Cutt_AlterUpdate_EyeCameraPosition_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_EyeCameraPosition", 2
		);

		auto Live_Cutt_AlterUpdate_EyeCameraLookAt_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_EyeCameraLookAt", 2
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
			"LiveTimelineControl", "AlterUpdate_MultiCameraLookAt", 2
		);

		auto Live_Cutt_AlterUpdate_MultiCameraPosition_addr = il2cpp_symbols::get_method_pointer(
			"umamusume.dll", "Gallop.Live.Cutt",
			"LiveTimelineControl", "AlterUpdate_MultiCameraPosition", 2
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

		gameObject_setActive = reinterpret_cast<void(*)(void*, bool)>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"GameObject", "SetActive", 1
				)
			);

		GetKeyDown = reinterpret_cast<bool(*)(int)> (
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.InputLegacyModule.dll", "UnityEngine",
				"Input", "GetKeyDown", 1
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
		
		auto unityengine_get_persistentDataPath_addr = reinterpret_cast<Il2CppString*(*)()>(
			il2cpp_symbols::get_method_pointer(
				"UnityEngine.CoreModule.dll", "UnityEngine",
				"Application", "get_persistentDataPath", 0
			));

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

		/*auto GachaBGController_GateDoor_SetRarity_addr =
			il2cpp_symbols::get_method_pointer(
				"umamusume.dll", "Gallop",
				"GachaBGController+GateDoor","SetRarity",1
			);*/

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
		ADD_HOOK(GachaBGController_OverrideRarity, "Gallop.GachaBGController.OverrideRarity(int, int(enum)) at %p\n");
		ADD_HOOK(GachaBGController_SetGateDoorRarity, "Gallop.GachaBGController.SetRarity(int) at %p\n");
		ADD_HOOK(LiveTitleController_Setup, "Gallop.Live.LiveTitleController.Setup at %p\n");
		ADD_HOOK(LiveTitleController_FadeIn, "Gallop.Live.LiveTitleController.FadeIn(float,float) at %p\n");
		ADD_HOOK(LiveTitleController_FadeOut, "Gallop.Live.LiveTitleController.FadeOut(float,float) at %p\n");
		//ADD_HOOK(Unity_Post, "Unity_Post at %p\n");
		ADD_HOOK(RaceUI_SetRaceUIActive, "RaceUI.SetRaceUIActive(bool) at %p\n");
		ADD_HOOK(RaceUI_SetVisibleRank, "RaceUI.SetVisibleRank(bool) at %p\n");
		ADD_HOOK(RaceUIRank_Setup, "RaceUIRank.Setup(7) at %p\n");
		ADD_HOOK(RaceUIRank_PlayPlayerRankUp, "RaceUIRank.PlayPlayerRankUp() at %p\n");
		ADD_HOOK(RaceUIRank_PlayPlayerRankDown, "RaceUIRank.PlayPlayerRankDown() at %p\n");
		ADD_HOOK(RaceManager_GetHorseDistanceByIndex, "RaceManager.GetHorseDistanceByIndex(int) at %p\n");
		ADD_HOOK(LiveTheaterInfo_GetDefaultDressid, "LiveTheaterInfo_GetDefaultDressid(...) at %p\n");
		ADD_HOOK(LiveTheaterInfo_UpdateCharaDressIds, "LiveTheaterInfo_UpdateCharaDressIds(LiveTheaterMemberInfo[]) at %p\n");
		ADD_HOOK(LiveTheaterInfo_CheckDress, "LiveTheaterInfo_CheckDress(int,CharaDressIdSet) at %p\n");
		ADD_HOOK(RaceResultScene_GetMotionVariationId, "RaceResultScene_GetMotionVariationId(int) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadBodyMotion, "RaceResultCutInHelper_LoadBodyMotion(...5) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadCameraMotion, "RaceResultCutInHelper_LoadCameraMotion(...5) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadEarMotion, "RaceResultCutInHelper_LoadEarMotion(...5) at %p\n");
		ADD_HOOK(RaceResultCutInHelper_LoadFacialMotion, "RaceResultCutInHelper_LoadFacialMotion(...5) at %p\n");
		ADD_HOOK(ResourcePath_GetCharacterRaceResultMotionPath,"ResourcePath_GetCharacterRaceResultMotionPath at %p\n")
		ADD_HOOK(RaceResultScene_PlayFinishOrderAnim, "RaceResultScene_PlayFinishOrderAnim(Action) at %p\n");
		//ADD_HOOK(RaceResultCutInHelper_GetModelController, "RaceResultCutInHelper_GetModelController(..) at %p");
		//ADD_HOOK(RaceResultCutInHelper_GetResultCuttCueId, "RaceResultCutInHelper_GetResultCuttCueId(int,int,int*) at %p");
		ADD_HOOK(RaceSkillCutInHelper_PreInstantiateCharaUser, "RaceSkillCutInHelper_PreInstantiateCharaFixed at %p\n");
		ADD_HOOK(RaceSkillCutInHelper_InitForGacha, "RaceSkillCutInHelper_InitForGacha at %p\n");
		ADD_HOOK(Gallop_Cutin_CutinCharacter_ctor, "Gallop_Cutin_CutinCharacter_.ctor at %p\n");
		ADD_HOOK(set_virt, "set_virt at %p\n");
		printf("CustomHost:%s\n",g_customHost);	
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
		ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraLookAt, "Live_Cutt_AlterUpdate_MultiCameraLookAt at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraPosition, "Live_Cutt_AlterUpdate_MultiCameraPosition at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_MultiCameraTiltShift, "Live_Cutt_AlterUpdate_MultiCameraTiltShift at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_PostEffect_DOF, "Live_Cutt_AlterUpdate_PostEffect_DOF at %p\n");
		ADD_HOOK(Live_Cutt_AlterLateUpdate_CameraMotion, "Live_Cutt_AlterLateUpdate_CameraMotion at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_EyeCameraPosition, "Live_Cutt_AlterUpdate_EyeCameraPosition at %p\n");
		ADD_HOOK(Live_Cutt_AlterUpdate_EyeCameraLookAt, "Live_Cutt_AlterUpdate_EyeCameraLookAt at %p\n");
		ADD_HOOK(GameSystem_SoftwareReset, " GameSystem_SoftwareReset at %p\n");
		ADD_HOOK(GallopUtil_GotoTitleOnError, "GallopUtil_GotoTitleOnError at %p\n");
		ADD_HOOK(TapEffectController_ctor, "TapEffectController_ctor at %p\n");
		ADD_HOOK(Cute_Core_Device_GetPersistentDataPath, "Cute_Core_Device_GetPersistentDataPath at %p\n");
		//ADD_HOOK(unityengine_get_persistentDataPath, "unityengine_get_persistentDataPath at %p\n");
		//ADD_HOOK(unity_font_ctor, "unity_font_ctor(string) at %p\n");
		//ADD_HOOK(assetbundleHelper_IsNeedManifestSetup, "assetbundleHelper_IsNeedManifestSetup at %p\n");
		//ADD_HOOK(assetbundleHelper_IsExistGallopResources, "assetbundleHelper_IsExistGallopResources at %p\n");
		//ADD_HOOK(UnityEngine_GameObject_ctor, "UnityEngine_GameObject_ctor(string) at %p\n");
		//ADD_HOOK(GetKeyDown, "UnityEngine.Input.GetKeyDown at %p\n");
		//ADD_HOOK(Unity_KeyboardEvent, "Unity_KeyboardEvent at %p\n");
		//ADD_HOOK(Live_Cutt_AlterUpdate, "Live_Cutt_AlterUpdate at %p\n");
		//ADD_HOOK(Race_GetRankNumSmallPath, "AtlasSpritePath.Race.GetRankNumSmallPath(int) at %p");
		//ADD_HOOK(GachaBGController_GateDoor_SetRarity, "Gallop.GachaBGController.GateDoor.SetRarity(int(enum)) at %p\n");
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
			//ADD_HOOK(setExclusiveFullScreen, "UnityEngine.Screen.SetResolution(int, int, int(enum),int) at %p\n");
			adjust_size();
		}
		
		if (g_dump_entries)
			dump_all_entries();

		
	}

	void startThread() {
		std::thread t1(meterDataSendThread);
		t1.detach();
		printf("Horse data send Thread started\n");

		std::thread t2(keyDownCheckThread);
		t2.detach();
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
		
		if (g_saveMsgPack) {

			auto out_path = std::string("CarrotJuicer\\").append(current_time()).append(ReplaceAll(currentUrl.path(),"/","_")).append("R.msgpack");
			write_file(out_path, decrypted, ret);
			printf("wrote response to %s\n", out_path.c_str());
		}
			
		
		if (g_sendserver) {
			printf("------Real Cygames server -> modding server(local) -> Client------\n");
			httplib::Headers headers = {
				{ "Url", lastUrl.str() }
			};
			
			httplib::Client cli(server_ip,server_port);
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
		if (g_sendserver) {
			httplib::Client cli(server_ip, server_port);
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
			if (g_saveMsgPack) {
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
			if (g_lz4Encrypt) {
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
			if (g_saveMsgPack) {
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
		if (g_saveMsgPack) {
			std::filesystem::create_directory("CarrotJuicer");
		}

		

		auto libnative_module = GetModuleHandle("libnative.dll");
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