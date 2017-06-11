#pragma once

#include "TargetDataParser.h"

using namespace TargetDataParser;

/**
\brief File manager class .
*  \author    Vadim Zagruny
*  \version   2.1
*  \date      2017
*  \copyright  Vadim Zagruny KhMBDB.
*/
class CFileManager
{
	CTargetDataParser *m_pTargetDataParser;

	GMaterialParser m_MaterialParser;

	string m_strWorkDirectory;

public:
	CFileManager(void);
	~CFileManager(void);

	bool Init();

	bool LoadTargetData(string FileName,_TATGETDATA&);
	bool SaveTargetData(string FileName,_TATGETDATA&);

	CMaterial *LoadMaterialFile(string &name);

};

