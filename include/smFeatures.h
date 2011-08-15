#pragma once
#include "simplemenu.h"
#include "smIntrusiveList.h"
#include "smScriptProvider.h"

namespace SimpleMenu
{
	void smFeaturesInit();
	void smFeaturesTerminate();

	class CsmFeature;

	TsmIntrusiveList<CsmFeature> * smGetActiveFeatures();

	class CsmFeature: public TsmIntrusiveListItem<CsmFeature>
	{
		bool m_started;
		time_t m_lasttime;
	public:
		CsmFeature();
		virtual ~CsmFeature();
		void Start();
		void Stop();

		virtual void Update();
	protected:
		virtual void StartFeature() {};
		virtual void StopFeature() {};
	};
}
