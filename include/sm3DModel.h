#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include <IwModel.h>
#include "smTerminalItem.h"
#include "sm3D.h"

namespace SimpleMenu
{

	class Csm3DModel : public CsmTerminalItem
	{
	private:
		uint32 modelHash;
		CIwModel* model;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(Csm3DModel);
		//Constructor
		Csm3DModel();
		//Constructor
		Csm3DModel(uint32 hash);
		//Desctructor
		virtual ~Csm3DModel();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		virtual void Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize);
		//Render 3DModel on the screen surface
		virtual void Render(smItemContext* renderContext);

		virtual void RearrangeChildItems();

		virtual uint32 GetElementNameHash();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	protected:
		void Init3DModel();
	};
}