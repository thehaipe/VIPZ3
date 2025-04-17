#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// Структура для представлення студента
struct Student {
    char szFirstName[25];  // Ім'я
    char szLastName[25];   // Прізвище
    char szDOB[20];        // Дата народження
    int anGrades[5];       // Оцінки
    Student* pstNext;      // Вказівник на наступного студента
};

// Прототипи функцій
void parseStudent(const char* szLine, char* szFirstName, char* szLastName, char* szDOB, int* anGrades);
void append(Student**, const char* szLastName, const char* szFirstName, const char* szDOB, const int* anGrades);
Student* filterStudents(const Student* pstHead);
Student* sortStudents(const Student* pstHead);
void display(const Student* pstHead);
void addStudent(Student** ppstHead);
void deleteStudent(Student** ppstHead);
void clearList(Student* pstHead);
void writeStudentsToFile(const Student* pstHead, const char* szFilename);

#endif
