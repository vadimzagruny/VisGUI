#include "stdafx.h"
#include "FileManager.h"


#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS

CFileManager::CFileManager(void)
{
	m_pTargetDataParser = nullptr;

	WCHAR wc_dir[MAX_PATH];
	CHAR  c_dir [MAX_PATH];
	DWORD dwSize = GetCurrentDirectory(0, NULL);
	GetCurrentDirectory(dwSize, wc_dir);
		
	WideCharToMultiByte(CP_ACP, 0, wc_dir, -1, c_dir,MAX_PATH,0,0);

	m_strWorkDirectory = c_dir;

	/*m_vecDirectories.push_back(string("data\\MTLS\\"));
	m_vecDirectories.push_back(string("data\\MDL\\"));
	m_vecDirectories.push_back(string("data\\MAP\\"));
	m_vecDirectories.push_back(string("data\\IMG\\"));
	m_vecDirectories.push_back(string("data\\sound\\"));*/

}


CFileManager::~CFileManager(void)
{
	delete m_pTargetDataParser;
}

bool CFileManager::LoadTargetData(string FileName,_TATGETDATA& tData)
{
	return m_pTargetDataParser->ParseXMLTargetCfg(FileName,tData);
}

bool CFileManager::SaveTargetData(string FileName,_TATGETDATA& tData)
{
	return m_pTargetDataParser->SaveXMLTargetCfg(FileName,tData);
}

bool CFileManager::Init()
{
	m_pTargetDataParser = new CTargetDataParser;

	return true;
}


CMaterial * CFileManager::LoadMaterialFile(string &name)
{

	CMaterial *pMat = NULL;
	pMat = m_MaterialParser.Parse(name);

	if (pMat == NULL){

		SetCurrentDirectoryA(m_strWorkDirectory.c_str());

		string str = "data\\mtls\\"; 
		str += name;

		pMat = m_MaterialParser.Parse(str);

		if (pMat == NULL){
			ostringstream streamReport;
			streamReport << "GMaterialParser: "<<" material <" << name << "> not found" << ends;
			//DEBUG_REPORT(streamReport.str().c_str());

			theApp.GetFileLogger()->log(streamReport.str());

			str = "data\\mtls\\MATERIAL_NOT_FOUND_mat";
			pMat = m_MaterialParser.Parse(str);

		}
	}

	

	return pMat;
}