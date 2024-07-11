#ifndef DBOPERATIONS_H
#define DBOPERATIONS_H

#include <cppconn/driver.h> 
#include <cppconn/exception.h> 
#include <cppconn/statement.h> 
#include <cppconn/prepared_statement.h> 
#include <iostream>
#include <mysql_connection.h> 
#include <mysql_driver.h> 
#include <chrono>
#include <vector>

#include "DBConnection.h"

using namespace sql;
using namespace std;

class DBOperations {
private:

	DBConnection dbConnection;
	PreparedStatement* pStmt;

	void initDB() {

		if (dbConnection.ConnectionStatus()) {
			Statement* stmt = dbConnection.getConn()->createStatement();

			stmt->execute("CREATE DATABASE IF NOT EXISTS InventoryManagementSystem");
			stmt->execute("USE InventoryManagementSystem");

			stmt->execute("CREATE TABLE IF NOT EXISTS Inventory ("
					      "ItemID INT PRIMARY KEY AUTO_INCREMENT,"
						  "ItemName VARCHAR(100) NOT NULL,"
						  "UnitsInStock INT NOT NULL,"
						  "UnitPrice DECIMAL(10, 2) NOT NULL,"
						  "LastUpdated TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP)");

		} else {
			cout << "Error: Database Connection Failed." << endl;
			exit(1);
		}
	}

public:

	struct InventoryItem {
		int itemID;
		string itemName;
		int unitsInStock;
		double unitPrice;
		string lastUpdated;
	};

	DBOperations() {
		initDB();
	}

	void CreateItem(string& itemName, double unitPrice, int unitsInStock) {
		try {
			pStmt = dbConnection.getConn()->prepareStatement("INSERT INTO Inventory(ItemName, UnitPrice, UnitsInStock) VALUES (?, ?, ?)");

			pStmt->setString(1, itemName);
			pStmt->setDouble(2, unitPrice);
			pStmt->setInt(3, unitsInStock);

			pStmt->executeUpdate();

			cout << "Item successfully added to inventory" << endl;

			delete pStmt;
		}
		catch (SQLException& e) {
			cerr << "An Error occured while attempting to add an item: " << e.what() << endl;
			throw;
		}
	}

	vector<InventoryItem> retrieveAllItems() {
		try {
			vector<InventoryItem> items;
			Statement* stmt = dbConnection.getConn()->createStatement();
			ResultSet* res = stmt->executeQuery("SELECT * FROM Inventory");

			while (res->next()) {
				InventoryItem item;
				item.itemID = res->getInt("ItemID");
				item.itemName = res->getString("ItemName");
				item.unitsInStock = res->getInt("UnitsInStock");
				item.unitPrice = res->getDouble("UnitPrice");
				item.lastUpdated = res->getString("LastUpdated");

				items.push_back(item);
			}

			delete res;
			delete stmt;

			return items;
		}
		catch (SQLException& e) {
			cerr << "An Error occured while attempting to view the inventory: " << e.what() << endl;
			throw;
		}
	}

	InventoryItem retrieveItemByID(int ID) {
		try {
			InventoryItem item;
			pStmt = dbConnection.getConn()->prepareStatement("SELECT * FROM Inventory WHERE ItemID = ?");
			pStmt->setInt(1, ID);

			ResultSet* res = pStmt->executeQuery();

			if (res->next()) {
				item.itemID = res->getInt("ItemID");
				item.itemName = res->getString("ItemName");
				item.unitsInStock = res->getInt("UnitsInStock");
				item.unitPrice = res->getDouble("UnitPrice");
				item.lastUpdated = res->getString("LastUpdated");
			}
			else {
				return InventoryItem {
					item.itemID = 0,
				};
			}

			delete res;
			delete pStmt;

			return item;
		}
		catch (SQLException& e) {
			cerr << "An Error occured while attempting to get an item: " << e.what() << endl;
			throw;
		}
	}

	void UpdateItem(int ID, string& itemName, double unitPrice, int unitsInStock) {
		try {
			pStmt = dbConnection.getConn()->prepareStatement("UPDATE Inventory SET ItemName = ?, UnitsInStock = ?, UnitPrice = ? WHERE ItemID = ?");

			pStmt->setString(1, itemName);
			pStmt->setInt(2, unitsInStock);
			pStmt->setDouble(3, unitPrice);
			pStmt->setInt(4, ID);

			pStmt->executeUpdate();

			cout << "Successfully updated item with ID: " << ID << endl;

			delete pStmt;
		}
		catch (SQLException& e) {
			cerr << "An Error occured while attempting to update an item: " << e.what() << endl;
			throw;
		}
	}

	void DeleteItem(int ID) {
		try {
			pStmt = dbConnection.getConn()->prepareStatement("DELETE FROM Inventory WHERE ItemID = ?");

			pStmt->setInt(1, ID);

			pStmt->executeUpdate();

			cout << "Successfully deleted item with ID: " << ID << endl;

			delete pStmt;
		}
		catch (SQLException& e) {
			cerr << "An Error occured while attempting to delete an item: " << e.what() << endl;
			throw;
		}
	}

	void Exit() {
		dbConnection.CloseConnection();
		exit(0);
	}
};

#endif // !DBOPERATIONS_H
