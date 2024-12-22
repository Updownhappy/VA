#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

class Assistant {
private:
    string name;
    vector<string> tasks;

    // Helper function to trim whitespace
    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t");
        return str.substr(first, (last - first + 1));
    }

public:
    Assistant(const string& name) : name(name) {
        loadTasksFromFile(); // Automatically load tasks when Assistant is initialized
    }

    string greet() const {
        return "Hello, I'm " + name + "! How can I assist you today?";
    }

    void addTask(const string& task) {
        string trimmedTask = trim(task);
        if (!trimmedTask.empty()) {
            tasks.push_back(trimmedTask);
            cout << "Task '" << trimmedTask << "' added!" << endl;
        } else {
            cout << "Task cannot be empty!" << endl;
        }
    }

    void deleteTask(const string& task) {
        string trimmedTask = trim(task);
        auto it = find(tasks.begin(), tasks.end(), trimmedTask);
        if (it != tasks.end()) {
            tasks.erase(it);
            cout << "Task '" << trimmedTask << "' deleted!" << endl;
        } else {
            cout << "Task not found!" << endl;
        }
    }

    void showTasks() const {
        if (tasks.empty()) {
            cout << "No tasks to show." << endl;
        } else {
            cout << "Tasks:" << endl;
            for (size_t i = 0; i < tasks.size(); i++) {
                cout << i + 1 << ". " << tasks[i] << endl;
            }
        }
    }

    void saveTasksToFile() const {
        ofstream file("tasks.txt");
        if (!file.is_open()) {
            cerr << "Error: Could not save tasks to file!" << endl;
            return;
        }
        for (const auto& task : tasks) {
            file << task << endl;
        }
        file.close();
        cout << "Tasks saved to file." << endl;
    }

    void loadTasksFromFile() {
        ifstream file("tasks.txt");
        if (!file.is_open()) {
            cout << "No saved tasks found." << endl;
            return;
        }
        string task;
        while (getline(file, task)) {
            tasks.push_back(task);
        }
        file.close();
        if (!tasks.empty()) {
            cout << "Tasks loaded from file." << endl;
        }
    }

    string getCurrentTime() const {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        stringstream ss;
        ss << "Date: " << 1900 + ltm->tm_year << "-" 
           << 1 + ltm->tm_mon << "-" 
           << ltm->tm_mday << ", Time: " 
           << ltm->tm_hour << ":" 
           << ltm->tm_min << ":" 
           << ltm->tm_sec;
        return ss.str();
    }

    void clearTasks() {
        tasks.clear();
        ofstream file("tasks.txt", ofstream::out | ofstream::trunc);
        file.close();
        cout << "All tasks cleared." << endl;
    }

    string respond(const string& query) {
        string trimmedQuery = trim(query);
        string lowerQuery = trimmedQuery;
        transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(), ::tolower);

        if (lowerQuery.find("weather") != string::npos) {
            return "I can't check the weather, but it looks sunny!";
        } else if (lowerQuery.find("who are you") != string::npos) {
            return "I'm Eve, your personal assistant!";
        } else if (lowerQuery.find("calculate") != string::npos) {
            return "I can calculate simple things for now!";
        } else if (lowerQuery.find("hello") != string::npos || lowerQuery.find("hi") != string::npos) {
            return greet();
        } else if (lowerQuery.find("add task") != string::npos) {
            string task = trimmedQuery.substr(trimmedQuery.find("add task") + 9);
            addTask(task);
            return "Task added!";
        } else if (lowerQuery.find("delete task") != string::npos) {
            string task = trimmedQuery.substr(trimmedQuery.find("delete task") + 12);
            deleteTask(task);
            return "Task deleted!";
        } else if (lowerQuery.find("what time is it?") != string::npos || lowerQuery.find("what's the date?") != string::npos) {
            return getCurrentTime();
        } else if (lowerQuery.find("clear all tasks") != string::npos) {
            clearTasks();
            return "All tasks cleared!";
        } else if (lowerQuery.find("tasks") != string::npos) {
            showTasks();
            return "";
        } else {
            return "I'm not sure how to respond to that.";
        }
    }
};

void showHelp() {
    cout << "Available commands:" << endl;
    cout << "- add task [task_name]" << endl;
    cout << "- delete task [task_name]" << endl;
    cout << "- show tasks" << endl;
    cout << "- clear all tasks" << endl;
    cout << "- what time is it?" << endl;
    cout << "- who are you?" << endl;
    cout << "- exit" << endl;
    cout << "- help" << endl;
}

int main() {
    Assistant assistant("Eve");
    string user_input;

    cout << assistant.greet() << endl;
    showHelp();

    while (true) {
        cout << "You: ";
        getline(cin, user_input);
        if (user_input == "exit") {
            assistant.saveTasksToFile(); // Save tasks before exiting
            break;
        } else if (user_input == "help") {
            showHelp();
        } else {
            string response = assistant.respond(user_input);
            if (!response.empty()) {
                cout << "Assistant: " << response << endl;
            }
        }
    }

    return 0;
}
