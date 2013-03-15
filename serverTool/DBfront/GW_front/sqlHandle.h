#ifndef _SQLHANDLE_H_
#define _SQLHANDLE_H_
#include <mysql.h>

#include "GWMutex.h"
#include "stForMysql.h"

class sqlHandle
{
public:
	sqlHandle(const db_config&     config);
	~sqlHandle();
	int sql_query(const char* stmt_str);

	const char* sql_error();

private:
	bool db_connect(const db_config&   config);
	bool db_disconnect();

private:
	GW_spin_lock   m_mtx;
	db_config      m_config;
	MYSQL*         m_db_ptr;
};

#endif //_SQLHANDLE_H_
