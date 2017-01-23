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
sql::SQLString SQLquery;

void MySQL::add(int value) {

    count+=value;
}

void MySQL::reset() {

    count=0;
}

void MySQL::mysql_connect(String hostname, String username, String password, String database) {
    
    try {
        
		sql::SQLString h = hostname.utf8().get_data();
        sql::SQLString u = username.utf8().get_data();
        sql::SQLString p = password.utf8().get_data();
        sql::SQLString d = database.utf8().get_data();
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(h, u, p);
        /* Connect to the MySQL test database */
       
        con->setSchema(d);

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT name FROM role_permissions ORDER BY id");
        while (res->next()) {
            const char * c = res->getString(1).c_str();
            String result = String::utf8((char *)c);
			print_line("MySQL replies: "+result);
            /* Access column data by alias or column name */
            cout << res->getString("_message") << endl;
            cout << "\t... MySQL says it again: ";
            /* Access column data by numeric offset, 1 is the first column */
            cout << res->getString(1) << endl;
        }
        delete res;
        delete stmt;
        delete con;

    } catch (sql::SQLException &e) {
        print_line("# EXCEPTION Caught ˇ");
        Variant file = __FILE__;
        Variant line = __LINE__;
		print_line("# ERR: SQLException in: "+String(file)+" on line "+String(line));
		print_line("# ERR: "+String(e.what()));
        Variant errCode = e.getErrorCode();
		print_line(" (MySQL error code: "+String(errCode)+")");
        const char * c = e.getSQLState().c_str();
        String s = String::utf8((char *)c);
		print_line("SQLState: "+s);
    }
}
int MySQL::get_total() const 
{

    return count;
}
void MySQL::set_login(String shost, String suser, String spass, String sdatabase)
{
    host=shost.utf8().get_data();
    user=suser.utf8().get_data();
    pass=spass.utf8().get_data();
    database=sdatabase.utf8().get_data();
}
void MySQL::query(String q)
{
    SQLquery = q.utf8().get_data();
    
    try {
        driver = get_driver_instance();
        con = driver->connect(host, user, pass);
        
        con->setSchema(database);
        
        stmt = con->createStatement();
        res = stmt->executeQuery(SQLquery);
        while (res->next()) 
        {
            const char * c = res->getString(1).c_str();
            String result = String::utf8((char *)c);
		    print_line("MySQL replies: "+result);
            /* Access column data by alias or column name */
            cout << res->getString("_message") << endl;
            cout << "\t... MySQL says it again: ";
            /* Access column data by numeric offset, 1 is the first column */
            cout << res->getString(1) << endl;
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
            const char * c = e.getSQLState().c_str();
            String s = String::utf8((char *)c);
		    print_line("SQLState: "+s);
    }
}
void MySQL::_bind_methods() {

    ObjectTypeDB::bind_method("add",&MySQL::add);
    ObjectTypeDB::bind_method("reset",&MySQL::reset);
    ObjectTypeDB::bind_method("mysql_connect",&MySQL::mysql_connect);
    ObjectTypeDB::bind_method("get_total",&MySQL::get_total);
    ObjectTypeDB::bind_method("set_login",&MySQL::set_login);
    ObjectTypeDB::bind_method("query",&MySQL::query);
}

MySQL::MySQL() {
    count=0;
}

