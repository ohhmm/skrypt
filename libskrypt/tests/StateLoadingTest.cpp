#include "stdafx.h"
#include <skrypt.h>
#include <boost/test/unit_test.hpp>
#include <sstream>
#include <fstream>

using namespace skrypt;

// Test fixture for state loading tests
struct StateLoadingFixture {
    StateLoadingFixture() {
        // Create a Skrypt instance for testing
        skrypt = std::make_shared<Skrypt>();
    }
    
    std::shared_ptr<Skrypt> skrypt;
};

BOOST_FIXTURE_TEST_SUITE(StateLoadingTests, StateLoadingFixture)

// Test loading Freesweeper state
BOOST_AUTO_TEST_CASE(TestLoadFreesweeperState) {
    // Sample Freesweeper state
    std::string state = 
        "........\n"
        ".1221...\n"
        ".1**1...\n"
        ".1221...\n"
        "........\n";
    
    // Load the state
    auto vars = skrypt->LoadFreesweeperState(state);
    
    // Verify that variables were created
    BOOST_TEST(!vars.empty());
    
    // Check for specific variables
    BOOST_TEST(vars.find("cell_2_2") != vars.end());
    BOOST_TEST(vars.find("cell_2_3") != vars.end());
    
    // Verify that the state loaded flag was set
    auto solutions = skrypt->Solve(skrypt->GetVarHost()->Host("freesweeper_state_loaded"));
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
}

// Test loading Sudoku state
BOOST_AUTO_TEST_CASE(TestLoadSudokuState) {
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
    
    // Load the state
    auto vars = skrypt->LoadSudokuState(state);
    
    // Verify that variables were created
    BOOST_TEST(!vars.empty());
    
    // Check for specific variables
    BOOST_TEST(vars.find("sudoku_0_0") != vars.end());
    
    // Verify that known values were set correctly
    auto solutions = skrypt->Solve(vars["sudoku_0_0"]);
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->GetInt() == 5);
    
    // Verify that the state loaded flag was set
    solutions = skrypt->Solve(skrypt->GetVarHost()->Host("sudoku_state_loaded"));
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
}

// Test loading C++ code
BOOST_AUTO_TEST_CASE(TestLoadCppCode) {
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
    
    // Load the code
    auto vars = skrypt->LoadCppCode(code);
    
    // Verify that variables were created
    BOOST_TEST(!vars.empty());
    
    // Check for specific variables (line variables)
    BOOST_TEST(vars.find("cpp_line_0") != vars.end());
    BOOST_TEST(vars.find("cpp_line_3") != vars.end());
    
    // Verify that the state loaded flag was set
    auto solutions = skrypt->Solve(skrypt->GetVarHost()->Host("cpp_code_loaded"));
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
}

// Test loading callstack data
BOOST_AUTO_TEST_CASE(TestLoadCallstack) {
    // Sample callstack
    std::string callstack = 
        "#0 0x00007ffff7c4a00f in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50\n"
        "#1 0x00007ffff7c4b3d9 in __GI_abort () at abort.c:79\n"
        "#2 0x00007ffff7c8e8ee in __libc_message (action=action@entry=do_abort, fmt=fmt@entry=0x7ffff7dc4a8a \"%s\\n\") at ../sysdeps/posix/libc_fatal.c:155\n"
        "#3 0x00007ffff7c9a2fc in malloc_printerr (str=str@entry=0x7ffff7dc4c22 \"double free or corruption (!prev)\") at malloc.c:5347\n"
        "#4 0x00007ffff7c9c9bf in _int_free (av=<optimized out>, p=<optimized out>, have_lock=0) at malloc.c:4177\n"
        "#5 0x0000555555555555 in main () at example.cpp:10\n";
    
    // Load the callstack
    auto vars = skrypt->LoadCallstack(callstack);
    
    // Verify that variables were created
    BOOST_TEST(!vars.empty());
    
    // Check for specific variables (frame variables)
    BOOST_TEST(vars.find("frame_0") != vars.end());
    BOOST_TEST(vars.find("frame_5") != vars.end());
    
    // Verify that the state loaded flag was set
    auto solutions = skrypt->Solve(skrypt->GetVarHost()->Host("callstack_loaded"));
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
}

// Test loading coredump data
BOOST_AUTO_TEST_CASE(TestLoadCoredump) {
    // Create a temporary file to simulate a coredump
    std::string temp_path = "/tmp/test_coredump";
    {
        std::ofstream file(temp_path);
        // Write ELF header magic bytes
        file.write("\x7F\x45\x4C\x46\x02", 5);
        // Write some dummy data
        file.write("TESTCOREDUMP", 12);
    }
    
    // Load the coredump
    auto vars = skrypt->LoadCoredump(temp_path);
    
    // Verify that variables were created
    BOOST_TEST(!vars.empty());
    
    // Check for specific variables
    BOOST_TEST(vars.find("coredump_path") != vars.end());
    BOOST_TEST(vars.find("coredump_size") != vars.end());
    BOOST_TEST(vars.find("coredump_valid") != vars.end());
    BOOST_TEST(vars.find("coredump_64bit") != vars.end());
    
    // Verify that the state loaded flag was set
    auto solutions = skrypt->Solve(skrypt->GetVarHost()->Host("coredump_loaded"));
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
    
    // Verify that the coredump was recognized as valid
    solutions = skrypt->Solve(vars["coredump_valid"]);
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
    
    // Verify that the coredump was recognized as 64-bit
    solutions = skrypt->Solve(vars["coredump_64bit"]);
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
    
    // Clean up
    std::remove(temp_path.c_str());
}

// Test loading invalid coredump
BOOST_AUTO_TEST_CASE(TestLoadInvalidCoredump) {
    // Create a temporary file with invalid data
    std::string temp_path = "/tmp/invalid_coredump";
    {
        std::ofstream file(temp_path);
        file.write("INVALID", 7);
    }
    
    // Load the invalid coredump
    auto vars = skrypt->LoadCoredump(temp_path);
    
    // Verify that variables were created
    BOOST_TEST(!vars.empty());
    
    // Verify that the coredump was recognized as invalid
    auto solutions = skrypt->Solve(vars["coredump_valid"]);
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsZero());
    
    // Clean up
    std::remove(temp_path.c_str());
}

// Test loading non-existent coredump
BOOST_AUTO_TEST_CASE(TestLoadNonExistentCoredump) {
    // Path to a non-existent file
    std::string non_existent_path = "/tmp/non_existent_coredump";
    
    // Make sure the file doesn't exist
    std::remove(non_existent_path.c_str());
    
    // Load the non-existent coredump
    auto vars = skrypt->LoadCoredump(non_existent_path);
    
    // Verify that error variables were created
    BOOST_TEST(!vars.empty());
    
    // Verify that the error flag was set
    auto solutions = skrypt->Solve(skrypt->GetVarHost()->Host("coredump_load_error"));
    BOOST_TEST(!solutions.empty());
    BOOST_TEST(solutions.begin()->IsInt());
    BOOST_TEST(solutions.begin()->IsOne());
}

BOOST_AUTO_TEST_SUITE_END()
