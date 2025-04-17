#include <iostream>
#include <fstream>
#include <cstring>
#include "functions.h"

using namespace std;

int main() {
    Student* pstHead = nullptr;
    string openFileName; // Ім'я відкритого файлу

    cout << "Виберіть дію:\n1 - Завантажити список студентів із файлу.\n2 - Створити новий список студентів.\n";
    int choice;
    cin >> choice;

    if (choice == 1) {
        // Завантаження списку студентів із файлу
        cout << "Введіть шлях до файлу: ";
        cin >> openFileName; // Зберігаємо ім'я файлу
        ifstream ifsFile(openFileName);

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

        bool continueActions = true;
        while (continueActions) {
            cout << "\nВиберіть дію:\n1 - Відсортувати список.\n2 - Відфільтрувати список.\n3 - Зберегти зміни до відкритого файлу.\n4 - Завершити.\n";
            int action;
            cin >> action;

            if (action == 1) {
                // Сортування списку
                Student* pstSortedHead = sortStudents(pstHead);
                clearList(pstHead);  // Очищаємо старий список
                pstHead = pstSortedHead; // Оновлюємо основний список
                cout << "\nВідсортований список студентів:\n";
                display(pstHead);
            } else if (action == 2) {
                // Фільтрація списку
                Student* pstFilteredHead = filterStudents(pstHead);
                clearList(pstHead); // Очищаємо старий список
                pstHead = pstFilteredHead; // Оновлюємо основний список
                cout << "\nВідфільтрований список студентів:\n";
                display(pstHead);
            } else if (action == 3) {
                // Збереження змін у файл
                ofstream ofsFile(openFileName);
                if (!ofsFile.is_open()) {
                    cerr << "Не вдалося відкрити файл для запису.\n";
                } else {
                    writeStudentsToFile(pstHead, openFileName.c_str());
                    cout << "Зміни успішно збережені до файлу \"" << openFileName << "\"!\n";
                    ofsFile.close();
                }
            } else if (action == 4) {
                // Завершення операцій
                continueActions = false;
            } else {
                cout << "Невірний вибір. Спробуйте ще раз.\n";
            }
        }

    } else if (choice == 2) {
        // Створення нового списку студентів
        char continueAdding = 'y';

        // Абсолютний шлях
        cout << "Введіть ім'я файлу для збереження списку (наприклад, students_new.txt): ";
        char szFileName[100];
        cin >> szFileName;

        // Формуємо повний шлях
        string fullPath = "/Users/mac/Desktop/Labs/VIPZ/Lab0/Lab0/";
        fullPath += szFileName;

        while (continueAdding == 'y') {
            addStudent(&pstHead);
            cout << "Додати ще одного студента? (y/n): ";
            cin >> continueAdding;
        }

        // Збереження списку до файлу
        writeStudentsToFile(pstHead, fullPath.c_str());

        cout << "Список успішно збережено у файл \"" << fullPath << "\"!\n";
        cout << "Повний список студентів:\n";
        display(pstHead);

        bool continueActions = true;
        while (continueActions) {
            cout << "\nВиберіть дію:\n1 - Відсортувати список.\n2 - Відфільтрувати список.\n3 - Зберегти зміни до файлу.\n4 - Завершити.\n";
            int action;
            cin >> action;

            if (action == 1) {
                // Сортування списку
                Student* pstSortedHead = sortStudents(pstHead);
                clearList(pstHead); // Очищаємо старий список
                pstHead = pstSortedHead; // Оновлюємо основний список
                cout << "\nВідсортований список студентів:\n";
                display(pstHead);
            } else if (action == 2) {
                // Фільтрація списку
                Student* pstFilteredHead = filterStudents(pstHead);
                clearList(pstHead); // Очищаємо старий список
                pstHead = pstFilteredHead; // Оновлюємо основний список
                cout << "\nВідфільтрований список студентів:\n";
                display(pstHead);
            } else if (action == 3) {
                // Збереження змін у файл
                ofstream ofsFile(fullPath);
                if (!ofsFile.is_open()) {
                    cerr << "Не вдалося відкрити файл для запису.\n";
                } else {
                    writeStudentsToFile(pstHead, fullPath.c_str());
                    cout << "Зміни успішно збережені до файлу \"" << fullPath << "\"!\n";
                    ofsFile.close();
                }
            } else if (action == 4) {
                // Завершення операцій
                continueActions = false;
            } else {
                cout << "Невірний вибір. Спробуйте ще раз.\n";
            }
        }
    }

    // Очищення пам'яті перед завершенням
    clearList(pstHead);
    return 0;
}
