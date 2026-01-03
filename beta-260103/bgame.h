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

//deepseek api密钥检测函数（deepseek生成）
std::string check_key(const std::string& apikey){
    if(apikey.length()<4||apikey.substr(0,3)!="sk-"){
        return"f";
    }
    std::string command="curl -s -X GET \"https://api.deepseek.com/user/balance\" -H \"Accept: application/json\" -H \"Authorization: Bearer "+apikey+"\" -w \"%{http_code}\" --silent --max-time 10";
    FILE* pipe=popen(command.c_str(),"r");
    if(!pipe){
        return"无法执行curl命令";
    }
    char buffer[128];
    std::string result;
    while(fgets(buffer,sizeof(buffer),pipe)!=NULL){
        result+=buffer;
    }
    int status=pclose(pipe);
    if(status==0){
        std::string http_code;
        for(char c:result){
            if(std::isdigit(static_cast<unsigned char>(c))){
                http_code+=c;
            }
        }
        if(http_code.length()>=3&&http_code.substr(http_code.length()-3)=="200"){
            return"t";
        }else{
            return"f";
        }
    }else{
        std::string error_msg;
        if(status==6){
            error_msg="无法解析主机";
        }else if(status==7){
            error_msg="连接主机失败";
        }else if(status==23){
            error_msg="写入文件失败";
        }else if(status==28){
            error_msg="连接超时";
        }else if(status==35){
            error_msg="SSL/TLS握手失败";
        }else if(status==60){
            error_msg="SSL证书问题";
        }else{
            error_msg="curl错误代码："+std::to_string(status);
        }
        return error_msg;
    }
}

//GB2312转UTF-8编码转换函数
std::string gb2312_to_utf8(const std::string& gb2312_str){
    if(gb2312_str.empty()) return "";
    int len=MultiByteToWideChar(CP_ACP,0,gb2312_str.c_str(),-1,NULL,0);
    if(len==0) return "";
    wchar_t* wstr=new wchar_t[len];
    MultiByteToWideChar(CP_ACP,0,gb2312_str.c_str(),-1,wstr,len);
    len=WideCharToMultiByte(CP_UTF8,0,wstr,-1,NULL,0,NULL,NULL);
    if(len==0){
        delete[] wstr;
        return "";
    }
    char* utf8_str=new char[len];
    WideCharToMultiByte(CP_UTF8,0,wstr,-1,utf8_str,len,NULL,NULL);
    std::string result(utf8_str);
    delete[] wstr;
    delete[] utf8_str;
    return result;
}

//UTF-8转GB2312编码转换函数
std::string utf8_to_gb2312(const std::string& utf8_str){
    if(utf8_str.empty()) return "";
    //先转换为宽字符（UTF-16）
    int len=MultiByteToWideChar(CP_UTF8,0,utf8_str.c_str(),-1,NULL,0);
    if(len==0) return "";
    wchar_t* wstr=new wchar_t[len];
    MultiByteToWideChar(CP_UTF8,0,utf8_str.c_str(),-1,wstr,len);
    //再转换为GB2312
    len=WideCharToMultiByte(CP_ACP,0,wstr,-1,NULL,0,NULL,NULL);
    if(len==0){
        delete[] wstr;
        return "";
    }
    char* gb2312_str=new char[len];
    WideCharToMultiByte(CP_ACP,0,wstr,-1,gb2312_str,len,NULL,NULL);
    std::string result(gb2312_str);
    delete[] wstr;
    delete[] gb2312_str;
    return result;
}

//JSON转义函数
std::string json_escape(const std::string& str){
    std::string result;
    result.reserve(str.length()*2);
    for(size_t i=0;i<str.length();++i){
        unsigned char c=str[i];
        if(c=='"'){
            result+="\\\"";
        }else if(c=='\\'){
            result+="\\\\";
        }else if(c=='\n'){
            result+="\\n";
        }else if(c=='\r'){
            result+="\\r";
        }else if(c=='\t'){
            result+="\\t";
        }else if(c<0x20){
            char buf[7];
            snprintf(buf,sizeof(buf),"\\u%04x",c);
            result+=buf;
        }else{
            result+=c;
        }
    }
    return result;
}

//deepseek api调用函数
std::string call_ai(std::string key,std::string content){
    //1.编码转换：GB2312->UTF-8
    std::string utf8_content=gb2312_to_utf8(content);
    if(utf8_content.empty()&&!content.empty()){
        return "E01编码转换失败";
    }
    //2.JSON转义
    std::string escaped_content=json_escape(utf8_content);
    //3.构建JSON请求
    std::string json_data="{\"model\":\"deepseek-chat\",\"messages\":[{\"role\":\"user\",\"content\":\""+escaped_content+"\"}],\"stream\":false,\"max_tokens\":2048}";
    //4.保存到临时文件
    std::string temp_file="request.json";
    std::ofstream file(temp_file,std::ios::binary);
    if(!file){
        return "E02无法创建临时文件";
    }
    file.write(json_data.c_str(),json_data.length());
    file.close();
    //5.执行curl命令
    std::string command="curl -s -X POST https://api.deepseek.com/v1/chat/completions -H \"Content-Type: application/json\" -H \"Authorization: Bearer "+key+"\" --data-binary @"+temp_file;
    std::string result;
    char buffer[4096];
    FILE* pipe=_popen(command.c_str(),"r");
    if(!pipe){
        remove(temp_file.c_str());
        return "E03无法执行curl命令";
    }
    while(fgets(buffer,sizeof(buffer),pipe)!=nullptr){
        result+=buffer;
    }
    int pipe_status=_pclose(pipe);
    remove(temp_file.c_str());
    //检查curl执行状态
    if(pipe_status!=0){
        return "E04curl命令执行失败(状态码:"+std::to_string(pipe_status)+")";
    }
    //6.处理响应
    if(result.empty()){
        return "E05API返回空响应";
    }
    //7.解析响应-先检查API错误
    if(result.find("\"error\":")!=std::string::npos){
        size_t msg_start=result.find("\"message\":\"");
        if(msg_start!=std::string::npos){
            msg_start+=11;
            size_t msg_end=result.find("\"",msg_start);
            if(msg_end!=std::string::npos){
                return "E06API错误:"+result.substr(msg_start,msg_end-msg_start);
            }
        }
        return "E07API返回未知错误";
    }
    //8.检查网络错误
    if(result.find("curl:")!=std::string::npos||result.find("Could not resolve host")!=std::string::npos){
        return "E08网络连接失败";
    }
    if(result.find("SSL certificate problem")!=std::string::npos){
        return "E09SSL证书验证失败";
    }
    if(result.find("401")!=std::string::npos||result.find("unauthorized")!=std::string::npos){
        return "E10API密钥无效或已过期";
    }
    if(result.find("429")!=std::string::npos){
        return "E11请求频率超限";
    }
    if(result.find("500")!=std::string::npos||result.find("502")!=std::string::npos||result.find("503")!=std::string::npos||result.find("504")!=std::string::npos){
        return "E12服务器内部错误";
    }
    //9.提取正常响应内容
    size_t content_start=result.find("\"content\":\"");
    if(content_start==std::string::npos){
        size_t error_start=result.find("\"message\":\"");
        if(error_start!=std::string::npos){
            error_start+=11;
            size_t error_end=result.find("\"",error_start);
            if(error_end!=std::string::npos){
                return "E13API错误:"+result.substr(error_start,error_end-error_start);
            }
        }
        return "E14API响应格式错误";
    }
    content_start+=11;
    size_t content_end=content_start;
    bool escaped=false;
    while(content_end<result.length()){
        if(escaped){
            escaped=false;
        }else if(result[content_end]=='\\'){
            escaped=true;
        }else if(result[content_end]=='"'){
            break;
        }
        content_end++;
    }
    if(content_end>=result.length()||content_end<=content_start){
        return "E15无法提取响应内容";
    }
    std::string ai_content=result.substr(content_start,content_end-content_start);
    //10.解码转义字符
    std::string decoded;
    for(size_t i=0;i<ai_content.length();++i){
        if(ai_content[i]=='\\'&&i+1<ai_content.length()){
            switch(ai_content[i+1]){
                case 'n':decoded+='\n';i++;break;
                case 'r':decoded+='\r';i++;break;
                case 't':decoded+='\t';i++;break;
                case '\\':decoded+='\\';i++;break;
                case '"':decoded+='"';i++;break;
                case '/':decoded+='/';i++;break;
                case 'u':
                    if(i+5<ai_content.length()){
                        decoded+='?';
                        i+=5;
                    }else{
                        decoded+=ai_content[i];
                    }
                    break;
                default:
                    decoded+=ai_content[i];
                    break;
            }
        }else{
            decoded+=ai_content[i];
        }
    }
    //11.直接返回解码后的UTF-8字符串
    return decoded;
}

//完结撒花
#endif