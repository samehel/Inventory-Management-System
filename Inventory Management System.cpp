#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <ctime>
#include <sstream> 
#include <iomanip>

#include "DBOperations.h"

using namespace std;


DBOperations dbOps;

enum GET_OPTIONS {
    DISPLAY_DATA,
    RETRIEVE_DATA
};

tm ParseDateTime(const std::string& datetimeStr) {
    std::tm t = {};
    std::istringstream ss(datetimeStr);
    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    return t;
}

void AddItem() {
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
DBOperations::InventoryItem GetItem(GET_OPTIONS getOptions) {
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

    if (item.itemID == 0) {
        cout << "Item was not found" << endl;
        return item;
    } 

    if (getOptions == DISPLAY_DATA) {
        cout << endl;
        cout << "Item ID: " << item.itemID << endl;
        cout << "Item Name: " << item.itemName << endl;
        cout << "Unit Price: " << item.unitPrice << endl;
        cout << "Units in Stock: " << item.unitsInStock << endl;
        cout << "Last Updated: " << item.lastUpdated << endl;
    }
    else if (getOptions == RETRIEVE_DATA) {
        return item;
    }
}
void UpdateItem() {
    string itemName = "";
    double unitPrice = 0.0;
    int unitsInStock = 0;

    DBOperations::InventoryItem oldItem = GetItem(RETRIEVE_DATA);

    if (oldItem.itemID == 0)
        return;

    cout << "Enter the following details: " << endl;
    cout << "Note: Keep field value empty if you do not want to update it" << endl;
    cout << "Item Name: ";
    getline(cin, itemName);
    cin.ignore();

    string tempInput;

    while (true) {
        cout << "Unit Price: ";
        getline(cin, tempInput);
        if (tempInput.empty()) 
            break;
        else {
            try {
                unitPrice = stod(tempInput);
                break;
            }
            catch (invalid_argument& e) {
                cout << "Invalid input. Please enter a number." << endl;
            }
        }
    }

    while (true) {
        cout << "Units in Stock: ";
        getline(cin, tempInput);
        if (tempInput.empty())
            break;
        else {
            try {
                unitsInStock = stoi(tempInput);
                break;
            }
            catch (invalid_argument& e) {
                cout << "Invalid input. Please enter a number." << endl;
            }
        }
    }

    dbOps.UpdateItem(oldItem.itemID, itemName == "" ? oldItem.itemName : itemName, unitPrice == 0.0 ? oldItem.unitPrice : unitPrice, unitsInStock == 0 ? oldItem.unitsInStock : unitsInStock);
}
void RemoveItem() {

    DBOperations::InventoryItem oldItem = GetItem(RETRIEVE_DATA);

    if (oldItem.itemID == 0)
        return;

    dbOps.DeleteItem(oldItem.itemID);
}
void GetAllItems() {
    vector<DBOperations::InventoryItem> items = dbOps.retrieveAllItems();

    if (items.size() == 0) {
        cout << "The Inventory is empty" << endl;
        return;
    }

    for(DBOperations::InventoryItem item : items)
    {
        cout << endl;
        cout << "---------------" << endl;
        cout << "Item ID: " << item.itemID << endl;
        cout << "Item Name: " << item.itemName << endl;
        cout << "Unit Price: " << item.unitPrice << endl;
        cout << "Units in Stock: " << item.unitsInStock << endl;
        cout << "Last Updated: " << item.lastUpdated << endl;
        cout << "---------------" << endl;
    }

}
void GenerateReport() {
    vector<DBOperations::InventoryItem> items = dbOps.retrieveAllItems();

    if (items.empty()) {
        cout << "Failed to Generate Report: The Inventory is empty" << endl;
        return;
    }

    int totalItems = items.size();
    int totalStock = 0;
    double totalPrice = 0.0;
    DBOperations::InventoryItem mostRecentItem = {};
    DBOperations::InventoryItem oldestItem = {};

    for (DBOperations::InventoryItem item : items) {
        totalStock += item.unitsInStock;
        totalPrice += item.unitPrice;
    }

    auto leastExpensiveItem = min_element(items.begin(), items.end(), [](const DBOperations::InventoryItem& lhs, const DBOperations::InventoryItem& rhs) {
        return lhs.unitPrice < rhs.unitPrice;
    });

    auto mostExpensiveItem = max_element(items.begin(), items.end(), [](const DBOperations::InventoryItem& lhs, const DBOperations::InventoryItem& rhs) {
        return lhs.unitPrice < rhs.unitPrice;
    });

    int mostRecentIndex = 0;
    int oldestIndex = 0;
    tm mostRecentTime = ParseDateTime(items[0].lastUpdated);
    tm oldestTime = ParseDateTime(items[0].lastUpdated);

    for (int i = 0; i < totalItems; i++) {

        tm currentDateTime = ParseDateTime(items[i].lastUpdated);
        if (difftime(mktime(&currentDateTime), mktime(&mostRecentTime)) > 0) {
            mostRecentTime = currentDateTime;
            mostRecentIndex = i;
        }

        if (difftime(mktime(&currentDateTime), mktime(&oldestTime)) < 0) {
            oldestTime = currentDateTime;
            oldestIndex = i;
        }
    }

    mostRecentItem = items[mostRecentIndex];
    oldestItem = items[oldestIndex];

    cout << endl;
    cout << "------------------------------------" << endl;
    cout << "          Generated Report          " << endl;
    cout << "------------------------------------" << endl;
    cout << "Total Items: " << totalItems << endl;
    cout << "Total Stock: " << totalStock << endl;
    cout << "Total Price: " << totalPrice << endl;
    cout << "Most Expensive Item: \n";
    cout << "\tItem ID: " << mostExpensiveItem->itemID << endl;
    cout << "\tItem Name: " << mostExpensiveItem->itemName << endl;
    cout << "\tUnits in Stock: " << mostExpensiveItem->unitsInStock << endl;
    cout << "\tUnit Price: " << mostExpensiveItem->unitPrice << endl;
    cout << "\tLast Updated: " << mostExpensiveItem->lastUpdated << endl;
    cout << "Least Expensive Item: \n";
    cout << "\tItem ID: " << leastExpensiveItem->itemID << endl;
    cout << "\tItem Name: " << leastExpensiveItem->itemName << endl;
    cout << "\tUnits in Stock: " << leastExpensiveItem->unitsInStock << endl;
    cout << "\tUnit Price: " << leastExpensiveItem->unitPrice << endl;
    cout << "\tLast Updated: " << leastExpensiveItem->lastUpdated << endl;
    cout << "Most Recent Item: \n";
    cout << "\tItem ID: " << mostRecentItem.itemID << endl;
    cout << "\tItem Name: " << mostRecentItem.itemName << endl;
    cout << "\tUnits in Stock: " << mostRecentItem.unitsInStock << endl;
    cout << "\tUnit Price: " << mostRecentItem.unitPrice << endl;
    cout << "\tLast Updated: " << mostRecentItem.lastUpdated << endl;
    cout << "Oldest Item: \n";
    cout << "\tItem ID: " << oldestItem.itemID << endl;
    cout << "\tItem Name: " << oldestItem.itemName << endl;
    cout << "\tUnits in Stock: " << oldestItem.unitsInStock << endl;
    cout << "\tUnit Price: " << oldestItem.unitPrice << endl;
    cout << "\tLast Updated: " << oldestItem.lastUpdated << endl;
    cout << "------------------------------------" << endl;
}

void main()
{
    int choice;

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
            AddItem();
            break;
        case 2:
            GetItem(DISPLAY_DATA);
            break;
        case 3:
            UpdateItem();
            break;
        case 4:
            RemoveItem();
            break;
        case 5:
            GetAllItems();
            break;
        case 6:
            GenerateReport();
            break;
        case 7:
            dbOps.Exit();
            break;
        }
    }
}