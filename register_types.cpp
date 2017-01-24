/* register_types.cpp */

#include "register_types.h"
#include "object_type_db.h"
#include "mysql.h"

#include "mysql_connection.h"

#include "cppconn/driver.h"
#include "cppconn/exception.h"
#include "cppconn/resultset.h"
#include "cppconn/statement.h"

void register_mysql_types() {

        ObjectTypeDB::register_type<MySQL>();
}

void unregister_mysql_types() {
   //nothing to do here
}

