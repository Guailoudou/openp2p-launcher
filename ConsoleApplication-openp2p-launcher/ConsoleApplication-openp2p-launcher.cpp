#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <cstring>
#include <random>
#include "configor/json.hpp"
#include "resource.h"
#include "releaseHelper.h"
enum TYPE
{
    add=1, //添加隧道
    del,   //删除隧道
    list,  //查看隧道列表
    start, //启动
};
//声明函数.
bool isFileExists_ifstream(std::string& name);
void ReadApp(const int& n), sconfig(const std::string& myuuid); 
int AddTunnel(), ClearTunnel();
enum TYPE type(const std::string& type);
int app(),Initialization();
std::string create_uuid();
std::string myuuid;
int appn;
int main()
{
    //初始化
    if (Initialization()) {
        std::cout << "*初始化完毕*\n*********************************************\n     Openp2pLauncher0.6.1\n     -被连接需要把你的uuid和端口发给对方\n     -本程序基于openp2p\n*********************************************\n" << std::endl;
        system("title openp2p-launcher-by-Guailoudou");
    }
    std::string types;
    while (true)
    {
        std::cout << "注意你的uuid是：\033[31;1m" << myuuid << "\033[0m\n\033[34;1m******指令语法******\n添加隧道：add <tcp/udp> <uuid> <对方端口> <本地接收端口>\n删除隧道：del <序号>\n隧道列表：list\n开始运行隧道/程序：start\n*******************\033[0m" << std::endl;
        std::cin >> types;
        system("cls");
        std::cout << types << std::endl;
        enum TYPE Etypes = type(types);
        switch (Etypes)
        {
        case add:
            if (AddTunnel())
            {
                std::cout << "新建隧道成功" << std::endl;
                ReadApp(appn);
            }
            else
            {
                std::cout << "新建隧道失败" << std::endl;
            }
            break;
        case del:
            if (ClearTunnel())
            {
                std::cout << "删除隧道成功" << std::endl;
                ReadApp(appn);
            }
            else
            {
                std::cout << "删除隧道失败" << std::endl;
            }
            break;
        case list:
            ReadApp(appn);
            break;
        case start:
            std::cout << "注意你的uuid是：\033[31;1m" << myuuid << "\033[0m" << std::endl;
            system("bin\\openp2p.exe");
            break;
        default:
            std::cout << "错误的指令" << std::endl;
            break;
        }

    }
    return 0;
}
////////////////////////////////////////////////////////////////////////////
//生成初始配置
void sconfig(const std::string& myuuid)
{
    configor::json::value j;
    j["LogLevel"] = 1;
    j["network"]["TCPPort"] = 50448;
    j["network"]["UDPPort2"] = 27183;
    j["network"]["UDPPort1"] = 27182;
    j["network"]["ServerPort"] = 27183;
    j["network"]["ServerHost"] = "api.openp2p.cn";
    j["network"]["ShareBandwidth"] = 10;
    j["network"]["User"] = "Guailoudou";
    j["network"]["Node"] = myuuid;
    j["network"]["Token"] = 9222084896127568278;
    //std::cout << json::wrap(j);
    std::ofstream ofs("bin/config.json");
    ofs << std::setw(4) << configor::json::wrap(j) << std::endl;
}
//添加隧道
int AddTunnel()
{
    configor::json::value array;
    configor::json::value root;
    std::string Protocol; //隧道类型
    std::string PeerNode; //远程设备名(uuid)
    int DstPort, SrcPort; //远程端口/本地端口
    try 
    {
        std::cin >> Protocol;
        std::cin >> PeerNode;
        std::cin >> DstPort;
        std::cin >> SrcPort;
    }
    catch (...)
    {
        std::cout << "错误的指令" << std::endl;
        return 0;
    }
    std::ifstream ifs("bin\\config.json");
    configor::value j;
    ifs >> configor::json::wrap(j);

    std::ifstream ifs1("bin\\opl.json");
    configor::value opl;
    ifs1 >> configor::json::wrap(opl);


    j["apps"][appn]["AppName"] = "***";
    j["apps"][appn]["Protocol"] = Protocol;
    j["apps"][appn]["SrcPort"] = SrcPort;//本地端口
    j["apps"][appn]["PeerNode"] = PeerNode;//远程设备名
    j["apps"][appn]["DstPort"] = DstPort;//远程端口
    j["apps"][appn]["DstHost"] = "localhost";
    j["apps"][appn]["PeerUser"] = "";
    j["apps"][appn]["Enabled"] = 1;
    j["apps"][appn]["AppName"] = "***";
    appn++;
    opl["appn"] = appn;
    std::ofstream ofs("bin/config.json");
    ofs << std::setw(4) << configor::json::wrap(j) << std::endl;
    std::ofstream ofs1("bin/opl.json");
    ofs1 << std::setw(4) << configor::json::wrap(opl) << std::endl;
    ifs.close();
    ifs1.close();
    ofs.close();
    ofs1.close();
    return 1;
}
//删除隧道
int ClearTunnel()
{
    int n;
    try
    {
        std::cin >> n;
    }
    catch (...) 
    {
        std::cout << "错误的指令" << std::endl;
        return 0;
    }
    n--;
    std::ifstream ifs("bin\\config.json");
    configor::value j;
    ifs >> configor::json::wrap(j);
    std::ifstream ifs1("bin\\opl.json");
    configor::value opl;
    ifs1 >> configor::json::wrap(opl);
    for (int i = n; i < appn; i++)
    {
        j["apps"][i] = j["apps"][i + 1];
    }
    j["apps"].erase(appn-1);
    j["apps"].erase(appn-1);
    appn--;
    opl["appn"] = appn;
    std::ofstream ofs("bin/config.json");
    ofs << std::setw(4) << configor::json::wrap(j) << std::endl;
    std::ofstream ofs1("bin/opl.json");
    ofs1 << std::setw(4) << configor::json::wrap(opl) << std::endl;
    ifs.close();
    ifs1.close();
    ofs.close();
    ofs1.close();
    return 1;
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
//初始化
int Initialization() {
    //文件路径
    std::string flie = "bin\\opl.json";
    std::string apps = "bin\\openp2p.exe";
    std::string oconig = "bin\\config.json";
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
    //生成/获取uuid,app
    if (isFileExists_ifstream(flie))
    {
        configor::json::value oplr;
        std::ifstream ifs("bin\\opl.json");
        ifs >> configor::json::wrap(oplr);
        myuuid = (std::string)oplr["uuid"];
        appn = oplr["appn"];
        ifs.close();
    }
    else
    {
        configor::json::value opld;
        myuuid = create_uuid();
        appn = 0;
        opld["uuid"] = myuuid;
        opld["appn"] = appn;
        std::ofstream ofs("bin\\opl.json");
        ofs << std::setw(4) << configor::json::wrap(opld) << std::endl;
        ofs.close();
    }
    if (!isFileExists_ifstream(oconig))
    {
        sconfig(myuuid);
    }
    return 1;
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
void ReadApp(const int& n)
{
    if (n != 0)
    {
        for (int i = 0; i < n; i++)
        {
            std::ifstream ifs("bin\\config.json");
            configor::value j;
            ifs >> configor::json::wrap(j);
            std::cout << "\033[32;1m*******隧道序号：" << i+1 << "*******\033[0m" << std::endl;
            std::cout << "类型:" << (std::string)j["apps"][i]["Protocol"] << std::endl;
            std::cout << "对方uuid:" << (std::string)j["apps"][i]["PeerNode"] << std::endl;
            std::cout << "对方端口:" << (int)j["apps"][i]["DstPort"] << std::endl;
            std::cout << "本地端口:" << (int)j["apps"][i]["SrcPort"] << std::endl;
        }
        std::cout << "\033[32;1m***************************\033[0m" << std::endl;
    }
    else
    {
        std::cout << "无隧道" << std::endl;
    }

}
enum TYPE type(const std::string& type)
{
    enum TYPE;
    if (type == "add")return add;
    if (type == "del")return del;
    if (type == "list")return list;
    if (type == "start")return start;
}