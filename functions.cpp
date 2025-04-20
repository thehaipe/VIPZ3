#include <iostream>
#include <cstring>
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include <ctime>
#include "functions.h"

using namespace std;
//Нова функція, перевірка імʼя на коректність.
bool isValidName(const char* name) {
    for (size_t i = 0; i < strlen(name); ++i) {
        if (!isalpha(name[i]) && name[i] != '-' && name[i] != '\'') {
            return false;
        }
    }
    return true;
}
//Нова функція, перевірка дати на коректніть, також враховано високосний рік.
bool isValidDate(const char* date) {
    int day, month, year;
    if (sscanf(date, "%d.%d.%d", &day, &month, &year) != 3) {
        return false;
    }

    time_t now = time(0);
    tm* ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    
    if (year < currentYear - 60 || year > currentYear - 15) {
        return false;
    }

    if (month < 1 || month > 12) {
        return false;
    }

    if (day < 1 || day > 31) {
        return false;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        return false;
    }

    if (month == 2) {
        bool isLeap = (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
        if (day > (isLeap ? 29 : 28)) {
            return false;
        }
    }

    return true;
}
//Нова функція, перевірка дати на коректність
bool isValidGrade(int grade) {
    return grade >= 1 && grade <= 5;
}
// Зчитує дані студента з рядка тексту.
// Параметри:
// - szLine: вхідний рядок із даними студента.
// - szFirstName, szLastName: буфери для імені та прізвища студента.
// - szDOB: буфер для дати народження (у форматі: YYYY.MM.DD).
// - anGrades: масив для збереження п'яти оцінок студента.
void parseStudent(const char* szLine, char* szLastName, char* szFirstName, char* szDOB, int* anGrades) {
    char tempLastName[151], tempFirstName[151], tempDOB[20];
    int tempGrades[5];
    
    if (sscanf(szLine, "%150s %150s %19s %d %d %d %d %d",
           tempLastName, tempFirstName, tempDOB,
           &tempGrades[0], &tempGrades[1], &tempGrades[2], &tempGrades[3], &tempGrades[4]) != 8) {
        strcpy(szLastName, "wrong data");
        strcpy(szFirstName, "wrong data");
        strcpy(szDOB, "wrong data");
        memset(anGrades, 0, 5 * sizeof(int));
        return;
    }

    strncpy(szLastName, isValidName(tempLastName) ? tempLastName : "wrong data", 150);
    szLastName[150] = '\0';
    
    strncpy(szFirstName, isValidName(tempFirstName) ? tempFirstName : "wrong data", 150);
    szFirstName[150] = '\0';
    
    strncpy(szDOB, isValidDate(tempDOB) ? tempDOB : "wrong data", 19);
    szDOB[19] = '\0';

    for (int i = 0; i < 5; ++i) {
        anGrades[i] = isValidGrade(tempGrades[i]) ? tempGrades[i] : 0;
    }
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
    strncpy(pNewStudent->szLastName, szLastName, 150);
    pNewStudent->szLastName[150] = '\0';
    strncpy(pNewStudent->szFirstName, szFirstName, 150);
    pNewStudent->szFirstName[150] = '\0';
    strncpy(pNewStudent->szDOB, szDOB, 19);
    pNewStudent->szDOB[19] = '\0';
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
    bool anyFiltered = false;

    while (pCurrent != nullptr) {
        bool hasLowGrades = false;
        for (int i = 0; i < 5; ++i) {
            if (pCurrent->anGrades[i] == 2 || pCurrent->anGrades[i] == 3 || pCurrent->anGrades[i] == 4 || pCurrent->anGrades[i] == 5) {
                hasLowGrades = true;
                anyFiltered = true;
                break;
            }
        }
        if (!hasLowGrades) {
            append(&pstFilteredHead, pCurrent->szLastName, pCurrent->szFirstName, pCurrent->szDOB, pCurrent->anGrades);
        }
        pCurrent = pCurrent->pstNext;
    }

    if (!anyFiltered && pstHead != nullptr) {
        cout << "Жодного студента не було відфільтровано - всі мають однакові оцінки.\n";
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
    // Копіюємо дані зі старого списку до нового
    Student* sortedHead = nullptr;
    const Student* current = pstHead;
    while (current != nullptr) {
        append(&sortedHead, current->szLastName, current->szFirstName, current->szDOB, current->anGrades);
        current = current->pstNext;
    }
    // Bubble Sort
    bool swapped;
    bool anySwapped = false;
    do {
        swapped = false;
        Student* pOuter = sortedHead;
        while (pOuter && pOuter->pstNext) {
            // ПОРІВНЮЄМО ПРІЗВИЩА (szLastName) замість імен (szFirstName). ТУТ БУВ КОСТИЛЬ.
            if (strcmp(pOuter->szLastName, pOuter->pstNext->szLastName) > 0) {
                // Обмін даних між вузлами списку. Спочатку пробував міняти вузли. Дітки, ніколи так не робіть, ДУЖЕ ЛЕГКО ЗГУБИТИ ВКАЗІВНИКИ.
                swap(pOuter->szFirstName, pOuter->pstNext->szFirstName);
                swap(pOuter->szLastName, pOuter->pstNext->szLastName);
                swap(pOuter->szDOB, pOuter->pstNext->szDOB);
                swap(pOuter->anGrades[0], pOuter->pstNext->anGrades[0]);
                swap(pOuter->anGrades[1], pOuter->pstNext->anGrades[1]);
                swap(pOuter->anGrades[2], pOuter->pstNext->anGrades[2]);
                swap(pOuter->anGrades[3], pOuter->pstNext->anGrades[3]);
                swap(pOuter->anGrades[4], pOuter->pstNext->anGrades[4]);
                swapped = true;
                anySwapped = true;
            }
            pOuter = pOuter->pstNext;
        }
    } while (swapped);

    if (!anySwapped && pstHead != nullptr) {
        cout << "Список вже відсортований - змін не відбулося.\n";
    }
    return sortedHead;
}
//--------------------------------------
// Виводить список студентів у вигляді таблиці з відформатованими даними.
// Параметри:
// - pstHead: вказівник на голову списку студентів, дані яких потрібно відобразити.
void display(const Student* pstHead) {
    if (!pstHead) {
        cout << "Список студентів порожній.\n";
        return;
    }

    cout << "| " << setw(30) << "Прізвище"
         << " | " << setw(30) << "Імʼя"
         << " | " << setw(12) << "Дата нар."
         << " | " << setw(7) << "Оцінка 1"
         << " | " << setw(7) << "Оцінка 2"
         << " | " << setw(7) << "Оцінка 3"
         << " | " << setw(7) << "Оцінка 4"
         << " | " << setw(7) << "Оцінка 5"
         << " |\n";
    cout << string(135, '-') << "\n";

    const Student* pCurrent = pstHead;
    while (pCurrent != nullptr) {
        cout << "| " << setw(30) << pCurrent->szLastName
             << " | " << setw(30) << pCurrent->szFirstName
             << " | " << setw(12) << pCurrent->szDOB;
        
        for (int i = 0; i < 5; ++i) {
            if (pCurrent->anGrades[i] == 0) {
                cout << " | " << setw(7) << "wrong data";
            } else {
                cout << " | " << setw(7) << pCurrent->anGrades[i];
            }
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
    char szFirstName[151], szLastName[151], szDOB[20];
    int anGrades[5];

    cout << "Введіть прізвище (до 150 символів): ";
    cin.ignore();
    cin.getline(szLastName, 151);
    
    cout << "Введіть ім'я (до 150 символів): ";
    cin.getline(szFirstName, 151);

    cout << "Введіть дату народження (дд.мм.рррр): ";
    cin >> szDOB;

    for (int i = 0; i < 5; ++i) {
        do {
            cout << "Введіть оцінку " << i+1 << " (1-5): ";
            cin >> anGrades[i];
            if (!isValidGrade(anGrades[i])) {
                cout << "Невірна оцінка! Спробуйте ще раз.\n";
                anGrades[i] = 0;
            }
        } while (anGrades[i] < 1 || anGrades[i] > 5);
    }

    if (!isValidName(szLastName)) strcpy(szLastName, "wrong data");
    if (!isValidName(szFirstName)) strcpy(szFirstName, "wrong data");
    if (!isValidDate(szDOB)) strcpy(szDOB, "wrong data");

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
        if (strcmp(pCurrent->pstNext->szLastName, szLastName) == 0 &&
            strcmp(pCurrent->pstNext->szFirstName, szFirstName) == 0) {
            Student* pTemp = pCurrent->pstNext;
            // Видаляємо знайдений вузол
            pCurrent->pstNext = pCurrent->pstNext->pstNext;
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
