#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "SimpleMenu.h"
#include "smImageGallery.h"
#include "smMenu.h"
#include "smImage.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmImageGallery);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmImageGallery);

//Constructor
CsmImageGallery::CsmImageGallery()
{
	carouselPosition = 0;
	touched = false;
}
//Desctructor
CsmImageGallery::~CsmImageGallery()
{
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmImageGallery::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmImageGallery> d (CsmGallery::GetClassDescription(), "CsmImageGallery",
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmImageGallery::Serialise ()
{
	CsmGallery::Serialise();
	smSerialiseString(path);
	smSerialiseString(pattern);
}
void CsmImageGallery::OnAttachToMenu(CsmMenu*m,CsmItem*i)
{
	CsmGallery::OnAttachToMenu(m,i);
	while (!childItems.empty())
		childItems.pop_back();

	if (path.empty()) return;

	s3eFileList *  list = s3eFileListDirectory(path.c_str());
	if (list)
	{
		char buf[1024];
		while (S3E_RESULT_SUCCESS == s3eFileListNext(list, buf, sizeof(buf)))
		{
			if (!pattern.empty() && (buf!=strstr(buf,pattern.c_str())))
				continue;
			std::stringstream s;
			s << path;
			s << buf;
			std::string p = s.str();
			CsmImage* img = new CsmImage(p.c_str());
			AddItem(img);
		}
		s3eFileListClose(list);
	}
}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	CsmImageGallery::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("path", pAttrName))
	{
		smReadString(pParser, &path);
		return true;
	}
	if (!stricmp("pattern", pAttrName))
	{
		smReadString(pParser, &pattern);
		return true;
	}
	
	return CsmGallery::ParseAttribute(pParser, pAttrName);
}

#endif