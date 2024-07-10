#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/statement.h> 
#include <iostream>
#include <mysql_connection.h> 
#include <mysql_driver.h> 
#include <string>

using namespace sql;
using namespace sql::mysql;
using namespace std;

class DBConnection {
private:
    MySQL_Driver* driver;
    Connection* conn;

public:
    DBConnection() : driver(nullptr), conn(nullptr) {
        try {
            driver = get_mysql_driver_instance();
            conn = driver->connect("tcp://127.0.0.1:3306", "root", "root");
            if (conn) {
                cout << "Successfully Connected to MySQL Database!" << endl;
            }
            else {
                cerr << "Failed to Connect to MySQL Database." << endl;
            }
        }
        catch (SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }

    DBConnection(string IP, string username, string password) : driver(nullptr), conn(nullptr) {
        try {
            driver = get_mysql_driver_instance();
            conn = driver->connect("tcp://" + IP + ":3306", username, password);
            if (conn) {
                cout << "Successfully Connected to MySQL Database!" << endl;
            }
            else {
                cerr << "Failed to Connect to MySQL Database." << endl;
            }
        }
        catch (SQLException& e) {
            cerr << "SQLException: " << e.what() << endl;
            cerr << "MySQL error code: " << e.getErrorCode() << endl;
            cerr << "SQLState: " << e.getSQLState() << endl;
        }
    }

    void CloseConnection() {
        delete conn;
    }

    bool ConnectionStatus() {
        return (conn != nullptr);
    }

    Connection* getConn() {
        return conn;
    }
};

#endif // !DBCONNECTION_H
