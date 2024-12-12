 
# Database of phone numbers and locations

## Description
This task involves creating a set of functions (not a full program) to manage a database storing phone numbers and their associated locations. Phone numbers are assumed to be in international format, including country codes like 420224351111. The data is structured hierarchically using a tree structure, where each node represents a digit (0–9) in the phone number. A 10-ary tree is used to organize and store location information based on these digits, allowing for hierarchical location identification (e.g., country, region, operator).

## Functions to implement
- void delBook(phonebook)
  Removes all nodes in the tree and frees all allocated resources.

- bool addPhone(phonebook, phone, name)
  Adds a prefix phone and associates it with a location name. Returns true for success, false if the phone number is invalid (non-numeric).
If the prefix already exists, updates the location name and returns true.

- bool delPhone(phonebook, phone)
Removes the prefix phone and associated nodes from the tree. Returns true for success, false if the phone number is invalid or not found.

- const char* findPhone(phonebook, phone)
Searches for and returns the location (name) associated with the phone number. Returns nullptr if the location is unknown.

## Structures
These structures are declared in the testing environment. Your implementation will use them, but cannot modify them.

- TPHONEBOOK

Contains:
m_Root – a pointer to the root of the tree, where prefixes are stored. Upon initialization, it will be set to nullptr.

m_Size – indicates the number of stored locations, i.e., the number of nodes that have the m_Name set to a valid name.

- TNODE

Contains:
m_Name – a pointer to a string representing the location name or nullptr if the location is not specified.

m_Child – references to child nodes, or nullptr if the tree does not continue further for the given digit.

## Testing
The main.cpp file includes assertions that verify the behavior of functions and the tree structure after each operation. 