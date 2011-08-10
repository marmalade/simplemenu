#pragma once

#include <smImageSource.h>
#include "smCurlRequest.h"

namespace SimpleMenu
{
	class CsmCurlImageSource: public CsmImageSource
	{
	public:
		// Check if imge is available (loaded, downloaded etc)
		virtual bool IsAvailable() const;
		// Get recommended size of the image
		virtual CIwSVec2 GetRecommendedSize(const CIwSVec2& area) const;
		// Prepare image source to render the image
		virtual void Prepare(const CIwSVec2& recommendedSize);
		// Get image material to draw quad
		virtual CIwMaterial* GetMaterial();
	};
}