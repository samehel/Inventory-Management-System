#include <iostream>
#include <string>
#include <limits>

#include "DBOperations.h"

using namespace std;

void AddItem(DBOperations dbOps) {
    string itemName;
    double unitPrice;
    int unitsInStock;

    cout << "Enter the following details: " << endl;
    cout << "Item Name: ";
    cin >> itemName;

    while (true) {
        cout << "Unit Price: ";
        cin >> unitPrice;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }

    while (true) {
        cout << "Units in Stock: ";
        cin >> unitsInStock;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break; 
        }
    }

    dbOps.CreateItem(itemName, unitPrice, unitsInStock);
}
void GetItem(DBOperations dbOps) {
    int itemID;

    while (true) {
        cout << "Item ID: ";
        cin >> itemID;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            break;
        }
    }

    DBOperations::InventoryItem item = dbOps.retrieveItemByID(itemID);

    cout << endl;
    cout << "Item ID: " << item.itemID << endl;
    cout << "Item Name: " << item.itemName << endl;
    cout << "Unit Price: " << item.unitPrice << endl;
    cout << "Units in Stock: " << item.unitsInStock << endl;
    cout << "Last Updated: " << item.lastUpdated << endl;
}


void main()
{
    int choice;
    DBOperations dbOps;

    while (true) {

        cout << endl;
        cout << "------------------------------------" << endl;
        cout << "     Inventory Mangement System     " << endl;
        cout << "------------------------------------" << endl;
        cout << "   1. Add Item to Inventory         " << endl;
        cout << "   2. Get Item from Inventory       " << endl;
        cout << "   3. Update Item in Inventory      " << endl;
        cout << "   4. Remove Item from Inventory    " << endl;
        cout << "   5. View Inventory                " << endl;
        cout << "   6. Generate Report               " << endl;
        cout << "   7. Exit                          " << endl;
        cout << "------------------------------------" << endl;

        cout << ">> ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            AddItem(dbOps);
            break;
        case 2:
            GetItem(dbOps);
            break;
        case 7:
            dbOps.Exit();
            break;
        }
    }


}