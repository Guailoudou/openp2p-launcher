#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <random>
#include <windows.h>
#pragma comment(lib, "wsock32.lib")
#include <nlohmann/json.hpp>
#include <io.h>
#include <process.h>
#include <thread>
#include "resource.h"
#include "releaseHelper.h"
int DstPort;
int main()
{
    //声明函数.
    bool isFileExists_ifstream(std::string & name);
    void play0(std::string myuuid), play1(int SrcPort,  std::string uuid, std::string myuuid), openp2p();
    int app(),UDPMC();
    std::string create_uuid();
    //声明变量
    int type, SrcPort;
    std::string uuid, myuuid;
    //system("pause");
    std::string flie = "bin\\uuid.dat";
    std::string apps = "bin\\openp2p.exe";
    std::string log = "bin\\bin\\log\\openp2p.log";
    std::string log0 = "bin\\log\\openp2p.log";
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
        std::ifstream infile;
        infile.open("bin\\uuid.dat");
        infile >> myuuid;
        infile.close();
    }
    else
    {
        myuuid = create_uuid();
        std::ofstream outfile;
        outfile.open("bin\\uuid.dat");
        outfile << myuuid << std::endl;
        outfile.close();
    }
    std::cout << "*初始化完毕*\n*********************************************\n                使用说明\n    1.根据提示输入参数\n    2.注意你的uuid是：" << myuuid << "\n    4.被连接需要把你的uuid和端口发给对方\n    3.本程序基于openp2p\n*********************************************\n" << std::endl;
    system("title openp2p-launcher-by-Guailoudou");
    std::cout << "被连接：输入0||连接：输入1，以上一次的连接方式连接输入2 " << std::endl;
    std::cin >> type;
    if (type == 0)
    {
        play0(myuuid);
        system("bin\\openp2p.exe");
    }
    else if (type == 1)
    {
        std::cout << "开始运行之后直接打开游戏从局域网进入\n请输入对方uuid：" << std::endl;
        std::cin >> uuid;
        std::cout << "请输入对方端口：" << std::endl;
        std::cin >> DstPort;
        SrcPort = DstPort;
        std::cout << "程序2s后开始运行,请直接打开游戏从局域网进入";
        Sleep(2000);
        play1(SrcPort, uuid, myuuid);
        std::thread t1(openp2p);
        std::thread t2(UDPMC);
        t1.join();
    }
    else if (type == 2)
    {
        nlohmann::json op;
        std::ifstream ifs2("bin\\config.json");
        ifs2 >> op;
        ifs2.close();
        DstPort = op["apps"][0]["SrcPort"];
        std::thread t1(openp2p);
        std::thread t2(UDPMC);
        t1.join();
    }
    else
    {
        std::cout << "输入错误" << std::endl;
    }
    system("pause");
    return 0;
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
void play1(int SrcPort, std::string uuid, std::string myuuid)
{
    nlohmann::json array;
    nlohmann::json root;
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
int UDPMC()
{
    if (DstPort != 0) {
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


        while (true) {
            // 使用 std::ostringstream 进行转换和编码
            std::ostringstream ss;
            ss << "[MOTD][OPL]remote server[/MOTD][AD]" << DstPort << "[/AD]";
            std::string message = ss.str();

            sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr*)&addr, sizeof(addr));
            //std::cout << "Sent: " << message << std::endl;
            Sleep(1500); // Sleep for a while before sending the next message
        }

        closesocket(sock);

        // 清理 Winsock 资源
        WSACleanup();
    }
    return 0;
}
