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
		throw std::exception("Error:��ȡ��ַʧ��");
	}
	//��ȡĿ¼
	memset(this->m_filePath, 0, MAX_DLL_PATH);
	_getcwd(this->m_filePath, MAX_DLL_PATH);
}
CReleaseDLL::~CReleaseDLL()
{
}
bool CReleaseDLL::FreeResFile(unsigned long m_lResourceID, const char* m_strResourceType, const char* m_strFileName)
{
	//���������ͷ��ļ�·��
	char strFullPath[MAX_DLL_PATH] = { 0 };
	sprintf_s(strFullPath, "%s\\%s", this->m_filePath, m_strFileName);
	//������Դ
	HRSRC hResID = ::FindResource(this->m_hModule, MAKEINTRESOURCE(m_lResourceID), m_strResourceType);
	//������Դ  
	HGLOBAL hRes = ::LoadResource(this->m_hModule, hResID);
	//������Դ
	LPVOID pRes = ::LockResource(hRes);
	if (pRes == NULL)
	{
		return FALSE;
	}
	//�õ����ͷ���Դ�ļ���С 
	unsigned long dwResSize = ::SizeofResource(this->m_hModule, hResID);
	//�����ļ� 
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
		//����ͷŵİ����ඨ����DLL�У�����������ķ�ʽ��ȡ��ַ
		MEMORY_BASIC_INFORMATION mbi;
		return ((::VirtualQuery((LPCVOID)&CReleaseDLL::GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0)
			? (HMODULE)mbi.AllocationBase : NULL);
#else
		//���ֱ�Ӷ�����exe����Ĵ�����
		return ::GetModuleHandle(NULL);
#endif
	}
	catch (...)
	{
		return NULL;
	}

}