# Inventory Management System

This project is an Inventory Management System implemented in C++ with MySQL integration for database operations.

## Overview

The Inventory Management System allows users to perform basic CRUD (Create, Read, Update, Delete) operations on inventory items stored in a MySQL database. It provides functionalities to add new items, retrieve item details by ID, update existing items, remove items from inventory, view all items, and generate a comprehensive report summarizing inventory statistics.

## Features

- **Add Item**: Add a new item to the inventory with details such as item name, unit price, and units in stock.
- **Get Item**: Retrieve item details by specifying the item ID.
- **Update Item**: Update existing item details including item name, unit price, and units in stock.
- **Remove Item**: Delete an item from the inventory based on its ID.
- **View Inventory**: Display all items currently stored in the inventory.
- **Generate Report**: Generate a detailed report summarizing total items, total stock, prices, and details of the most/least expensive and most recent/oldest items.

## Requirements

- C++ compiler with C++11 support.
- MySQL database server (tested with MySQL Community Server 10.4.27-MariaDB).
- MySQL Connector/C++ for database connectivity.
- MySQL Connector/C++ headers and libraries.
- Basic knowledge of C++ programming and SQL.

## Installation and Setup

1. **MySQL Setup**: Ensure MySQL server is installed and running. Create a database named `InventoryManagementSystem`.
   
2. **Connector/C++ Setup**: Install MySQL Connector/C++ library.

3. **Project Configuration**: Update database connection details (`DBConnection.h`) with your MySQL server IP, username, and password.

4. **Build**: Compile the project using a C++ compiler that supports C++11.

## Usage

1. Compile the source files (`Inventory Management System.cpp`, `DBOperations.h`, `DBConnection.h`) into an executable.
   
2. Run the executable to start the Inventory Management System.

3. Use the console menu to perform various operations like adding, retrieving, updating, and deleting items.
