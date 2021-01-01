#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours = seconds/3600;
    std::string hour;
    if (hours < 10){
        hour = "0" + std::to_string(hours);
    }
    else{
        hour = std::to_string(hours);
    }
    int minutes = (seconds%3600)/60;
    std::string minute;
    if (minutes < 10){
        minute = "0" + std::to_string(minutes);
    }
    else{
        minute = std::to_string(minutes);
    }
    int sec = seconds - ((hours * 3600) + (minutes*60));
    std::string second;
    if (sec < 10){
        second = "0" + std::to_string(sec);
    }
    else{
        second = std::to_string(sec);
    }
    return hour + ":" + minute + ":" + second; }