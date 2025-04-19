#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct Student {
    char szLastName[25]; //було 25
    char szFirstName[25]; //було 25
    char szDOB[20];
    int anGrades[5];
    Student* pstNext;
};

void parseStudent(const char* szLine, char* szLastName, char* szFirstName, char* szDOB, int* anGrades);
void append(Student** ppstHead, const char* szLastName, const char* szFirstName, const char* szDOB, const int* anGrades);
Student* filterStudents(const Student* pstHead);
Student* sortStudents(const Student* pstHead);
void display(const Student* pstHead);
void addStudent(Student** ppstHead);
void deleteStudent(Student** ppstHead, const char* szLastName, const char* szFirstName);
void clearList(Student* pstHead);
void writeStudentsToFile(const Student* pstHead);

#endif
