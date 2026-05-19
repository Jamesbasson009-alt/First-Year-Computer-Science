#include <iostream>
#include <string>
#include "gradeAnalyser.h"


int main() {
    std::string names[MAX_STUDENTS];
    int grades[MAX_STUDENTS][MAX_ASSESSMENTS];
    float averages[MAX_STUDENTS];

    for (int i = 0; i < MAX_STUDENTS; i++) {
        averages[i] = -1;
    }

    int studentCount = loadGrades("test_grades.txt", names, grades);
    std::cout << "Loaded " << studentCount << " students from file.\n";

    for (int i = 0; i < studentCount; i++) {
        averages[i] = calculateStudentAverage(grades, i);
    }

    printGradeSheet(names, grades, averages);

    int index = findStudent(names, "Alice");
    if (index != -1) {
        std::cout << "Found Alice at index " << index << "\n";
    } else {
        std::cout << "Alice not found.\n";
    }

    for (int j = 0; j < MAX_ASSESSMENTS; j++) {
        std::cout << "Assessment " << j + 1 << " class average: " << getAssessmentAverage(grades, j) << "\n";
    }

    applyCurve(grades, 0, 5);
    for (int i = 0; i < studentCount; i++) {
        averages[i] = calculateStudentAverage(grades, i);
    }
    printGradeSheet(names, grades, averages);

    sortResults(names, averages);
    printGradeSheet(names, grades, averages);

    int topIndex = getTopStudentIndex(averages);
    std::cout << "Top student: " << names[topIndex] << " with " << averages[topIndex] << "%\n";

    printAtRiskStudents(names, averages, 60.0f);

    int transposed[MAX_ASSESSMENTS][MAX_STUDENTS];
    transposeGrades(grades, transposed);
    for (int j = 0; j < MAX_ASSESSMENTS; j++) {
        std::cout << "Assessment " << j + 1 << ": ";
        for (int i = 0; i < studentCount; i++) {
            std::cout << transposed[j][i] << " ";
        }
        std::cout << "\n";
    }

    return 0;
}