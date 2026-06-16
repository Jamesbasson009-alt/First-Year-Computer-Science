#include "gradeAnalyser.h"
#include <iostream>
#include <fstream>


void readStudentNames(std::string students[MAX_STUDENTS]) {

    for (int i = 0; i < MAX_STUDENTS; i++) {
        std::cout << "Enter name for student " << i + 1 << ": ";
        std::cin >> students[i];
    }

}

void readGrades(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], std::string names[MAX_STUDENTS]) {

    for (int i = 0; i < MAX_STUDENTS; i++) {

        if (names[i].empty()) {
            for (int j = 0; j < MAX_ASSESSMENTS; j++) {
                grades[i][j] = -1;
            }
            continue;
        }

        std::cout << "Entering data for " << names[i] << ":\n";

        for (int j = 0; j < MAX_ASSESSMENTS; j++) {
            std::cout << "Assessment " << j + 1 << ": ";
            std::cin >> grades[i][j];

        }
    }
}

float calculateStudentAverage(int grades[MAX_STUDENTS] [MAX_ASSESSMENTS], int studentIndex) {

    double weights[MAX_ASSESSMENTS] = {0.20, 0.30, 0.50};

    float weightedTotal = 0.0;
    for (int j = 0; j < MAX_ASSESSMENTS; j++) {
        weightedTotal += static_cast<double>(grades[studentIndex][j]) * weights[j];
    }

    return weightedTotal / 3.0;
    
}

void sortResults(std::string names[MAX_STUDENTS], float averages[MAX_STUDENTS]) {
    for (int i = 0; i < MAX_STUDENTS; i++) {
        
        
        if (averages[i] == -1) break;

        int maxIndex = i;

        for (int j = i + 1; j < MAX_STUDENTS; j++) {
            
           
            if (averages[j] == -1) break;

            if (averages[j] > averages[maxIndex]) {
                maxIndex = j;
            }
        }

        
        float tempAvg = averages[i];
        averages[i] = averages[maxIndex];
        averages[maxIndex] = tempAvg;

        
        std::string tempName = names[i];
        names[i] = names[maxIndex];
        names[maxIndex] = tempName;
    }
}

int findStudent(std::string names[MAX_STUDENTS], std::string searchName) {

    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (names[i] == searchName) {
            return i;
        }
    }
    return -1;
}

float getAssessmentAverage(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], int assessmentIndex) {
    float total = 0;
    int count = 0;

    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (grades[i][assessmentIndex] == -1) continue; 
        
        total += grades[i][assessmentIndex];
        count++;
    }

    return total / count;
}

void applyCurve(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], int assessmentIndex, int bonusPoints) {
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (grades[i][assessmentIndex] == -1) continue;
        grades[i][assessmentIndex] += bonusPoints;

        if (grades[i][assessmentIndex] > 100) {
            grades[i][assessmentIndex] = 100;
        }

    }
}

void printAtRiskStudents(std::string names[MAX_STUDENTS], float averages[MAX_STUDENTS], float threshold) {
    bool found = false;
    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (averages[i] == -1) break;
        if (averages[i] < threshold) {
            std::cout << "- " << names[i] << " (Final: " << averages[i] << "%)\n";
            found = true;
        }
    }

    if(!found) {
        std::cout << "No students are currently at risk.\n";
    }

}

void transposeGrades(int grades[MAX_STUDENTS][MAX_ASSESSMENTS], int transposed[MAX_ASSESSMENTS][MAX_STUDENTS]) {
    for (int i = 0; i < MAX_STUDENTS; i++) {
        for (int j = 0; j < MAX_ASSESSMENTS; j++) {
            transposed[j][i] = grades[i][j];
        }
    }
}

int loadGrades(std::string filename, std::string names[MAX_STUDENTS], int grades[MAX_STUDENTS][MAX_ASSESSMENTS]) {
    std::ifstream file(filename.c_str());
    int count = 0;

    while (count < MAX_STUDENTS && file >> names[count]) {
        for (int j = 0; j < MAX_ASSESSMENTS; j++) {
            file >> grades[count][j];
        }
        count++;
    }

    file.close();
    return count;
}

int getTopStudentIndex(float averages[MAX_STUDENTS]) {
    int currentTop = 0;
    for (int i = 1; i < MAX_STUDENTS; i++) {
        if (averages[i] > averages[currentTop]) {
            currentTop = i;
        }

    }
    return currentTop;

}

void printGradeSheet(std::string names[MAX_STUDENTS], int grades[MAX_STUDENTS][MAX_ASSESSMENTS], float averages[MAX_STUDENTS]) {

    std::cout << "\nStudent\tA1\tA2\tA3\tFinal Avg\n";
    std::cout << "--------------------------------------------------------\n";

    for (int i = 0; i < MAX_STUDENTS; i++) {
        if (averages[i] == -1) break;

        std::cout << names[i] << "\t";

        for (int j = 0; j < MAX_ASSESSMENTS; j++) {
            std::cout << grades[i][j] << "\t";
        }

        std::cout << averages[i] << "%\n";
    }

}
