#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
Process::Process(int i){
    PID = i;
    USER = LinuxParser::User(i);
    RAM = LinuxParser::Ram(i);
    UPTIME = LinuxParser::UpTime(i);
    COMMAND = LinuxParser::Command(i);
    CPU = ((float)(LinuxParser::ActiveJiffies(PID)/(float)sysconf(_SC_CLK_TCK))/(float)LinuxParser::UpTime(PID));
}
int Process::Pid() { 
    return PID; }
//void Process::SetPid(int i){
   // PID = i;
//}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    //return LinuxParser::ActiveJiffies(PID);}
    CPU = ((float)(LinuxParser::ActiveJiffies(PID)/(float)sysconf(_SC_CLK_TCK))/(float)LinuxParser::UpTime(PID));
    return CPU; }

// TODO: Return the command that generated this process
string Process::Command() { return COMMAND; }

// TODO: Return this process's memory utilization
string Process::Ram() { 
    RAM = LinuxParser::Ram(PID);
    return RAM; 
    }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    return USER; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    UPTIME = LinuxParser::UpTime(PID);
    return UPTIME; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return a.CPU > this->CPU; }
bool Process::operator>(Process const& a) const { 
    return a.CPU < this->CPU; }