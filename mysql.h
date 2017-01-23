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
    bool status;
    char data;
    char resultset;
    const char* database;

protected:
    static void _bind_methods();

public:
    void add(int value);
    void reset();
    void mysql_connect(String hostname, String username, String password);
    int get_total() const;

    MySQL();
};

#endif
