#include "../include/state_loading.h"
#include <iostream>
#include <cassert>
#include <fstream>

using namespace skrypt;

// Helper function to create a temporary file
std::string createTempFile(const std::string& content) {
    std::string temp_path = "/tmp/test_file";
    std::ofstream file(temp_path);
    file << content;
    file.close();
    return temp_path;
}

// Test loading Freesweeper state
void testLoadFreesweeperState() {
    std::cout << "Testing LoadFreesweeperState..." << std::endl;
    
    // Sample Freesweeper state
    std::string state = 
        "........\n"
        ".1221...\n"
        ".1**1...\n"
        ".1221...\n"
        "........\n";
    
    StateLoader loader;
    auto vars = loader.LoadFreesweeperState(state);
    
    // Verify that variables were created
    assert(!vars.empty());
    
    // Check for specific variables
    assert(vars.find("cell_2_2") != vars.end());
    assert(vars.find("cell_2_3") != vars.end());
    
    // Verify that the state loaded flag was set
    assert(vars.find("freesweeper_state_loaded") != vars.end());
    assert(vars["freesweeper_state_loaded"] == 1.0);
    
    // Verify mine cells
    assert(vars["cell_2_2"] == -1.0);
    assert(vars["cell_2_3"] == -1.0);
    
    std::cout << "LoadFreesweeperState test passed!" << std::endl;
}

// Test loading Sudoku state
void testLoadSudokuState() {
    std::cout << "Testing LoadSudokuState..." << std::endl;
    
    // Sample Sudoku puzzle
    std::string state = 
        "53..7....\n"
        "6..195...\n"
        ".98....6.\n"
        "8...6...3\n"
        "4..8.3..1\n"
        "7...2...6\n"
        ".6....28.\n"
        "...419..5\n"
        "....8..79\n";
    
    StateLoader loader;
    auto vars = loader.LoadSudokuState(state);
    
    // Verify that variables were created
    assert(!vars.empty());
    
    // Check for specific variables
    assert(vars.find("sudoku_0_0") != vars.end());
    
    // Verify that known values were set correctly
    assert(vars["sudoku_0_0"] == 5.0);
    assert(vars["sudoku_0_2"] == 0.0); // Empty cell
    
    // Verify that the state loaded flag was set
    assert(vars.find("sudoku_state_loaded") != vars.end());
    assert(vars["sudoku_state_loaded"] == 1.0);
    
    std::cout << "LoadSudokuState test passed!" << std::endl;
}

// Test loading C++ code
void testLoadCppCode() {
    std::cout << "Testing LoadCppCode..." << std::endl;
    
    // Sample C++ code
    std::string code = 
        "#include <iostream>\n"
        "\n"
        "int main() {\n"
        "    for (int i = 0; i < 10; i++) {\n"
        "        if (i % 2 == 0) {\n"
        "            std::cout << i << std::endl;\n"
        "        }\n"
        "    }\n"
        "    return 0;\n"
        "}\n";
    
    StateLoader loader;
    auto vars = loader.LoadCppCode(code);
    
    // Verify that variables were created
    assert(!vars.empty());
    
    // Check for specific variables (line variables)
    assert(vars.find("cpp_line_0") != vars.end());
    assert(vars.find("cpp_line_3") != vars.end());
    
    // Verify that the state loaded flag was set
    assert(vars.find("cpp_code_loaded") != vars.end());
    assert(vars["cpp_code_loaded"] == 1.0);
    
    // Verify line count
    assert(vars["cpp_line_count"] == 10.0);
    
    std::cout << "LoadCppCode test passed!" << std::endl;
}

// Test loading callstack data
void testLoadCallstack() {
    std::cout << "Testing LoadCallstack..." << std::endl;
    
    // Sample callstack
    std::string callstack = 
        "#0 0x00007ffff7c4a00f in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50\n"
        "#1 0x00007ffff7c4b3d9 in __GI_abort () at abort.c:79\n"
        "#2 0x00007ffff7c8e8ee in __libc_message (action=action@entry=do_abort, fmt=fmt@entry=0x7ffff7dc4a8a \"%s\\n\") at ../sysdeps/posix/libc_fatal.c:155\n"
        "#3 0x00007ffff7c9a2fc in malloc_printerr (str=str@entry=0x7ffff7dc4c22 \"double free or corruption (!prev)\") at malloc.c:5347\n"
        "#4 0x00007ffff7c9c9bf in _int_free (av=<optimized out>, p=<optimized out>, have_lock=0) at malloc.c:4177\n"
        "#5 0x0000555555555555 in main () at example.cpp:10\n";
    
    StateLoader loader;
    auto vars = loader.LoadCallstack(callstack);
    
    // Verify that variables were created
    assert(!vars.empty());
    
    // Check for specific variables (frame variables)
    assert(vars.find("frame_0") != vars.end());
    assert(vars.find("frame_5") != vars.end());
    
    // Verify that the state loaded flag was set
    assert(vars.find("callstack_loaded") != vars.end());
    assert(vars["callstack_loaded"] == 1.0);
    
    std::cout << "LoadCallstack test passed!" << std::endl;
}

// Test loading coredump data
void testLoadCoredump() {
    std::cout << "Testing LoadCoredump..." << std::endl;
    
    // Create a temporary file to simulate a coredump
    std::string temp_path = "/tmp/test_coredump";
    {
        std::ofstream file(temp_path, std::ios::binary);
        // Write ELF header magic bytes
        file.write("\x7F\x45\x4C\x46\x02", 5);
        // Write some dummy data
        file.write("TESTCOREDUMP", 12);
    }
    
    StateLoader loader;
    auto vars = loader.LoadCoredump(temp_path);
    
    // Verify that variables were created
    assert(!vars.empty());
    
    // Check for specific variables
    assert(vars.find("coredump_path") != vars.end());
    assert(vars.find("coredump_size") != vars.end());
    assert(vars.find("coredump_valid") != vars.end());
    assert(vars.find("coredump_64bit") != vars.end());
    
    // Verify that the state loaded flag was set
    assert(vars.find("coredump_loaded") != vars.end());
    assert(vars["coredump_loaded"] == 1.0);
    
    // Verify that the coredump was recognized as valid
    assert(vars["coredump_valid"] == 1.0);
    
    // Verify that the coredump was recognized as 64-bit
    assert(vars["coredump_64bit"] == 1.0);
    
    // Clean up
    std::remove(temp_path.c_str());
    
    std::cout << "LoadCoredump test passed!" << std::endl;
}

// Test loading invalid coredump
void testLoadInvalidCoredump() {
    std::cout << "Testing LoadInvalidCoredump..." << std::endl;
    
    // Create a temporary file with invalid data
    std::string temp_path = "/tmp/invalid_coredump";
    {
        std::ofstream file(temp_path);
        file.write("INVALID", 7);
    }
    
    StateLoader loader;
    auto vars = loader.LoadCoredump(temp_path);
    
    // Verify that variables were created
    assert(!vars.empty());
    
    // Verify that the coredump was recognized as invalid
    assert(vars["coredump_valid"] == 0.0);
    
    // Clean up
    std::remove(temp_path.c_str());
    
    std::cout << "LoadInvalidCoredump test passed!" << std::endl;
}

// Test loading non-existent coredump
void testLoadNonExistentCoredump() {
    std::cout << "Testing LoadNonExistentCoredump..." << std::endl;
    
    // Path to a non-existent file
    std::string non_existent_path = "/tmp/non_existent_coredump";
    
    // Make sure the file doesn't exist
    std::remove(non_existent_path.c_str());
    
    StateLoader loader;
    auto vars = loader.LoadCoredump(non_existent_path);
    
    // Verify that error variables were created
    assert(!vars.empty());
    
    // Verify that the error flag was set
    assert(vars.find("coredump_load_error") != vars.end());
    assert(vars["coredump_load_error"] == 1.0);
    
    std::cout << "LoadNonExistentCoredump test passed!" << std::endl;
}

int main() {
    std::cout << "Running state loading tests..." << std::endl;
    
    testLoadFreesweeperState();
    testLoadSudokuState();
    testLoadCppCode();
    testLoadCallstack();
    testLoadCoredump();
    testLoadInvalidCoredump();
    testLoadNonExistentCoredump();
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
