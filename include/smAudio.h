	class CsmAudio: public CsmFeature
	{
	public: 
		//Get scriptable class declaration
		static CsmScriptableClassDeclaration* GetClassDescription();

		static bool IsAvailable();
		static bool PlayFile(const char*, int repeatCount);
	};