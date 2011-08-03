#include <IwDebug.h>
#include <smFeatures.h>
#include <smAllFeatures.h>
#include <smConfig.h>

namespace SimpleMenu
{
	int initFeaturesCounter = 0;

	TsmIntrusiveList<CsmFeature> * sm_featuresList = 0;

	TsmIntrusiveList<CsmFeature> * smGetActiveFeatures()
	{
		if (!sm_featuresList)
			sm_featuresList = new TsmIntrusiveList<CsmFeature>();
		return sm_featuresList;
	}

}

using namespace SimpleMenu;

void SimpleMenu::smFeaturesInit()
{
	++initFeaturesCounter;
	if (initFeaturesCounter != 1)
		return;

	smInit();
	smConfigInit();
	smRegisterClass(CsmLocation::GetClassDescription());
}

void SimpleMenu::smFeaturesTerminate()
{
	--initFeaturesCounter;
	if (initFeaturesCounter < 0)
		IwAssertMsg(SIMPLEMENU,false,("smFeaturesTerminate doesn't match smFeaturesInit"));
	if (initFeaturesCounter != 0)
		return;

	if (sm_featuresList)
	{
		while (sm_featuresList->GetFirstChild())
			delete sm_featuresList->GetFirstChild();
		delete sm_featuresList;
		sm_featuresList = 0;
	}
	smConfigTerminate();
	smTerminate();
}

CsmFeature::CsmFeature():m_started(false),m_lasttime(time(0))
{
	AttachTail(smGetActiveFeatures());
}
CsmFeature::~CsmFeature()
{
	Stop();
}

void CsmFeature::Start()
{
	m_lasttime = time(0);
	if (m_started)
		return;
	m_started = true;
	StartFeature();
}
void CsmFeature::Stop()
{
	if (!m_started)
		return;
	m_started = false;
	StopFeature();
}
