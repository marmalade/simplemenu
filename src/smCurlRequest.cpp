#include "smCurlRequest.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCurlRequest::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCurlRequest> d ("CsmCurlRequest",
			//ScriptTraits::Method("Create", &CsmCurlRequest::Create),
			//ScriptTraits::Method("Destroy", &CsmCurlRequest::Destroy),
			ScriptTraits::Method("SetUrl", &CsmCurlRequest::SetUrl),
			ScriptTraits::Method("Perform", &CsmCurlRequest::Perform),
			0);
	return &d;
}

//Constructor
CsmCurlRequest::CsmCurlRequest()
{
	curl = 0;
}

//Desctructor
CsmCurlRequest::~CsmCurlRequest()
{
	if (curlm)
	{
		curl_multi_cleanup(curlm);
		curlm = 0;
	}
	if (curl)
	{
		curl_free(curl);
		curl = 0;
	}
}
CURL* CsmCurlRequest::GetCurl()
{
	if (!curl)
	{
		curl = curl_easy_init();
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, ReadFunction);
		curl_easy_setopt(curl, CURLOPT_READDATA, this);
	}
	return curl;
}
void CsmCurlRequest::Perform()
{
	// TODO: add the waiting dialog box


	int running_handles;
	CURLMcode res = curl_multi_perform(GetCurlM(), &running_handles);
	if (res != CURLM_OK)
	{

	}
}

CURLM* CsmCurlRequest::GetCurlM()
{
	if (!curlm)
	{
		curlm = curl_multi_init( );
		curl_multi_add_handle(curlm, GetCurl());
	}
	return curlm;
}
void CsmCurlRequest::SetUrl(const char* url)
{
	curl_easy_setopt(GetCurl(), CURLOPT_URL, url);
}
void CsmCurlRequest::SetUserPassword(const char* userPass)
{
	curl_easy_setopt(GetCurl(), CURLOPT_USERPWD, userPass);
}
size_t CsmCurlRequest::OnWrite(void *buffer, size_t size, size_t nmemb)
{
	return 0;
}
size_t CsmCurlRequest::OnRead(void *buffer, size_t size, size_t nmemb)
{
	return 0;
}
size_t CsmCurlRequest::WriteFunction(void *buffer, size_t size, size_t nmemb, void *userp)
{
	return ((CsmCurlRequest*)userp)->OnWrite(buffer,size,nmemb);
}
size_t CsmCurlRequest::ReadFunction(void *buffer, size_t size, size_t nmemb, void *userp)
{
	return ((CsmCurlRequest*)userp)->OnRead(buffer,size,nmemb);
}
CsmCurlRequest* CsmCurlRequest::Create()
{
	return new CsmCurlRequest();
}
void CsmCurlRequest::Destroy(CsmCurlRequest* r)
{
	delete r;
}
