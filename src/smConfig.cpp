#include <s3eSecureStorage.h>
#include <pugixml.hpp>
#include <sstream>
#include "smConfig.h"
#include "simplemenu.h"

#include "smConfigSelect.h"
#include "smConfigSlider.h"
#include "smConfigSwitch.h"
#include "smConfigCheckbox.h"
#include "smConfigTextBox.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	const char* smConfigFilePath = "ram://app.config";
	smConfigStorageType smConfigType = SM_CONFIG_FILE;//SM_CONFIG_SECURITY_STORAGE;
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

	class CsmXmlWriter: public pugi::xml_writer
	{
	public:
		CIwArray<char> buffer;
		virtual void write(const void* data, size_t size)
		{
			size_t pos = buffer.size();
			buffer.resize(pos+size);
			memcpy(&buffer[pos], data, size);
		}
	};
}
void SimpleMenu::smConfigInit()
{
	smConfigInit(smConfigType);
}
void SimpleMenu::smConfigInit(smConfigStorageType t)
{
	if (initConfigCounter && smConfigType != t)
		IwAssertMsg(SM,false,("Can't change config type after initialization"));
	++initConfigCounter;
	if (initConfigCounter != 1)
		return;
	smConfigType = t;
	smInit();
	smRegisterClass(CsmConfig::GetClassDescription());

	IW_CLASS_REGISTER(CsmConfigSelect);
	IW_CLASS_REGISTER(CsmConfigSwitch);
	IW_CLASS_REGISTER(CsmConfigSlider);
	IW_CLASS_REGISTER(CsmConfigCheckbox);
	IW_CLASS_REGISTER(CsmConfigTextBox);

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
	static  TsmScriptableClassDeclaration<CsmConfig> d (0, "CsmConfig",
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
void CsmConfig::LoadSS()
{
	uint16 size = 1024;
	CIwArray<char> buf;
	retry:
	buf.resize(size);
	s3eResult err = s3eSecureStorageGet(&buf[0], size);
	if (err == S3E_RESULT_ERROR)
	{
		switch (s3eSecureStorageGetError())
		{
			case S3E_SECURESTORAGE_ERR_INSUFF:
				size*=2;
				goto retry;
			case S3E_SECURESTORAGE_ERR_NOT_FOUND:
				break;
			default:
				IwAssertMsg(SM, false, (s3eSecureStorageGetErrorString()));
				break;
		}
		LoadDefault();
		return;
	}
	smConfigDocument->load_buffer(&buf[0], strlen(&buf[0]));

}
void CsmConfig::SaveSS()
{
	if (!smConfigDocument)
		return;
	CsmXmlWriter w;
	smConfigDocument->save(w);
	w.write("\0", 1);
	size_t s = w.buffer.size()-1;
	if (s > 0x0FFFF)
	{
		IwAssertMsg(SM, false, ("Security storage isn't big enough to store the file"));
	}
	s3eResult err = s3eSecureStoragePut(&w.buffer[0], (uint16)s);
	if (err == S3E_RESULT_ERROR)
		IwAssertMsg(SM, false, (s3eSecureStorageGetErrorString()));
}
void CsmConfig::LoadDefault()
{
	smConfigDocument->load("<?xml version=\"1.0\" encoding=\"utf-8\" ?><configuration><appSettings></appSettings></configuration>");
}
void CsmConfig::LoadFile()
{
	if (s3eFileCheckExists(smConfigFilePath))
	{
		pugi::xml_parse_result r = smConfigDocument->load_file(smConfigFilePath);
		if (r.status == pugi::status_ok)
			return;
		IwAssertMsg(sm, false, (r.description()));
	}
	LoadDefault();
}
void CsmConfig::SaveFile()
{
	if (smConfigDocument)
		smConfigDocument->save_file(smConfigFilePath);
}
void CsmConfig::Load()
{
	if (!smConfigDocument)
		smConfigDocument = new pugi::xml_document();

	switch (smConfigType)
	{
	case SM_CONFIG_FILE:
	default:
		LoadFile();
		break;
	case SM_CONFIG_SECURITY_STORAGE:
		LoadSS();
		break;
	}
}
void CsmConfig::Save()
{
	switch (smConfigType)
	{
	case SM_CONFIG_FILE:
	default:
		SaveFile();
		break;
	case SM_CONFIG_SECURITY_STORAGE:
		SaveSS();
		break;
	}
}

void CsmConfig::Close()
{
	if (smConfigDocument)
	{
		delete smConfigDocument;
		smConfigDocument = 0;
	}
}
