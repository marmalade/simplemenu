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
	fileName = "app.db";
}
CsmSQLite::~CsmSQLite()
{
	Close();
}
void CsmSQLite::Open() 
{
	if (SQLITE_OK != sqlite3_open(fileName.c_str(),&connection))
	{
		IwAssertMsg(SM, false, ("Can't open SQLite3 database file"));
	}
}
void CsmSQLite::Close()
{
	if (connection)
	{
		sqlite3_close(connection);
		connection = 0;
	}
}
