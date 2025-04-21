#include <iostream>
#include <fstream>
#include <cstring>
#include "functions.h"

using namespace std;

int main() {
    Student* pstHead = nullptr;
    int choice;

    do {
        cout << "\nВиберіть дію:\n1 - Завантажити список студентів із файлу\n2 - Створити новий список студентів\n3 - Вийти\n";
        cin >> choice;

        if (choice == 1) {
            // Хотфікс: очищення попереднього списку перед завантаженням нового
            clearList(pstHead);
            pstHead = nullptr;

            char szFilename[100];
            cout << "Введіть шлях до файлу: ";
            cin >> szFilename;

            ifstream ifsFile(szFilename);
            if (!ifsFile.is_open()) {
                cerr << "Не вдалося відкрити файл.\n";
                continue;
            }

            char szLine[500];
            bool isEmpty = true;
            while (ifsFile.getline(szLine, sizeof(szLine))) {
                isEmpty = false;
                char szLastName[151], szFirstName[151], szDOB[20];
                int anGrades[5];
                parseStudent(szLine, szLastName, szFirstName, szDOB, anGrades);
                append(&pstHead, szLastName, szFirstName, szDOB, anGrades);
            }
            ifsFile.close();

            if (isEmpty) {
                cout << "Файл порожній. Будь ласка, виберіть інший файл або створіть новий список.\n";
                continue;
            }

            cout << "Повний список студентів:\n";
            display(pstHead); //debug point 

            bool continueActions = true;
            while (continueActions) {
                cout << "\nВиберіть дію:\n1 - Відсортувати список\n2 - Відфільтрувати список\n3 - Додати студента\n4 - Видалити студента\n5 - Зберегти список\n6 - Повернутися до головного меню\n";
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
                        addStudent(&pstHead);
                        cout << "\nОновлений список:\n";
                        display(pstHead);
                        break;
                    }
                    case 4: {
                        char szLastName[151], szFirstName[151];
                        cout << "Введіть прізвище студента для видалення: ";
                        cin >> szLastName;
                        cout << "Введіть ім'я студента для видалення: ";
                        cin >> szFirstName;
                        deleteStudent(&pstHead, szLastName, szFirstName);
                        cout << "\nОновлений список:\n";
                        display(pstHead);
                        break;
                    }
                    case 5: {
                        writeStudentsToFile(pstHead);
                        break;
                    }
                    case 6: {
                        continueActions = false;
                        break;
                    }
                    default:
                        cout << "Невірний вибір\n";
                }
            }
        }
        else if (choice == 2) {
            // Хотфікс: очищення попереднього списку перед створенням нового
            clearList(pstHead);
            pstHead = nullptr;

            char continueAdding = 'y';
            while (continueAdding == 'y' || continueAdding == 'Y') {  // Хотфікс: додано перевірку на 'Y'
                addStudent(&pstHead);
                display(pstHead); //дебаг поінт
                cout << "Додати ще одного студента? (y/n): ";
                cin >> continueAdding;
            }

            bool continueActions = true;
            while (continueActions) {
                cout << "\nВиберіть дію:\n1 - Відсортувати список\n2 - Відфільтрувати список\n3 - Додати студента\n4 - Видалити студента\n5 - Зберегти список\n6 - Повернутися до головного меню\n";
                int action;
                cin >> action;

                switch (action) {
                    case 1: {
                        Student* pstSortedHead = sortStudents(pstHead);
                        clearList(pstHead);
                        pstHead = pstSortedHead;
                        cout << "\nВідсортований список:\n";
                        display(pstHead);  // Хотфікс: виправлено - було clearList замість display
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
                        addStudent(&pstHead);
                        cout << "\nОновлений список:\n";
                        display(pstHead);
                        break;
                    }
                    case 4: {
                        char szLastName[151], szFirstName[151];
                        cout << "Введіть прізвище студента для видалення: ";
                        cin >> szLastName;
                        cout << "Введіть ім'я студента для видалення: ";
                        cin >> szFirstName;
                        deleteStudent(&pstHead, szLastName, szFirstName);
                        cout << "\nОновлений список:\n";
                        display(pstHead);
                        break;
                    }
                    case 5: {
                        writeStudentsToFile(pstHead);
                        break;
                    }
                    case 6: {
                        continueActions = false;
                        break;
                    }
                    default:
                        cout << "Невірний вибір\n";
                }
            }
        }
        else if (choice == 3) {
            break;
        }
        else {
            cout << "Невірний вибір\n";
        }

    } while (true);

    clearList(pstHead);
    return 0;
}
