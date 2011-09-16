#include <smSQLite.h>

using namespace SimpleMenu;

namespace SimpleMenu
{
}

void SimpleMenu::smSQLiteInit()
{
}
void SimpleMenu::smSQLiteTerminate()
{
}


CsmSQLite::CsmSQLite()
{
	connection = 0;
	compiledStatement = 0;
	fileName = "app.db";
}
CsmSQLite::~CsmSQLite()
{
	Close();
}
void CsmSQLite::Open() 
{
	if (connection)
		return;
	if (SQLITE_OK != sqlite3_open(fileName.c_str(),&connection))
	{
		IwAssertMsg(SM, false, ("Can't open SQLite3 database file"));
	}
}
void CsmSQLite::Close()
{
	Finalize();
	if (connection)
	{
		sqlite3_close(connection);
		connection = 0;
	}
}
bool CsmSQLite::Prepare(const char* sqlStatement)
{
	Open();
	// Setup the SQL Statement and compile it for faster access
	return sqlite3_prepare_v2(connection, sqlStatement, -1, (sqlite3_stmt **)&compiledStatement, NULL) == SQLITE_OK;
}
void CsmSQLite::Finalize()
{
	if (compiledStatement)
	{
		// Release the compiled statement from memory
		sqlite3_finalize((sqlite3_stmt *)compiledStatement);
		compiledStatement = 0;
	}
}
bool CsmSQLite::Step()
{
	if (!compiledStatement) return false;
	return sqlite3_step((sqlite3_stmt *)compiledStatement) == SQLITE_ROW;
}
int CsmSQLite::ExecuteScalar (const char* sqlStatement)
{
	Open();
	if (compiledStatement) IwAssertMsg(SM,false,("Please finalize"));

	int res = 0;
	
	if(Prepare(sqlStatement)) {
		// Loop through the results and add them to the feeds array
		if(Step()) {
			// Read the data from the result row
			res = GetColumnInt(0);
		}
	}
	Finalize();
	return res;
}
int CsmSQLite::GetColumnInt(int index)
{
	if (!compiledStatement) return 0;
	return sqlite3_column_int((sqlite3_stmt *)compiledStatement,index);
}
const char* CsmSQLite::GetColumnString(int index)
{
	if (!compiledStatement) return 0;
	return (const char*)sqlite3_column_text((sqlite3_stmt *)compiledStatement,index);
}
void CsmSQLite::ExecuteNonQuery(const char* sqlStatement)
{
	Open();
	if (compiledStatement) IwAssertMsg(SM,false,("Please finalize"));

	int res = 0;
	
	if(Prepare(sqlStatement)) {
		while(Step()) {
		}
	}
	Finalize();
}
