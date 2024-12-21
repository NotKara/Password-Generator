#include <iostream>
#include <string>
#include <random>
#include <algorithm>

// Function to generate a secure random password
std::string generatePassword(int length, bool includeUpper, bool includeNumbers, bool includeSpecial) {
    const std::string lowerChars = "abcdefghijklmnopqrstuvwxyz";
    const std::string upperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  
    const std::string numbers = "0123456789";
    const std::string specialChars = "!@#$%^&*()_+{}|:<>?";
    
    std::string allChars = lowerChars;
    std::string password;

    // Using random device and generator for better randomness
    std::random_device rd;
    std::mt19937 generator(rd());

    // Collect required character sets
    std::vector<std::string> requiredSets;

    if (includeUpper) {
        allChars += upperChars;
        requiredSets.push_back(upperChars);
    }
    if (includeNumbers) {
        allChars += numbers;
        requiredSets.push_back(numbers);
    }
    if (includeSpecial) {
        allChars += specialChars;
        requiredSets.push_back(specialChars);
    }

    if (length < requiredSets.size()) {
        std::cerr << "Password length too short for the selected character sets." << std::endl;
        return "";
    }

    // Ensure at least one character from each selected set is included
    for (const auto& set : requiredSets) {
        std::uniform_int_distribution<> dist(0, set.size() - 1);
        password += set[dist(generator)];
    }

    // Fill the remaining length with random characters from allChars
    std::uniform_int_distribution<> dist(0, allChars.size() - 1);
    for (int i = password.size(); i < length; ++i) {
        password += allChars[dist(generator)];
    }

    // Shuffle the password to avoid predictable sequences
    std::shuffle(password.begin(), password.end(), generator);

    return password;
}

int main() {
    int length;
    char includeUpper, includeNumbers, includeSpecial;

    std::cout << "=== Secure Password Generator ===\n\n";

    // Get password length
    while (true) {
        std::cout << "Enter the desired password length (minimum 1): ";
        std::cin >> length;
        if (std::cin.fail() || length < 1) {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            std::cout << "Please enter a valid positive integer.\n";
        } else {
            break;
        }
    }

    // Get character set preferences
    auto getYesNo = [](const std::string& prompt) -> bool {
        char response;
        while (true) {
            std::cout << prompt << " (y/n): ";
            std::cin >> response;
            response = std::tolower(response);
            if (response == 'y') return true;
            if (response == 'n') return false;
            std::cout << "Please enter 'y' or 'n'.\n";
        }
    };

    bool upper = getYesNo("Include uppercase letters");
    bool numbers = getYesNo("Include numbers");
    bool special = getYesNo("Include special characters");

    // Ensure at least one character set is selected
    if (!upper && !numbers && !special) {
        std::cout << "No additional character sets selected. The password will contain only lowercase letters.\n";
    }

    // Generate the password
    std::string password = generatePassword(length, upper, numbers, special);
    if (!password.empty()) {
        std::cout << "\n=== Generated Password ===\n" << password << "\n";
    } else {
        std::cout << "Failed to generate password. Please try again with different parameters.\n";
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer
    std::cin.get(); // Wait for Enter key press
    return 0;
}
