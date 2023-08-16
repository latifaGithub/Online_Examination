#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

struct Question {
    string question;
    vector<string> options;
    char correctOption;
};

bool signIn(const string& userType) {
    string filename;
    if (userType == "Student") {
        filename = "Scredentials.txt";
    } else if (userType == "Teacher") {
        filename = "Tcredentials.txt";
    } else if (userType == "Administrator") {
        filename = "Acredentials.txt";
        string storedPasscode;
        ifstream passcodeFile("Acredentials.txt");
        passcodeFile >> storedPasscode;
        passcodeFile.close();
        
        string enteredPasscode;
        cout << "Enter 4-digit passcode: ";
        cin >> enteredPasscode;
        
        if (enteredPasscode == storedPasscode) {
            cout << "Access granted as Administrator." << endl;
            return true;
        } else {
            cout << "Access denied." << endl;
            return false;
        }
    } else {
        cout << "Invalid user type." << endl;
        return false;
    }

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream credentialsFile(filename.c_str());
    string storedUsername, storedPassword;

    while (credentialsFile >> storedUsername >> storedPassword) {
        if (storedUsername == username && storedPassword == password) {
            cout << "Access granted as " << userType << "." << endl;
            credentialsFile.close();
            return true;
        }
    }

    cout << "Access denied." << endl;
    credentialsFile.close();
    return false;
}

vector<Question> loadQuestions(const string& fileName) {
    vector<Question> questions;
    ifstream file(fileName.c_str());

    if (!file) {
        cerr << "Error opening " << fileName << endl;
        return questions;
    }

    string line;
    while (getline(file, line)) {
        Question q;
        q.question = line;
        for (int i = 0; i < 4; ++i) {
            getline(file, line);
            q.options.push_back(line);
        }
        file >> q.correctOption;
        file.ignore(); // Ignore newline
        questions.push_back(q);
    }

    file.close();
    return questions;
}

char askQuestion(const Question& question) {
    cout << question.question << endl;
    for (int i = 0; i < 4; ++i) {
        cout << static_cast<char>('a' + i) << ") " << question.options[i] << endl;
    }

    char userAnswer;
    cout << "Your answer: ";
    cin >> userAnswer;

    return tolower(userAnswer);
}

int calculateScore(const vector<char>& userAnswers, const vector<char>& correctAnswers) {
    int score = 0;
    for (size_t i = 0; i < userAnswers.size(); ++i) {
        if (userAnswers[i] == correctAnswers[i]) {
            score += 10;
        }
    }
    return score;
}

void saveUserAnswers(const string& fileName, const vector<char>& userAnswers) {
    ofstream userAnswersFile(fileName.c_str(), ios::app);
    for (size_t i = 0; i < userAnswers.size(); ++i) {
        userAnswersFile << userAnswers[i] << endl;
    }
    userAnswersFile.close();
}


void takeFillExam(const string& studentName) {
    ifstream fillQuestionsFile("Fquestions.txt.c_str()");
    ifstream fillAnswersFile("Fanswers.txt.c_str()");

    vector<string> fillQuestions;
    string question;
    while (getline(fillQuestionsFile, question)) {
        fillQuestions.push_back(question);
    }
    fillQuestionsFile.close();

    vector<char> fillAnswers;
    char answer;
    while (fillAnswersFile >> answer) {
        fillAnswers.push_back(answer);
    }
    fillAnswersFile.close();

    vector<char> userAnswers;

    cout << "Fill Exam: Answer the following questions:" << endl;

    cin.ignore(); // Clear the newline left in the buffer

    for (size_t i = 0; i < fillQuestions.size(); ++i) {
        cout << "Question " << (i + 1) << ": " << fillQuestions[i] << endl;
        cout << "Your answer: ";
        string userAnswer; // Use string to handle sentences as answers
        getline(cin, userAnswer);
        userAnswers.push_back(userAnswer.empty() ? ' ' : userAnswer[0]);
    }

    int fillScore = calculateScore(userAnswers, fillAnswers);
    cout << "Your score for the Fill exam: " << fillScore << " out of " << (10 * fillQuestions.size()) << endl;

    saveUserAnswers("Fuanswers.txt", userAnswers);
    ofstream scoreFile("Score.txt", ios::app);
    scoreFile <<studentName<< " -Fill Exam Score: " << fillScore << " out of " << (10 * fillQuestions.size()) << endl;
    scoreFile.close();
}

void teacherCreateTest() {
    ofstream fillQuestionsFile("Fquestions.txt", ios::app);
    ofstream fillAnswersFile("Fanswers.txt", ios::app);

    cin.ignore(); // Clear the newline left in the buffer

    cout << "Teacher: Create Fill Exam Questions" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << "Enter question " << (i + 1) << ": ";
        string question;
        getline(cin, question);
        fillQuestionsFile << question << endl;

        cout << "Enter correct answer for question " << (i + 1) << ": ";
        char answer;
        cin >> answer;
        cin.ignore(); // Clear the newline left in the buffer
        fillAnswersFile << answer << endl;
    }

    fillQuestionsFile.close();
    fillAnswersFile.close();
}
void displayStudentScores() {
    ifstream scoreFile("Score.txt");
    string line;
    while (getline(scoreFile, line)) {
        cout << line << endl;
    }
    scoreFile.close();
}

void displayExamQuestions() {
    cout << "Teacher: Displaying Exam Questions" << endl;
    
    ifstream multiQuestionsFile("Multiquestions.txt");
    cout << "Multichoice Exam Questions:" << endl;
    string line;
    while (getline(multiQuestionsFile, line)) {
        cout << line << endl;
    }
    multiQuestionsFile.close();
    
    ifstream fillQuestionsFile("Fquestions.txt");
    cout << "Fill Exam Questions:" << endl;
    while (getline(fillQuestionsFile, line)) {
        cout << line << endl;
    }
    fillQuestionsFile.close();
}


int main() {
    int choice;
    cout << "Welcome To The Exams Portal!" << endl;
    cout << "1. Sign up" << endl;
    cout << "2. Sign in" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        string userType;
    cout << "Select user type: Student, Teacher, Administrator" << endl;
    cin >> userType;

    string filename;
    if (userType == "Student") {
        filename = "Scredentials.txt";
    } else if (userType == "Teacher") {
        filename = "Tcredentials.txt";
    } else if (userType == "Administrator") {
        filename = "Acredentials.txt";
    } else {
        cout << "Invalid user type." << endl;
        return 1; // Exit the program with an error status
    }

    ofstream credentialsFile(filename.c_str(), ios::app);
    if (!credentialsFile.is_open()) {
        cout << "Error opening file." << endl;
        return 1; // Exit the program with an error status
    }

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    credentialsFile << username << " " << password << endl;
    credentialsFile.close();

    cout << "Credentials saved successfully." << endl;
    } else if (choice == 2) {
        int signInChoice;
        cout << "Select user type to sign in:" << endl;
        cout << "1. Student" << endl;
        cout << "2. Teacher" << endl;
        cout << "3. Administrator" << endl;
        cout << "Enter your choice: ";
        cin >> signInChoice;

        string userType;
        switch (signInChoice) {
            case 1:
                userType = "Student";
                break;
            case 2:
                userType = "Teacher";
                break;
            case 3:
                userType = "Administrator";
                break;
            default:
                cout << "Invalid choice." << endl;
                return 0;
        }

        if (signIn(userType)) {
            if (userType == "Student") {
                string studentName;
                cout << "Enter your name: ";
                cin.ignore(); // Clear the newline left in the buffer
                getline(cin, studentName);

                cout << "Hello, " << studentName << "! Select the type of exam:" << endl;
                cout << "1. Multichoice" << endl;
                cout << "2. Fill" << endl;
                cout << "Enter your choice: ";
                int examChoice;
                cin >> examChoice;

                if (examChoice == 1) {
                    vector<Question> allQuestions = loadQuestions("Multiquestions.txt");
                    srand(time(NULL));

                    random_shuffle(allQuestions.begin(), allQuestions.end());

                    vector<Question> selectedQuestions(allQuestions.begin(), allQuestions.begin() + 10);

                    vector<char> userAnswers;

                    for (size_t i = 0; i < selectedQuestions.size(); ++i) {
                        char answer = askQuestion(selectedQuestions[i]);
                        userAnswers.push_back(answer);
                    }

                    ifstream correctAnswersFile("Canswers.txt");
                    vector<char> correctAnswers;

                    char answer;
                    while (correctAnswersFile >> answer) {
                        correctAnswers.push_back(answer);
                    }

                    correctAnswersFile.close();

                    int score = calculateScore(userAnswers, correctAnswers);
                    cout << "Your score: " << score << " out of " << (10 * selectedQuestions.size()) << endl;

                    saveUserAnswers("Uanswers.txt", userAnswers);
                } else if (examChoice == 2) {
                    takeFillExam(studentName);
                } else {
                    cout << "Invalid choice." << endl;
                }
            } else if (userType == "Teacher") {
                cout << "Do you want to create a test? (y/n): ";
                char createTestChoice;
                cin >> createTestChoice;

                if (createTestChoice == 'y' || createTestChoice == 'Y') {
                    ofstream fillQuestionsFile("Fquestions.txt", ios::app);
                    ofstream fillAnswersFile("Fanswers.txt", ios::app);

                    for (int i = 0; i < 10; ++i) {
                        cout << "Enter question " << (i + 1) << ": ";
                        string question;
                        cin.ignore();
                        getline(cin, question);
                        fillQuestionsFile << question << endl;

                        cout << "Enter correct answer for question " << (i + 1) << ": ";
                        char correctAnswer;
                        cin >> correctAnswer;
                        fillAnswersFile << correctAnswer << endl;
                    }

                    fillQuestionsFile.close();
                    fillAnswersFile.close();

                    cout << "Test questions and answers saved successfully." << endl;
                }
            }
            else if (userType == "Administrator") {
                cout << "Administrator options:" << endl;
                cout << "1. Student Scores" << endl;
                cout << "2. Exam Questions (Teacher)" << endl;
                cout << "Enter your choice: ";
                int adminChoice;
                cin >> adminChoice;

                switch (adminChoice) {
                    case 1:
                        displayStudentScores();
                        break;
                    case 2:
                        displayExamQuestions();
                        break;
                    default:
                        cout << "Invalid choice." << endl;
                }
            }
            
        }
        
    } else {
        cout << "Invalid choice." << endl;
    }

    return 0;
}
