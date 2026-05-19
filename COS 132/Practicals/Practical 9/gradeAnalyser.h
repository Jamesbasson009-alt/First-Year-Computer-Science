#include <string>
#ifndef GRADE_ANALYSER_H
#define GRADE_ANALYSER_H

const int MAX_STUDENTS = 10;
const int MAX_ASSESSMENTS = 3;
void readStudentNames(std::string names[MAX_STUDENTS]);
void readGrades(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], std::string names[MAX_STUDENTS]);
float calculateStudentAverage(int grades[MAX_STUDENTS] [MAX_ASSESSMENTS], int studentIndex);
void sortResults(std::string names[MAX_STUDENTS], float averages[MAX_STUDENTS]);
int findStudent(std::string names[MAX_STUDENTS], std::string searchName);
float getAssessmentAverage(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], int assessmentIndex);
void applyCurve(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], int assessmentIndex, int bonusPoints);
void printAtRiskStudents(std::string names[MAX_STUDENTS], float averages[MAX_STUDENTS], float threshold);
void transposeGrades(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], int transposed[MAX_ASSESSMENTS][MAX_STUDENTS]);
int loadGrades(std::string filename, std::string names[MAX_STUDENTS], int grades[MAX_STUDENTS][MAX_ASSESSMENTS]);
int getTopStudentIndex(float averages[MAX_STUDENTS]);
void printGradeSheet(std::string names[MAX_STUDENTS], int grades[MAX_STUDENTS][MAX_ASSESSMENTS], float averages[MAX_STUDENTS]);

#endif

