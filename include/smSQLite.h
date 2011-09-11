#pragma once

#include <sqlite3.h>
#include <IwResManager.h>
#include <smScriptProvider.h>

namespace SimpleMenu
{
	class CsmSQLite
	{
		sqlite3* connection;
		std::string fileName;
	public:
		CsmSQLite();
		~CsmSQLite();

		void Open();
		void Close();

	};

	void smSQLiteInit();
	void smSQLiteTerminate();
}