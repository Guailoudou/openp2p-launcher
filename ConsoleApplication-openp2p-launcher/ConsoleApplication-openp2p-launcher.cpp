#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0600 // 根据需要设置合适的版本
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <chrono>
#include <string>
#include <random>
#include <windows.h>
#pragma comment(lib, "wsock32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <nlohmann/json.hpp>
#include <io.h>
#include <process.h>
#include <thread>
#include "resource1.h"
#include "releaseHelper.h"
int SrcPort,openn=0,udpopen;
std::string version = "0.5.5.3";
//声明函数.
bool isFileExists_ifstream(std::string & name),checkMCServerOnline(const char* serverIP, int serverPort);
void play0(std::string myuuid), play1(int SrcPort,  std::string uuid, std::string myuuid), openp2p(), heart(),seeduuid(),startapp();
int app(),UDPMC(),mobapp();
std::string create_uuid();
int main()
{
    //声明变量
    int type, DstPort;
    std::string uuid, myuuid;
    //system("pause");
    std::string flie = "bin\\uuid.dat";
    std::string apps = "bin\\openp2p.exe";
    std::string log = "bin\\bin\\log\\openp2p.log";
    std::string log0 = "bin\\log\\openp2p.log";
    std::string oconig = "bin\\config.json";
    //释放openp2p文件
    if (!isFileExists_ifstream(apps))
    {
        app();
    }
    //清除log
    if (isFileExists_ifstream(log))
    {
        system("del bin\\bin\\log /q");
    }
    if (isFileExists_ifstream(log0))
    {
        system("del bin\\log /q");
        std::cout << "检查到你自己打开了bin里面的openp2p.exe，最好不要直接打开bin启动里面的文件哦\n" << std::endl;
    }
    //生成/获取uuid
    if (isFileExists_ifstream(flie))
    {
        if(isFileExists_ifstream(oconig))
        {
            nlohmann::json op;
            std::ifstream ifs2("bin\\config.json");
            ifs2 >> op;
            ifs2.close();
            myuuid = op["network"]["Node"];
        }
        else
        {
            std::ifstream infile;
            infile.open("bin\\uuid.dat");
            infile >> myuuid;
            infile.close();
        }
    }
    else
    {
        myuuid = create_uuid();
        std::ofstream outfile;
        outfile.open("bin\\uuid.dat");
        outfile << myuuid << std::endl;
        outfile.close();
    }
    std::cout << "*初始化完毕*\n***************OPL-"<< version <<" * *********************\n                使用说明\n    1.根据提示输入参数\n    2.注意你的uuid是：" << myuuid << "\n    4.被连接需要把你的uuid和端口发给对方\n    3.程序文档：https://gld.rth1.link/md/opl\n    4.本程序基于openp2p\n*********************************************\n" << std::endl;
    system("title openp2p-launcher-by-Guailoudou");
    std::cout << "被连接：输入0||连接：输入1，以上一次的连接方式连接输入2 " << std::endl;
    std::cin >> type;
    if (type == 0)
    {
        play0(myuuid);
        std::thread t1(openp2p);
        Sleep(2000);
        nlohmann::json op2;
        std::ifstream ifs3("bin\\config.json");
        ifs3 >> op2;
        ifs3.close();
        myuuid = op2["network"]["Node"];
        std::ostringstream ss;
        ss << "\033[31;1m(以这个为准)你的UUID为:" << myuuid << "\033[0m";
        std::string message = ss.str();
        std::cout << message << std::endl;
        t1.join();
    }
    else if (type == 1)
    {
        const char* serverIP = "127.0.0.1";
        std::cout << "开始运行之后直接打开游戏从局域网进入\n请输入对方uuid：" << std::endl;
        std::cin >> uuid;
        if (uuid == myuuid) {
            std::cout << "你不能自己连自己，如果uuid是别人发你的，请去删除bin文件夹后重启软件" << std::endl;
            system("pause");
            return 0;
        }
        std::cout << "请输入对方端口：" << std::endl;
        std::cin >> DstPort;
        SrcPort = DstPort;
        while (true)
        {
            if (checkMCServerOnline(serverIP, SrcPort)) 
            {
                SrcPort = SrcPort - 1;
            }
            else
            {
                break;
            }
        }
        std::cout << "程序2s后开始运行,请直接打开游戏等待提示连接成功后从局域网进入";
        Sleep(2000);
        play1(DstPort, uuid, myuuid);
        while (true)
        {
            startapp();
            Sleep(2000);
        }
    }
    else if (type == 2)
    {
        nlohmann::json op;
        std::ifstream ifs2("bin\\config.json");
        ifs2 >> op;
        ifs2.close();
        SrcPort = op["apps"][0]["SrcPort"];
        while (true)
        {
            startapp();
            Sleep(2000);
        }
    }
    else
    {
        std::cout << "输入错误" << std::endl;
    }
    system("pause");
    return 0;
}
//发送当前uuid，读op配置
void seeduuid() {
    std::string myuuid;
    nlohmann::json op2;
    std::ifstream ifs22("bin\\config.json");
    ifs22 >> op2;
    ifs22.close();
    myuuid = op2["network"]["Node"];
    std::ostringstream ss;
    ss << "\033[31;1m(以这个为准)你的UUID为:" << myuuid << "\033[0m";
    std::string message = ss.str();
    std::cout << message << std::endl;
}
//启动程序（连接）
void startapp() {
    std::thread t1(openp2p);
    std::thread hea(heart);
    std::thread udp(UDPMC);
    Sleep(2000);
    seeduuid();
    t1.join();
    mobapp();
    hea.join();
    udp.join();
}
//程序端口心跳
void heart() {
    bool checkMCServerOnline(const char* serverIP, int serverPort);
    const char* serverIP = "127.0.0.1";
    udpopen = 0;
    while (true)
    {
        if (checkMCServerOnline(serverIP, SrcPort)) {
            if (openn == 0) {
                std::cout << "\033[32;1m连接成功,请直接打开游戏从局域网进入,如果5s内没有出现在局域网，请手动添加服务器127.0.0.1:"<< SrcPort << "\033[0m" << std::endl;
                openn = 1;
                udpopen = 1;
            }
            std::cout << "\033[32;1m连接正常\033[0m  ";
            Sleep(7000);
        }
        else
        {
            if (openn == 1) {
                std::cout << "\033[31;1m检测到异常断开连接，开始尝试重新连接\033[0m" << std::endl;
                openn = 0;
                udpopen = 0;
            }
            Sleep(2000);
        }
    }
}
void openp2p() {
    system("bin\\openp2p.exe");
}
////////////////////////////////////////////////////////////////////////////
//生成仅供连接配置
void play0(std::string myuuid)
{
    nlohmann::json j;
    j["network"]["Token"] = 11602319472897248650ULL;
    j["network"]["Node"] = myuuid;
    j["network"]["User"] = "gldoffice";
    j["network"]["ShareBandwidth"] = 100;
    j["network"]["ServerHost"] = "api.openp2p.cn";
    j["network"]["ServerPort"] = 27183;
    j["network"]["UDPPort1"] = 27182;
    j["network"]["UDPPort2"] = 27183;
    j["network"]["TCPPort"] = 50448;
    j["LogLevel"] = 1;
    //std::cout << json::wrap(j);
    std::ofstream ofs("bin/config.json");
    ofs << std::setw(4) << j << std::endl;
}
//生成tcp-app配置
void play1(int DstPort, std::string uuid, std::string myuuid)
{
    nlohmann::json j;
    j["network"]["Token"] = 11602319472897248650ULL;
    j["network"]["Node"] = myuuid;
    j["network"]["User"] = "gldoffice";
    j["network"]["ShareBandwidth"] = 100;
    j["network"]["ServerHost"] = "api.openp2p.cn";
    j["network"]["ServerPort"] = 27183;
    j["network"]["UDPPort1"] = 27182;
    j["network"]["UDPPort2"] = 27183;
    j["network"]["TCPPort"] = 50448;

    j["apps"][0]["AppName"] = "***";
    j["apps"][0]["Protocol"] = "tcp";
    j["apps"][0]["SrcPort"] = SrcPort;//本地端口
    j["apps"][0]["PeerNode"] = uuid;//远程设备名
    j["apps"][0]["DstPort"] = DstPort;//远程端口
    j["apps"][0]["DstHost"] = "localhost";
    j["apps"][0]["PeerUser"] = "";
    j["apps"][0]["Enabled"] = 1;
    j["apps"][0]["AppName"] = "***";
    j["LogLevel"] = 1;
    //std::cout << configor::json::wrap(j);
    std::ofstream ofs("bin/config.json");
    ofs << std::setw(4) << j << std::endl;
}
//生成uuid
std::string create_uuid()
{
    std::stringstream stream;
    auto random_seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 seed_engine(random_seed);
    std::uniform_int_distribution<std::size_t> random_gen;
    std::size_t value = random_gen(seed_engine);
    stream << std::hex << value;

    return stream.str();
}
//判断文件存在
bool isFileExists_ifstream(std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}
//释放资源文件
int app()
{
    CReleaseDLL releasehelper;
    bool blRes;
    blRes = releasehelper.FreeResFile(IDR_APP1, "APP", "openp2p.exe");

    if (blRes)
    {
        std::cout << "EXE文件释放成功" << std::endl;
        system("md bin");
        system("move openp2p.exe bin\\openp2p.exe");
    }
    else
    {
        std::cout << "EXE文件释放失败" << std::endl;
    }
    return 0;
}
int mobapp() {
    std::string upapps = "bin\\bin\\openp2p.exe";
    if (isFileExists_ifstream(upapps)) {
        system("move bin\\bin\\openp2p.exe bin\\openp2p.exe");
        std::cout << "更新openp2p版本成功" << std::endl;
        system("bin\\openp2p.exe");
    }
    return 0;
}
//发送udp多播
int UDPMC()
{
    if (SrcPort != 0&&udpopen==1) {
        // 初始化 Winsock
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize Winsock." << std::endl;
            return 1;
        }

        const char* MULTICAST_GROUP = "224.0.2.60";
        const int MULTICAST_PORT = 4445;

        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock == -1) {
            std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
            WSACleanup(); // 清理 Winsock 资源
            return 1;
        }

        int ttl = 2; // Time-to-live for multicast
        setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl));

        struct sockaddr_in addr {};
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP);
        addr.sin_port = htons(MULTICAST_PORT);

        std::cout << "开启局域网发现";
        while (udpopen) {
            // 使用 std::ostringstream 进行转换和编码
            std::ostringstream ss;
            ss << u8"[MOTD]§2§l[OPL]§b远程世界 §7-by GLD[/MOTD][AD]" << SrcPort << "[/AD]";
            std::string message = ss.str();

            sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
            //std::cout << "Sent: " << message << std::endl;
            Sleep(1500); // Sleep for a while before sending the next message
        }
        std::cout << "关闭局域网发现";
        closesocket(sock);

        // 清理 Winsock 资源
        WSACleanup();
    }
    Sleep(5000);
    UDPMC();
    return 0;
}

//检测是否在线
bool checkMCServerOnline(const char* serverIP, int serverPort) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    // 使用 inet_pton 函数将 IP 地址从字符串转换为二进制格式
    if (inet_pton(AF_INET, serverIP, &serverAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address" << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }

    if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) != 0) {
        std::cerr << "连接中..." << std::endl;
        closesocket(sock);
        WSACleanup();
        return false;
    }

    closesocket(sock);
    WSACleanup();
    return true;
}
