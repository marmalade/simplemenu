#pragma once

#include <pugixml.hpp>
#include <smCurlRequest.h>

namespace SimpleMenu
{
	class CsmOsmNode
	{
		public:
			int id;
			int version;
			int uid;
			float lon;
			float lat;
	};
	class CsmOsmWay
	{
		public:
	};
	class CsmOsmTag
	{
		public:
	};
	class CsmOsmXApiRequest
	{
	protected:
		CsmCurlMemoryRequest request;
		CIwArray<CsmOsmNode> nodes;
		CIwArray<CsmOsmWay> ways;
	public:
		//Constructor
		CsmOsmXApiRequest();
		//Desctructor
		~CsmOsmXApiRequest();

		const char* GetBaseUrl() const {return "http://jxapi.openstreetmap.org/xapi/api/0.6/"; }
		void RequestArea(float leftLon, float bottomLat, float rightLon, float topLat);
		void Perform();

		int GetNumNodes() const { return (int)nodes.size(); }
		int GetNumWays() const { return (int)ways.size(); }

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		static CsmOsmXApiRequest* Create();
		static void Destroy(CsmOsmXApiRequest*r);
	};
}