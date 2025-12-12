ELECTRO STORE – CONSOLE-BASED SHOPPING CART

This application is a C++ console-based project simulating a realistic electronic store experience. It involves user authentication, product management, a search feature, and automated receipts generation.

FEATURES:

User Authentication: A system of secure login and registration with data stored in a file, users.txt.
Global Search: This allows for case-insensitive searching to find products across all categories quickly.
Inventory Management: Real-time stock updates where stock is deducted upon purchase, restored upon checkout cancellation.
Shopping Cart: Add items, remove items, and view items with the ability to compute subtotals.
Automated Billing: Creates a receipt.txt with customer details and adds a 20% discount for large purchases.

Technical Stack:
Language: C++
Data Storage: Flat-file data storage; text files
Libraries: iostream, fstream, iomanip, string, cctype

FILE STRUCTURE
main.cpp: Contains the principal logic together with the menu system.
products.txt: Database of all items, including ID, Model, Price, and Stock.
users.txt: The registered usernames with their passwords.
receipt.txt: This file is generated automatically after a checkout.

FUTURE ENHANCEMENTS -
Introduction of an Admin Panel for console-based product management. 
Password masking on the login phase.
Addition of category filtering to search results. 
Passwords stored in users.txt should be encrypted. 
