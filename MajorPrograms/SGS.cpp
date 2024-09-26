#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <fstream>

using namespace std;

// Class to represent a Student
class Student {
public:
    string name;
    vector<double> grades;

    Student(string n) : name(n) {}

    void addGrade(double grade) {
        grades.push_back(grade);
    }

    void removeGrade(int index) {
        if (index >= 0 && index < grades.size()) {
            grades.erase(grades.begin() + index);
            cout << "Grade removed successfully." << endl;
        } else {
            cout << "Invalid index. Grade not removed." << endl;
        }
    }

    double calculateAverage() const {
        if (grades.empty()) return 0.0;
        double sum = 0.0;
        for (double grade : grades) {
            sum += grade;
        }
        return sum / grades.size();
    }

    double calculateHighestGrade() const {
        if (grades.empty()) return 0.0;
        return *max_element(grades.begin(), grades.end());
    }

    double calculateLowestGrade() const {
        if (grades.empty()) return 0.0;
        return *min_element(grades.begin(), grades.end());
    }

    void display() const {
        cout << setw(20) << left << name 
             << setw(10) << left << calculateAverage() << endl;
    }

    void displayGrades() const {
        cout << "Grades for " << name << ": ";
        for (const auto& grade : grades) {
            cout << grade << " ";
        }
        cout << endl;
    }

    void clearGrades() {
        grades.clear();
        cout << "All grades cleared for " << name << endl;
    }

    void displayStatistics() const {
        cout << "Statistics for " << name << ":" << endl;
        cout << "Number of grades: " << grades.size() << endl;
        cout << "Average grade: " << calculateAverage() << endl;
        cout << "Highest grade: " << calculateHighestGrade() << endl;
        cout << "Lowest grade: " << calculateLowestGrade() << endl;
    }

    void updateName(const string& newName) {
        name = newName;
        cout << "Student name updated to: " << name << endl;
    }

    bool isPassing() const {
        return calculateAverage() >= 60.0; // Assuming passing grade is 60
    }
};

// Class to manage students
class GradingSystem {
private:
    vector<Student> students;
    vector<pair<string, double>> lastAction; // to store last action for undo

public:
    void addStudent(const string& name) {
        students.emplace_back(name);
    }

    void recordGrade(const string& name, double grade) {
        for (auto& student : students) {
            if (student.name == name) {
                student.addGrade(grade);
                lastAction.push_back({name, grade}); // record last action
                cout << "Grade recorded for " << name << endl;
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    void removeGrade(const string& name, int index) {
        for (auto& student : students) {
            if (student.name == name) {
                student.removeGrade(index);
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    void updateStudentName(const string& oldName, const string& newName) {
        for (auto& student : students) {
            if (student.name == oldName) {
                student.updateName(newName);
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    void displayStudents() const {
        cout << setw(20) << left << "Student Name"
             << setw(10) << left << "Average" << endl;
        cout << string(30, '-') << endl;
        for (const auto& student : students) {
            student.display();
        }
    }

    void displayAllGrades() const {
        for (const auto& student : students) {
            student.displayGrades();
        }
    }

    void clearAllGrades(const string& name) {
        for (auto& student : students) {
            if (student.name == name) {
                student.clearGrades();
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    void sortStudentsByAverage() {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.calculateAverage() > b.calculateAverage();
        });
        cout << "Students sorted by average grades." << endl;
    }

    void searchStudent(const string& name) const {
        for (const auto& student : students) {
            if (student.name == name) {
                cout << "Student found: " << student.name << endl;
                student.displayStatistics();
                return;
            }
        }
        cout << "Student not found." << endl;
    }

    void undoLastAction() {
        if (lastAction.empty()) {
            cout << "No actions to undo." << endl;
            return;
        }
        
        auto last = lastAction.back();
        lastAction.pop_back();
        
        for (auto& student : students) {
            if (student.name == last.first) {
                student.removeGrade(student.grades.size() - 1); // remove the last grade
                cout << "Last action undone for " << last.first << endl;
                return;
            }
        }
        cout << "Student not found for undo operation." << endl;
    }

    void saveToFile(const string& filename) const {
        ofstream outFile(filename);
        if (outFile) {
            for (const auto& student : students) {
                outFile << student.name << endl;
                for (const auto& grade : student.grades) {
                    outFile << grade << " ";
                }
                outFile << endl;
            }
            cout << "Data saved to " << filename << endl;
        } else {
            cout << "Error opening file for writing." << endl;
        }
    }

    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile) {
            students.clear();
            string name;
            while (getline(inFile, name)) {
                Student student(name);
                double grade;
                while (inFile >> grade) {
                    student.addGrade(grade);
                }
                inFile.clear(); // Clear EOF flag
                inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
                students.push_back(student);
            }
            cout << "Data loaded from " << filename << endl;
        } else {
            cout << "Error opening file for reading." << endl;
        }
    }

    void displayPassingStudents() const {
        cout << "\nPassing Students:" << endl;
        cout << setw(20) << left << "Student Name"
             << setw(10) << left << "Average" << endl;
        cout << string(30, '-') << endl;
        for (const auto& student : students) {
            if (student.isPassing()) {
                student.display();
            }
        }
    }

    void displayFailingStudents() const {
        cout << "\nFailing Students:" << endl;
        cout << setw(20) << left << "Student Name"
             << setw(10) << left << "Average" << endl;
        cout << string(30, '-') << endl;
        for (const auto& student : students) {
            if (!student.isPassing()) {
                student.display();
            }
        }
    }
};

// Function to display the menu and get user choice
int displayMenu() {
    int choice;
    cout << "\nStudent Grading System" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Record Grade" << endl;
    cout << "3. Remove Grade" << endl;
    cout << "4. Update Student Name" << endl;
    cout << "5. Display Students" << endl;
    cout << "6. Display All Grades" << endl;
    cout << "7. Clear All Grades" << endl;
    cout << "8. Sort Students by Average" << endl;
    cout << "9. Search for Student" << endl;
    cout << "10. Undo Last Action" << endl;
    cout << "11. Save Data" << endl;
    cout << "12. Load Data" << endl;
    cout << "13. Display Passing Students" << endl;
    cout << "14. Display Failing Students" << endl;
    cout << "15. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}

// Main function
int main() {
    GradingSystem gradingSystem;
    int choice;

    do {
        choice = displayMenu();

        switch (choice) {
        case 1: {
            string name;
            cout << "Enter student name: ";
            cin >> ws; // Clear input buffer
            getline(cin, name);
            gradingSystem.addStudent(name);
            cout << "Student added successfully." << endl;
            break;
        }
        case 2: {
            string name;
            double grade;
            cout << "Enter student name: ";
            cin >> ws;
            getline(cin, name);
            cout << "Enter grade: ";
            cin >> grade;

            if (cin.fail()) {
                cout << "Invalid grade. Please enter a number." << endl;
                cin.clear(); // Clear the fail state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            } else {
                gradingSystem.recordGrade(name, grade);
            }
            break;
        }
        case 3: {
            string name;
            int index;
            cout << "Enter student name: ";
            cin >> ws;
            getline(cin, name);
            cout << "Enter index of grade to remove (0 for first grade): ";
            cin >> index;

            if (cin.fail()) {
                cout << "Invalid index. Please enter a number." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else {
                gradingSystem.removeGrade(name, index);
            }
            break;
        }
        case 4: {
            string oldName, newName;
            cout << "Enter current student name: ";
            cin >> ws;
            getline(cin, oldName);
            cout << "Enter new student name: ";
            cin >> ws;
            getline(cin, newName);
            gradingSystem.updateStudentName(oldName, newName);
            break;
        }
        case 5:
            gradingSystem.displayStudents();
            break;
        case 6:
            gradingSystem.displayAllGrades();
            break;
        case 7: {
            string name;
            cout << "Enter student name to clear grades: ";
            cin >> ws;
            getline(cin, name);
            gradingSystem.clearAllGrades(name);
            break;
        }
        case 8:
            gradingSystem.sortStudentsByAverage();
            break;
        case 9: {
            string name;
            cout << "Enter student name to search: ";
            cin >> ws;
            getline(cin, name);
            gradingSystem.searchStudent(name);
            break;
        }
        case 10:
            gradingSystem.undoLastAction();
            break;
        case 11: {
            string filename;
            cout << "Enter filename to save data: ";
            cin >> ws;
            getline(cin, filename);
            gradingSystem.saveToFile(filename);
            break;
        }
        case 12: {
            string filename;
            cout << "Enter filename to load data: ";
            cin >> ws;
            getline(cin, filename);
            gradingSystem.loadFromFile(filename);
            break;
        }
        case 13:
            gradingSystem.displayPassingStudents();
            break;
        case 14:
            gradingSystem.displayFailingStudents();
            break;
        case 15: {
            char confirm;
            cout << "Are you sure you want to exit? (y/n): ";
            cin >> confirm;
            if (confirm == 'y' || confirm == 'Y') {
                cout << "Exiting..." << endl;
            } else {
                choice = 0; // Reset choice to continue
            }
            break;
        }
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 15);

    return 0;
}
