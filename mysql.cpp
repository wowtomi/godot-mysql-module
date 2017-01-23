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

void MySQL::add(int value) {

    count+=value;
}

void MySQL::reset() {

    count=0;
}

void MySQL::mysql_connect(String hostname, String username, String password) {
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        
		sql::SQLString h = hostname.utf8().get_data();
        sql::SQLString u = username.utf8().get_data();
        sql::SQLString p = password.utf8().get_data();
        
        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect(h, u, p);
        /* Connect to the MySQL test database */
       
        con->setSchema("emails");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
        while (res->next()) {
			print_line("MySQL reply: ");
			//print_line(String(res->getString(1)));
            cout << "\t... MySQL replies: ";
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
        print_line("### EXCEPTION Caught");
        Variant file = __FILE__;
        //Varient func = __FUNCTION__;
        Variant line = __LINE__;
		print_line("# ERR: SQLException in: "+String(file)+" on line "+String(line));
		print_line("# ERR: "+String(e.what()));
        Variant errCode = e.getErrorCode();
		print_line(" (MySQL error code: "+String(errCode)+")");
		print_line("SQLState: N/A"); // e.getSQLState()
    }
}
int MySQL::get_total() const {

    return count;
}
void MySQL::_bind_methods() {

    ObjectTypeDB::bind_method("add",&MySQL::add);
    ObjectTypeDB::bind_method("reset",&MySQL::reset);
    ObjectTypeDB::bind_method("mysql_connect",&MySQL::mysql_connect);
    ObjectTypeDB::bind_method("get_total",&MySQL::get_total);
}

MySQL::MySQL() {
    count=0;

    status=false;
    data='*';
}

