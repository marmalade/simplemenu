#pragma once

#include "smScriptProvider.h"

namespace SimpleMenu
{
	enum smConfigStorageType
	{
		SM_CONFIG_FILE,
		SM_CONFIG_SECURITY_STORAGE,
	};

	class CsmConfig
	{
	public:
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static void SetInteger(const char* name, int val);
		static void SetLong(const char* name, int64 val);
		static void SetFloat(const char* name, float val);
		static void SetString(const char* name, const char* val);

		static bool IsExist(const char* name);

		static int GetInteger(const char* name);
		static int64 GetLong(const char* name);
		static float GetFloat(const char* name);
		static const char* GetString(const char* name);

		static void Load();
		static void Save();
		static void Close();

	protected:
		static void LoadFile();
		static void SaveFile();
		static void LoadSS();
		static void SaveSS();
		static void LoadDefault();
	};

	void smConfigInit();
	void smConfigInit(smConfigStorageType);
	void smConfigTerminate();
}
