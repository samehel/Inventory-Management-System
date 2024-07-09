#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <mysql/jdbc.h>
#include <iostream>
#include <string>

using namespace sql;
using namespace std;

class DBConnection {
private:
	Driver* driver;
	Connection* conn;

public:

	DBConnection() {
		driver = get_driver_instance();
		conn = driver->connect("tcp://127.0.0.1:3306", "root", "");
	}

	DBConnection(string IP, string username, string password) {
		driver = get_driver_instance();
		conn = driver->connect("tcp://" + IP, username, password);
	}

	bool ConnectionStatus() {
		if (!conn)
			return false && cout << "Failed to Connect to MySQL Database.";
		
		cout << "Successfully Connected to MySQL Database!" << endl;
		return true;
	}

	void CloseConnection() {
		delete conn;
	}

	Connection* getConn() {
		return conn;
	}
};

#endif // !DBCONNECTION_H
