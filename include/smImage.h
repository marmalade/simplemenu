#pragma once

#include <IwResManager.h>
#include <IwManagedList.h>
#include "smTerminalItem.h"

namespace SimpleMenu
{
	class CsmImageSource;

	class CsmImage : public CsmTerminalItem
	{
	private:
		CIwSVec2 rectPos;
		CIwSVec2 rectSize;
		CIwColour rectColour;
		CsmImageSource* source;

		uint32 styleSheetHash;
		CsmStyleSheet* styleSheet;
	public:
		//Declare managed class
		IW_MANAGED_DECLARE(CsmImage);
		//Constructor
		CsmImage();
		//Constructor
		CsmImage(const char* hash);
		//Desctructor
		virtual ~CsmImage();

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		//Reads/writes a binary file using @a IwSerialise interface.
		virtual void Serialise ();

		//Animate item and all child items
		virtual void Animate(iwfixed timespan);

		virtual void Prepare(smItemContext* renderContext, const CIwSVec2& recommendedSize);
		//Render image on the screen surface
		virtual void Render(smItemContext* renderContext);

		virtual void RearrangeChildItems();

		virtual uint32 GetElementNameHash();
#ifdef IW_BUILD_RESOURCES
		//Parses from text file: parses attribute/value pair.
		virtual	bool	ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName);
		//Extends CIwParseable interface with this extra function: called on any "parent" object, if the "child" has not implemented ParseClose.
		virtual	void	ParseCloseChild(CIwTextParserITX* pParser, CIwManaged* pChild);
#endif
	protected:
		void InitImage();
	};

	bool smLoadImage(const char* name, CIwImage* image);
	void smDecodeJpeg(void*buf, size_t len, CIwImage* image);
	void smDecodePng(void*buf, size_t len, CIwImage* image);
}