---

# Table Library on C++

This repository contains a C++ library for managing a searchable table based on linked lists. The table consists of keys and corresponding information with multiple versions.

## Task Description

The task involves implementing a table structure where each key can have multiple versions of associated information. The library provides functions for inserting, searching, and deleting elements based on keys and versions.

## Features

- **Insertion:** Insert elements into the table with keys and associated information. If a key already exists, a new version of the information is added.
- **Search by Key:** Search for all versions of information associated with a given key.
- **Search by Key and Version:** Search for a specific version of information associated with a given key.
- **Deletion:** Delete all versions of information associated with a given key, or a specific version of information associated with a key.

## Usage

To use this library, include the `table.h` header file in your C++ project. Make sure to compile the `table.cpp` file along with your project.

### Functions

- `Table* createTable()`: Creates a new table.
- `void insertItem(Table* table, KeyType key, InfoType info, int release)`: Inserts an item into the table with the specified key, information, and version.
- `Table* searchItemByKey(Table* table, KeyType key)`: Searches for all versions of information associated with the specified key.
- `InfoType searchItemByVersion(Table* table, KeyType key, int release)`: Searches for a specific version of information associated with the specified key.
- `void deleteItemByKey(Table* table, KeyType key)`: Deletes all versions of information associated with the specified key.
- `void deleteItemByVersion(Table* table, KeyType key, int release)`: Deletes a specific version of information associated with the specified key.

## Example

The `main.cpp` file provides an example of how to use the table library functions. It performs the following tests:

1. **Table Creation:** Creates a new table.
2. **Insertion:** Inserts elements into the table with different keys and versions.
3. **Display Contents:** Prints the contents of the table before and after insertion.
4. **Search by Key:** Searches for all versions of information associated with a specified key.
5. **Search by Key and Version:** Searches for a specific version of information associated with a specified key.
6. **Deletion:** Deletes all versions of information associated with a specified key.
7. **Memory Management:** Ensures proper memory deallocation by freeing the table after use.

The example demonstrates the basic usage of the table library functions to manage and manipulate data within the table structure.
