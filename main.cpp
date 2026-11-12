             

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>

using namespace std;

const string SCORE_FILE = "scores.txt";

class Question {
private:
    string text;
    vector<string> options;
    int correctOption;

public:
    Question(const string& t, const vector<string>& opts, int correct)
        : text(t), options(opts), correctOption(correct) {}

    void display(int number) const {
        cout << "\nQ" << number << ". " << text << "\n";
        for (size_t i = 0; i < options.size(); i++) {
            cout << "   " << (i + 1) << ") " << options[i] << "\n";
        }
    }

    int getOptionCount() const { return static_cast<int>(options.size()); }

    bool isCorrect(int choice) const { return choice == correctOption; }

    string getCorrectAnswerText() const { return options[correctOption - 1]; }
};


class Quiz {
private:
    vector<Question> questions;
    string playerName;
    int score;

    void loadQuestions() {
        questions.push_back(Question(
            "Which OOP concept binds data and functions together?",
            {"Inheritance", "Polymorphism", "Encapsulation", "Abstraction"}, 3));
        questions.push_back(Question(
            "Which operator accesses a member through a pointer to an object?",
            {".", "->", "::", "&"}, 2));
        questions.push_back(Question(
            "What is the return type of a constructor?",
            {"void", "int", "Same as the class", "It has no return type"}, 4));
        questions.push_back(Question(
            "Which keyword is used to achieve runtime polymorphism?",
            {"static", "virtual", "inline", "const"}, 2));
        questions.push_back(Question(
            "Which header file is required for file handling?",
            {"<fstream>", "<iostream>", "<string>", "<cstdio>"}, 1));
        questions.push_back(Question(
            "Which access specifier allows access only inside the class?",
            {"public", "protected", "private", "friend"}, 3));
        questions.push_back(Question(
            "Which block handles an exception in C++?",
            {"try", "throw", "catch", "finally"}, 3));
        questions.push_back(Question(
            "What is the index of the first element of an array?",
            {"0", "1", "-1", "Depends on the compiler"}, 1));
    }

    
    int readChoice(int maxOption) const {
        string input;
        if (!getline(cin, input)) {
            throw runtime_error("Input stream closed unexpectedly.");
        }

        size_t pos = 0;
        int choice = stoi(input, &pos);         
        if (pos != input.size()) {
            throw invalid_argument("Extra characters after the number.");
        }
        if (choice < 1 || choice > maxOption) {
            throw out_of_range("Choice outside the allowed range.");
        }
        return choice;
    }

    
    void askQuestion(const Question& q, int number) {
        q.display(number);

        int choice = 0;
        bool valid = false;
        while (!valid) {
            cout << "Your answer (1-" << q.getOptionCount() << "): ";
            try {
                choice = readChoice(q.getOptionCount());
                valid = true;
            } catch (const invalid_argument&) {
                cout << "Invalid input. Please enter a number.\n";
            } catch (const out_of_range&) {
                cout << "Choice out of range. Select between 1 and "
                     << q.getOptionCount() << ".\n";
            }
        }

        if (q.isCorrect(choice)) {
            cout << "Correct!\n";
            score++;
        } else {
            cout << "Incorrect. Correct answer: " << q.getCorrectAnswerText() << "\n";
        }
    }

public:
    Quiz() : score(0) { loadQuestions(); }

    void askPlayerName() {
        do {
            cout << "Enter your name: ";
            if (!getline(cin, playerName)) {
                throw runtime_error("Input stream closed unexpectedly.");
            }
        } while (playerName.empty());
    }

    void start() {
        score = 0;
        for (size_t i = 0; i < questions.size(); i++) {
            askQuestion(questions[i], static_cast<int>(i) + 1);
        }
    }

    int getTotal() const { return static_cast<int>(questions.size()); }

    double getPercentage() const {
        return (static_cast<double>(score) / getTotal()) * 100.0;
    }

    void showResult() const {
        cout << "\n========== RESULT ==========\n";
        cout << "Name       : " << playerName << "\n";
        cout << "Score      : " << score << "/" << getTotal() << "\n";
        cout << "Percentage : " << fixed << setprecision(2) << getPercentage() << "%\n";
        cout << "============================\n";
    }

    void saveResult() const {
        ofstream file(SCORE_FILE, ios::app);     
        if (!file) {
            throw runtime_error("Could not open " + SCORE_FILE + " for writing.");
        }
        file << playerName << " | " << score << "/" << getTotal()
             << " | " << fixed << setprecision(2) << getPercentage() << "%\n";
        cout << "Result saved to " << SCORE_FILE << ".\n";
    }
};

void viewSavedScores() {
    ifstream file(SCORE_FILE);
    if (!file) {
        cout << "\nNo saved scores found yet.\n";
        return;
    }
    cout << "\n------ Saved Scores ------\n";
    string line;
    while (getline(file, line)) {
        cout << line << "\n";
    }
    cout << "--------------------------\n";
}


int main() {
    cout << "=== Console Quiz Application ===\n";

    try {
        string option;
        while (true) {
            cout << "\n1) Start Quiz\n2) View Saved Scores\n3) Exit\nChoose: ";
            if (!getline(cin, option)) break;

            if (option == "1") {
                Quiz quiz;
                quiz.askPlayerName();
                quiz.start();
                quiz.showResult();
                quiz.saveResult();
            } else if (option == "2") {
                viewSavedScores();
            } else if (option == "3") {
                cout << "Goodbye!\n";
                break;
            } else {
                cout << "Invalid option. Enter 1, 2 or 3.\n";
            }
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
