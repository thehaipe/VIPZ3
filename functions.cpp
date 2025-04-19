#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "functions.h"

using namespace std;

// Зчитує дані студента з рядка тексту.
// Параметри:
// - szLine: вхідний рядок із даними студента.
// - szFirstName, szLastName: буфери для імені та прізвища студента.
// - szDOB: буфер для дати народження (у форматі: YYYY.MM.DD).
// - anGrades: масив для збереження п'яти оцінок студента.
void parseStudent(const char* szLine, char* szLastName, char* szFirstName, char* szDOB, int* anGrades) {
    sscanf(szLine, "%24s %24s %19s %d %d %d %d %d",
           szLastName,    // Прізвище (перше у файлі)
           szFirstName,   // Ім'я (друге у файлі)
           szDOB,
           &anGrades[0], &anGrades[1], &anGrades[2], &anGrades[3], &anGrades[4]);
}
//--------------------------------------
// Додає нового студента у кінець однозв'язного списку.
// Параметри:
// - ppstHead: вказівник на голову списку студентів.
// - szFirstName, szLastName: ім'я та прізвище студента.
// - szDOB: дата народження студента (у форматі YYYY-MM-DD).
// - anGrades: масив із п'яти оцінок студента.
void append(Student** ppstHead, const char* szLastName, const char* szFirstName, const char* szDOB, const int* anGrades) {
    // Додавання студента в список
    Student* pNewStudent = new Student;
    strncpy(pNewStudent->szLastName, szLastName, sizeof(pNewStudent->szLastName) - 1);
    strncpy(pNewStudent->szFirstName, szFirstName, sizeof(pNewStudent->szFirstName) - 1);
    strncpy(pNewStudent->szDOB, szDOB, sizeof(pNewStudent->szDOB) - 1);
    memcpy(pNewStudent->anGrades, anGrades, 5 * sizeof(int));
    pNewStudent->pstNext = nullptr;

    if (*ppstHead == nullptr) {
        *ppstHead = pNewStudent;
    } else {
        Student* pTemp = *ppstHead;
        while (pTemp->pstNext != nullptr) {
            pTemp = pTemp->pstNext;
        }
        pTemp->pstNext = pNewStudent;
    }
}
//--------------------------------------
// Фільтрує список студентів, виключаючи тих, хто має оцінки 3 або менше.
// Параметри:
// - pstHead: вказівник на голову списку студентів для фільтрації.
// Повертає:
// - Вказівник на голову нового списку, що містить лише студентів без низьких оцінок.
Student* filterStudents(const Student* pstHead) {
    Student* pstFilteredHead = nullptr;
    const Student* pCurrent = pstHead;

    while (pCurrent != nullptr) {
        bool hasLowGrades = false;
        for (int i = 0; i < 5; ++i) {
            if (pCurrent->anGrades[1] == 2 && pCurrent->anGrades[2] == 3 && pCurrent->anGrades[3] == 4 && pCurrent->anGrades[4] == 5) {
                hasLowGrades = true;
                break;
            }
        }
        if (!hasLowGrades) {
            append(&pstFilteredHead, pCurrent->szLastName, pCurrent->szFirstName, pCurrent->szDOB, pCurrent->anGrades);
        }
        pCurrent = pCurrent->pstNext;
    }
    return pstFilteredHead;
}
//--------------------------------------
// Сортує список студентів за прізвищем у алфавітному порядку.
// Параметри:
// - pstHead: вказівник на голову списку студентів, який потрібно відсортувати.
// Повертає:
// - Вказівник на голову нового відсортованого списку студентів.
Student* sortStudents(const Student* pstHead) {
    // Перевіряємо, чи список порожній
    if (!pstHead) return nullptr;

    Student* sortedHead = nullptr;

    // Копіюємо дані зі старого списку до нового
    const Student* current = pstHead;
    while (current != nullptr) {
        append(&sortedHead, current->szLastName, current->szFirstName, current->szDOB, current->anGrades);
        current = current->pstNext;
    }

    // Bubble Sort
    bool swapped;
    do {
        swapped = false;
        Student* pOuter = sortedHead;
        while (pOuter && pOuter->pstNext) {
            // ПОРІВНЮЄМО ПРІЗВИЩА (szLastName) замість імен (szFirstName)
            if (strcmp(pOuter->szLastName, pOuter->pstNext->szLastName) > 0) {
                // Обмін даних між вузлами списку
                char tempFirstName[25], tempLastName[25], tempDOB[20];
                int tempGrades[5];

                // Обмін: Ім'я
                strncpy(tempFirstName, pOuter->szFirstName, sizeof(tempFirstName));
                strncpy(pOuter->szFirstName, pOuter->pstNext->szFirstName, sizeof(pOuter->szFirstName));
                strncpy(pOuter->pstNext->szFirstName, tempFirstName, sizeof(pOuter->pstNext->szFirstName));

                // Обмін: Прізвище
                strncpy(tempLastName, pOuter->szLastName, sizeof(tempLastName));
                strncpy(pOuter->szLastName, pOuter->pstNext->szLastName, sizeof(pOuter->szLastName));
                strncpy(pOuter->pstNext->szLastName, tempLastName, sizeof(pOuter->pstNext->szLastName));

                // Обмін: Дата народження
                strncpy(tempDOB, pOuter->szDOB, sizeof(tempDOB));
                strncpy(pOuter->szDOB, pOuter->pstNext->szDOB, sizeof(pOuter->szDOB));
                strncpy(pOuter->pstNext->szDOB, tempDOB, sizeof(pOuter->pstNext->szDOB));

                // Обмін: Оцінки
                memcpy(tempGrades, pOuter->anGrades, sizeof(tempGrades));
                memcpy(pOuter->anGrades, pOuter->pstNext->anGrades, sizeof(pOuter->anGrades));
                memcpy(pOuter->pstNext->anGrades, tempGrades, sizeof(pOuter->pstNext->anGrades));

                swapped = true; // Вказуємо, що відбулася зміна
            }
            pOuter = pOuter->pstNext;
        }
    } while (swapped);

    return sortedHead;
}
//--------------------------------------
// Виводить список студентів у вигляді таблиці з відформатованими даними.
// Параметри:
// - pstHead: вказівник на голову списку студентів, дані яких потрібно відобразити.
void display(const Student* pstHead) {
    cout << "| " << setw(15) << "Прізвище"
         << " | " << setw(12) << "Імʼя"
         << " | " << setw(12) << "Дата нар."
         << " | " << setw(7) << "Оцінка 1"
         << " | " << setw(7) << "Оцінка 2"
         << " | " << setw(7) << "Оцінка 3"
         << " | " << setw(7) << "Оцінка 4"
         << " | " << setw(7) << "Оцінка 5"
         << " |\n";
    cout << string(105, '-') << "\n";

    const Student* pCurrent = pstHead;
    while (pCurrent != nullptr) {
        cout << "| " << setw(15) << pCurrent->szLastName        // Прізвище перше!
             << " | " << setw(12) << pCurrent->szFirstName      // Ім'я друге
             << " | " << setw(12) << pCurrent->szDOB;          // Дата народження
        for (int i = 0; i < 5; ++i) {
            cout << " | " << setw(7) << pCurrent->anGrades[i];
        }
        cout << " |\n";
        pCurrent = pCurrent->pstNext;
    }
}
//--------------------------------------
// Додає нового студента до списку шляхом введення даних з клавіатури.
// Параметри:
// - ppstHead: вказівник на голову списку, до якого буде доданий студент.
void addStudent(Student** ppstHead) {
    char szFirstName[25], szLastName[25], szDOB[20];
    int anGrades[5];

    cout << "Введіть прізвище: (максимум 25 символів)";
    cin >> szLastName;
    
    cout << "Введіть ім'я: (максимум 25 символів)";
    cin >> szFirstName;

    cout << "Введіть дату народження (дд.мм.рррр): ";
    cin >> szDOB;

    cout << "Введіть 5 оцінок через пробіл: (діапазон від 1 до 5)";
    for (int i = 0; i < 5; ++i) {
        cin >> anGrades[i];
    }

    append(ppstHead, szLastName, szFirstName, szDOB, anGrades);
}
//--------------------------------------
// Видаляє студента з однозв'язного списку за прізвищем та імʼям.
// Параметри:
// - ppstHead: вказівник на голову списку студентів.
// - szLastName: прізвище студента, якого потрібно видалити.
// - szFirstName: імʼя студента, якого потрібно видалити.
// Якщо студент знайдений, видаляє вузол із памʼяті та оновлює звʼязки в списку.
void deleteStudent(Student** ppstHead, const char* szLastName, const char* szFirstName) {
    if (*ppstHead == nullptr) {
        cout << "Список порожній, нічого видаляти.\n";
        return;
    }

    // Перевіряємо перший елемент
    if (strcmp((*ppstHead)->szLastName, szLastName) == 0 && strcmp((*ppstHead)->szFirstName, szFirstName) == 0) {
        Student* pTemp = *ppstHead;
        *ppstHead = (*ppstHead)->pstNext; // Голову змінюємо на наступний вузол
        delete pTemp;
        cout << "Студент " << szFirstName << " " << szLastName << " успішно видалений.\n";
        return;
    }

    // Рухаємося по списку, шукаючи вузол для видалення
    Student* pCurrent = *ppstHead;
    while (pCurrent->pstNext != nullptr) {
        if (strcmp(pCurrent->pstNext->szLastName, szLastName) == 0 && strcmp(pCurrent->pstNext->szFirstName, szFirstName) == 0) {
            // Видаляємо знайдений вузол
            Student* pTemp = pCurrent->pstNext;
            pCurrent->pstNext = pCurrent->pstNext->pstNext; // Пропускаємо вузол
            delete pTemp;
            cout << "Студент " << szFirstName << " " << szLastName << " успішно видалений.\n";
            return;
        }
        pCurrent = pCurrent->pstNext;
    }

    cout << "Студент " << szFirstName << " " << szLastName << " не знайдений у списку.\n";
}
//--------------------------------------
// Очищення всього списку студентів
// Параметри: Вказівник на голову списку який потрібно видалити
void clearList(Student* pstHead) {
    while (pstHead != nullptr) {
        Student* pTemp = pstHead;
        pstHead = pstHead->pstNext;
        delete pTemp;
    }
}
//--------------------------------------
// Записує список студентів у вказаний файл.
// Параметри:
// - pstHead: вказівник на голову списку студентів, дані яких потрібно записати.
// - szFilename: шлях до файлу, куди потрібно записати дані.
// Якщо файл не відкривається для запису, виводить повідомлення про помилку.

void writeStudentsToFile(const Student* pstHead) {
    // Запитуємо ім'я файлу у користувача
    char szFilename[100];
    cout << "Введіть ім'я файлу для збереження (наприклад, students_new.txt): ";
    cin >> szFilename;

    // Формуємо повний шлях (можна адаптувати під вашу структуру папок)
    string fullPath = "/Users/mac/Desktop/Labs/VIPZ/Lab0/Lab0/";
    fullPath += szFilename;

    ofstream outFile(fullPath);
    if (!outFile) {
        cerr << "Помилка відкриття файлу для запису: " << fullPath << endl;
        return;
    }

    const Student* pCurrent = pstHead;
    while (pCurrent != nullptr) {
        outFile << pCurrent->szLastName << " "
                << pCurrent->szFirstName << " "
                << pCurrent->szDOB << " ";
        for (int i = 0; i < 5; ++i) {
            outFile << pCurrent->anGrades[i] << " ";
        }
        outFile << "\n";
        pCurrent = pCurrent->pstNext;
    }

    outFile.close();
    cout << "Дані успішно збережено у файл: " << fullPath << endl;
}
//--------------------------------------
