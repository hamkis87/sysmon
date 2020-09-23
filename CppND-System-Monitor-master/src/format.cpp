#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    long nHours, nMinutes, nSeconds;
    nHours = seconds / 3600;
    nMinutes = (seconds % 3600) / 60;
    nSeconds = seconds % 60;
    string result = std::to_string(nHours) + ":" + std::to_string(nMinutes)
                    + ":" + std::to_string(nSeconds);
    return result; 
}