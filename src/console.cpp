#include "stdinclude.hpp"

namespace
{
	std::vector<std::string> explode(const std::string& str, const char& ch) {
		std::string next;
		std::vector<std::string> result;

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

	void console_thread()
	{
		std::string line;

		while (true)
		{
			std::cin >> line;

			std::cout << "\numamusume_console> " << line << "\n";

			if (line == "reload")
			{
				std::ifstream config_stream{ "config.json" };
				std::vector<std::string> dicts{};

				rapidjson::IStreamWrapper wrapper{ config_stream };
				rapidjson::Document document;

				document.ParseStream(wrapper);

				if (!document.HasParseError())
				{
					
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
				local::reload_textdb(&dicts);
			}
			else if (line == "exit") {
				exit(0);
			}
			else if (line == "crash") {
				raise(SIGSEGV);
			}
			else if (line.rfind("fps",0)==0) {
				std::vector<std::string> arg = explode(line, ':');
				g_max_fps = std::stoi(arg[1].c_str());
				printf("fps limit setted to : %d\n", g_max_fps);
			}
			else if (line == "autofps") {
				g_autofps = !g_autofps;
				printf("Auto fps limit %s\n", g_autofps ? "enabled." : "disabled.");
			}
			else if (line == "useExclusiveFullscreen") {
				g_useExclusiveFullScreen = !g_useExclusiveFullScreen;
				printf("Exclusive Fullscreen mode %s\n", g_useExclusiveFullScreen ? "enabled." : "disabled.");
			}
			else {
				
				printf("%s:Unknown command\n",line.c_str());
			}
		}
	}
}

void start_console()
{
#ifdef _DEBUG
	std::thread(console_thread).detach();
#endif
}

