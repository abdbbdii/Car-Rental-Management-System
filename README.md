# Rental Car Management System

## Introduction
This C++ program is a Rental Car Management System that allows users to manage user accounts, car inventory, and transactions. It provides functionality for both customers and administrators.

## Code Overview
The code is organized into several sections, including structures, global variables, function prototypes, and the main function. Key features include user authentication, car rental transactions, and administrative tools.

### Structures
- `sCar`: Represents information about a car, including model, number plate, color, condition, status, price, repairs, and times rented.
- `sUser`: Represents information about a user, including ID, password, full name, email address, card details, account type, and current rented car.
- `sTransaction`: Represents information about a transaction, including ID, customer ID, car number plate, start and end times, and amount.
- `sTime`: Represents time with year, month, day, hour, minute, and second.

### Global Variables
- `current_user`: Stores information about the currently logged-in user.
- `database_folder`: Specifies the folder where database files are stored.
- `user_accounts_path`, `customer_transactions_path`, `car_inventory_path`: Paths to user accounts, customer transactions, and car inventory files.
- `user_accounts_map`, `car_inventory_map`, `customer_transactions_map`: Maps to store data loaded from CSV files.

### Functions
- Utility functions for CSV parsing, time manipulation, and file operations.
- Functions to write data back to CSV files.
- Functions to map data from CSV files to maps.
- Validation functions for account details and regular expression pattern matching.
- Wizards for adding, deleting, and modifying user accounts, transactions, and cars.
- Specific functions for calculating total income, generating reports, and displaying data.
- Main function that acts as the program's entry point.

## How to Use
1. Run the program.
2. Sign up or log in as a customer or administrator.
3. Perform actions based on the user type (customer or admin).

## Notes
- The code uses escape characters for formatting console output.
- Make sure to save changes in Excel after editing records in the program.
