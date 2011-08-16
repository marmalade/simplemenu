#include <sstream>
#include "smOsmXApiRequest.h"

using namespace SimpleMenu;

namespace SimpleMenu
{
	
}

CsmOsmXApiRequest::CsmOsmXApiRequest()
{
}
CsmOsmXApiRequest::~CsmOsmXApiRequest()
{
	Clear();
}
void CsmOsmXApiRequest::Clear()
{
	nodes.Delete();
	ways.Delete();
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOsmNode::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOsmNode> d (0, "CsmOsmNode",
		ScriptTraits::Method("GetLongitude", &CsmOsmNode::GetLongitude),
		ScriptTraits::Method("GetLatitude", &CsmOsmNode::GetLatitude),
		ScriptTraits::Method("GetTag", &CsmOsmNode::GetTag),
			0);
	return &d;
}
//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOsmWay::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOsmWay> d (0, "CsmOsmWay",
		ScriptTraits::Method("GetTag", &CsmOsmWay::GetTag),
			0);
	return &d;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOsmTag::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOsmTag> d (0, "CsmOsmTag",
		ScriptTraits::Method("Is", &CsmOsmTag::Is),
			0);
	return &d;
}

//Get scriptable class declaration
CsmScriptableClassDeclaration* CsmOsmXApiRequest::GetClassDescription()
{
	static  TsmScriptableClassDeclaration<CsmOsmXApiRequest> d (0, "CsmOsmXApiRequest",
			ScriptTraits::Method("RequestArea", &CsmOsmXApiRequest::RequestArea),
			ScriptTraits::Method("RequestBuildingsInArea", &CsmOsmXApiRequest::RequestBuildingsInArea),
			ScriptTraits::Method("RequestNodesInArea", &CsmOsmXApiRequest::RequestNodesInArea),
			ScriptTraits::Method("Create", &CsmOsmXApiRequest::Create),
			ScriptTraits::Method("Destroy", &CsmOsmXApiRequest::Destroy),
			ScriptTraits::Method("Perform", &CsmOsmXApiRequest::Perform),
			ScriptTraits::Method("GetNumNodes", &CsmOsmXApiRequest::GetNumNodes),
			ScriptTraits::Method("GetNodeAt", &CsmOsmXApiRequest::GetNodeAt),
			ScriptTraits::Method("GetNumWays", &CsmOsmXApiRequest::GetNumWays),
			ScriptTraits::Method("GetWayAt", &CsmOsmXApiRequest::GetWayAt),
			
			0);
	return &d;
}
CsmOsmXApiRequest* CsmOsmXApiRequest::Create(){return new CsmOsmXApiRequest(); };
void CsmOsmXApiRequest::Destroy(CsmOsmXApiRequest*r){delete r; };


void CsmOsmXApiRequest::Request(const char* query)
{
	std::stringstream s; s << GetBaseUrl();
	s << query;
	request.SetUrl(s.str().c_str());
}
void CsmOsmXApiRequest::RequestArea(float leftLon, float bottomLat, float rightLon, float topLat)
{
	std::stringstream s; s << GetBaseUrl();
	s << "*[bbox=";
	s << leftLon;
	s << ",";
	s << bottomLat;
	s << ",";
	s << rightLon;
	s << ",";
	s << topLat;
	s << "]";
	request.SetUrl(s.str().c_str());
}
void CsmOsmXApiRequest::RequestAmenitiesInArea(const char* a, float leftLon, float bottomLat, float rightLon, float topLat)
{
	std::stringstream s; s << GetBaseUrl();
	s << "node[amenity=";
	if (a)
		while (*a)
		{
			switch (*a)
			{
			case '|': s << "\\|"; break;
			case '[': s << "\\["; break;
			case ']': s << "\\]"; break;
			case '*': s << "\\*"; break;
			case '/': s << "\\/"; break;
			case '=': s << "\\="; break;
			case '(': s << "\\("; break;
			case ')': s << "\\)"; break;
			case '\\': s << "\\\\"; break;
			default: s << *a; break;
			}
			++a;
		}
	else
		s << "*";
	s << "][bbox=";
	s << leftLon;
	s << ",";
	s << bottomLat;
	s << ",";
	s << rightLon;
	s << ",";
	s << topLat;
	s << "]";
	request.SetUrl(s.str().c_str());
}
void CsmOsmXApiRequest::RequestNodesInArea(float leftLon, float bottomLat, float rightLon, float topLat)
{
	std::stringstream s; s << GetBaseUrl();
	s << "node[bbox=";
	s << leftLon;
	s << ",";
	s << bottomLat;
	s << ",";
	s << rightLon;
	s << ",";
	s << topLat;
	s << "]";
	request.SetUrl(s.str().c_str());
}
void CsmOsmXApiRequest::RequestBuildingsInArea(float leftLon, float bottomLat, float rightLon, float topLat)
{
	std::stringstream s; s << GetBaseUrl();
	s << "way[building=*][bbox=";
	s << leftLon;
	s << ",";
	s << bottomLat;
	s << ",";
	s << rightLon;
	s << ",";
	s << topLat;
	s << "]";
	request.SetUrl(s.str().c_str());
}
void CsmOsmXApiRequest::Perform()
{
	Clear();
	request.SetTimeout(120);
	request.Perform("Open Street Map", "Please wait...");

	if (request.GetResponseCode() != 200)
	{
		return;
	}

	pugi::xml_document doc;
	doc.load_buffer(request.GetInputBuffer(), request.GetInputBufferSize());
	pugi::xml_node osm = doc.child("osm");
	for (pugi::xml_node osm_child = osm.first_child(); osm_child; osm_child = osm_child.next_sibling())
	{
		if (!stricmp("node",osm_child.name()))
		{
			nodes.push_back(new CsmOsmNode());
			CsmOsmNode & n = *nodes.back();
			n.id = osm_child.attribute("id").as_int();
			n.version = osm_child.attribute("version").as_int();
			n.uid = osm_child.attribute("uid").as_int();
			n.SetLatitude(osm_child.attribute("lat").as_float());
			n.SetLongitude(osm_child.attribute("lon").as_float());

			for (pugi::xml_node tag = osm_child.first_child(); tag; tag = tag.next_sibling())
			{
				if (!stricmp("tag",tag.name()))
				{
					n.AddTag(tag.attribute("k").value(),tag.attribute("v").value());
				}
			}
		}
		else if (!stricmp("way",osm_child.name()))
		{
			ways.push_back(new CsmOsmWay());
			CsmOsmWay & w = *ways.back();

			for (pugi::xml_node tag = osm_child.first_child(); tag; tag = tag.next_sibling())
			{
				if (!stricmp("tag",tag.name()))
				{
					w.AddTag(tag.attribute("k").value(),tag.attribute("v").value());
				}
			}
		}
		else
		{
		}
	}

	//<?xml version='1.0' encoding='UTF-8'?>
	//<osm version="0.6" generator="Osmosis SNAPSHOT-r25956M" xapi:planetDate="2011-08-15T15:43:02Z" xmlns:xapi="http://jxapi.openstreetmap.org/">
	//<bound box="51.51250,-0.19158,51.51350,-0.19058" origin="Osmosis SNAPSHOT-r25956M"/>
	//<node id="102086" version="6" timestamp="2011-04-03T10:03:39Z" uid="40196" user="pfr" changeset="7752226" lat="51.5100306" lon="-0.1898893">
	//	<tag k="highway" v="crossing"/>
	//</node>
	//<way id="25800856" version="7" timestamp="2011-01-22T22:18:23Z" uid="106786" user="Levente B" changeset="7056051">
	//<nd ref="25421185"/>
	//<nd ref="536009941"/>
	//<nd ref="1116224990"/>
	//<nd ref="1116225198"/>
	//<tag k="highway" v="residential"/>
	//<tag k="name" v="Ilchester Gardens"/>
	//<tag k="oneway" v="yes"/>
	//</way>
	//<relation id="51781" version="43" timestamp="2011-07-16T23:22:12Z" uid="489885" user="theStork" changeset="8743808">
	//<member type="way" ref="112980340" role=""/>
	//<member type="way" ref="5406386" role=""/>
	//<member type="way" ref="4009024" role=""/>
	//<member type="way" ref="5079973" role=""/>
	//<member type="way" ref="4398656" role=""/>
	//<member type="way" ref="5038287" role=""/>
	//<member type="way" ref="5038290" role=""/>
	//<member type="way" ref="5038315" role=""/>
	//<member type="way" ref="33854643" role=""/>
	//<member type="way" ref="33854365" role=""/>
	//<member type="way" ref="38246567" role=""/>
	//<member type="way" ref="33854209" role=""/>
	//<member type="way" ref="5038619" role=""/>
	//<member type="way" ref="5079977" role=""/>
	//<member type="way" ref="5079978" role=""/>
	//<member type="way" ref="3040458" role=""/>
	//<member type="way" ref="3995501" role=""/>
	//<member type="way" ref="4365376" role=""/>
	//<member type="way" ref="5079980" role=""/>
	//<member type="way" ref="4365372" role=""/>
	//<member type="way" ref="4365384" role=""/>
	//<member type="way" ref="5038621" role=""/>
	//<member type="way" ref="4365344" role=""/>
	//<member type="way" ref="65489632" role=""/>
	//<member type="way" ref="5079983" role=""/>
	//<member type="way" ref="4365339" role=""/>
	//<member type="way" ref="4365335" role=""/>
	//<member type="way" ref="5079982" role=""/>
	//<member type="way" ref="4365334" role=""/>
	//<member type="way" ref="5038622" role=""/>
	//<member type="way" ref="5079985" role=""/>
	//<member type="way" ref="48456958" role=""/>
	//<member type="way" ref="5079986" role=""/>
	//<member type="way" ref="4003433" role=""/>
	//<member type="way" ref="21072502" role=""/>
	//<member type="way" ref="4364526" role=""/>
	//<member type="way" ref="35919155" role=""/>
	//<member type="way" ref="5038623" role=""/>
	//<member type="way" ref="38419319" role=""/>
	//<member type="way" ref="5006059" role=""/>
	//<member type="way" ref="49797449" role=""/>
	//<member type="way" ref="4992891" role=""/>
	//<member type="way" ref="49797451" role=""/>
	//<member type="way" ref="4992893" role=""/>
	//<member type="way" ref="4992942" role=""/>
	//<member type="way" ref="4992943" role=""/>
	//<member type="way" ref="4992944" role=""/>
	//<member type="way" ref="4992947" role=""/>
	//<member type="way" ref="5003469" role=""/>
	//<member type="way" ref="92431424" role=""/>
	//<member type="way" ref="5022268" role=""/>
	//<member type="way" ref="5003470" role=""/>
	//<member type="way" ref="41660590" role=""/>
	//<member type="way" ref="5003488" role=""/>
	//<member type="way" ref="5120011" role=""/>
	//<member type="way" ref="9778591" role=""/>
	//<member type="way" ref="121955696" role=""/>
	//<member type="way" ref="5082631" role=""/>
	//<member type="way" ref="5082613" role=""/>
	//<member type="way" ref="5182963" role=""/>
	//<member type="way" ref="121962435" role=""/>
	//<member type="way" ref="8161351" role=""/>
	//<member type="way" ref="35543801" role=""/>
	//<member type="way" ref="5182964" role=""/>
	//<member type="way" ref="5182973" role=""/>
	//<member type="way" ref="5406355" role=""/>
	//<member type="way" ref="5406362" role=""/>
	//<member type="way" ref="3986344" role=""/>
	//<member type="way" ref="71797002" role=""/>
	//<member type="way" ref="48647854" role=""/>
	//<tag k="admin_level" v="8"/>
	//<tag k="ons_code" v="00BK"/>
	//<tag k="name" v="City of Westminster"/>
	//<tag k="wikipedia" v="en:City of Westminster"/>
	//<tag k="source:ons_code" v="OS_OpenData_CodePoint Codelist.txt"/>
	//<tag k="type" v="boundary"/>
	//<tag k="boundary" v="administrative"/>
	//</relation>
	//</osm>
}
