#include <IwTextParserITX.h>
#include <IwResManager.h>
#include <IwGx.h>
#include "sm3DModel.h"
#include "smMenu.h"
#include "fthFont.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(Csm3DModel);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(Csm3DModel);

//Get scriptable class declaration
CsmScriptableClassDeclaration* Csm3DModel::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<Csm3DModel> d ("Csm3DModel",
			ScriptTraits::Method("GetRoot", &Csm3DModel::GetRoot),
			ScriptTraits::Method("GetChildAt", &Csm3DModel::GetChildAt),
			ScriptTraits::Method("GetChildItemsCount", &Csm3DModel::GetChildItemsCount),
			0);
	return &d;
}

//Constructor
Csm3DModel::Csm3DModel()
{
	Init3DModel();
}
//Constructor
Csm3DModel::Csm3DModel(uint32 t)
{
	Init3DModel();
	modelHash = t;
}
//Desctructor
Csm3DModel::~Csm3DModel()
{
}
void Csm3DModel::Init3DModel()
{
	modelHash = 0;
	model = 0;
}

//Reads/writes a binary file using @a IwSerialise interface.
void Csm3DModel::Serialise ()
{
	CsmTerminalItem::Serialise();
	IwSerialiseUInt32(modelHash);
}
void Csm3DModel::Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize)
{
	if (modelHash == 0)
		return;
	if (model == 0)
		model = (CIwModel*)IwGetResManager()->GetResHashed(modelHash, "CIwModel", IW_RES_PERMIT_NULL_F);

	size = recommendedSize;

	RearrangeChildItems();
}
void Csm3DModel::RearrangeChildItems()
{
}
//Render 3DModel on the screen surface
void Csm3DModel::Render(smItemContext* renderContext)
{
	if (modelHash == 0)
		return;

	if (model == 0)
		return;

	
	model->Render();
}
uint32 Csm3DModel::GetElementNameHash()
{
	static uint32 name = IwHashString("3DModel");
	return name;
}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	Csm3DModel::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("model",pAttrName))
	{
		pParser->ReadStringHash(&modelHash);
		return true;
	}
	return CsmTerminalItem::ParseAttribute(pParser, pAttrName);
}

#endif
