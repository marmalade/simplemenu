#pragma once

#include <pugixml.hpp>
#include <smCurlRequest.h>
#include <smManagedList.h>

namespace SimpleMenu
{
	class CsmOsmTag: public CIwManaged
	{
		std::string key;
		std::string value;
	public:
		CsmOsmTag(){};
		CsmOsmTag(const char*k, const char*v):key(k),value(v){};
		bool Is(const char*k) const {return key == k;}
		const char* GetValue() const {return value.c_str();}

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
	};
	class CsmOsmNode: public CIwManaged
	{
	protected:
		float m_lon;
		float m_lat;
		TsmManagedList<CsmOsmTag> tags;
	public:
		CsmOsmNode(){}
		CsmOsmNode(float lon,float lat):m_lon(lon),m_lat(lat){}
		~CsmOsmNode(){ tags.Delete(); }
		int id;
		int version;
		int uid;

		float GetLongitude() const { return m_lon; }
		float GetLatitude() const { return m_lat; }
		const char* GetTag(const char*k) const { for (CsmOsmTag**t=tags.begin(); t!=tags.end();++t) if ((*t)->Is(k)) return (*t)->GetValue(); return ""; }

		void AddTag(const char*k, const char*v) { tags.push_back(new CsmOsmTag(k,v)); }
		void SetLongitude(float lon) { m_lon = lon; }
		void SetLatitude(float lat) { m_lat = lat; }

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
	};
	class CsmOsmWay
	{
		TsmManagedList<CsmOsmTag> tags;
	public:
		~CsmOsmWay(){ tags.Delete(); }

		void AddTag(const char*k, const char*v) { tags.push_back(new CsmOsmTag(k,v)); }
		const char* GetTag(const char*k) const { for (CsmOsmTag**t=tags.begin(); t!=tags.end();++t) if ((*t)->Is(k)) return (*t)->GetValue(); return ""; }

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };
	};
	
	class CsmOsmXApiRequest
	{
	protected:
		CsmCurlMemoryRequest request;
		TsmManagedList<CsmOsmNode> nodes;
		TsmManagedList<CsmOsmWay> ways;
	public:
		//Constructor
		CsmOsmXApiRequest();
		//Desctructor
		~CsmOsmXApiRequest();

		void Clear();

		const char* GetBaseUrl() const {return "http://jxapi.openstreetmap.org/xapi/api/0.6/"; }
		void Request(const char* query);
		void RequestNodesInArea(float leftLon, float bottomLat, float rightLon, float topLat);
		void RequestBuildingsInArea(float leftLon, float bottomLat, float rightLon, float topLat);
		void RequestArea(float leftLon, float bottomLat, float rightLon, float topLat);
		void RequestAmenitiesInArea(const char*a, float leftLon, float bottomLat, float rightLon, float topLat);
		void Perform();

		int GetNumNodes() const { return (int)nodes.size(); }
		int GetNumWays() const { return (int)ways.size(); }
		CsmOsmNode* GetNodeAt(int i) const { return nodes[i]; }
		CsmOsmWay* GetWayAt(int i) const { return ways[i]; }

		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();
		//Get scriptable class declaration
		virtual CsmScriptableClassDeclaration* GetInstanceClassDescription() {return GetClassDescription(); };

		static CsmOsmXApiRequest* Create();
		static void Destroy(CsmOsmXApiRequest*r);
	};
}