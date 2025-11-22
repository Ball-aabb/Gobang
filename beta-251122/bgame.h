//起这个名字是因为它在很多程序中都有用
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

//懒人缩写
#define cleard() cleardevice()
//懒人缩写2
#define rgbc COLORREF
//按键检测必备
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME)&0x8000)?1:0)&&(GetForegroundWindow()==GetHWnd()) 
//懒人缩写3
#define L_MOUSE KEY_DOWN(VK_LBUTTON)
//懒人缩写4
#define R_MOUSE KEY_DOWN(VK_RBUTTON)
//将窗口移动到屏幕正中央
#define move_window() MoveWindow(GetHWnd(),(GetSystemMetrics(SM_CXFULLSCREEN)-getwidth())/2,(GetSystemMetrics(SM_CYFULLSCREEN)-getheight())/2,getwidth(),getheight(),1)

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
特地借助ds之力编写了以下2个函数以在程序运行时关闭命令提示符窗口*/
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

//AI模式中用于判断api密钥是否有效（deepseek生成）
std::string call_AI(std::string key, std::string content){
    // 如果content为空，设置默认值
    if(content.empty()){
        content = "Hello";
    }
    
    std::string escaped_content;
    for(char c : content){
        if(c == '"'){
            escaped_content += "\\\"";
        }else if(c == '\\'){
            escaped_content += "\\\\";
        }else if(c == '\n'){
            escaped_content += "\\n";
        }else if(c == '\r'){
            escaped_content += "\\r";
        }else if(c == '\t'){
            escaped_content += "\\t";
        }else{
            escaped_content += c;
        }
    }
    
    // 使用临时文件避免编码问题
    std::string json_data = "{\"model\":\"deepseek-chat\",\"messages\":[{\"role\":\"user\",\"content\":\"" + escaped_content + "\"}],\"stream\":false}";
    
    // 将JSON数据写入临时文件
    std::string temp_file = "temp_request.json";
    FILE* temp_fp = fopen(temp_file.c_str(), "wb");
    if(temp_fp){
        fwrite(json_data.c_str(), 1, json_data.length(), temp_fp);
        fclose(temp_fp);
        
        // 使用临时文件进行curl请求
        std::string command = "curl -s -X POST https://api.deepseek.com/chat/completions -H \"Content-Type: application/json\" -H \"Authorization: Bearer " + key + "\" --compressed -d @" + temp_file;
        
        std::string result;
        FILE* pipe = _popen(command.c_str(), "r");
        if(!pipe){
            remove(temp_file.c_str());
            return "Error: Failed to execute command";
        }
        
        char buffer[128];
        while(fgets(buffer, sizeof(buffer), pipe) != NULL){
            result += buffer;
        }
        _pclose(pipe);
        remove(temp_file.c_str());
        
        // 提取AI回复内容
        size_t content_start = result.find("\"content\":\"");
        if(content_start == std::string::npos){
            return "API响应格式错误: " + result;
        }
        
        content_start += 11;
        size_t content_end = content_start;
        while(content_end < result.length()){
            if(result[content_end] == '"' && (content_end == 0 || result[content_end - 1] != '\\')){
                break;
            }
            content_end++;
        }
        
        if(content_end >= result.length()){
            return "API响应格式错误: " + result;
        }
        
        std::string ai_content = result.substr(content_start, content_end - content_start);
        std::string decoded_content;
        for(size_t i = 0; i < ai_content.length(); ++i){
            if(ai_content[i] == '\\' && i + 1 < ai_content.length()){
                if(ai_content[i + 1] == 'n'){
                    decoded_content += '\n';
                    i++;
                }else if(ai_content[i + 1] == '\\' || ai_content[i + 1] == '"' || ai_content[i + 1] == '/'){
                    decoded_content += ai_content[i + 1];
                    i++;
                }else if(ai_content[i + 1] == 't'){
                    decoded_content += '\t';
                    i++;
                }else if(ai_content[i + 1] == 'r'){
                    decoded_content += '\r';
                    i++;
                }else{
                    decoded_content += ai_content[i];
                }
            }else{
                decoded_content += ai_content[i];
            }
        }
        return decoded_content;
    }else{
        return "Error: Failed to create temporary file";
    }
}

//完结撒花
#endif