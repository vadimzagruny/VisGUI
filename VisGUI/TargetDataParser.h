#pragma once


namespace TargetDataParser
{

/**	Mesh Data struct*/
struct _MESHDATA{
		string MeshName;	/**< Mesh name member*/
		string ParentName;  /**< Mesh parent name member*/
		float Pos[3];		/**< Mesh position (float3) member*/
		float Rot[3];		/**< Mesh rotation (float3) member*/
};

/**	Target Data struct*/
struct _TATGETDATA{
		string ClassName;
		list<_MESHDATA> listMesh;	/**< MeshData List member*/
};


/**
\brief Target data parser class.
*  \author    Vadim Zagruny
*  \version   2.1
*  \date      2017
*  \copyright  Vadim Zagruny KhMBDB.
*	
*	Save and Parse Target configs in XML file
*/
class CTargetDataParser
{


private :

	_TATGETDATA m_TargetData; /**< Target data member*/

	bool	QueryBoolAttribute(const char* AttrName, TiXmlElement *root);
	float	QueryFloatAttribute(const char* AttrName, TiXmlElement *root);
	double	QueryDoubleAttribute(const char* AttrName, TiXmlElement *root);
	int		QueryIntAttribute(const char* AttrName, TiXmlElement *root);
	UINT	QueryUIntAttribute(const char* AttrName, TiXmlElement *root);

	void ReadFloats(const char *str, float *floats, unsigned int numFloats);

	bool MeshDataParse(TiXmlElement *element);
	bool ClassDataParse(TiXmlElement *element);

public:

	/**
	\brief Parse XML file and read targets data.
	\param	FileName is a path to config file argument
	\param	TargetData is a link to _TATGETDATA struct
	return	true if success
	*/
	bool ParseXMLTargetCfg(string FileName,_TATGETDATA& TargetData);

	/**
	\brief Wright targets data and Save XML file.
	\param	newFileName is a path to save config file argument
	\param	TargetData is a link to _TATGETDATA struct
	return	true if success
	*/
	bool SaveXMLTargetCfg(string newFileName,_TATGETDATA& TargetData);

	CTargetDataParser();
	~CTargetDataParser();
};

}