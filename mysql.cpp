/* Copyright 2008, 2010, Oracle and/or its affiliates. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 2 of the License.

There are special exceptions to the terms and conditions of the GPL
as it is applied to this software. View the full text of the
exception in file EXCEPTIONS-CONNECTOR-C++ in the directory of this
software distribution.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

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

void MySQL::connect_db() {
    
    //driver = sql::mysql::get_mysql_driver_instance();
    //con = driver->connect("tamas.schabi.org:3306", "root", "gtaIV");
    status = true;
    cout << "lol?";
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("localhost:3306", "root", "gtaIV");
        /* Connect to the MySQL test database */
       
        con->setSchema("emails");

        stmt = con->createStatement();
        res = stmt->executeQuery("SELECT 'Hello World!' AS _message");
        while (res->next()) {
           // string result* = res->getString(1);
            //resultset = result;
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
        exception = e.getErrorCode();
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}
/*void MySQL::close_connection(sql::Connection *con) {
    
    delete con;
} */
int MySQL::get_total() const {

    return count;
}

bool MySQL::get_status() {

    return status;
}
char MySQL::get_data() const {
    
    return data;
}

char MySQL::get_exception() const {
    
    return exception;
}
/*char MySQL::get_result() const {
    
    return *resultset;
} */
void MySQL::_bind_methods() {

    ObjectTypeDB::bind_method("add",&MySQL::add);
    ObjectTypeDB::bind_method("reset",&MySQL::reset);
    ObjectTypeDB::bind_method("connect_db",&MySQL::connect_db);
    //ObjectTypeDB::bind_method("fetch",&MySQL::fetch);
    //ObjectTypeDB::bind_method("close_connection",&MySQL::close_connection);
    ObjectTypeDB::bind_method("get_total",&MySQL::get_total);
    ObjectTypeDB::bind_method("get_status",&MySQL::get_status);
    ObjectTypeDB::bind_method("get_data",&MySQL::get_data);
    ObjectTypeDB::bind_method("get_exception",&MySQL::get_exception);
    //ObjectTypeDB::bind_method("get_result",&MySQL::get_result);
}

MySQL::MySQL() {
    count=0;

    status=false;
    data='*';
    exception='*';
}

