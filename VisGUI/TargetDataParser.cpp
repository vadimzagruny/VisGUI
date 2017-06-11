#include "stdafx.h"
#include "TargetDataParser.h"

#pragma setlocale("English")


#ifdef _DETECT_MEMORY_LEAKS

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#endif // _DETECT_MEMORY_LEAKS

using namespace TargetDataParser;

CTargetDataParser::CTargetDataParser()
{
}


CTargetDataParser::~CTargetDataParser()
{
	m_TargetData.listMesh.clear();
}

bool CTargetDataParser::ParseXMLTargetCfg(string FileName , _TATGETDATA& TargetData)
{
	TiXmlDocument doc;
	doc.LoadFile(FileName.c_str());

	TiXmlElement* root = doc.RootElement();

	for (TiXmlNode* node = root->IterateChildren(0); node; node = root->IterateChildren(node)){
		string str = node->Value();

		if (!str.compare("Class")){
			ClassDataParse((TiXmlElement*)node);
		}
		else if (!str.compare("Mesh")){
			MeshDataParse((TiXmlElement*)node);
		}

		//cout <<(str.c_str())<<endl;
		int a = 0;
	}

	
	TargetData = m_TargetData;
	
	return true;
}

bool CTargetDataParser::SaveXMLTargetCfg(string newFileName,_TATGETDATA& TargetData)
{
	TiXmlDocument sdoc(newFileName.c_str());

	TiXmlElement Root("Target");
	//setClass
	TiXmlElement className("Class");
	className.SetAttribute("name", TargetData.ClassName.c_str());
	Root.InsertEndChild(className);

	//setMesh
	for (auto it :  TargetData.listMesh){
		TiXmlElement eMesh("Mesh");
		//save mesh name 
		eMesh.SetAttribute("name", it.MeshName.c_str());
		//save parent name
		if(it.ParentName.compare("\0") != 0)
		eMesh.SetAttribute("ParentName", it.ParentName.c_str());
		//save position
		char pos[100] = {0};
		sprintf(pos,"%f %f %f",it.Pos[0],it.Pos[1],it.Pos[2]);
		eMesh.SetAttribute("Position", string(pos));
		//save rotation
		char rot[100] = {0};
		sprintf(rot,"%f %f %f",it.Rot[0],it.Rot[1],it.Rot[2]);
		eMesh.SetAttribute("Rotation", string(rot));

		Root.InsertEndChild(eMesh);

	}
	//
	sdoc.InsertEndChild(Root);
	sdoc.SaveFile();

	return true;
}


bool CTargetDataParser::MeshDataParse(TiXmlElement *element)
{
	string name		= element->Attribute("Name");
	string Parent	= "NONE"; 
	string Pos		= element->Attribute("Position");
	string Rot		= element->Attribute("Rotation");

	if(element->Attribute ("ParentName"))
	Parent = element->Attribute("ParentName");

	_MESHDATA MeshData;
	
	MeshData.MeshName = name;
	
	ReadFloats(Pos.c_str(), MeshData.Pos, 3);
	ReadFloats(Rot.c_str(), MeshData.Rot, 3);

	m_TargetData.listMesh.push_back(MeshData);


	int c1 = name.find("root");
	int c2 = name.find("Root");

	return true;
}
bool CTargetDataParser::ClassDataParse(TiXmlElement *element)
{
	string name = element->Attribute("Name");

	m_TargetData.ClassName = name;

	return true;
}

bool CTargetDataParser::QueryBoolAttribute(const char* AttrName, TiXmlElement *root)
{
	assert(root);
	bool bVal;
	root->QueryBoolAttribute(AttrName, &bVal);
	return (bool)bVal;
}

float CTargetDataParser::QueryFloatAttribute(const char* AttrName, TiXmlElement *root)
{
	float fVal;
	root->QueryFloatAttribute(AttrName, &fVal);
	return  (float)fVal;
}

double CTargetDataParser::QueryDoubleAttribute(const char* AttrName, TiXmlElement *root)
{
	double dVal;
	root->QueryDoubleAttribute(AttrName, &dVal);
	return  (double)dVal;
}

int	 CTargetDataParser::QueryIntAttribute(const char* AttrName, TiXmlElement *root)
{
	int iVal;
	root->QueryIntAttribute(AttrName, &iVal);
	return  (int)iVal;
}
UINT CTargetDataParser::QueryUIntAttribute(const char* AttrName, TiXmlElement *root)
{
	UINT uVal;
	root->QueryUnsignedAttribute(AttrName, &uVal);
	return  (UINT)uVal;
}

void CTargetDataParser::ReadFloats(const char *str, float *floats, unsigned int numFloats)
{
	float fcount = 0;
	while (*str && !((*str >= '0'&&*str <= '9') || *str == '.')) str++;
	for (unsigned int i = 0; i<numFloats; i++)
	{
		if (*str)
		{
			floats[i] = (float)atof(str);
			while (*str && ((*str >= '0'&&*str <= '9') || *str == '.')) str++;
			while (*str && !((*str >= '0'&&*str <= '9') || *str == '.')) str++;
			fcount++;
		}
	}
}
