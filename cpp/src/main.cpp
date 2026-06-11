#include "../include/censorer.hpp"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    std::cout << "=== Testing C++ Censorer Library ===\n\n";

    // 1. Initialize
    cen::set_filter("badwords", {"apple", "banana"});

    // 2. Append extra word
    cen::awf("orange", "badwords");

    // 3. Scenario A: Standard Case-Sensitive test
    std::string rawTextA = "I hate apple and orange!";
    std::string cleanTextA = cen::censor(rawTextA, "badwords");
    std::cout << "[Standard Censor]\n";
    std::cout << "Original: " << rawTextA << "\n";
    std::cout << "Censored: " << cleanTextA << "\n\n";

    // 4. Scenario B: Case-Insensitive evaluation test
    std::string rawTextB = "I hate ApPlE, BANANA, and an ORANGE tree!";
    std::string cleanTextB = cen::censor_case_insensitive(rawTextB, "badwords");
    std::cout << "[Case-Insensitive Censor]\n";
    std::cout << "Original: " << rawTextB << "\n";
    std::cout << "Censored: " << cleanTextB << "\n\n";

    // 5. Scenario C: Verify standard method fails on uppercase mutations
    std::string cleanTextC = cen::censor(rawTextB, "badwords");
    std::cout << "[Standard Censor running on mutated text]\n";
    std::cout << "Censored: " << cleanTextC << "\n\n";

    // 6. Scenario D: Clear & Delete
    std::cout << "--- Clearing Filters ---\n";
    cen::clear_filter("badwords");
    std::cout << "--- Deleting Filter Key Completely ---\n";
    cen::delete_filter("badwords");

    return 0;
}
