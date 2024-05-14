#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include<fstream>

class Logs
{
public:

    struct LogSetting
    {
        std::string logFilePath;
        bool logFile = true;
    } logSetting;

    Logs() = default;

    template<typename... Args>
    void Log(const std::string& firstArg, const Args&... args)
    {    
        std::string result = firstArg;
        processVars(args...);

        int idx = result.find("{$}");
        while(idx != -1)
        {
            if (logItems.empty())
            {
                idx = -1;
                continue;
            }

            std::string tmp = logItems.front();
            logItems.pop();

            result.replace(idx, 3, tmp.c_str());

            idx = result.find("{$}");
        }

        std::cout << result << std::endl;
        if (logSetting.logFile)
        {
            std::fstream f;
            f.open(logSetting.logFilePath , std::ios::out|std::ios::app);
            f << result << std::endl;
            f.close();
        }
    }
private:
    template<typename T>
    void processVars(const T& firstArg) 
    {
        std::stringstream ss;
        ss << firstArg;
        logItems.push(ss.str());
    }

    template<typename T, typename... Args>
    void processVars(const T& firstArg, const Args&... args) {
        std::stringstream ss;
        ss << firstArg;
        logItems.push(ss.str());
        processVars(args...);
    }

    std::queue<std::string> logItems;
};

int main() {
    Logs log;
    log.logSetting.logFilePath = "C:/Users/wuchenglin/Desktop/test/test.log";

    log.Log("{$} is a stupid guy, which has {$} mouse and {$} nose.", "Lao fan", 3.14, 2);
    log.Log("{$} is a clever boy, which has {$} brain.", "Lao Knee", 65565);
    return 0;
}