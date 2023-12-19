
//   /$$$$$$                            /$$$$$$$                        /$$               /$$        /$$$$$$                                 /$$
//  /$$__  $$                          | $$__  $$                      | $$              | $$       /$$__  $$                               |__/
// | $$  \__/  /$$$$$$   /$$$$$$       | $$  \ $$  /$$$$$$  /$$$$$$$  /$$$$$$    /$$$$$$ | $$      | $$  \__/  /$$$$$$   /$$$$$$  /$$    /$$ /$$  /$$$$$$$  /$$$$$$   /$$$$$$$
// | $$       |____  $$ /$$__  $$      | $$$$$$$/ /$$__  $$| $$__  $$|_  $$_/   |____  $$| $$      |  $$$$$$  /$$__  $$ /$$__  $$|  $$  /$$/| $$ /$$_____/ /$$__  $$ /$$_____/
// | $$        /$$$$$$$| $$  \__/      | $$__  $$| $$$$$$$$| $$  \ $$  | $$      /$$$$$$$| $$       \____  $$| $$$$$$$$| $$  \__/ \  $$/$$/ | $$| $$      | $$$$$$$$|  $$$$$$
// | $$    $$ /$$__  $$| $$            | $$  \ $$| $$_____/| $$  | $$  | $$ /$$ /$$__  $$| $$       /$$  \ $$| $$_____/| $$        \  $$$/  | $$| $$      | $$_____/ \____  $$
// |  $$$$$$/|  $$$$$$$| $$            | $$  | $$|  $$$$$$$| $$  | $$  |  $$$$/|  $$$$$$$| $$      |  $$$$$$/|  $$$$$$$| $$         \  $/   | $$|  $$$$$$$|  $$$$$$$ /$$$$$$$/
//  \______/  \_______/|__/            |__/  |__/ \_______/|__/  |__/   \___/   \_______/|__/       \______/  \_______/|__/          \_/    |__/ \_______/ \_______/|_______/

#include <conio.h>
#include <cstdlib>  // for exiting the program execution
#include <ctime>    // for obtaining current time
#include <fstream>  // for file I/O operations
#include <iomanip>  // for specifying the output width
#include <iostream> // for console I/O operations
#include <map>      // for key-value pairs
#include <regex>    // for regular expression matching
#include <sstream>  // for treating a string object like a stream
#include <string>   // for string operations
#include <vector>   // for making a dynamic list array (only for menu-making perposes and not for storing records)

// all escape characters can be found in https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797
#define CLEARLINE "\e[2K"      // erase
#define MOVEUP "\e[A"          // cursor control
#define BOLD "\e[1m"           // formatting
#define UNDERLINE "\e[4m"      // formatting
#define BULLETS "\e[46m\e[30m" // formatting
#define BULLETTEXT "\e[36m"    // color
#define INPUTFIELD "\e[37m"    // color
#define HEADING "\e[37m\e[4m"  // color
#define RED "\e[91m"           // color
#define GREEN "\e[92m"         // color
#define YELLOW "\e[93m"        // color
#define RESET "\e[0m"          // erase all formatting
using namespace std;

// --STRUCTURES--
struct sCar {
    string Model, NumberPlate, Color, Condition, Status;
    float Price;
    int Repairs, TimesRented;
};
struct sUser {
    string ID, Password, FullName, EmailAddress, CardNumber, CVV, ExpiryDate, AccountType, CurrentCar;
    int TimesRented;
};
struct sTransaction {
    int ID;
    string CustomerID, CarNumberPlate, StartTime, EndTime;
    float Amount;
};
struct sTime {
    int Year, Month, Day, Hour, Minute, Second;
};

// --GLOBALS--
sUser current_user;
string database_folder = "Database";
string user_accounts_path = database_folder + "/UserAccounts.csv";
string customer_transactions_path = database_folder + "/CustomerTransactions.csv";
string car_inventory_path = database_folder + "/CarInventory.csv";
map<string, sUser> user_accounts_map;
map<string, sCar> car_inventory_map;
map<int, sTransaction> customer_transactions_map;

// --FUNCTIONS PROTOTYPES--
// utility functions
void parseCSV(const string &filePath, string **&data, int &Nrows, int &Ncols); // converts and excel file into dynamic 2D array
sTime addTimes(const sTime &time1, const sTime &time2);                        // time summation
string timeToStr(const sTime &time);                                           // converts time structure to string object
sTime getCurrentTime();                                                        // gets current time
void removeLastLines(const int &n);                                            // removes last lines from the console
int createMenu(const vector<string> &arr);                                     // creates a user friendly menu from a list
void openFile(const string &filePath);                                         // opens a file in its Microsoft Excel
// write functions
void writeCSV_user_accounts();         // updates user accounts file
void writeCSV_customer_transactions(); // updates user transactions file
void writeCSV_car_inventory();         // updates car inventory file
// map functions
void map_user_accounts();         // defines key-value pairs from user dynamic array
void map_car_inventory();         // defines key-value pairs from car dynamic array
void map_customer_transactions(); // defines key-value pairs from transaction dynamic array
// validation functions
// bool validateAccount(const string &ID, string password = "");    // checks if the username matches with the accounts in file
bool rePatternMatch(const string &str, const string &rePattern); // matches a regular expression pattern to a string
// wizards
void wizard_add_user(const char &mode);        // adds a user to database
void wizard_add_transaction(const char &mode); // adds a transaction to database
void wizard_add_car();                         // adds a car to database
void wizard_delete_user();                     // deletes a user from database
void wizard_delete_transaction();              // deletes a transaction from database
void wizard_delete_car();                      // deletes a car from the database
void wizard_login();                           // gives access to the user to interact with the database
void wizard_change_car_status();               // changes car status
void wizard_change_account_settings();         // changes account settings of a user from the database
void wizard_return_car(const char &mode);      // cancels the car ownership of the user
// specific functions
float total_income(); // calculates the total income generated by the company
// reports
void maintenance_report(); // displays which car needs maintainence
void utilization_report(); // displays the usage of the car in its lifespan
// display functions
void view_car_inventory();         // displays all available cars
void view_customer_transactions(); // displays all transactions
void view_user_accounts();         // displays all user accounts

// function calling on user choices
int main() {
    map_user_accounts();
    map_car_inventory();
    map_customer_transactions();
startAgain:
    system("cls");
    switch (createMenu({
        "WELCOME TO RENTAL CAR SERVICES",
        "Signup",
        "Login",
        "Quit",
    })) {
    case 1:
        cout << "CUSTOMER SIGNUP PAGE\n"
             << endl;
        wizard_add_user('c');
        system("cls");
    case 2:
        cout << "LOGIN PAGE\n"
             << endl;
        wizard_login();
        system("cls");
    main_menuAgain:
        system("cls");
        cout << BOLD << UNDERLINE << "Welcome " << current_user.FullName << "!" << RESET << endl
             << endl;
        if (current_user.AccountType == "Admin") {
            switch (createMenu({
                "MAIN MENU",
                "View/Edit Transactional Record",
                "Calculate Total Income",
                "Inventory Status Report",
                "Take Back a Rented Car",
                "Vehicle Utilization Report",
                "Maintenance Report",
                "View/Edit Users",
                "Change Car Status",
                "Logout",
            })) {
            case 1:
            view_customer_transactionsAgain:
                view_customer_transactions();
                switch (createMenu({
                    "",
                    "Calculate the total income",
                    "Add a transactional record",
                    "Remove a transactional record",
                    "Edit in Excel",
                    "Back",
                })) {
                case 1:
                    cout << "Your total income is: " << total_income() << endl;
                    system("pause");
                    system("cls");
                    goto view_customer_transactionsAgain;
                case 2:
                    cout << "Enter the transactional details that you want to add to the database.\n"
                         << endl;
                    wizard_add_transaction('a');
                    system("cls");
                    goto view_customer_transactionsAgain;
                case 3:
                    wizard_delete_transaction();
                    system("cls");
                    goto view_customer_transactionsAgain;
                case 4:
                    system("cls");
                    openFile(customer_transactions_path);
                    cout << "Please save the file by pressing [CTRL+S] after you finish entering the records.\nOnce you have saved the file, you can ";
                    system("pause");
                    map_customer_transactions();
                    system("cls");
                    goto view_customer_transactionsAgain;
                case 5:
                    goto main_menuAgain;
                }
            case 2:
                cout << "Your total income is: " << total_income() << endl;
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 3:
            view_car_inventoryAgain:
                view_car_inventory();
                switch (createMenu({
                    "",
                    "Add a new car",
                    "Delete a car",
                    "Edit in Excel",
                    "Back",
                })) {
                case 1:
                    cout << "Enter the car details that you want to add to the database.\n"
                         << endl;
                    wizard_add_car();
                    cout << GREEN << "Car record is added to the database successfully." << RESET << endl;
                    system("pause");
                    system("cls");
                    goto view_car_inventoryAgain;
                case 2:
                    wizard_delete_car();
                    system("pause");
                    system("cls");
                    goto view_car_inventoryAgain;
                case 3:
                    system("cls");
                    openFile(car_inventory_path);
                    cout << "Please save the file by pressing [CTRL+S] after you finish entering the records.\nOnce you have saved the file, you can ";
                    system("pause");
                    map_car_inventory();
                    goto view_car_inventoryAgain;
                case 4:
                    goto main_menuAgain;
                }
            case 4:
                wizard_return_car('a');
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 5:
                utilization_report();
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 6:
                maintenance_report();
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 7:
            view_user_accountsAgain:
                view_user_accounts();
                switch (createMenu({
                    "",
                    "Add a new user",
                    "Delete a user",
                    "Edit in Excel",
                    "Back",
                })) {
                case 1:
                    cout << "Enter the user details you want to add to the database.\n"
                         << endl;
                    wizard_add_user('a');
                    cout << GREEN << "User is added to the database successfully." << RESET << endl;
                    system("pause");
                    system("cls");
                    goto view_user_accountsAgain;
                case 2:
                    wizard_delete_user();
                    system("pause");
                    system("cls");
                    goto view_user_accountsAgain;
                case 3:
                    openFile(user_accounts_path);
                    cout << "Please save the file by pressing [CTRL+S] after you finish entering the records.\nOnce you have saved the file, you can ";
                    system("pause");
                    map_user_accounts();
                    system("cls");
                    goto view_user_accountsAgain;
                case 4:
                    goto main_menuAgain;
                }
            case 8:
                view_car_inventory();
                cout << endl;
                wizard_change_car_status();
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 9:
                switch (createMenu({
                    "Are you sure that you want to log out?",
                    "Yes",
                    "No",
                })) {
                case 1:
                    goto startAgain;
                case 2:
                    goto main_menuAgain;
                }
            }
        } else if (current_user.AccountType == "Customer") {
            switch (createMenu({
                "MAIN MENU",
                "View Available Cars",
                "Rent a car",
                "Rental Report",
                "Return a Car",
                "Change account settings",
                "Logout",
            })) {
            case 1:
            c_view_car_inventoryAgain:
                view_car_inventory();
                switch (createMenu({
                    "",
                    "Rent a car",
                    "Back",
                })) {
                case 1:
                    wizard_add_transaction('c');
                    system("cls");
                    goto c_view_car_inventoryAgain;
                case 2:
                    goto main_menuAgain;
                }
            case 2:
                view_car_inventory();
                cout << endl;
                wizard_add_transaction('c');
                system("cls");
                goto main_menuAgain;
            case 3:
                if (current_user.CurrentCar != "None") {
                    cout << BOLD << UNDERLINE << "Rental Report" << RESET << endl
                         << endl;
                    cout << "Car Info" << endl
                         << "Number Plate: " << current_user.CurrentCar << endl
                         << "Model: " << car_inventory_map[current_user.CurrentCar].Model << endl
                         << "Color: " << car_inventory_map[current_user.CurrentCar].Color << endl
                         << "Condition: " << car_inventory_map[current_user.CurrentCar].Condition << endl
                         << "Price/hour: " << car_inventory_map[current_user.CurrentCar].Price << endl
                         << endl;
                } else
                    cout << YELLOW << "You do not have any car." << RESET << endl;
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 4:
                wizard_return_car('c');
                system("pause");
                system("cls");
                goto main_menuAgain;
            case 5:
                wizard_change_account_settings();
                goto main_menuAgain;
            case 6:
                switch (createMenu({
                    "Are you sure that you want to log out?",
                    "Yes",
                    "No",
                })) {
                case 1:
                    goto startAgain;
                case 2:
                    goto main_menuAgain;
                }
            }
        }
    case 3:
        switch (createMenu({
            "Are you sure you want to quit this database?",
            "Yes",
            "No",
        })) {
        case 1:
            cout << YELLOW << "Shutting down services..." << RESET << endl;
            break;
        case 2:
            goto startAgain;
        }
    }
    return 0;
}

// --FUNCTION DEFINITIONS--

void parseCSV(const string &filePath, string **&data, int &Nrows, int &Ncols) {
    ifstream fin(filePath);
    if (!fin.is_open()) {
        cerr << RED << "Error: " << filePath << " is missing from the directory" << RESET << endl;
        exit(1);
    }
    Nrows = Ncols = 0;
    string line;
    getline(fin, line); // ignoring the header of the excel file
    while (getline(fin, line)) {
        Nrows++;
        istringstream iss(line);
        string cell;
        int currentCol = 0;
        while (getline(iss, cell, ',')) {
            currentCol++;
        }
        Ncols = max(Ncols, currentCol);
    }
    fin.clear();
    fin.seekg(0, ios::beg);
    data = new string *[Nrows];
    for (int i = 0; i < Nrows; ++i) {
        data[i] = new string[Ncols];
    }
    int row = 0;
    getline(fin, line); // ignoring the header
    while (getline(fin, line)) {
        istringstream iss(line);
        string cell;
        int col = 0;
        while (getline(iss, cell, ',')) {
            data[row][col] = cell;
            col++;
        }
        row++;
    }
    fin.close();
}
sTime addTimes(const sTime &time1, const sTime &time2) {
    sTime result;
    result.Second = time1.Second + time2.Second;
    result.Minute = time1.Minute + time2.Minute + result.Second / 60;
    result.Second %= 60;
    result.Hour = time1.Hour + time2.Hour + result.Minute / 60;
    result.Minute %= 60;
    result.Day = time1.Day + time2.Day + result.Hour / 24;
    result.Hour %= 24;
    result.Month = time1.Month + time2.Month + result.Day / 30;
    result.Day %= 30;
    result.Year = time1.Year + time2.Year + result.Month / 12;
    result.Month %= 12;
    return result;
}
string timeToStr(const sTime &time) {
    string str = to_string(time.Day) + '-' + to_string(time.Month) + '-' + to_string(time.Year) + ' ' + to_string(time.Hour) + ':' + to_string(time.Minute) + ':' + to_string(time.Second);
    return str;
}
sTime getCurrentTime() {
    time_t currentTimeObj = time(nullptr);
    tm *localTime = localtime(&currentTimeObj);
    return {localTime->tm_year + 1900, localTime->tm_mon, localTime->tm_mday, localTime->tm_hour, localTime->tm_min, localTime->tm_sec};
}
void removeLastLines(const int &n) {
    for (int i = 0; i < n; i++) {
        cout << CLEARLINE;
        cout << MOVEUP;
    }
    cout << CLEARLINE << "\r";
}
int createMenu(const vector<string> &arr) {
    int input;
    if (arr[0] != "")
        cout << HEADING << arr[0] << RESET << endl;
    cout << endl;
menuAgain:
    for (int i = 1; i < arr.size(); i++) {
        cout << BULLETS << "[" << i << "]" << RESET << ' ' << BULLETTEXT << arr[i] << RESET << endl;
    }
    cout << INPUTFIELD << "\nYour input here: ";
    string inputStr;
    cin >> inputStr;
    cout << RESET;
    if (!isdigit(inputStr[0]) || (input = stoi(inputStr)) < 1 || input > arr.size() - 1) {
        removeLastLines(arr.size() + 1);
        goto menuAgain;
    }
    if (arr[0] != "")
        system("cls");
    else
        removeLastLines(arr.size() + 1);
    return input;
}
void openFile(const string &filePath) {
    string command = "start excel " + filePath;
    system(command.c_str());
}
void writeCSV_user_accounts() {
    ofstream fout(user_accounts_path);
    fout << "ID,Password,Full Name,Email Address,Card Number,CVV,Expiry Date,Times Rented,Current Car,Account Type" << endl;
    for (auto account : user_accounts_map) {
        fout << account.second.ID << ','
             << account.second.Password << ','
             << account.second.FullName << ','
             << account.second.EmailAddress << ','
             << account.second.CardNumber << ','
             << account.second.CVV << ','
             << account.second.ExpiryDate << ','
             << account.second.TimesRented << ','
             << account.second.CurrentCar << ','
             << account.second.AccountType << endl;
    }
    fout.close();
}
void writeCSV_customer_transactions() {
    ofstream fout(customer_transactions_path);
    fout << "ID,Customer ID,Car Number Plate,Amount,Start Time,End Time" << endl;
    for (auto transaction : customer_transactions_map) {
        fout << transaction.second.ID << ','
             << transaction.second.CustomerID << ','
             << transaction.second.CarNumberPlate << ','
             << transaction.second.Amount << ','
             << transaction.second.StartTime << ','
             << transaction.second.EndTime << endl;
    }
    fout.close();
}
void writeCSV_car_inventory() {
    ofstream fout(car_inventory_path);
    fout << "Number Plate,Model,Color,Price/Hour,Times Rented,Repairs,Condition,Status" << endl;
    for (auto car : car_inventory_map) {
        fout << car.second.NumberPlate << ','
             << car.second.Model << ','
             << car.second.Color << ','
             << car.second.Price << ','
             << car.second.TimesRented << ','
             << car.second.Repairs << ','
             << car.second.Condition << ','
             << car.second.Status << endl;
    }
    fout.close();
}

// mapping
void map_user_accounts() {
    int row, col;
    string **users;
    parseCSV(user_accounts_path, users, row, col);
    map<string, sUser> map;
    for (int i = 0; i < row; i++) {
        sUser user;
        user.ID = users[i][0];
        user.Password = users[i][1];
        user.FullName = users[i][2];
        user.EmailAddress = users[i][3];
        user.CardNumber = users[i][4];
        user.CVV = users[i][5];
        user.ExpiryDate = users[i][6];
        user.TimesRented = stoi(users[i][7]);
        user.CurrentCar = users[i][8];
        user.AccountType = users[i][9];
        map[user.ID] = user;
    }
    delete[] users;
    user_accounts_map = map;
    current_user = map[current_user.ID];
}
void map_car_inventory() {
    int row, col;
    string **cars;
    parseCSV(car_inventory_path, cars, row, col);
    map<string, sCar> map;
    for (int i = 0; i < row; i++) {
        sCar car;
        car.NumberPlate = cars[i][0];
        car.Model = cars[i][1];
        car.Color = cars[i][2];
        car.Price = stof(cars[i][3]);
        car.TimesRented = stoi(cars[i][4]);
        car.Repairs = stoi(cars[i][5]);
        car.Condition = cars[i][6];
        car.Status = cars[i][7];
        map[car.NumberPlate] = car;
    }
    delete[] cars;
    car_inventory_map = map;
}
void map_customer_transactions() {
    int row, col;
    string **transactions;
    parseCSV(customer_transactions_path, transactions, row, col);
    map<int, sTransaction> map;
    for (int i = 0; i < row; i++) {
        sTransaction transaction;
        transaction.ID = stoi(transactions[i][0]);
        transaction.CustomerID = transactions[i][1];
        transaction.CarNumberPlate = transactions[i][2];
        transaction.Amount = stof(transactions[i][3]);
        transaction.StartTime = transactions[i][4];
        transaction.EndTime = transactions[i][5];
        map[transaction.ID] = transaction;
    }
    delete[] transactions;
    customer_transactions_map = map;
}

// validating
bool validateAccount(const string &ID, string password = "") {
    if (user_accounts_map.find(ID) != user_accounts_map.end()) {
        if (password == "")
            return true;
        sUser current = user_accounts_map[ID];
        if (current.Password == password) {
            current_user = current;
            return true;
        }
    }
    return false;
}
bool rePatternMatch(const string &str, const string &rePattern) {
    regex pattern(rePattern);
    return regex_match(str, pattern);
}

// special purpose functions and wizards
// c = customer
// a = admin
void wizard_add_user(const char &mode) {
    string id, password, name, email, card, cvv, expiry, type;
UsernameAgain:
    cout << "Username: ";
    cin >> id;
    if (validateAccount(id)) {
        cout << RED << "This username is not available.\nPlease choose another one." << RESET << endl;
        system("pause");
        removeLastLines(4);
        goto UsernameAgain;
    }
passwordAgain:
    cout << "Password: ";
    cin >> password;
    if (!rePatternMatch(password, "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?!.*\\,).{8,}$")) {
        cout << RED << "Your password must be at least 8 characters long, with at least\n- One uppercase character\n- One lowercase character\n- One digit\nand no commas." << RESET << endl;
        system("pause");
        removeLastLines(7);
        goto passwordAgain;
    }
nameAgain:
    cout << "Full Name: ";
    cin.ignore();
    getline(cin, name);
    if (!rePatternMatch(name, "^[a-zA-Z ]+$")) {
        cout << RED << "Please enter a valid name." << RESET << endl;
        system("pause");
        removeLastLines(3);
        goto nameAgain;
    }
emailAgain:
    cout << "Email Address: ";
    cin >> email;
    if (!rePatternMatch(email, "^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)+$")) {
        cout << RED << "Your email address must be of the format 'username@email.com'." << RESET << endl;
        system("pause");
        removeLastLines(3);
        goto emailAgain;
    }
cardAgain:
    cout << "Card Number: ";
    cin >> card;
    if (!rePatternMatch(card, "^\\d{13,19}$")) {
        cout << RED << "Card number you entered is not a valid card.\nPlease enter a valid card number." << RESET << endl;
        system("pause");
        removeLastLines(4);
        goto cardAgain;
    }
cvvAgain:
    cout << "Card Verification Value: ";
    cin >> cvv;
    if (!rePatternMatch(cvv, "^\\d{3}$")) {
        cout << RED << "Card Verification Value you entered is not a valid one.\nPlease enter a valid CVV." << RESET << endl;
        system("pause");
        removeLastLines(4);
        goto cvvAgain;
    }
expiryAgain:
    cout << "Expiry Date: ";
    cin >> expiry;
    if (!rePatternMatch(expiry, "^(0[1-9]|1[0-2])/(\\d{2})$")) {
        cout << RED << "Your card expiry date must be of the format 'MM/YY'." << RESET << endl;
        system("pause");
        removeLastLines(3);
        goto expiryAgain;
    }
    if (mode == 'c') {
        type = "Customer";
    } else if (mode == 'a') {
        vector<string> typeOptions = {
            "Select account type for the user",
            "Admin",
            "Customer",
        };
        type = typeOptions[createMenu(typeOptions)];
    }
    sUser user;
    user.ID = id;
    user.Password = password;
    user.FullName = name;
    user.EmailAddress = email;
    user.CardNumber = card;
    user.CVV = cvv;
    user.ExpiryDate = expiry;
    user.TimesRented = 0;
    user.CurrentCar = "None";
    user.AccountType = type;
    user_accounts_map[user.ID] = user;
    writeCSV_user_accounts();
    map_user_accounts();
}
void wizard_add_transaction(const char &mode) {
    int id;
    string customer_id, number_plate, amount, hours, days, from_time, to_time;
    id = customer_transactions_map.size() + 1;
customerAgain:
    if (mode == 'a') {
        cout << "Customer username: ";
        cin >> customer_id;
        if (!validateAccount(customer_id)) {
            cout << RED << "This user account does not exists." << RESET << endl;
            system("pause");
            removeLastLines(3);
            goto customerAgain;
        }
    } else if (mode == 'c')
        customer_id = current_user.ID;
    if (user_accounts_map[customer_id].AccountType == "Admin") {
        cout << RED << "An admin cannot buy a car." << RESET << endl;
        system("pause");
        removeLastLines(3);
        goto customerAgain;
    }
    if (user_accounts_map[customer_id].CurrentCar == "None") {
    number_plateAgain:
        cout << "Choose a car by typing the car's number plate: ";
        cin >> number_plate;
        if (car_inventory_map.find(number_plate) == car_inventory_map.end()) {
            cout << RED << "This car could not be found in the database." << RESET << endl;
            system("pause");
            removeLastLines(3);
            goto number_plateAgain;
        }
        if (car_inventory_map[number_plate].Status != "Available") {
            cout << RED << "This car is not available right now." << RESET << endl;
            system("pause");
            removeLastLines(3);
            goto number_plateAgain;
        }
    daysAgain:
        cout << "For how many days? Enter days from 0 to 30 days: ";
        cin >> days;
        if (!rePatternMatch(days, "^([0-9]|[1-2][0-9]|30)$")) {
            cout << RED << "This time limit is not valid.\nPlease specify again." << RESET << endl;
            system("pause");
            removeLastLines(4);
            goto daysAgain;
        }
    hoursAgain:
        cout << "And hours? Enter hours from 1 to 23: ";
        cin >> hours;
        if (!rePatternMatch(hours, "^([1-9]|1[0-9]|2[0-3])$")) {
            cout << RED << "This time limit is not valid.\nPlease specify again." << RESET << endl;
            system("pause");
            removeLastLines(4);
            goto hoursAgain;
        }
        sTransaction transaction;
        transaction.ID = id;
        transaction.CustomerID = customer_id;
        transaction.CarNumberPlate = number_plate;
        transaction.Amount = car_inventory_map[number_plate].Price * (stoi(hours) + (stoi(days) * 24));
        transaction.StartTime = timeToStr(getCurrentTime());
        transaction.EndTime = timeToStr(addTimes(getCurrentTime(), {0, 0, stoi(days), stoi(hours), 0, 0}));
        system("cls");
        cout << "Customer ID: " << transaction.CustomerID << endl
             << "Car Number Plate: " << transaction.CarNumberPlate << endl
             << "Amount: " << transaction.Amount << endl
             << "Start Time: " << transaction.StartTime << endl
             << "End Time: " << transaction.EndTime << endl
             << endl;
        switch (createMenu({
            "Confirm this transaction?",
            "Yes",
            "No",
        })) {
        case 1:
            customer_transactions_map[transaction.ID] = transaction;
            car_inventory_map[number_plate].Status = "Rented";
            car_inventory_map[number_plate].TimesRented++;
            user_accounts_map[customer_id].TimesRented++;
            user_accounts_map[customer_id].CurrentCar = number_plate;
            writeCSV_user_accounts();
            map_user_accounts();
            writeCSV_customer_transactions();
            map_customer_transactions();
            writeCSV_car_inventory();
            map_car_inventory();
            cout << GREEN << "Transactional record is added to the database successfully." << RESET << endl;
            system("pause");
            break;
        case 2:
            cout << YELLOW << "This transaction is canceled." << RESET << endl;
            system("pause");
        }
    } else {
        cout << RED << "This user account has already rented a car." << RESET << endl;
        system("pause");
    }
}
void wizard_add_car() {
    string number_plate, model, color, price, condition, status;
number_plateAgain:
    cout << "Car number plate: ";
    cin >> number_plate;
    if (car_inventory_map.find(number_plate) != car_inventory_map.end()) {
        cout << "This car already exists." << endl;
        system("pause");
        removeLastLines(3);
        goto number_plateAgain;
    }
modelAgain:
    cout << "Car model: ";
    cin.ignore();
    getline(cin, model);
    if (!rePatternMatch(model, "^[A-Za-z1-9-\\._ ]+$")) {
        cout << "The model you entered is invalid.\nPlease enter a valid name." << endl;
        system("pause");
        removeLastLines(4);
        goto modelAgain;
    }
priceAgain:
    cout << "Price: ";
    cin >> price;
    if (!rePatternMatch(price, "^\\d+\\.\\d*$")) {
        cout << "The price you entered is invalid.\nPlease enter a valid price." << endl;
        system("pause");
        removeLastLines(4);
        goto priceAgain;
    }
    vector<string> colorOptions = {
        "Select a color of the car",
        "Red",
        "Blue",
        "Green",
        "Yellow",
        "Orange",
        "Purple",
        "Pink",
        "Brown",
        "Black",
        "White",
    };
    color = colorOptions[createMenu(colorOptions)];
    vector<string> statusOptions = {
        "Select the status of car",
        "Available",
        "Maintainence",
        "Coming Soon",
    };
    status = statusOptions[createMenu(statusOptions)];
    sCar car;
    car.NumberPlate = number_plate;
    car.Model = model;
    car.Color = color;
    car.Price = stof(price);
    car.TimesRented = 0;
    car.Repairs = 0;
    if (status == "Maintainence")
        car.Repairs++;
    car.Condition = condition;
    car.Status = status;
    car_inventory_map[number_plate] = car;
    writeCSV_car_inventory();
    map_car_inventory();
}

void wizard_delete_user() {
    string id;
    cout << "Enter the username of an account that you want to delete.\n\nUsername: ";
    cin >> id;
    if (user_accounts_map.find(id) != user_accounts_map.end()) {
        if (current_user.ID == id)
            cout << YELLOW << "You cannot remove yourself from the database.\nTo do that, contact your IT administrator." << RESET << endl;
        else {
            switch (createMenu({
                "Are you sure you want to delete this account from database?",
                "Yes",
                "No",
            })) {
            case 1:
                user_accounts_map.erase(id);
                writeCSV_user_accounts();
                map_user_accounts();
                cout << GREEN << "User account is deleted successfully." << RESET << endl;
                break;
            case 2:
                cout << YELLOW << "Delete request canceled." << RESET << endl;
            }
        }
    } else {
        cout << YELLOW << "This user account does not exists." << RESET << endl;
    }
}
void wizard_delete_transaction() {
    int id;
    cout << "Enter the Transaction ID of a transaction record that you want to delete.\nTransaction ID: ";
    cin >> id;
    if (customer_transactions_map.find(id) != customer_transactions_map.end()) {
        switch (createMenu({
            "Are you sure you want to delete this transaction record?",
            "Yes",
            "No",
        })) {
        case 1:
            customer_transactions_map.erase(id);
            writeCSV_customer_transactions();
            cout << GREEN << "Transactional record is deleted successfully." << RESET << endl;
            system("pause");
            break;
        case 2:
            cout << YELLOW << "Delete request canceled." << RESET << endl;
            system("pause");
        }
    } else {
        cout << YELLOW << "This transaction record does not exists." << RESET << endl;
        system("pause");
    }
}
void wizard_delete_car() {
    string number_plate;
    cout << "Enter the car's number plate that you want to delete.\nNumber Plate: ";
    cin >> number_plate;
    if (car_inventory_map.find(number_plate) != car_inventory_map.end()) {
        switch (createMenu({
            "Are you sure you want to delete this car from the database?",
            "Yes",
            "No",
        })) {
        case 1:
            car_inventory_map.erase(number_plate);
            writeCSV_car_inventory();
            cout << GREEN << "This car is deleted successfully." << RESET << endl;
            break;
        case 2:
            cout << YELLOW << "Delete request canceled." << RESET << endl;
        }
    } else {
        cout << YELLOW << "This car does not exists." << RESET << endl;
    }
}
void wizard_login() {
    string id, password;
    char ch;
UsernameAgain:
    cout << "Username: ";
    cin >> id;
    cout << "Password: ";
    cin >> password;
    if (!validateAccount(id, password)) {
        cout << RED << "Incorrect username or password.\nPlease Enter again." << RESET << endl;
        system("pause");
        removeLastLines(5);
        goto UsernameAgain;
    }
}
void wizard_change_car_status() {
    string number_plate;
number_plateAgain:
    cout << "Choose a car by typing the car's number plate: ";
    cin >> number_plate;
    if (car_inventory_map.find(number_plate) == car_inventory_map.end()) {
        cout << RED << "This car could not be found in the database." << RESET << endl;
        system("pause");
        removeLastLines(3);
        goto number_plateAgain;
    }
    system("cls");
    vector<string> statusOptions = {
        "Select the status of car",
        "Available",
        "Maintainence",
        "Coming Soon",
    };
    car_inventory_map[number_plate].Status = statusOptions[createMenu(statusOptions)];
    if (car_inventory_map[number_plate].Status == "Maintainence")
        car_inventory_map[number_plate].Repairs++;
    writeCSV_car_inventory();
    map_car_inventory();
    cout << GREEN << "Status of the car '" << number_plate << "' Successfully set to '" << car_inventory_map[number_plate].Status << "'." << RESET << endl;
}
void wizard_change_account_settings() {
    string password, name, email, card, cvv, expiry;
change_account_settingsAgain:
    switch (createMenu({
        "Change Account Settings",
        "Password",
        "Name",
        "Email",
        "Change Card Settings",
        "Back",
    })) {
    case 1:
    passwordAgain:
        cout << "Password: ";
        cin >> password;
        if (!rePatternMatch(password, "^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)(?!.*\\,).{8,}$")) {
            cout << RED << "Your password must be at least 8 characters, with at least\n- One uppercase character\n- One lowercase character\n- One digit\nand no commas." << RESET << endl;
            system("pause");
            removeLastLines(7);
            goto passwordAgain;
        }
        current_user.Password = password;
        break;
    case 2:
    nameAgain:
        cout << "Full Name: ";
        cin.ignore();
        getline(cin, name);
        if (!rePatternMatch(name, "^[a-zA-Z ]+$")) {
            cout << RED << "Please enter a valid name." << RESET << endl;
            system("pause");
            removeLastLines(3);
            goto nameAgain;
        }
        current_user.FullName = name;
        break;
    case 3:
    emailAgain:
        cout << "Email Address: ";
        cin >> email;
        if (!rePatternMatch(email, "^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+(\\.[a-zA-Z0-9-]+)+$")) {
            cout << RED << "Your email address must be of the format 'username@email.com'." << RESET << endl;
            system("pause");
            removeLastLines(3);
            goto emailAgain;
        }
        current_user.EmailAddress = email;
        break;
    case 4:
    cardAgain:
        cout << "Card Number: ";
        cin >> card;
        if (!rePatternMatch(card, "^\\d{13,19}$")) {
            cout << RED << "Card number you entered is not a valid card.\nPlease enter a valid card number." << RESET << endl;
            system("pause");
            removeLastLines(4);
            goto cardAgain;
        }
    cvvAgain:
        cout << "Card Verification Value: ";
        cin >> cvv;
        if (!rePatternMatch(cvv, "^\\d{3}$")) {
            cout << RED << "Card Verification Value you entered is not a valid one.\nPlease enter a valid CVV." << RESET << endl;
            system("pause");
            removeLastLines(4);
            goto cvvAgain;
        }
    expiryAgain:
        cout << "Expiry Date: ";
        cin >> expiry;
        if (!rePatternMatch(expiry, "^(0[1-9]|1[0-2])/(\\d{2})$")) {
            cout << RED << "Your card expiry date must be of the format 'MM/YY'." << RESET << endl;
            system("pause");
            removeLastLines(3);
            goto expiryAgain;
        }
        current_user.CardNumber = card;
        current_user.CVV = cvv;
        current_user.ExpiryDate = expiry;
        break;
    case 5:
        return;
    }
    system("cls");
    cout << GREEN << "Account settings are saved successfully." << RESET << endl;
    system("pause");
    system("cls");
    user_accounts_map[current_user.ID] = current_user;
    writeCSV_user_accounts();
    map_user_accounts();
    goto change_account_settingsAgain;
}
void wizard_return_car(const char &mode) {
    string id;
    if (mode == 'a') {
        cout << left
             << setw(17) << "Number Plate"
             << setw(25) << "Model"
             << setw(20) << "Customer"
             << endl
             << endl;
        for (auto car : car_inventory_map) {
            if (car.second.Status == "Rented")
                cout << left
                     << setw(17) << car.second.NumberPlate
                     << setw(25) << car.second.Model
                     << setw(20);
            for (auto user : user_accounts_map)
                if (user.second.CurrentCar == car.second.NumberPlate)
                    cout << user.first;
        }
        cout << endl;
        cout << "Enter a username that you want to take back a car from.\nUsername: ";
        cin >> id;
    } else if (mode == 'c')
        id = current_user.ID;
    if (user_accounts_map.find(id) == user_accounts_map.end())
        cout << YELLOW << "This user account does not exists." << RESET << endl;
    else if (user_accounts_map[id].CurrentCar == "None") {
        if (mode == 'c')
            cout << YELLOW << "You do not have any car." << RESET << endl;
        else if (mode == 'a')
            cout << YELLOW << "This user account does not exists." << RESET << endl;
    } else {
        switch (createMenu({
            "Are you sure that you want to return back this car?",
            "Yes",
            "No",
        })) {
        case 1:
            car_inventory_map[user_accounts_map[id].CurrentCar].Status = "Available";
            user_accounts_map[id].CurrentCar = "None";
            writeCSV_user_accounts();
            map_user_accounts();
            writeCSV_car_inventory();
            map_car_inventory();
            if (mode == 'c')
                cout << GREEN << "Return request was successful. Please return this car to the company." << RESET << endl;
            else if (mode == 'a')
                cout << GREEN << "Car record was successfully set to available." << RESET << endl;
            break;
        case 2:
            cout << YELLOW << "Delete request canceled." << RESET << endl;
        }
    }
}
float total_income() {
    float total_income = 0;
    for (auto transaction : customer_transactions_map)
        total_income += customer_transactions_map[transaction.first].Amount;
    return total_income;
}
void maintenance_report() {
    cout << BOLD << UNDERLINE << "Maintenance Report" << RESET << endl
         << endl;
    cout << RED << "[x]" << RESET << " Need maintenance" << endl
         << YELLOW << "[x]" << RESET << " Good Health" << endl
         << GREEN << "[x]" << RESET << " Excellent Health" << endl
         << endl;
    cout << left
         << setw(17) << "Number Plate"
         << setw(25) << "Model"
         << setw(20) << "Condition"
         << endl
         << endl;
    cout << RED;
    for (auto car : car_inventory_map) {
        if (car.second.Condition == "Fair")
            cout << left
                 << setw(17) << car.second.NumberPlate
                 << setw(25) << car.second.Model
                 << setw(20) << car.second.Condition
                 << endl;
    }
    cout << YELLOW;
    for (auto car : car_inventory_map) {
        if (car.second.Condition == "Good")
            cout << left
                 << setw(17) << car.second.NumberPlate
                 << setw(25) << car.second.Model
                 << setw(20) << car.second.Condition
                 << endl;
    }
    cout << GREEN;
    for (auto car : car_inventory_map) {
        if (car.second.Condition == "Excellent")
            cout << left
                 << setw(17) << car.second.NumberPlate
                 << setw(25) << car.second.Model
                 << setw(20) << car.second.Condition
                 << endl;
    }
    cout << RESET;
}
void utilization_report() {
    cout << BOLD << UNDERLINE << "Utilization Report" << RESET << endl
         << endl;
    cout << left
         << setw(17) << "Number Plate"
         << setw(25) << "Model"
         << setw(15) << "Times Rented"
         << setw(15) << "Repairs"
         << setw(20) << "Status"
         << endl;
    for (auto car : car_inventory_map) {
        cout << left
             << setw(17) << car.second.NumberPlate
             << setw(25) << car.second.Model
             << setw(15) << car.second.TimesRented
             << setw(15) << car.second.Repairs
             << setw(20) << car.second.Status
             << endl;
    }
}
void view_car_inventory() {
    cout << left
         << setw(17) << "Number Plate"
         << setw(25) << "Model"
         << setw(15) << "Color"
         << setw(15) << "Price"
         << setw(15) << "Times Rented"
         << setw(15) << "Repairs"
         << setw(20) << "Condition"
         << setw(20) << "Status"
         << endl
         << endl;
    for (auto car : car_inventory_map)
        cout << left
             << setw(17) << car.second.NumberPlate
             << setw(25) << car.second.Model
             << setw(15) << car.second.Color
             << setw(15) << car.second.Price
             << setw(15) << car.second.TimesRented
             << setw(15) << car.second.Repairs
             << setw(20) << car.second.Condition
             << setw(20) << car.second.Status
             << endl;
}
void view_customer_transactions() {
    cout << left
         << setw(10) << "ID"
         << setw(15) << "Customer ID"
         << setw(20) << "Car Number Plate"
         << setw(15) << "Amount"
         << setw(25) << "Start Time"
         << setw(25) << "End Time"
         << endl
         << endl;
    for (auto transaction : customer_transactions_map)
        cout << left
             << setw(10) << transaction.second.ID
             << setw(15) << transaction.second.CustomerID
             << setw(20) << transaction.second.CarNumberPlate
             << setw(15) << transaction.second.Amount
             << setw(25) << transaction.second.StartTime
             << setw(25) << transaction.second.EndTime
             << endl;
}
void view_user_accounts() {
    cout << left
         << setw(20) << "ID"
         << setw(20) << "Password"
         << setw(20) << "Full Name"
         << setw(40) << "Email Address"
         << setw(20) << "Card Number"
         << setw(10) << "CVV"
         << setw(15) << "Expiry Date"
         << setw(15) << "Times Rented"
         << setw(15) << "Current Car"
         << setw(15) << "Account Type"
         << endl
         << endl;
    for (auto account : user_accounts_map)
        cout << left
             << setw(20) << account.second.ID
             << setw(20) << account.second.Password
             << setw(20) << account.second.FullName
             << setw(40) << account.second.EmailAddress
             << setw(20) << account.second.CardNumber
             << setw(10) << account.second.CVV
             << setw(15) << account.second.ExpiryDate
             << setw(15) << account.second.TimesRented
             << setw(15) << account.second.CurrentCar
             << setw(15) << account.second.AccountType
             << endl;
}
