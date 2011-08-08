#include "smConfig.h"
#include "simplemenu.h"
#include <pugixml.hpp>
#include <sstream>

using namespace SimpleMenu;

namespace SimpleMenu
{
	const char* smConfigFilePath = "ram://app.config";
	pugi::xml_document* smConfigDocument=0;
	pugi::xml_node smGetConfigNodeText(const char* name)
	{
		pugi::xml_node configuration = smConfigDocument->child("configuration");
		if (!configuration) configuration = smConfigDocument->append_child("configuration");
		pugi::xml_node appSettings = configuration.child("appSettings");
		if (!appSettings) appSettings = configuration.append_child("appSettings");
		pugi::xml_node item = appSettings.child(name);
		if (!item)
		{
			item = appSettings.append_child(name);
		}
		pugi::xml_node text = item.first_child();
		if (!text) text = item.append_child(pugi::node_pcdata);
		return text;
	}
	int initConfigCounter = 0;
}

void SimpleMenu::smConfigInit()
{
	++initConfigCounter;
	if (initConfigCounter != 1)
		return;

	smInit();
	smRegisterClass(CsmConfig::GetClassDescription());
	CsmConfig::Load();
}

void SimpleMenu::smConfigTerminate()
{
	--initConfigCounter;
	if (initConfigCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smConfigTerminate doesn't match smConfigInit"));
	if (initConfigCounter != 0)
		return;

	if (smConfigDocument)
	{
		CsmConfig::Save();
		delete smConfigDocument;
		smConfigDocument = 0;
	}

	smTerminate();
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmConfig::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmConfig> d ("CsmConfig",
			ScriptTraits::Method("GetInteger", &CsmConfig::GetInteger),
			ScriptTraits::Method("GetFloat", &CsmConfig::GetFloat),
			ScriptTraits::Method("GetString", &CsmConfig::GetString),
			ScriptTraits::Method("SetInteger", &CsmConfig::SetInteger),
			ScriptTraits::Method("SetFloat", &CsmConfig::SetFloat),
			ScriptTraits::Method("SetString", &CsmConfig::SetString),
			0);
	return &d;
}
void CsmConfig::SetInteger(const char* name, int val)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	std::stringstream s;
	s << val;
	text.set_value(s.str().c_str());
}
void CsmConfig::SetLong(const char* name, int64 val)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	std::stringstream s;
	s << val;
	text.set_value(s.str().c_str());
}
void CsmConfig::SetFloat(const char* name, float val)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	char buf [32];
	sprintf(buf,"%g",val);
	text.set_value(buf);
}
void CsmConfig::SetString(const char* name, const char* val)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	if (!val)
		text.set_value("");
	else
		text.set_value(val);
}
bool CsmConfig::IsExist(const char* name)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	const char* v = text.value();
	if (!v || !*v)
		return false;
	return true;
}
int64 CsmConfig::GetLong(const char* name)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	const char* v = text.value();
	int64 r = 0;
	if (!v || !*v)
		return r;

//	std::stringstream stream = std::stringstream(std::string(v));
	std::stringstream(std::string(v)) >> r;

	//stream >> r;
	//sscanf(v,"%ld",&r);
	return r;
}
int CsmConfig::GetInteger(const char* name)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	const char* v = text.value();
	int r = 0;
	if (!v || !*v)
		return r;
	sscanf(v,"%d",&r);
	return r;
}
float CsmConfig::GetFloat(const char* name)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	const char* v = text.value();
	float r = 0;
	if (!v || !*v)
		return r;
	sscanf(v,"%f",&r);
	return r;
}
const char* CsmConfig::GetString(const char* name)
{
	pugi::xml_node text = smGetConfigNodeText(name);
	const char* v = text.value();
	return v;
}
void CsmConfig::Load()
{
	if (!smConfigDocument)
		smConfigDocument = new pugi::xml_document();

	if (s3eFileCheckExists(smConfigFilePath))
	{
		pugi::xml_parse_result r = smConfigDocument->load_file(smConfigFilePath);
		if (r.status == pugi::status_ok)
			return;
		IwAssertMsg(sm, false, (r.description()));
	}
	smConfigDocument->load("<?xml version=\"1.0\" encoding=\"utf-8\" ?><configuration><appSettings></appSettings></configuration>");
}
void CsmConfig::Save()
{
	if (smConfigDocument)
		smConfigDocument->save_file(smConfigFilePath);
}
void CsmConfig::Close()
{
	if (smConfigDocument)
	{
		delete smConfigDocument;
		smConfigDocument = 0;
	}
}
