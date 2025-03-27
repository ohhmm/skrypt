#include "skrypt.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

using namespace skrypt;
using namespace omnn::math;

// Implementation of state loading methods for Skrypt

const Valuable::va_names_t& Skrypt::LoadFreesweeperState(const std::string& state_text)
{
    // Create variables to represent the Freesweeper state
    auto& varHost = *GetVarHost();
    
    // Parse the state text (assuming a grid format with . for empty, numbers for hints, * for mines)
    std::istringstream iss(state_text);
    std::string line;
    int row = 0;
    
    while (std::getline(iss, line)) {
        for (size_t col = 0; col < line.length(); ++col) {
            char cell = line[col];
            std::string varName = "cell_" + std::to_string(row) + "_" + std::to_string(col);
            
            if (cell == '*') {
                // Mine cell
                varHost.Host(varName) = 9; // Use 9 to represent a mine
            } else if (cell >= '1' && cell <= '8') {
                // Number cell
                varHost.Host(varName) = cell - '0';
            } else {
                // Empty cell
                varHost.Host(varName) = 0;
            }
        }
        ++row;
    }
    
    // Set a flag to indicate that a Freesweeper state has been loaded
    varHost.Host("freesweeper_state_loaded") = 1;
    
    return GetVarNames();
}

const Valuable::va_names_t& Skrypt::LoadSudokuState(const std::string& state_text)
{
    // Create variables to represent the Sudoku state
    auto& varHost = *GetVarHost();
    
    // Parse the state text (assuming a 9x9 grid with . or 0 for empty cells)
    std::istringstream iss(state_text);
    std::string line;
    int row = 0;
    
    while (std::getline(iss, line) && row < 9) {
        int col = 0;
        for (size_t i = 0; i < line.length() && col < 9; ++i) {
            char cell = line[i];
            if (cell == '.' || cell == ' ' || cell == '\t') {
                continue; // Skip separators
            }
            
            std::string varName = "sudoku_" + std::to_string(row) + "_" + std::to_string(col);
            
            if (cell >= '1' && cell <= '9') {
                // Known value
                varHost.Host(varName) = cell - '0';
            } else {
                // Empty cell or 0
                varHost.Host(varName) = 0;
            }
            
            ++col;
        }
        ++row;
    }
    
    // Set a flag to indicate that a Sudoku state has been loaded
    varHost.Host("sudoku_state_loaded") = 1;
    
    return GetVarNames();
}

const Valuable::va_names_t& Skrypt::LoadCppCode(const std::string& code)
{
    // Create variables to represent the C++ code
    auto& varHost = *GetVarHost();
    
    // Parse the code line by line
    std::istringstream iss(code);
    std::string line;
    int lineNum = 0;
    
    while (std::getline(iss, line)) {
        std::string varName = "cpp_line_" + std::to_string(lineNum);
        
        // Store the line as a string (represented as an integer hash for simplicity)
        std::hash<std::string> hasher;
        size_t hash = hasher(line);
        varHost.Host(varName) = static_cast<int>(hash % 1000000); // Use modulo to keep the hash manageable
        
        // Detect and store basic code constructs
        if (line.find("if") != std::string::npos) {
            varHost.Host("cpp_has_if_" + std::to_string(lineNum)) = 1;
        }
        if (line.find("for") != std::string::npos) {
            varHost.Host("cpp_has_for_" + std::to_string(lineNum)) = 1;
        }
        if (line.find("while") != std::string::npos) {
            varHost.Host("cpp_has_while_" + std::to_string(lineNum)) = 1;
        }
        if (line.find("class") != std::string::npos) {
            varHost.Host("cpp_has_class_" + std::to_string(lineNum)) = 1;
        }
        if (line.find("struct") != std::string::npos) {
            varHost.Host("cpp_has_struct_" + std::to_string(lineNum)) = 1;
        }
        
        ++lineNum;
    }
    
    // Store the total number of lines
    varHost.Host("cpp_line_count") = lineNum;
    
    // Set a flag to indicate that C++ code has been loaded
    varHost.Host("cpp_code_loaded") = 1;
    
    return GetVarNames();
}

const Valuable::va_names_t& Skrypt::LoadCallstack(const std::string& callstack)
{
    // Create variables to represent the callstack
    auto& varHost = *GetVarHost();
    
    // Parse the callstack line by line
    std::istringstream iss(callstack);
    std::string line;
    int frameNum = 0;
    
    while (std::getline(iss, line)) {
        // Check if this line looks like a callstack frame
        if (line.find("#") == 0) {
            std::string varName = "frame_" + std::to_string(frameNum);
            
            // Store the frame as a string (represented as an integer hash for simplicity)
            std::hash<std::string> hasher;
            size_t hash = hasher(line);
            varHost.Host(varName) = static_cast<int>(hash % 1000000); // Use modulo to keep the hash manageable
            
            // Extract function name if present
            size_t funcStart = line.find(" in ");
            if (funcStart != std::string::npos) {
                funcStart += 4; // Skip " in "
                size_t funcEnd = line.find(" (", funcStart);
                if (funcEnd != std::string::npos) {
                    std::string funcName = line.substr(funcStart, funcEnd - funcStart);
                    varHost.Host("frame_func_" + std::to_string(frameNum)) = static_cast<int>(hasher(funcName) % 1000000);
                }
            }
            
            // Extract file and line if present
            size_t fileStart = line.find(" at ");
            if (fileStart != std::string::npos) {
                fileStart += 4; // Skip " at "
                size_t lineStart = line.find(":", fileStart);
                if (lineStart != std::string::npos) {
                    std::string fileName = line.substr(fileStart, lineStart - fileStart);
                    varHost.Host("frame_file_" + std::to_string(frameNum)) = static_cast<int>(hasher(fileName) % 1000000);
                    
                    std::string lineNum = line.substr(lineStart + 1);
                    try {
                        varHost.Host("frame_line_" + std::to_string(frameNum)) = std::stoi(lineNum);
                    } catch (...) {
                        // Ignore conversion errors
                    }
                }
            }
            
            ++frameNum;
        }
    }
    
    // Store the total number of frames
    varHost.Host("callstack_frame_count") = frameNum;
    
    // Set a flag to indicate that a callstack has been loaded
    varHost.Host("callstack_loaded") = 1;
    
    return GetVarNames();
}

const Valuable::va_names_t& Skrypt::LoadCoredump(const std::string& coredump_path)
{
    // Create variables to represent the coredump
    auto& varHost = *GetVarHost();
    
    // Store the coredump path
    std::hash<std::string> hasher;
    size_t hash = hasher(coredump_path);
    varHost.Host("coredump_path") = static_cast<int>(hash % 1000000);
    
    // Check if the file exists
    std::ifstream file(coredump_path);
    if (!file) {
        // File doesn't exist or can't be opened
        varHost.Host("coredump_exists") = 0;
        varHost.Host("coredump_load_error") = 1;
        return GetVarNames();
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    varHost.Host("coredump_size") = static_cast<int>(size);
    
    // Check if it's a valid ELF file (simple check for the magic bytes)
    char magic[5] = {0};
    file.read(magic, 4);
    
    bool isValidElf = (magic[0] == 0x7F && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F');
    varHost.Host("coredump_valid") = isValidElf ? 1 : 0;
    
    if (isValidElf) {
        // Check ELF class (32-bit or 64-bit)
        char elfClass;
        file.seekg(4, std::ios::beg);
        file.read(&elfClass, 1);
        
        bool is64bit = (elfClass == 2);
        varHost.Host("coredump_64bit") = is64bit ? 1 : 0;
    }
    
    // Set a flag to indicate that a coredump has been loaded
    varHost.Host("coredump_loaded") = 1;
    
    return GetVarNames();
}
