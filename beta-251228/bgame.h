#ifndef BGAME_H
#define BGAME_H

//常用头文件
#include<iostream>
#include<fstream>
#include<string>
#include<graphics.h>
#include<conio.h>
#include<ctime>
#include<cmath>
#include<vector>
#include<windows.h>
#include<sstream>

//缩写
#define cleard() cleardevice()
#define rgbc COLORREF
#define L_MOUSE KEY_DOWN(VK_LBUTTON)
#define R_MOUSE KEY_DOWN(VK_RBUTTON)
//将窗口移动到屏幕正中央
#define move_window() MoveWindow(GetHWnd(),(GetSystemMetrics(SM_CXFULLSCREEN)-getwidth())/2,(GetSystemMetrics(SM_CYFULLSCREEN)-getheight())/2,getwidth(),getheight(),1)
//按键检测必备
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)&&(GetForegroundWindow()==GetHWnd()) 

//获取针对当前窗口的鼠标指针
void getmouse(LPPOINT lpPoint){
    GetCursorPos(lpPoint);
    ScreenToClient(GetHWnd(),lpPoint);
}

//改背景颜色（因为改完要清屏，这里把两个函数加到了一起）
void _setbkcolor(rgbc color){
    setbkcolor(color);
    cleard();
}

/*针对win11的sb新版命令提示符会自动弹出的问题（以前版本都不会），
特地借助编写了以下2个函数以在程序运行时关闭命令提示符窗口*/
std::string st = "\\main";
bool windowHidden = false;
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    const std::string* targetString = reinterpret_cast<const std::string*>(lParam);
    char windowTitle[256];
    GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));
    std::string title(windowTitle);
    if (title.find(*targetString) != std::string::npos && !title.empty()) {
        if (ShowWindow(hwnd, SW_HIDE)) {
            windowHidden = true;
        }
    }
    return TRUE;
}

bool HideWindowByPartialTitle(const std::string& partialTitle = "\\main") {
    windowHidden=false;
    st=partialTitle;
    EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&st));
    return windowHidden;
}

//AI模式中检测deepseek api key是否有效（deepseek生成）
bool check_key(std::string apikey) {
    std::string command = "curl -s -X GET \"https://api.deepseek.com/user/balance\" "
                          "-H \"Accept: application/json\" "
                          "-H \"Authorization: Bearer " + apikey + "\" "
                          "-w \"%{http_code}\" -o /dev/null";
    FILE* pipe = popen(command.c_str(), "r");
    if(!pipe){
        return false;
    }
    char buffer[128];
    std::string result;
    while(fgets(buffer, sizeof(buffer), pipe) != NULL){
        result += buffer;
    }
    int status = pclose(pipe);
    while(!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
        result.pop_back();
    }
    //std::cout << "Curl exit status: " << status << std::endl;
    //std::cout << "HTTP status code: " << result << std::endl;
    return result == "200";
}

// GB2312 转 UTF-8 编码转换函数
std::string gb2312_to_utf8(const std::string& gb2312_str) {
    if (gb2312_str.empty()) return "";
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312_str.c_str(), -1, NULL, 0);
    if (len == 0) return "";
    wchar_t* wstr = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, gb2312_str.c_str(), -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (len == 0) {
        delete[] wstr;
        return "";
    }
    char* utf8_str = new char[len];
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, utf8_str, len, NULL, NULL);
    std::string result(utf8_str);
    delete[] wstr;
    delete[] utf8_str;
    return result;
}

// JSON转义函数
std::string json_escape(const std::string& str) {
    std::string result;
    result.reserve(str.length() * 2);
    for (size_t i = 0; i < str.length(); ++i) {
        unsigned char c = str[i];
        if (c == '"') {
            result += "\\\"";
        } else if (c == '\\') {
            result += "\\\\";
        } else if (c == '\n') {
            result += "\\n";
        } else if (c == '\r') {
            result += "\\r";
        } else if (c == '\t') {
            result += "\\t";
        } else if (c < 0x20) {
            char buf[7];
            snprintf(buf, sizeof(buf), "\\u%04x", c);
            result += buf;
        } else {
            result += c;
        }
    }
    return result;
}

//AI模式中与deepseek api对话（deepseek生成）
std::string call_ai(std::string key, std::string content) {
    // 1. 编码转换：GB2312 -> UTF-8
    std::string utf8_content = gb2312_to_utf8(content);
    // 2. JSON转义
    std::string escaped_content = json_escape(utf8_content);
    // 3. 构建JSON请求
    std::string json_data = "{\"model\":\"deepseek-chat\",\"messages\":[{\"role\":\"user\",\"content\":\"" + 
                            escaped_content + "\"}],\"stream\":false,\"max_tokens\":2048}";
    // 4. 保存到临时文件
    std::string temp_file = "request.json";
    std::ofstream file(temp_file, std::ios::binary);
    if (!file) {
        return "E1无法创建临时文件";
    }
    file.write(json_data.c_str(), json_data.length());
    file.close();
    // 5. 执行curl命令
    std::string command = "curl -s -X POST "
                          "https://api.deepseek.com/v1/chat/completions "
                          "-H \"Content-Type: application/json\" "
                          "-H \"Authorization: Bearer " + key + "\" "
                          "--data-binary @" + temp_file;
    
    std::string result;
    char buffer[4096];
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        remove(temp_file.c_str());
        return "E2无法执行curl命令";
    }
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    _pclose(pipe);
    remove(temp_file.c_str());
    // 6. 处理响应
    if (result.empty()) {
        return "E3API返回空响应";
    }
    // 7. 解析响应
    size_t content_start = result.find("\"content\":\"");
    if (content_start == std::string::npos) {
        size_t error_start = result.find("\"message\":\"");
        if (error_start != std::string::npos) {
            error_start += 11;
            size_t error_end = result.find("\"", error_start);
            if (error_end != std::string::npos) {
                return "E4API错误: " + result.substr(error_start, error_end - error_start);
            }
        }
        return "E5API响应格式错误";
    }
    content_start += 11;
    size_t content_end = content_start;
    while (content_end < result.length()) {
        if (result[content_end] == '"' && (content_end == 0 || result[content_end-1] != '\\')) {
            break;
        }
        content_end++;
    }
    std::string ai_content = result.substr(content_start, content_end - content_start);
    // 解码转义字符
    std::string decoded;
    for (size_t i = 0; i < ai_content.length(); ++i) {
        if (ai_content[i] == '\\' && i + 1 < ai_content.length()) {
            switch (ai_content[i + 1]) {
                case 'n': decoded += '\n'; i++; break;
                case 'r': decoded += '\r'; i++; break;
                case 't': decoded += '\t'; i++; break;
                case '\\': decoded += '\\'; i++; break;
                case '"': decoded += '"'; i++; break;
                default: decoded += ai_content[i]; break;
            }
        } else {
            decoded += ai_content[i];
        }
    }
    return decoded;
}

//完结撒花
#endif