#include "processor.h"
#include "linux_parser.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    int idle = LinuxParser::IdleJiffies();
    int active = LinuxParser::ActiveJiffies();
    //std::cout << active << "\n";
    //std::cout << (float)active/(float)(idle + active) << "\n";
    return (float)active/(float)(idle + active);

    }