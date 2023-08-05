#pragma once
#ifndef _RELEASEHELPER_H_
#define _RELEASEHELPER_H_

#include <windows.h>

#define MAX_DLL_PATH 1024
//如果操作代码位于DLL工程中，就定义下面的宏，否则注释掉
//#define _USER_RELEASEDLL_

//释放DLL帮助类
class CReleaseDLL
{
public:
	CReleaseDLL();
	~CReleaseDLL();
	/*
	*function:释放DLL文件帮助函数
	*[IN]:
	m_lResourceID:资源的ID
	m_strResourceType:资源类型
	m_strReleasePath:需要释放的文件名称
	*[OUT]:
	释放成功，返回TRUE，否则返回FALSE
	*/
	bool FreeResFile(unsigned long m_lResourceID, const char* m_strResourceType, const char* m_strFileName);
private:
	/*
	*function:获取模块的基址
	*[IN]:
	*[OUT]:
	返回获取的基址
	*/
	HMODULE GetSelfModuleHandle();
private:
	//模块基址
	HMODULE m_hModule;
	//程序当前目录
	char m_filePath[MAX_DLL_PATH];
};
#endif