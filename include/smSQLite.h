#pragma once

#include <sqlite3.h>
#include <IwResManager.h>
#include <smScriptProvider.h>

namespace SimpleMenu
{
	class CsmSQLite
	{
		sqlite3* connection;
		void* compiledStatement;
		std::string fileName;
	public:
		CsmSQLite();
		~CsmSQLite();

		void Open();
		void Close();

		int ExecuteScalar(const char* query);
		void ExecuteNonQuery(const char* sqlStatement);

		bool Prepare(const char* sqlStatement);
		bool Step();
		void Finalize();

		int GetColumnInt(int index);
		const char* GetColumnString(int index);
	};

	void smSQLiteInit();
	void smSQLiteTerminate();
}