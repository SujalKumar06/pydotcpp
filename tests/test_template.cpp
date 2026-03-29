#pragma once

#include <iostream>
#include <vector>
#include <string>

class TestRunner {
private:
    int passed = 0;
    int failed = 0;
    std::vector<std::string> failure_messages;

public:
    // Constructor prints the start header
    TestRunner() {
        std::cout << "============================= test session starts ==============================\n";
        std::cout << "collecting items...\n";
    }

    // Call this when a test passes
    void pass() {
        std::cout << "." << std::flush;
        passed++;
    }

    // Call this when a test fails, passing the test name/input and the error details
    void fail(const std::string& test_name, const std::string& details) {
        std::cout << "F" << std::flush;
        std::string full_message = "FAIL: \"" + test_name + "\"\n" + details;
        failure_messages.push_back(full_message);
        failed++;
    }

    // Call this at the very end of main() to print the summary and get the exit code
    int finish() {
        std::cout << "\n\n";

        if (!failure_messages.empty()) {
            std::cout << "=================================== FAILURES ===================================\n";
            for (const auto& msg : failure_messages) {
                std::cout << msg << "\n";
            }
        }

        std::cout << "========================= ";
        if (failed > 0) {
            std::cout << failed << " failed, " << passed << " passed";
        } else {
            std::cout << passed << " passed";
        }
        std::cout << " =========================\n";

        // Return 1 if tests failed so Make registers the error, 0 if all good
        return failed > 0 ? 1 : 0; 
    }
};