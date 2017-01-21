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
    char exception, data;
    char resultset;

protected:
    static void _bind_methods();

public:
    void add(int value);
    void reset();
    void connect_db(wchar_t hostname);
    //void close_connection(sql::Connection *con);
    int get_total() const;

    bool get_status() const;
    char get_data() const;
    char get_exception() const;
    //char get_result() const;

    MySQL();
};

#endif
