#ifndef DBOPERATIONS_H
#define DBOPERATIONS_H

#include <iostream>
#include <mysql/jdbc.h>
#include <chrono>
#include <vector>

#include "DBConnection.h"

using namespace sql;
using namespace std;

class DBOperations {
private:

	DBConnection dbConnection;

	struct InventoryItem {
		int itemID;
		string itemName;
		int unitsInStock;
		double unitPrice;
		string lastUpdated;
	};

	void initDB() {
		if (dbConnection.ConnectionStatus()) {
			Statement* stmt = dbConnection.getConn()->createStatement();

			stmt->execute("CREATE DATABASE IF NOT EXISTS InventoryManagementSystem");
			stmt->execute("USE InventoryManagementSystem");

			stmt->execute("CREATE TABLE IF NOT EXISTS CREATE TABLE Inventory ("
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

	DBOperations() {
		initDB();
	}

	void CreateItem(string& itemName, int unitPrice, int unitsInStock) {
		PreparedStatement* pStmt = dbConnection.getConn()->prepareStatement("INSERT INTO Inventory(ItemName, UnitsInStock, UnitPrice) VALUES (?, ?, ?)");

		pStmt->setString(1, itemName);
		pStmt->setInt(2, unitPrice);
		pStmt->setInt(3, unitsInStock);

		pStmt->executeUpdate();

		delete pStmt;
	}

	vector<InventoryItem> retrieveAllItems() {
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


};

#endif // !DBOPERATIONS_H
