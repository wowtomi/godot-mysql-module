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

//sql::Driver *driver;
//sql::Connection *con;
//sql::Statement *stmt;
//sql::ResultSet *res;

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

Variant MySQL::query(String q, Variant column)
{
    sql::SQLString SQLquery = q.utf8().get_data();

    String r= "";
    PoolStringArray arr = {};

    try {
        sql::Driver *d = get_driver_instance();
        sql::Connection *conn = d->connect(host, user, pass);

        if(database != "")
        {
            conn->setSchema(database);
        }

        sql::Statement *stmt = conn->createStatement();
        sql::ResultSet *res = stmt->executeQuery(SQLquery);

        while (res->next())
          {
            int type = column.get_type();

            if (type == Variant::INT) // TYPE INT
            {
                //print_line("# column type INT");
                int columnIndex = Variant(column);
                r=sql2String(res->getString(columnIndex));
                arr.append(r);
            }
            if (type == Variant::STRING) // TYPE STRING
            {
                print_line("## this isn't working, try array or int");
                sql::SQLString columnName = String(column).utf8().get_data();
                r=sql2String(res->getString(columnName));
                arr.append(r);
            }
            if (type == Variant::ARRAY) // TYPE ARRAY
            {
              //print_line("# column type ARRAY");
              Array array = Array(column);
                for (int x=0;x<array.size();x++)
                {
                  sql::SQLString columnName = String(array[x]).utf8().get_data();
                  r=sql2String(res->getString(columnName));
                  arr.append(r);
                }
            }

        	/*bool is_num = has_only_digits(column);
        	if (is_num)
        	{
        		/* Access column data by numeric offset, 1 is the first column */
        	/* int columnIndex = Variant(column);
        		r=sql2String(res->getString(columnIndex));
            arr.append(r);
        	}
        	else
        	{
            /* Access column data by alias or column name */
            /*
        		sql::SQLString columnName = column.utf8().get_data();
           		r=sql2String(res->getString(columnName));
              arr.append(r);
           	}
            */

        }
        delete res;
        delete stmt;
        delete conn;

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

    if (arr.size() == 1)
    {
      return arr[0];
    }


    return arr;
}

void MySQL::execute(String s)
{
    sql::SQLString sql = s.utf8().get_data();

    try {
        sql::Driver *driver = get_driver_instance();
        sql::Connection *con = driver->connect(host, user, pass);

        if(database != "")
        {
            con->setSchema(database);
        }

        sql::Statement *stmt = con->createStatement();
        //stmt->execute("USE " +database)
        stmt->execute(sql);
        print_line("executed: "+s);
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

    	ClassDB::bind_method(D_METHOD("credentials","hostname","username","password"),&MySQL::credentials);
    	ClassDB::bind_method(D_METHOD("query","sql_query","colum_id='1'"),&MySQL::query);
    	ClassDB::bind_method(D_METHOD("execute","sql"),&MySQL::execute);
    	ClassDB::bind_method(D_METHOD("select_database","database"),&MySQL::select_database);
}

MySQL::MySQL() {
}
