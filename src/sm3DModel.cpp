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
			//ScriptTraits::Method("GetRoot", &Csm3DModel::GetRoot),
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
	if (surface)
		delete surface;
	if (material)
		delete material;
	if (texture)
		delete texture;
}
void Csm3DModel::Init3DModel()
{
	modelHash = 0;
	model = 0;
	texture = 0;
	surface = 0;
	material = 0;
	isValid = false;
}

//Reads/writes a binary file using @a IwSerialise interface.
void Csm3DModel::Serialise ()
{
	CsmImageSource::Serialise();
	IwSerialiseUInt32(modelHash);
}

bool Csm3DModel::IsAvailable() const
{
	return modelHash != 0;
}
CIwSVec2 Csm3DModel::GetRecommendedSize(const CIwSVec2& area) const
{
	return area;
}
void Csm3DModel::Prepare(const CIwSVec2& recommendedSize)
{
	if (isValid)
		return;
	if (!model)
	{
		model = (CIwModel*)IwGetResManager()->GetResHashed(modelHash, "CIwModel", IW_RES_PERMIT_NULL_F);
	}
	if (!model)
		return;
	if (!surface)
	{
		surface = new CIwGxSurface();
		if( !surface->CreateSurface(0, recommendedSize.x, recommendedSize.y, 0) )
		  IwError(("Surface creation failed"));
	}
	if (!material)
	{
		material = new CIwMaterial();
		material->SetTexture(surface->GetTexture());
		material->SetColAmbient(255,255,255,255);
	}
	surface->MakeCurrent();
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
	//IwGxClear(IW_GX_DEPTH_BUFFER_F);
	model->Render();
	IwGxFlush();
	CIwGxSurface::MakeDisplayCurrent();
	isValid = true;

}
CIwMaterial* Csm3DModel::GetMaterial()
{
	return material;
}
//void Csm3DModel::Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize)
//{
//	if (modelHash == 0)
//		return;
//	if (model == 0)
//		model = (CIwModel*)IwGetResManager()->GetResHashed(modelHash, "CIwModel", IW_RES_PERMIT_NULL_F);
//
//	size = recommendedSize;
//
//	RearrangeChildItems();
//}
//void Csm3DModel::RearrangeChildItems()
//{
//}
////Render 3DModel on the screen surface
//void Csm3DModel::Render(smItemContext* renderContext)
//{
//	if (modelHash == 0)
//		return;
//
//	if (model == 0)
//		return;
//
//	
//	model->Render();
//}
//uint32 Csm3DModel::GetElementNameHash()
//{
//	static uint32 name = IwHashString("3DModel");
//	return name;
//}
#ifdef IW_BUILD_RESOURCES

//Parses from text file: parses attribute/value pair.
bool	Csm3DModel::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("model",pAttrName))
	{
		pParser->ReadStringHash(&modelHash);
		return true;
	}
	return CsmImageSource::ParseAttribute(pParser, pAttrName);
}

#endif
