#pragma once

#include <smScriptProvider.h>
#include <curl/curlbuild.h>
#include <curl/curl.h>

namespace SimpleMenu
{
	class CsmCurlRequest
	{
	private:
		CURL* curl;
		CURLM *curlm;
	public:
		//Constructor
		CsmCurlRequest();
		//Desctructor
		virtual ~CsmCurlRequest();

		void SetUrl(const char* url);
		//Set "USER:PASSWORD" string
		void SetUserPassword(const char* userPass);

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		virtual size_t OnWrite(void *buffer, size_t size, size_t nmemb);
		virtual size_t OnRead(void *buffer, size_t size, size_t nmemb);

		void Perform();

		static CsmCurlRequest* Create();
		static void Destroy(CsmCurlRequest*);
	protected:
		CURL* GetCurl();
		CURLM* GetCurlM();

		static size_t WriteFunction(void *buffer, size_t size, size_t nmemb, void *userp);
		static size_t ReadFunction(void *buffer, size_t size, size_t nmemb, void *userp);
	};
}
