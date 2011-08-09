#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include <IwModel.h>
#include "smImageSource.h"
#include "sm3D.h"

namespace SimpleMenu
{

	class Csm3DModel : public CsmImageSource
	{
	private:
		uint32 modelHash;
		CIwModel* model;
		// Texture object
		CIwTexture* texture;
		// Texture object to receive surface
		CIwGxSurface* surface;
		CIwMaterial* material;
		bool isValid;
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

		virtual bool IsAvailable() const;
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const;
		virtual void Prepare(const CIwSVec2& recommendedSize);
		virtual CIwMaterial* GetMaterial();

#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
#endif
	protected:
		void Init3DModel();
	};
}