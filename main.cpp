#include <iostream>
#include <fstream>
#include <cstring>
#include "functions.h"

using namespace std;

int main() {
    Student* pstHead = nullptr;
    int choice;

    cout << "Виберіть дію:\n1 - Завантажити список студентів із файлу\n2 - Створити новий список студентів\n";
    cin >> choice;

    if (choice == 1) {
        char szFilename[100];
        cout << "Введіть шлях до файлу: ";
        cin >> szFilename;

        ifstream ifsFile(szFilename);
        if (!ifsFile.is_open()) {
            cerr << "Не вдалося відкрити файл.\n";
            return 1;
        }

        char szLine[256];
        while (ifsFile.getline(szLine, sizeof(szLine))) {
            char szLastName[25], szFirstName[25], szDOB[20];
            int anGrades[5];
            parseStudent(szLine, szLastName, szFirstName, szDOB, anGrades);
            append(&pstHead, szLastName, szFirstName, szDOB, anGrades);
        }
        ifsFile.close();

        cout << "Повний список студентів:\n";
        display(pstHead);
    }
    else if (choice == 2) {
        char continueAdding = 'y';
        while (continueAdding == 'y') {
            addStudent(&pstHead);
            cout << "Додати ще одного студента? (y/n): ";
            cin >> continueAdding;
        }
    }

    bool continueActions = true;
    while (continueActions) {
        cout << "\nВиберіть дію:\n1 - Відсортувати список\n2 - Відфільтрувати список\n3 - Зберегти список\n4 - Видалити студента\n5 - Завершити\n";
        int action;
        cin >> action;

        switch (action) {
            case 1: {
                Student* pstSortedHead = sortStudents(pstHead);
                clearList(pstHead);
                pstHead = pstSortedHead;
                cout << "\nВідсортований список:\n";
                display(pstHead);
                break;
            }
            case 2: {
                Student* pstFilteredHead = filterStudents(pstHead);
                clearList(pstHead);
                pstHead = pstFilteredHead;
                cout << "\nВідфільтрований список:\n";
                display(pstHead);
                break;
            }
            case 3: {
                writeStudentsToFile(pstHead);
                break;
            }
            case 4: {
                char szLastName[25], szFirstName[25];
                cout << "Введіть прізвище студента для видалення: ";
                cin >> szLastName;
                cout << "Введіть ім'я студента для видалення: ";
                cin >> szFirstName;
                deleteStudent(&pstHead, szLastName, szFirstName);
                break;
            }
            case 5: {
                continueActions = false;
                break;
            }
            default:
                cout << "Невірний вибір\n";
        }
    }

    clearList(pstHead);
    return 0;
}
