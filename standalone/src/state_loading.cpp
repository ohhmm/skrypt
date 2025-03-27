#include "state_loading.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>

namespace skrypt {

StateLoader::StateLoader() : state_loaded_(false) {
}

StateLoader::~StateLoader() {
}

std::map<std::string, double> StateLoader::LoadFreesweeperState(const std::string& state_text) {
    std::map<std::string, double> variables;
    
    // Parse the Freesweeper state
    std::istringstream iss(state_text);
    std::string line;
    int row = 0;
    
    while (std::getline(iss, line)) {
        for (int col = 0; col < line.length(); col++) {
            char cell = line[col];
            std::string var_name = "cell_" + std::to_string(row) + "_" + std::to_string(col);
            
            if (cell == '*') {
                // Mine
                variables[var_name] = -1.0;
            } else if (cell >= '1' && cell <= '8') {
                // Number
                variables[var_name] = static_cast<double>(cell - '0');
            } else {
                // Empty cell
                variables[var_name] = 0.0;
            }
        }
        row++;
    }
    
    // Set state loaded flag
    variables["freesweeper_state_loaded"] = 1.0;
    state_loaded_ = true;
    
    return variables;
}

std::map<std::string, double> StateLoader::LoadSudokuState(const std::string& state_text) {
    std::map<std::string, double> variables;
    
    // Parse the Sudoku state
    std::istringstream iss(state_text);
    std::string line;
    int row = 0;
    
    while (std::getline(iss, line)) {
        for (int col = 0; col < line.length(); col++) {
            char cell = line[col];
            std::string var_name = "sudoku_" + std::to_string(row) + "_" + std::to_string(col);
            
            if (cell >= '1' && cell <= '9') {
                // Known value
                variables[var_name] = static_cast<double>(cell - '0');
            } else {
                // Unknown value
                variables[var_name] = 0.0;
            }
        }
        row++;
    }
    
    // Set state loaded flag
    variables["sudoku_state_loaded"] = 1.0;
    state_loaded_ = true;
    
    return variables;
}

std::map<std::string, double> StateLoader::LoadCppCode(const std::string& code) {
    std::map<std::string, double> variables;
    
    // Parse the C++ code
    std::istringstream iss(code);
    std::string line;
    int line_num = 0;
    
    while (std::getline(iss, line)) {
        std::string var_name = "cpp_line_" + std::to_string(line_num);
        variables[var_name] = static_cast<double>(line.length());
        
        // Count braces for nesting level
        int open_braces = 0;
        int close_braces = 0;
        for (char c : line) {
            if (c == '{') open_braces++;
            if (c == '}') close_braces++;
        }
        
        variables["cpp_line_" + std::to_string(line_num) + "_open_braces"] = static_cast<double>(open_braces);
        variables["cpp_line_" + std::to_string(line_num) + "_close_braces"] = static_cast<double>(close_braces);
        
        line_num++;
    }
    
    // Set state loaded flag
    variables["cpp_code_loaded"] = 1.0;
    variables["cpp_line_count"] = static_cast<double>(line_num);
    state_loaded_ = true;
    
    return variables;
}

std::map<std::string, double> StateLoader::LoadCallstack(const std::string& callstack) {
    std::map<std::string, double> variables;
    
    // Parse the callstack
    std::istringstream iss(callstack);
    std::string line;
    int frame_num = 0;
    
    std::regex frame_regex(R"(#(\d+)\s+0x([0-9a-f]+)\s+in\s+([^\s]+)\s+)");
    
    while (std::getline(iss, line)) {
        std::smatch matches;
        if (std::regex_search(line, matches, frame_regex)) {
            std::string frame_id = matches[1].str();
            std::string address = matches[2].str();
            std::string function = matches[3].str();
            
            std::string var_name = "frame_" + frame_id;
            variables[var_name] = static_cast<double>(std::stoi(frame_id));
            variables[var_name + "_address"] = static_cast<double>(std::stoul(address, nullptr, 16));
        }
        
        frame_num++;
    }
    
    // Set state loaded flag
    variables["callstack_loaded"] = 1.0;
    variables["callstack_frame_count"] = static_cast<double>(frame_num);
    state_loaded_ = true;
    
    return variables;
}

std::map<std::string, double> StateLoader::LoadCoredump(const std::string& coredump_path) {
    std::map<std::string, double> variables;
    
    // Try to open the coredump file
    std::ifstream file(coredump_path, std::ios::binary);
    if (!file) {
        // File doesn't exist or can't be opened
        variables["coredump_load_error"] = 1.0;
        variables["coredump_valid"] = 0.0;
        return variables;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Read first few bytes to check if it's a valid ELF file
    char header[5];
    file.read(header, 5);
    
    bool is_valid_elf = (header[0] == 0x7F && header[1] == 'E' && header[2] == 'L' && header[3] == 'F');
    bool is_64bit = (is_valid_elf && header[4] == 2);
    
    variables["coredump_path"] = 1.0;  // Just a flag that path was provided
    variables["coredump_size"] = static_cast<double>(size);
    variables["coredump_valid"] = is_valid_elf ? 1.0 : 0.0;
    variables["coredump_64bit"] = is_64bit ? 1.0 : 0.0;
    
    // Set state loaded flag
    variables["coredump_loaded"] = 1.0;
    state_loaded_ = true;
    
    return variables;
}

} // namespace skrypt
