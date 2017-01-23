/* mysql.h */
#ifndef MYSQL_H
#define MYSQL_H

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "reference.h"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <mysql_error.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class MySQL : public Reference {
    OBJ_TYPE(MySQL,Reference);

    int count;

    sql::SQLString host;
    sql::SQLString user;
    sql::SQLString pass;
    sql::SQLString database;
    sql::SQLString SQLquery;

protected:
    static void _bind_methods();

public:
    void add(int value);
    void reset();
    void mysql_connect(String hostname, String username, String password, String database);
    
    void set_login(String shost, String suser, String spass, String sdatabase);
    void query(String q);    
    int get_total() const;

    MySQL();
};

#endif
