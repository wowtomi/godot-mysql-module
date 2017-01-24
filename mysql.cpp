/*
mysql.cpp
*/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "mysql.h"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

sql::Driver *driver;
sql::Connection *con;
sql::Statement *stmt;
sql::ResultSet *res;

sql::SQLString host;
sql::SQLString user;
sql::SQLString pass;
sql::SQLString database;

void MySQL::credentials(String shost, String suser, String spass)
{
    host=shost.utf8().get_data();
    user=suser.utf8().get_data();
    pass=spass.utf8().get_data();
}
void MySQL::select_database(String db)
{
	database=db.utf8().get_data();
}
Variant MySQL::query(String q, String columnId)
{
    sql::SQLString SQLquery = q.utf8().get_data();
    
    Variant r = Variant();
    
    try {
        driver = get_driver_instance();
        con = driver->connect(host, user, pass);
        
        if(database != "")
        {
            con->setSchema(database);
        }
        
        stmt = con->createStatement();
        res = stmt->executeQuery(SQLquery);
       // 
        while (res->next()) 
        {
        	bool is_num = has_only_digits(columnId);
        	if (is_num)
        	{
        		/* Access column data by numeric offset, 1 is the first column */
        		int columnIndex = Variant(columnId);
        		r=sql2String(res->getString(columnIndex));
        	}
        	else
        	{
        		/* Access column data by alias or column name */
        		sql::SQLString columnName = columnId.utf8().get_data();
           		r=sql2String(res->getString(columnName));
           	}

        }
        delete res;
        delete stmt;
        delete con;
        
        }
        catch (sql::SQLException &e) {
            print_line("# EXCEPTION Caught ˇ");
            Variant file = __FILE__;
            Variant line = __LINE__;
            Variant func = __FUNCTION__;
		    print_line("# ERR: SQLException in: "+String(file)+" in function: "+String(func)+"() on line "+String(line));
		    print_line("# ERR: "+String(e.what()));
            Variant errCode = e.getErrorCode();
		    print_line(" (MySQL error code: "+String(errCode)+")");
		    print_line("SQLState: "+sql2String(e.getSQLState()));
    }

    return r;
}
void MySQL::execute(String s)
{
    sql::SQLString sql = s.utf8().get_data();
    
    try {
        driver = get_driver_instance();
        con = driver->connect(host, user, pass);
        
        if(database != "")
        {
            con->setSchema(database);
        }
        
        stmt = con->createStatement();
        stmt->execute(sql);
        print_line("executed: "+s);
        delete res;
        delete stmt;
        delete con;
        }
        catch (sql::SQLException &e) {
            print_line("# EXCEPTION Caught ˇ");
            Variant file = __FILE__;
            Variant line = __LINE__;
            Variant func = __FUNCTION__;
		    print_line("# ERR: SQLException in: "+String(file)+" in function: "+String(func)+"() on line "+String(line));
		    print_line("# ERR: "+String(e.what()));
            Variant errCode = e.getErrorCode();
		    print_line(" (MySQL error code: "+String(errCode)+")");
		    print_line("SQLState: "+sql2String(e.getSQLState()));
    }
}
String MySQL::sql2String(sql::SQLString s)
{
    const char * c = s.c_str();
    String str = String::utf8((char *)c);
    return str;
}
bool MySQL::has_only_digits(String s){
	string st = s.utf8().get_data();
	return (st.find_first_not_of( "0123456789" ) == string::npos);
}
void MySQL::_bind_methods() {

    ObjectTypeDB::bind_method(_MD("credentials","hostname","username","password"),&MySQL::credentials);
    ObjectTypeDB::bind_method(_MD("query","sql_query","colum_id='1'"),&MySQL::query);
    ObjectTypeDB::bind_method(_MD("execute","sql"),&MySQL::execute);
    ObjectTypeDB::bind_method(_MD("select_database","database"),&MySQL::select_database);
}

MySQL::MySQL() {
}

