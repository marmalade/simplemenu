#include "smCamera.h"
#include "s3eCamera.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	CsmCamera* g_smCamera = 0;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCamera::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCamera> d ("CsmCamera",
		ScriptTraits::Method("IsAvailable", &CsmCamera::IsAvailable),
		ScriptTraits::Method("SetQualityLow", &CsmCamera::SetQualityLow),
		ScriptTraits::Method("SetQualityMedium", &CsmCamera::SetQualityMedium),
		ScriptTraits::Method("SetQualityHigh", &CsmCamera::SetQualityHigh),
		ScriptTraits::Method("SetSizeSmallest", &CsmCamera::SetSizeSmallest),
		ScriptTraits::Method("SetSizeMedium", &CsmCamera::SetSizeMedium),
		ScriptTraits::Method("SetSizeLargest", &CsmCamera::SetSizeLargest),
		
			0);
	return &d;
}
CsmCamera::CsmCamera()
{
	image = 0;
	texture = 0;
	activeBuffer= 0;
	completeBuffer = 0;
	feedUpdated = false;
	started = false;
	sizeHint = S3E_CAMERA_STREAMING_SIZE_HINT_SMALLEST;
}
CsmCamera::~CsmCamera()
{
	g_smCamera = 0;

	if (image)
		delete image;
	if (activeBuffer) 
		delete []activeBuffer;
	if (completeBuffer) 
		delete []completeBuffer;
	if (texture)
		delete texture;
}
CsmCamera* CsmCamera::RequestFeature()
{
	if (g_smCamera)
		return g_smCamera;
	if (!IsAvailable())
		return 0;
	g_smCamera = new CsmCamera();
	g_smCamera->Start();
	return g_smCamera;
}
bool CsmCamera::IsAvailable()
{
	return s3eCameraAvailable() != 0;
}
void CsmCamera::StartFeature()
{
	OpenFeed();
}
void CsmCamera::StopFeature()
{
}
void CsmCamera::CloseFeed()
{
	if (!started)
		return;
	s3eCameraStop();
	s3eCameraUnRegister(S3E_CAMERA_UPDATE_STREAMING, CameraUpdate);
	started = false;
}
void CsmCamera::OpenFeed()
{
	if (started)
		return;
	if (S3E_RESULT_ERROR == s3eCameraRegister(S3E_CAMERA_UPDATE_STREAMING, CameraUpdate, this))
	{
		return;
	}
	quality = s3eCameraGetInt(S3E_CAMERA_QUALITY);
	type = s3eCameraGetInt(S3E_CAMERA_TYPE);
	contrast = s3eCameraGetInt(S3E_CAMERA_CONTRAST);
	brightness = s3eCameraGetInt(S3E_CAMERA_BRIGHTNESS);
	focus = s3eCameraGetInt(S3E_CAMERA_AUTO_FOCUS);

	s3eCameraSetInt(S3E_CAMERA_QUALITY, quality);
	s3eCameraSetInt(S3E_CAMERA_TYPE, type);
	s3eCameraSetInt(S3E_CAMERA_CONTRAST, contrast);
	s3eCameraSetInt(S3E_CAMERA_BRIGHTNESS, brightness);
	s3eCameraSetInt(S3E_CAMERA_AUTO_FOCUS, focus);

	if (s3eCameraStart(sizeHint, S3E_CAMERA_PIXEL_TYPE_RGB565_CONVERTED) == S3E_RESULT_SUCCESS)
	{
		started = true;
	}
}
void CsmCamera::SetQuality(int q)
{
	if (q == quality)
		return;
	quality = q;
	if (S3E_RESULT_SUCCESS != s3eCameraSetInt(S3E_CAMERA_QUALITY, q))
	{
		switch (s3eCameraGetError())
		{
		case S3E_CAMERA_ERR_UNSUPPORTED:
			break;
		default:
			break;
		}
	}
}
void CsmCamera::SetSize(s3eCameraStreamingSizeHint s)
{
	if (sizeHint != s)
	{
		sizeHint = s;
		CloseFeed(); OpenFeed();
	}
}
void CsmCamera::SetQualityLow()
{
	CsmCamera*c = CsmCamera::RequestFeature();	if (!c) return;
	c->SetQuality(S3E_CAMERA_STREAMING_QUALITY_HINT_LOW);
}
void CsmCamera::SetQualityMedium()
{
	CsmCamera*c = CsmCamera::RequestFeature();	if (!c) return;
	c->SetQuality(S3E_CAMERA_STREAMING_QUALITY_HINT_MED);
}
void CsmCamera::SetQualityHigh()
{
	CsmCamera*c = CsmCamera::RequestFeature();	if (!c) return;
	c->SetQuality(S3E_CAMERA_STREAMING_QUALITY_HINT_HIGH);
}
void CsmCamera::SetSizeSmallest()
{
	CsmCamera*c = CsmCamera::RequestFeature();	if (!c) return;
	c->SetSize(S3E_CAMERA_STREAMING_SIZE_HINT_SMALLEST);
}
void CsmCamera::SetSizeMedium()
{
	CsmCamera*c = CsmCamera::RequestFeature();	if (!c) return;
	c->SetSize(S3E_CAMERA_STREAMING_SIZE_HINT_MEDIUM);
}
void CsmCamera::SetSizeLargest()
{
	CsmCamera*c = CsmCamera::RequestFeature();	if (!c) return;
	c->SetSize(S3E_CAMERA_STREAMING_SIZE_HINT_LARGEST);
}
CIwTexture* CsmCamera::GetTexture()
{
	if (feedUpdated)
	{
		if (texture)
		{
			texture->ChangeTexels((uint8*)completeBuffer, CIwImage::RGB_565);
			texture->Upload();
			feedUpdated = false;
		}
	}
	return texture;
}

// Callback that gets filled with camera buffer data
int32 CsmCamera::CameraUpdate(void* systemData, void* userData)
{
	CsmCamera* g_currentFeed = (CsmCamera*)userData;
	if (!g_currentFeed)
		return 0;
	if (g_currentFeed->feedUpdated)
		return 0;

	s3eCameraFrameData* m_Frame = (s3eCameraFrameData*)systemData;
	IwTrace(TOE,("Frame %dx%d",m_Frame->m_Width, m_Frame->m_Height));
	g_currentFeed->RequireImage(m_Frame->m_Width,m_Frame->m_Height);
	int16* texels;

	//texels = g_currentFeed->completeBuffer; g_currentFeed->completeBuffer=g_currentFeed->activeBuffer; g_currentFeed->activeBuffer = texels;
	texels = g_currentFeed->activeBuffer;

	for (int yy=0; yy<(int)m_Frame->m_Height;++yy)
		for (int xx=0; xx<(int)m_Frame->m_Width;++xx)
		{
			int dx = xx;
			int dy = yy;
			// Rotate image on 90'
			if (g_currentFeed->FlipCoordinates())
			{
				dx = (int)m_Frame->m_Height-1-yy;
				dy = xx;
			}
			*(int16*)(texels+g_currentFeed->image->GetPitch()/2*dy+dx) = *(int16*)((uint8*)m_Frame->m_Data+xx*2+yy*m_Frame->m_Pitch);
		}

	g_currentFeed->activeBuffer=g_currentFeed->completeBuffer; g_currentFeed->completeBuffer = texels;
	g_currentFeed->feedUpdated = true;

	return 0;

}

void CsmCamera::RequireImage(int16 reqw, int16 reqh)
{
	// Rotate image on 90'
	if (FlipCoordinates())
	{
		int16 a = reqw; reqw = reqh; reqh = a;
	}

	if (image)
	{
		int16 imageW = image->GetWidth();
		int16 imageH = image->GetHeight();
		if (imageW == reqw && imageH == reqh)
			return;
		if (texture)
		{
			delete texture;
			texture = 0;
		}
		delete image;
		image = 0;
		delete [] completeBuffer;
		completeBuffer = 0;
		delete [] activeBuffer;
		activeBuffer = 0;
	}
	feedWidth = reqw;
	feedHeight = reqh;

	//int w = 1; while (w < reqw) w = w<<1;
	//int h = 1; while (h < reqh) h = h<<1;
	int w = reqw;
	int h = reqh;
	
	image = new CIwImage();
	image->SetFormat(CIwImage::RGB_565);
	image->SetWidth(w);
	image->SetHeight(h);
	image->SetBuffers();
	completeBuffer = new int16[image->GetPitch()/2*image->GetHeight()];
	activeBuffer = new int16[image->GetPitch()/2*image->GetHeight()];
	if (!texture)
	{
		texture = new CIwTexture();
		texture->SetModifiable(true);
		texture->SetMipMapping(false);
	}
	texture->SetImage(image);
}



// ================================================================

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmCameraImage);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmCameraImage);

//Constructor
CsmCameraImage::CsmCameraImage()
{
	material = 0;
}
//Desctructor
CsmCameraImage::~CsmCameraImage()
{
	if (material)
		delete material;
}
//Reads/writes a binary file using @a IwSerialise interface.
void CsmCameraImage::Serialise ()
{
	CsmImageSource::Serialise();
}

// Check if imge is available (loaded, downloaded etc)
bool CsmCameraImage::IsAvailable() const
{
	if (!CsmCamera::IsAvailable())
		return false;

	CsmCamera*c = CsmCamera::RequestFeature();
	return c->GetTexture() != 0;
}
// Get recommended size of the image
CIwSVec2 CsmCameraImage::GetRecommendedSize(const CIwSVec2& area) const
{
	CsmCamera*c = CsmCamera::RequestFeature();

	if (c)
	{
		CIwTexture* t = c->GetTexture();
		if (t)
			return CIwSVec2((int16)t->GetWidth(), (int16)t->GetHeight());
	}
	return area;
}
// Prepare image source to render the image
void CsmCameraImage::Prepare(const CIwSVec2& recommendedSize)
{
	CsmCamera*c = CsmCamera::RequestFeature();
	if (c)
	{
	};
}
// Get image material to draw quad
CIwMaterial* CsmCameraImage::GetMaterial()
{
	return material;
}
//Animate item and all child items
void CsmCameraImage::Animate(iwfixed timespan)
{
	if (!material)
		material = new CIwMaterial();
	CsmCamera*c = CsmCamera::RequestFeature();
	if (c)
		material->SetTexture(c->GetTexture());
}
