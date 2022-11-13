#include "stdinclude.hpp"

extern void(*setExclusiveFullScreen)(int, int, int, int);
extern void adjust_size(int w = 0, int h = 0);
extern void set_resolution_hook(int, int, bool);
extern void(*sql_query)(Connection* connection, Il2CppString* sql);
extern bool(*sql_exec)();
extern Connection* masterDBconnection;

namespace {
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

    void console_thread() {
        std::string line;

        while (true) {
            std::getline(std::cin, line);

            std::cout << "\numamusume_console> " << line << "\n";

            if (line == "reload") {
                std::ifstream config_stream{
                  "config.json"
                };
                std::vector < std::string > dicts{};

                rapidjson::IStreamWrapper wrapper{
                  config_stream
                };
                rapidjson::Document document;

                document.ParseStream(wrapper);

                if (!document.HasParseError()) {

                    auto& dicts_arr = document["dicts"];
                    auto len = dicts_arr.Size();

                    for (size_t i = 0; i < len; ++i) {
                        auto dict = dicts_arr[i].GetString();

                        dicts.push_back(dict);
                    }
                }
                else {
                    MessageBox(NULL, L"Config.json parse error", L"Error", MB_OK | MB_ICONERROR);
                }

                config_stream.close();
                local::reload_textdb(&dicts);
            }
            else if (line == "exit") {
                exit(0);
            }
            else if (line == "crash") {
                raise(SIGSEGV);
            }
            else if (line.rfind("fps", 0) == 0) {
                std::vector < std::string > arg = explode(line, ' ');
                try {
                    g_max_fps = std::stoi(arg.at(1).c_str());
                    printf("fps limit setted to : %d\n", g_max_fps);
                }
                catch (std::invalid_argument&) {
                    printf("Fps Value Error: please enter number only\n");
                }
                catch (std::out_of_range&) {
                    printf("Argument Error: please enter the fps value\n");
                }

            }
            else if (line == "autofps") {
                g_autofps = !g_autofps;
                printf("Auto fps limit %s\n", g_autofps ? "enabled." : "disabled.");
            }
            else if (line == "useExclusiveFullscreen") {
                g_useExclusiveFullScreen = !g_useExclusiveFullScreen;
                printf("Exclusive Fullscreen mode %s\n", g_useExclusiveFullScreen ? "enabled." : "disabled.");
            }
            else if (line == "resetFullscreen") {
                if (!g_useExclusiveFullScreen) {
                    printf_s("Error: Exclusive fullscreen mode is not enabled.\n");
                }
                else {

                    //setExclusiveFullScreen(g_exclusiveFullScreenWidth, g_exclusiveFullScreenHeight, ExclusiveFullScreen, g_max_fps);
                    adjust_size(g_exclusiveFullScreenWidth, g_exclusiveFullScreenHeight);

                }

            }
            else if (line.starts_with("setScreen")) {
                std::vector < std::string > arg = explode(line, ' ');
                try {
                    int weight = std::stoi(arg.at(1).c_str());
                    int height = std::stoi(arg.at(2).c_str());
                    //int windowmode = std::stoi(arg.at(3).c_str());
                    //int fps = std::stoi(arg.at(4).c_str());
                    printf_s("Set Screen: %dx%d %dfps, screenMode %d\n", weight, height);

                    //set_resolution_hook(weight, height,false);
                    adjust_size(weight, height);
                }
                catch (std::invalid_argument&) {
                    printf("Value Error: please enter number only\n");
                }
                catch (std::out_of_range&) {
                    printf_s("Argument Error:\nUsage: setScreen {weight} {height} {windowmode} {fps}\n");
                }
            }
            else if (line.starts_with("sql")) {
                Il2CppString* q = il2cpp_string_new("update text_data set `text`=\"Test01\" where `id`=16 and `category`=16 and `index`=1001");
                sql_query(masterDBconnection, q);
                //sql_exec();
            }
            else if (line.starts_with("cutinCharSet")) {
                std::vector < std::string > arg = explode(line, ' ');
                try {
                    int type = std::stoi(arg.at(1).c_str());
                    int charaId = std::stoi(arg.at(2).c_str());
                    int dressId = std::stoi(arg.at(3).c_str());
                    int headid = std::stoi(arg.at(4).c_str());
                    printf_s("Set cutin character: Type=%d, CharaID=%d, DressID=%d, HeadID=%d\n", type, charaId, dressId, headid);
                    c_gachaCharaType = (TimelineKeyCharacterType)type;
                    c_gachaCutinChara = charaId;
                    c_gachaCutinDress = dressId;
                    c_gachaCutinHeadid = headid;
                    //set_resolution_hook(weight, height,false);
                    //adjust_size(weight, height);
                }
                catch (std::invalid_argument&) {
                    printf("Value Error: please enter number only\n");
                }
                catch (std::out_of_range&) {
                    printf_s("Argument Error:\nUsage: cutinCharSet {type} {Character id} {Dress id} {Head id}\n");
                }
            }
            else if (line.starts_with("cutinRaceCharMotionSet")) {
                std::vector < std::string > arg = explode(line, ' ');
                try {

                    int charaId = std::stoi(arg.at(1).c_str());
                    int dressId = std::stoi(arg.at(2).c_str());
                    int grade = std::stoi(arg.at(3).c_str());
                    int rank = std::stoi(arg.at(4).c_str());
                    printf_s("Set raceresult cutin motion: CharaID=%d, DressID=%d, RaceGrade=%d, ResultRank=%d\n", charaId, dressId, grade, rank);
                    c_raceResultCutinMotionChara = charaId;
                    c_raceResultCutinMotionDress = dressId;
                    c_raceResultCutinMotionGrade = grade;
                    c_raceResultCutinMotionRank = rank;
                }
                catch (std::invalid_argument&) {
                    printf("Value Error: please enter number only\n");
                }
                catch (std::out_of_range&) {
                    printf_s("Argument Error:\nUsage: cutinRaceCharMotionSet {Character id} {Dress id} {Race Grade(G1:100,G2:200, G3:300)} {Result Rank}\n");
                }
            }
            else if (line.starts_with("resetCutin")) {
                printf_s("Reset all cutin character currently setted.\n");
                c_gachaCharaType = (TimelineKeyCharacterType)-1;
                c_gachaCutinChara = -1;
                c_gachaCutinDress = -1;
                c_gachaCutinHeadid = -1;
            }
            else if (line.starts_with("stopLiveCam")) {
                printf_s("%s live camara\n", c_stopLiveCam ? "Restored" : "Stopped");
                c_stopLiveCam = !c_stopLiveCam;
            }
            else if (line.starts_with("skipResDL")) {
                printf_s("%s Resource Download\n", g_skipResourceDownload ? "Skip" : "Enable");
                g_skipResourceDownload = !g_skipResourceDownload;
            }
            else if (line.starts_with("story3dCharSetEnable")) {
                c_changeStoryChar = !c_changeStoryChar;
                printf("StoryChar3D change mode %s\n", c_changeStoryChar ? "enabled." : "disabled.");
            }
            else if (line.starts_with("story3dCharSet")) {
                std::vector < std::string > arg = explode(line, ' ');
                try {

                    int charaId = std::stoi(arg.at(1).c_str());
                    int dressId = std::stoi(arg.at(2).c_str());
                    int mobid = std::stoi(arg.at(3).c_str());
                    int headid = std::stoi(arg.at(4).c_str());
                    printf_s("Set Story3d model : CharaID=%d, DressID=%d, MobID=%d, HeadID=%d\n", charaId, dressId, mobid, headid);
                    c_story3dCharID = charaId;
                    c_story3dClothID = dressId;
                    c_story3dMobid = mobid ;
                    c_story3dHeadID = headid;
                }
                catch (std::invalid_argument&) {
                    printf("Value Error: please enter number only\n");
                }
                catch (std::out_of_range&) {
                    printf_s("Argument Error:\nUsage: story3dCharSet {Character id} {Dress id} {MobID} {HeadID}\n");
                }
            }
            else if (line.starts_with("setCardId")) {
                try {
                    std::vector < std::string > arg = explode(line, ' ');
                    int cardId = std::stoi(arg.at(1).c_str());
                    g_cardid = cardId;
                    printf_s("Set GetCardid ret: %d\n", cardId);
                }
                catch (std::invalid_argument&) {
                    printf_s("Value Error: please enter number only\n");
                }
                catch (std::out_of_range&) {
                    printf_s("Argument Error:\nUsage: setCardId {Card id}\n");
                }
               
            }
            else if (line.starts_with("reset")) {
                printf("Reset game!\n");
                ResetGame();
            }
            else {
                printf("%s:Unknown command\n", line.c_str());
            }
        }
    }
}

void start_console() {
#ifdef _DEBUG
    std::thread(console_thread).detach();
#endif
}