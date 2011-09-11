#include "simplemenu.h"
#include "smCurlImageSource.h"
#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include <sys/param.h>

extern "C"
{
#include <jpeglib.h>
}

using namespace SimpleMenu;

namespace SimpleMenu
{
	// dummy funcs to help libjpeg
	static void JPEGInitSource(j_decompress_ptr cinfo)
	{
	}

	static boolean JPEGFillInputBuffer(j_decompress_ptr cinfo)
	{
		return 0;
	}

	static void JPEGSkipInputData(j_decompress_ptr cinfo, long num_bytes)
	{
		cinfo->src->next_input_byte += num_bytes;
		cinfo->src->bytes_in_buffer -= num_bytes;
	}

	static void JPEGTermSource(j_decompress_ptr cinfo)
	{
	}
}

//Instantiate the default factory function for a named class 
IW_CLASS_FACTORY(CsmCurlImageSource);
//This macro is required within some source file for every class derived from CIwManaged. It implements essential functionality
IW_MANAGED_IMPLEMENT(CsmCurlImageSource);

//Constructor
CsmCurlImageSource::CsmCurlImageSource()
{
	isActive = false;
	material = 0;
	texture = 0;
	image = 0;
}
//Desctructor
CsmCurlImageSource::~CsmCurlImageSource()
{
	if (material) delete material;
	if (texture) delete texture;
	if (image) delete image;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCurlImageSource::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCurlImageSource> d (0, "CsmCurlImageSource",
			//ScriptTraits::Method("GetRoot", &CsmCurlImageSource::GetRoot),
			0);
	return &d;
}

//Reads/writes a binary file using @a IwSerialise interface.
void CsmCurlImageSource::Serialise ()
{
	smSerialiseString(url);
}

// Check if imge is available (loaded, downloaded etc)
bool CsmCurlImageSource::IsAvailable() const
{
	return material != 0;
}
// Get recommended size of the image
CIwSVec2 CsmCurlImageSource::GetRecommendedSize(const CIwSVec2& area) const
{
	if (texture)
		return CIwSVec2((int16)texture->GetWidth(),(int16)texture->GetHeight());
	return area;
}
// Prepare image source to render the image
void CsmCurlImageSource::Prepare(const CIwSVec2& recommendedSize)
{
}
// Get image material to draw quad
CIwMaterial* CsmCurlImageSource::GetMaterial()
{
	return material;
}
void CsmCurlImageSource::SetImageToMaterial()
{
	if (!material) material = new CIwMaterial();
	if (!texture) texture = new CIwTexture();
	material->SetColAmbient(255,255,255,255);
	if (image && image->GetWidth() > 0)
	{
		texture->SetImage(image);
		texture->Upload();
		material->SetTexture(texture);
	}
}
void CsmCurlImageSource::DecodeJpeg(void*buf, size_t len)
{
    jpeg_decompress_struct cinfo;
    bzero(&cinfo, sizeof(cinfo));

    JSAMPARRAY buffer;      /* Output row buffer */
    int row_stride;     /* physical row width in output buffer */

    jpeg_source_mgr srcmgr;

    srcmgr.bytes_in_buffer = len;
    srcmgr.next_input_byte = (JOCTET*) buf;
    srcmgr.init_source = JPEGInitSource;
    srcmgr.fill_input_buffer = JPEGFillInputBuffer;
    srcmgr.skip_input_data = JPEGSkipInputData;
    srcmgr.resync_to_restart = jpeg_resync_to_restart;
    srcmgr.term_source = JPEGTermSource;
	
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);

    jpeg_create_decompress(&cinfo);
    cinfo.src = &srcmgr;

    if (JPEG_HEADER_OK != jpeg_read_header(&cinfo, TRUE))
	{
		return;
	}
    jpeg_start_decompress(&cinfo);

    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;

    /* Make a one-row-high sample array that will go away when done with image */
    buffer = (*cinfo.mem->alloc_sarray)
        ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

	int y = 0;
	image = new CIwImage();
	image->SetFormat(CIwImage::RGB_888);
	image->SetWidth(cinfo.output_width);
	image->SetHeight(cinfo.output_height);
	image->SetBuffers();
    while (cinfo.output_scanline < cinfo.output_height)// count through the image
    {
        /* jpeg_read_scanlines expects an array of pointers to scanlines.
         * Here the array is only one element long, but you could ask for
         * more than one scanline at a time if that's more convenient.
         */
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
		uint8* t = image->GetTexels()+image->GetPitch()*y;
		for (JDIMENSION x=0; x<cinfo.output_width; ++x)
		{
			t[2] = buffer[0][x*3+0];
			t[1] = buffer[0][x*3+1];
			t[0] = buffer[0][x*3+2];
			t += 3;
		}
		++y;
    }

    (void) jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
}
void CsmCurlImageSource::DecodePng(void*buf, size_t len)
{
	image = new CIwImage();
	{
		s3eFile* f =  s3eFileOpenFromMemory(buf, len);
		if (f)
		{
			image->ReadFile(f);
			s3eFileClose(f);
		}
	}
}
bool CsmCurlImageSource::LoadFile(const char* fileName)
{
    char* data;
    int len;
    s3eFile *f = 0;

	if (!(f = s3eFileOpen( fileName, "rb")))
    {
        return false;
    }

    len = (int)s3eFileGetSize(f);
    if (len <= 0)
	{
        s3eFileClose(f);
        return false;
	}

    data = (char*)s3eMalloc(len);
    if (!data)
    {
        s3eFileClose(f);
        return false;
    }

    uint32 rtn = s3eFileRead(data, 1, len, f);
    s3eFileClose(f);

    if (rtn == (uint32)len)
    {
		if (data[0] == 0x89 && data[1] == 'P' && data[2] == 'N' && data[3] == 'G')
		{
			DecodePng(data,len);
		}
		else
		{
			DecodeJpeg(data,len);
		}
    }
    s3eFree(data);
	return true;
}
//Animate item and all child items
void CsmCurlImageSource::Animate(iwfixed timespan)
{
	if (!material && !isActive && !url.empty())
	{
		if ((url.length() > 5) && (url[0] == 'r') &&
			((url[1] == 'a') || (url[1] == 'o')) && (url[2] == 'm') && (url[3] == ':') && (url[4] == '/'))
		{
			LoadFile(url.c_str());
			SetImageToMaterial();
		}
		else
		{
			isActive = true;
			request.SetUrl(url.c_str());
		}
	}
	if (isActive)
	{
		if (!request.PerformStep())
		{
			isActive = false;

			if (request.GetInputBufferSize() != 0)
			{
				const char* t = request.GetContentType();
				if (!stricmp(t, "image/jpg") || !stricmp(t, "image/jpeg"))
				{
					DecodeJpeg((void*)request.GetInputBuffer(), request.GetInputBufferSize());
				}
				else
				{
					DecodePng((void*)request.GetInputBuffer(), request.GetInputBufferSize());
				}
			}
			SetImageToMaterial();
		}
	}
}

#ifdef IW_BUILD_RESOURCES
//Parses from text file: parses attribute/value pair.
bool	CsmCurlImageSource::ParseAttribute(CIwTextParserITX* pParser, const char* pAttrName)
{
	if (!stricmp("url",pAttrName) || !stricmp("get",pAttrName))
	{
		smReadString(pParser, &url);
		return true;
	}
	return CsmImageSource::ParseAttribute(pParser, pAttrName);
}
#endif