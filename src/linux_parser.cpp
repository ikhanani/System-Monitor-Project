#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  double MemTotal;
  double MemFree;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //std::replace(line.begin(), line.end(), ' ', '_');
      //std::replace(line.begin(), line.end(), '=', ' ');
      //std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          //std::replace(value.begin(), value.end(), '_', ' ');
          MemTotal = std::stod(value);
        }
        if (key == "MemFree:"){
          MemFree = std::stod(value);
        }
      }
    }
  }
  return (MemTotal - MemFree)/MemTotal; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string key;
  string value;
  
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  std::getline(filestream, line); 
  std::istringstream linestream(line);
  while(linestream >> key >> value){
    //std::cout << std::stol(key) << "\n";
    return std::stol(key);
  }

  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::UpTime()*sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line;
  string value;
  long result = 0;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      int count = 0;
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (count >= 13 && count <= 16) {
          
          result = result + std::stol(value);
      
        }
        count ++;
      }
    }
    
  return result;
   }
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> tokens = LinuxParser::CpuUtilization();
  int result = 0;
  for(std::string i : tokens){
    result += std::stoi(i);
  } 
  return result;
    
   }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
    vector<string> tokens = LinuxParser::CpuUtilization();
    return std::stoi(tokens[3]) + std::stoi(tokens[4]);
   }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string key;
  string value;
  vector<string> result;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "cpu") {
          result.push_back(value);
          while(linestream >> value){
            //std::cout << value << "\n";
            result.push_back(value);
          }
          
        }
        
      }
    }
  
  return result; }
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int Total;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //std::replace(line.begin(), line.end(), ' ', '_');
      //std::replace(line.begin(), line.end(), '=', ' ');
      //std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          //std::replace(value.begin(), value.end(), '_', ' ');
          //std::cout << value <<"\n";
          Total = std::stoi(value);
        }
        
      }
    }
  }
  return Total;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key;
  string value;
  int Total;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      //std::replace(line.begin(), line.end(), ' ', '_');
      //std::replace(line.begin(), line.end(), '=', ' ');
      //std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          //std::replace(value.begin(), value.end(), '_', ' ');
          //std::cout << value <<"\n";
          Total = std::stoi(value);
        }
        
      }
    }
  }
  return Total; }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  string key;
  string value;
  string result;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      //std::istringstream linestream(line);
      result = line;
    }
  return result;
   }
   
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line;
  string key;
  string value;
  string result;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          
          result = std::to_string(std::stoi(value)/1000);
        }
        
      }
    }

  return result; }
  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string key;
  string value;
  string result;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          
          result = value;
        }
        
      }
    }

  return result; }
  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string key;
  string x;
  string value;
  string result;
  string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value == uid) {
          
          result = key;
        }
        
      }
    }

  return result; }
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line;
  //string key;
  string value;
  long result;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      int count = 0;
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (count == 21) {
          
          result = std::stol(value);
          return LinuxParser::UpTime() - result/sysconf(_SC_CLK_TCK);
        }
        count ++;
      }
    }
    
  return LinuxParser::UpTime() - result/sysconf(_SC_CLK_TCK); }
  
}