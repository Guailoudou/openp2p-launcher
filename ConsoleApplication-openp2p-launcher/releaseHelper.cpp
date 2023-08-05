#include "releaseHelper.h"
#include <cstdio>
#include <string.h>
#include <direct.h>
#include <exception>

CReleaseDLL::CReleaseDLL()
{
	this->m_hModule = GetSelfModuleHandle();
	if (m_hModule == NULL)
	{
		throw std::exception("Error:获取基址失败");
	}
	//获取目录
	memset(this->m_filePath, 0, MAX_DLL_PATH);
	_getcwd(this->m_filePath, MAX_DLL_PATH);
}
CReleaseDLL::~CReleaseDLL()
{
}
bool CReleaseDLL::FreeResFile(unsigned long m_lResourceID, const char* m_strResourceType, const char* m_strFileName)
{
	//构造完整释放文件路径
	char strFullPath[MAX_DLL_PATH] = { 0 };
	sprintf_s(strFullPath, "%s\\%s", this->m_filePath, m_strFileName);
	//查找资源
	HRSRC hResID = ::FindResource(this->m_hModule, MAKEINTRESOURCE(m_lResourceID), m_strResourceType);
	//加载资源  
	HGLOBAL hRes = ::LoadResource(this->m_hModule, hResID);
	//锁定资源
	LPVOID pRes = ::LockResource(hRes);
	if (pRes == NULL)
	{
		return FALSE;
	}
	//得到待释放资源文件大小 
	unsigned long dwResSize = ::SizeofResource(this->m_hModule, hResID);
	//创建文件 
	HANDLE hResFile = CreateFile(strFullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hResFile)
	{
		return FALSE;
	}
	DWORD dwWrited = 0;
	if (FALSE == WriteFile(hResFile, pRes, dwResSize, &dwWrited, NULL))
	{

		return FALSE;
	}

	CloseHandle(hResFile);
	UnlockResource(hRes);
	FreeResource(hRes);
	return TRUE;
}

HMODULE CReleaseDLL::GetSelfModuleHandle()
{
	try
	{
#ifdef _USER_RELEASEDLL_
		//如果释放的帮助类定义在DLL中，将调用下面的方式获取基址
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery((LPCVOID)&CReleaseDLL::GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
			? (HMODULE)mbi.AllocationBase : NULL);
#else
		//如果直接定义在exe本身的代码中
		return ::GetModuleHandle(NULL);
#endif
	}
	catch (...)
	{
		return NULL;
	}

}