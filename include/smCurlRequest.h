#pragma once

#include <smScriptProvider.h>
#include <curl/curlbuild.h>
#include <curl/curl.h>

namespace SimpleMenu
{
	class CsmMenu;
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

		void Perform(const char* title, const char* message);

		static CsmCurlRequest* Create();
		static void Destroy(CsmCurlRequest*);

		static std::string GetString(const char* title,const char* message,const char* url);
	protected:
		CURL* GetCurl();
		CURLM* GetCurlM();

		static size_t WriteFunction(void *buffer, size_t size, size_t nmemb, void *userp);
		static size_t ReadFunction(void *buffer, size_t size, size_t nmemb, void *userp);
		static bool PerformCallback(CsmMenu*m, CsmCurlRequest*);
	};

	class CsmCurlMemoryRequest: public CsmCurlRequest
	{
		CIwArray<uint8> outputBuffer;
		CIwArray<uint8> inputBuffer;
	public:
		//Constructor
		CsmCurlMemoryRequest();
		//Desctructor
		virtual ~CsmCurlMemoryRequest();

		const uint8* GetInputBuffer() const { if (inputBuffer.empty()) return 0; return &inputBuffer.front();}
		size_t GetInputBufferSize() const {return inputBuffer.size();}
		virtual size_t OnWrite(void *buffer, size_t size, size_t nmemb);
		virtual size_t OnRead(void *buffer, size_t size, size_t nmemb);
	};
}
