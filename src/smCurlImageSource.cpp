#include "smCurlImageSource.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
}

// Check if imge is available (loaded, downloaded etc)
bool CsmCurlImageSource::IsAvailable() const
{
	return false;
}
// Get recommended size of the image
CIwSVec2 CsmCurlImageSource::GetRecommendedSize(const CIwSVec2& area) const
{
	return area;
}
// Prepare image source to render the image
void CsmCurlImageSource::Prepare(const CIwSVec2& recommendedSize)
{
}
// Get image material to draw quad
CIwMaterial* CsmCurlImageSource::GetMaterial()
{
	return 0;
}