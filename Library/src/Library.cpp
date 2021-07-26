#include "Library.h"
#include <locale.h>  
#include <locale>  
#include "StlUtil.h"
#include "XDir.h"
#include "XFile.h"

#ifdef WIN32
#include <windows.h>
#elif defined __linux__
#include <dlfcn.h>
#endif

using std::locale; 

CLibrary::CLibrary( const char* strFileName /*= ""*/ ):
	  m_hHandle(NULL)
	, m_IsSucc(false)
{
	memset(m_strFileName, '\0', sizeof(char) * 52);
	memset(m_strFilePath, '\0', sizeof(char) * 256);
	IOx::XFile xfile(strFileName);
	xfile.Normalize();
	strcat(m_strFilePath, xfile.absolutePath());
	std::string fileName;
	stlu::parseFileName(&fileName,xfile.absolutePath());
	strcat(m_strFileName, fileName.c_str());
}
CLibrary::~CLibrary(void)
{
#ifdef WIN32
	if(m_hHandle != NULL)
	{	
		FreeLibrary((HMODULE)m_hHandle);
		m_hHandle = NULL;
	}
#elif defined __linux__
	if(m_hHandle != NULL)
	{
		free(m_hHandle);
		m_hHandle = NULL;
	}
#elif defined VXWORKS
#endif

}

bool CLibrary::Load()
{
	locale loc = std::locale::global(std::locale(""));

#ifdef WIN32
	//char szTmp[_MAX_PATH]; 
	//strcpy(szTmp,m_strFilePath.c_str());
	//TCHAR wLocation[_MAX_PATH] = { 0 }; 
	//MultiByteToWideChar(0, 0, szTmp, _MAX_PATH, (LPWSTR)wLocation, _MAX_PATH * 2);
	m_hHandle = LoadLibrary(m_strFilePath);
#elif defined __linux__
	 m_hHandle = dlopen(m_strFilePath.c_str(), RTLD_LAZY );
#elif defined VXWORKS
#endif
	m_IsSucc = (m_hHandle == nullptr)?false:true;
	locale::global(loc);
	return m_IsSucc;
}

bool CLibrary::Unload()
{
	return true;
}

void* CLibrary::Resolve( const char* strSymbol )
{
#ifdef WIN32
	return (void *)GetProcAddress((HMODULE)m_hHandle, strSymbol);
#elif defined __linux__
	return dlsym(m_hHandle,strSymbol.c_str());
#elif defined VXWORKS
#endif
}

void* CLibrary::Resolve( const char* strFileName,const char* strSymbol )
{
	locale loc = std::locale::global(std::locale(""));
	static LibraryHandle handle = NULL;
#ifdef WIN32
	char szTmp[_MAX_PATH]; 
	strcpy(szTmp,strFileName);
	TCHAR wLocation[_MAX_PATH] = { 0 }; 
	MultiByteToWideChar(0, 0, szTmp, _MAX_PATH, (LPWSTR)wLocation, _MAX_PATH * 2);   
	handle  = LoadLibrary(wLocation);
	locale::global(loc);
	return (void *)GetProcAddress((HMODULE)handle, strSymbol);
#elif defined __linux__
	handle = LoadLibrary(wLocation);
	dlopen(strFileName.c_str(), RTLD_LAZY );
	locale::global(loc);
	return dlsym(handle,strSymbol.c_str());
#elif defined VXWORKS
#endif

}

CLibrary * CLibrary::Clone()
{
	CLibrary *pCLibrary = NULL;
	if(strlen(m_strFilePath) >= 0)
	{
		pCLibrary = new CLibrary(m_strFilePath);
		pCLibrary->SetFileName(m_strFilePath);
	}
	return pCLibrary;
}

bool CLibrary::IsLoaded()
{
	return m_IsSucc;
}

bool CLibrary::SetFileName( const char* strFileName )
{
	// 简单检查文件是否合法
	IOx::XFile xfile(strFileName);
	if (!xfile.valid())
	{
		return false;
	}
	memset(m_strFilePath, '\0', sizeof(char) * 256);
	strcat(m_strFilePath, strFileName);
	return true;
}

const char* CLibrary::GetFilePath()
{
	return m_strFilePath;
}
