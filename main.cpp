
#include <iostream>
#include <string>
#include <sqlite3.h>

using namespace std;

void displayStudents(sqlite3* DB) {
    sqlite3_stmt* statement;

    string sql = "SELECT * FROM Students;";

    if (sqlite3_prepare_v2(DB, sql.c_str(), -1,
                           &statement, nullptr) != SQLITE_OK) {
        cout << "Error retrieving students!" << endl;
        return;
    }

    cout << "\n===== REGISTERED STUDENTS =====\n";

    bool found = false;

    while (sqlite3_step(statement) == SQLITE_ROW) {
        found = true;

        cout << "\nStudent ID: "
             << sqlite3_column_text(statement, 1);

        cout << "\nFull Name: "
             << sqlite3_column_text(statement, 2);

        cout << "\nEmail: "
             << sqlite3_column_text(statement, 3);

        cout << "\nProgram: "
             << sqlite3_column_text(statement, 4);

        cout << "\nPhone: "
             << sqlite3_column_text(statement, 5);

        cout << "\n---------------------------\n";
    }

    if (!found) {
        cout << "No students registered yet.\n";
    }

    sqlite3_finalize(statement);
}

int main() {
    sqlite3* DB;
    char* errorMessage = nullptr;

    if (sqlite3_open("students.db", &DB) != SQLITE_OK) {
        cout << "Database connection failed!" << endl;
        return 1;
    }

    string sql =
        "CREATE TABLE IF NOT EXISTS Students ("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
        "StudentID TEXT,"
        "FullName TEXT,"
        "Email TEXT,"
        "Program TEXT,"
        "Phone TEXT);";

    sqlite3_exec(DB, sql.c_str(), nullptr, nullptr, &errorMessage);

    
    int choice;

    do {
        cout << "\n===== STUDENT INFORMATION SYSTEM =====\n";
        cout << "1. Register Student\n";
        cout << "2. View All Students\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            string studentID, fullName, email, program, phone;

            cout << "\nEnter Student ID: ";
            getline(cin, studentID);

            cout << "Enter Full Name: ";
            getline(cin, fullName);

            cout << "Enter Email: ";
            getline(cin, email);

            cout << "Enter Program: ";
            getline(cin, program);

            cout << "Enter Phone Number: ";
            getline(cin, phone);

            sqlite3_stmt* statement;

            string sql =
                "INSERT INTO Students "
                "(StudentID, FullName, Email, Program, Phone) "
                "VALUES (?, ?, ?, ?, ?);";

            if (sqlite3_prepare_v2(DB, sql.c_str(), -1,
                &statement, nullptr) == SQLITE_OK) {

                sqlite3_bind_text(statement, 1, studentID.c_str(),
                    -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(statement, 2, fullName.c_str(),
                    -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(statement, 3, email.c_str(),
                    -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(statement, 4, program.c_str(),
                    -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(statement, 5, phone.c_str(),
                    -1, SQLITE_TRANSIENT);

                if (sqlite3_step(statement) == SQLITE_DONE) {
                    cout << "\nStudent saved successfully!\n";
                } else {
                    cout << "\nError saving student!\n";
                }

                sqlite3_finalize(statement);
            } else {
                cout << "\nCould not prepare registration!\n";
            }
        }
        else if (choice == 2) {
            displayStudents(DB);
        }
        else if (choice == 3) {
            cout << "\nThank you for using the system!\n";
        }
        else {
            cout << "\nInvalid choice. Try again!\n";
        }

    } while (choice != 3);

    sqlite3_close(DB);


    return 0;
}
