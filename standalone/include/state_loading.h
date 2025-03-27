#pragma once

#include <string>
#include <map>
#include <vector>

namespace skrypt {

/**
 * @brief Class for loading and analyzing different types of state data
 */
class StateLoader {
public:
    /**
     * @brief Default constructor
     */
    StateLoader();

    /**
     * @brief Destructor
     */
    ~StateLoader();

    /**
     * @brief Loads Freesweeper state and represents it in the knowledge matrix
     * @param state_text Text representation of the Freesweeper state
     * @return Map of variable names to their values
     */
    std::map<std::string, double> LoadFreesweeperState(const std::string& state_text);

    /**
     * @brief Loads Sudoku state and represents it in the knowledge matrix
     * @param state_text Text representation of the Sudoku puzzle
     * @return Map of variable names to their values
     */
    std::map<std::string, double> LoadSudokuState(const std::string& state_text);

    /**
     * @brief Loads C++ code and represents it in the knowledge matrix for analysis
     * @param code C++ code to analyze
     * @return Map of variable names to their values
     */
    std::map<std::string, double> LoadCppCode(const std::string& code);

    /**
     * @brief Loads callstack data and represents it in the knowledge matrix for analysis
     * @param callstack Text representation of the callstack
     * @return Map of variable names to their values
     */
    std::map<std::string, double> LoadCallstack(const std::string& callstack);

    /**
     * @brief Loads coredump data and represents it in the knowledge matrix for analysis
     * @param coredump_path Path to the coredump file
     * @return Map of variable names to their values
     */
    std::map<std::string, double> LoadCoredump(const std::string& coredump_path);

private:
    // Internal state
    bool state_loaded_;
};

} // namespace skrypt
