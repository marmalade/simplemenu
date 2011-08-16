#include "smCurlRequest.h"
#include "smWindowHistory.h"
#include "curl_ldap.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmCurlRequest::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmCurlRequest> d (0, "CsmCurlRequest",
			ScriptTraits::Method("Create", &CsmCurlRequest::Create),
			ScriptTraits::Method("Destroy", &CsmCurlRequest::Destroy),
			ScriptTraits::Method("SetUrl", &CsmCurlRequest::SetUrl),
			ScriptTraits::Method("Perform", &CsmCurlRequest::Perform),
			ScriptTraits::Method("GetString", &CsmCurlRequest::GetString),
			0);
	return &d;
}

//Constructor
CsmCurlRequest::CsmCurlRequest()
{
	curl = 0;
	curlm = 0;
}

//Desctructor
CsmCurlRequest::~CsmCurlRequest()
{
	if (curlm)
	{
		curl_multi_remove_handle(curlm, curl);
		curl_multi_cleanup(curlm);
		curlm = 0;
	}
	if (curl)
	{
		curl_easy_cleanup(curl);
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

		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 15);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);

		//SSL
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		//curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/CA.crt");
		//curl_easy_setopt(curl, CURLOPT_SSLCERT, "/path/to/client.pem");
	}
	return curl;
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
bool CsmCurlRequest::PerformCallback(CsmMenu*m, CsmCurlRequest* r)
{
	return r->PerformStep();
}
const char* CsmCurlRequest::GetContentType() const
{
	const char* p;
	curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &p);
	return p;
}
void CsmCurlRequest::SetTimeout(int t)
{
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, t);
}
int CsmCurlRequest::GetContentLength() const
{
	long p;
	curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &p);
	return (int)p;
}
int CsmCurlRequest::GetResponseCode() const
{
	long p;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &p);
	return (int)p;
}
bool CsmCurlRequest::PerformStep()
{
	int running_handles;
	CURLMcode res = CURLM_CALL_MULTI_PERFORM;
	while (res == CURLM_CALL_MULTI_PERFORM)
	{
		res = curl_multi_perform(GetCurlM(), &running_handles);
	}
	
	if (!running_handles)
	{
		CURLMsg *msg;
		int msgs_left;
		while ((msg = curl_multi_info_read(GetCurlM(), &msgs_left))) {
			if (msg->msg == CURLMSG_DONE) {
				if( msg->data.result != CURLE_OK ) {
					smAlert("Network error!", curl_easy_strerror(msg->data.result));
					break;
				}
			}
		}
	}
	return (running_handles == 1);
}
void CsmCurlRequest::Perform(const char* title, const char* message)
{
	BeforeRequest();
	smOpenWaitDialog(title, message, (smUpdateCallback)PerformCallback, this);
	AfterRequest();
}
void CsmCurlRequest::BeforeRequest()
{
}
void CsmCurlRequest::AfterRequest()
{
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

std::string CsmCurlRequest::GetString(const char* title,const char* message,const char* url)
{
	CsmCurlMemoryRequest r;
	r.SetUrl(url);
	r.Perform(title, message);
	const char* s = (const char*)r.GetInputBuffer();
	std::string res(s, s+ r.GetInputBufferSize());
	return res;
}

CsmCurlMemoryRequest::CsmCurlMemoryRequest()
{
}
CsmCurlMemoryRequest::~CsmCurlMemoryRequest()
{
}

size_t CsmCurlMemoryRequest::OnWrite(void *buffer, size_t size, size_t nmemb)
{
	size_t real_size = size * nmemb;
	size_t pos = inputBuffer.size();
	inputBuffer.resize(pos+real_size);
	memcpy(&inputBuffer[pos], buffer,real_size);
	return real_size;
}
size_t CsmCurlMemoryRequest::OnRead(void *buffer, size_t size, size_t nmemb)
{
	return 0;
}
void CsmCurlMemoryRequest::BeforeRequest()
{
	inputBuffer.clear();
}
void CsmCurlMemoryRequest::AfterRequest()
{
	outputBuffer.clear();
}