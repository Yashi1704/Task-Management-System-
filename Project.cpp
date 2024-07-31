#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <random>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

// Function to generate a random salt
std::string generateSalt(size_t length = 16) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::string salt;
    for (size_t i = 0; i < length; ++i) {
        salt += static_cast<char>(dis(gen));
    }
    return salt;
}

// Function to hash a password with a given salt
std::string hashPassword(const std::string& password, const std::string& salt) {
    std::string salted_password = password + salt;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(salted_password.c_str()), salted_password.size(), hash);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

// User class definition
class User {
private:
    int user_id;
    std::string username;
    std::string password;
    std::string email;
    std::string salt;

public:
    User(int user_id, const std::string& username, const std::string& password, const std::string& email)
        : user_id(user_id), username(username), email(email) {
        salt = generateSalt();
        this->password = hashPassword(password, salt);
    }

    std::unordered_map<std::string, std::string> getUserInfo() const {
        return {{"User ID", std::to_string(user_id)}, {"Username", username}, {"Email", email}};
    }
};

// Task class definition
class Task {
public:
    int task_id;
    int user_id;
    std::string task_description;

    Task(int task_id, int user_id, const std::string& task_description)
        : task_id(task_id), user_id(user_id), task_description(task_description) {}
};

// TaskStatus class definition
class TaskStatus {
public:
    int task_id;
    std::string status;

    TaskStatus(int task_id, const std::string& status)
        : task_id(task_id), status(status) {}
};

// TaskDetails class definition
class TaskDetails {
public:
    int task_id;
    std::string due_date;
    std::string priority;
    std::string archived;

    TaskDetails(int task_id, const std::string& due_date, const std::string& priority, const std::string& archived)
        : task_id(task_id), due_date(due_date), priority(priority), archived(archived) {}
};

int main() {
    int user_id;
    std::string username, password, email;

    std::cout << "Enter User ID: ";
    std::cin >> user_id;
    std::cin.ignore(); // Clear newline character from the input buffer
    std::cout << "Enter Username: ";
    std::getline(std::cin, username);
    std::cout << "Enter Password: ";
    std::getline(std::cin, password);
    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    User user(user_id, username, password, email);

    std::vector<Task> tasks;
    int num_tasks;
    std::cout << "Enter the number of tasks: ";
    std::cin >> num_tasks;
    std::cin.ignore();

    for (int i = 0; i < num_tasks; ++i) {
        int task_id;
        std::string task_description;
        std::cout << "Enter Task ID: ";
        std::cin >> task_id;
        std::cin.ignore();
        std::cout << "Enter Task Description: ";
        std::getline(std::cin, task_description);

        tasks.emplace_back(task_id, user_id, task_description);
    }

    std::vector<TaskStatus> task_statuses;
    for (const auto& task : tasks) {
        std::string status;
        std::cout << "Enter Task Status for Task ID " << task.task_id << ": ";
        std::getline(std::cin, status);

        task_statuses.emplace_back(task.task_id, status);
    }

    std::vector<TaskDetails> task_details_list;
    for (const auto& task : tasks) {
        std::string due_date, priority, archived;
        std::cout << "Enter Due Date for Task ID " << task.task_id << ": ";
        std::getline(std::cin, due_date);
        std::cout << "Enter Priority for Task ID " << task.task_id << ": ";
        std::getline(std::cin, priority);
        std::cout << "Enter Archived (Yes/No) for Task ID " << task.task_id << ": ";
        std::getline(std::cin, archived);

        task_details_list.emplace_back(task.task_id, due_date, priority, archived);
    }

    // Display User information
    std::cout << "\nUser Information:\n";
    for (const auto& [key, value] : user.getUserInfo()) {
        std::cout << key << ": " << value << "\n";
    }

    // Display Task information for each task in the array
    std::cout << "\nTask Information:\n";
    for (const auto& task : tasks) {
        std::cout << "Task ID: " << task.task_id << "\n";
        std::cout << "User ID: " << task.user_id << "\n";
        std::cout << "Task Description: " << task.task_description << "\n";
    }

    // Display TaskStatus information for each task in the array
    std::cout << "\nTask Status Information:\n";
    for (const auto& task_status : task_statuses) {
        std::cout << "Task ID: " << task_status.task_id << "\n";
        std::cout << "Status: " << task_status.status << "\n";
    }

    // Display TaskDetails information for each task in the array
    std::cout << "\nTask Details Information:\n";
    for (const auto& task_details : task_details_list) {
        std::cout << "Task ID: " << task_details.task_id << "\n";
        std::cout << "Due Date: " << task_details.due_date << "\n";
        std::cout << "Priority: " << task_details.priority << "\n";
        std::cout << "Archived: " << task_details.archived << "\n";
    }

    return 0;
}
