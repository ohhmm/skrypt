#!/usr/bin/env python3
"""
Test script for Skrypt Python bindings with state loading capabilities.
"""

import sys
import os

# Add the build directory to the Python path
sys.path.append(os.path.join(os.path.dirname(__file__), 'build'))

try:
    # Import the Skrypt Python module
    import skrypt
    print("Successfully imported Skrypt Python module")
except ImportError as e:
    print(f"Failed to import Skrypt Python module: {e}")
    sys.exit(1)

def test_freesweeper_state():
    """Test loading Freesweeper state."""
    print("\nTesting LoadFreesweeperState...")
    
    # Sample Freesweeper state
    state = """........
.1221...
.1**1...
.1221...
........"""
    
    try:
        # Create a Skrypt instance
        s = skrypt.Skrypt()
        
        # Load the Freesweeper state
        vars = s.LoadFreesweeperState(state)
        
        # Check if variables were created
        if vars:
            print(f"Created {len(vars)} variables")
            print(f"Variables: {', '.join(list(vars)[:5])}...")
            print("LoadFreesweeperState test passed!")
            return True
        else:
            print("No variables were created")
            return False
    except Exception as e:
        print(f"Error in LoadFreesweeperState: {e}")
        return False

def test_sudoku_state():
    """Test loading Sudoku state."""
    print("\nTesting LoadSudokuState...")
    
    # Sample Sudoku puzzle
    state = """53..7....
6..195...
.98....6.
8...6...3
4..8.3..1
7...2...6
.6....28.
...419..5
....8..79"""
    
    try:
        # Create a Skrypt instance
        s = skrypt.Skrypt()
        
        # Load the Sudoku state
        vars = s.LoadSudokuState(state)
        
        # Check if variables were created
        if vars:
            print(f"Created {len(vars)} variables")
            print(f"Variables: {', '.join(list(vars)[:5])}...")
            print("LoadSudokuState test passed!")
            return True
        else:
            print("No variables were created")
            return False
    except Exception as e:
        print(f"Error in LoadSudokuState: {e}")
        return False

def test_cpp_code():
    """Test loading C++ code."""
    print("\nTesting LoadCppCode...")
    
    # Sample C++ code
    code = """#include <iostream>

int main() {
    for (int i = 0; i < 10; i++) {
        if (i % 2 == 0) {
            std::cout << i << std::endl;
        }
    }
    return 0;
}"""
    
    try:
        # Create a Skrypt instance
        s = skrypt.Skrypt()
        
        # Load the C++ code
        vars = s.LoadCppCode(code)
        
        # Check if variables were created
        if vars:
            print(f"Created {len(vars)} variables")
            print(f"Variables: {', '.join(list(vars)[:5])}...")
            print("LoadCppCode test passed!")
            return True
        else:
            print("No variables were created")
            return False
    except Exception as e:
        print(f"Error in LoadCppCode: {e}")
        return False

def test_callstack():
    """Test loading callstack data."""
    print("\nTesting LoadCallstack...")
    
    # Sample callstack
    callstack = """#0 0x00007ffff7c4a00f in __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
#1 0x00007ffff7c4b3d9 in __GI_abort () at abort.c:79
#2 0x00007ffff7c8e8ee in __libc_message (action=action@entry=do_abort, fmt=fmt@entry=0x7ffff7dc4a8a "%s\\n") at ../sysdeps/posix/libc_fatal.c:155
#3 0x00007ffff7c9a2fc in malloc_printerr (str=str@entry=0x7ffff7dc4c22 "double free or corruption (!prev)") at malloc.c:5347
#4 0x00007ffff7c9c9bf in _int_free (av=<optimized out>, p=<optimized out>, have_lock=0) at malloc.c:4177
#5 0x0000555555555555 in main () at example.cpp:10"""
    
    try:
        # Create a Skrypt instance
        s = skrypt.Skrypt()
        
        # Load the callstack
        vars = s.LoadCallstack(callstack)
        
        # Check if variables were created
        if vars:
            print(f"Created {len(vars)} variables")
            print(f"Variables: {', '.join(list(vars)[:5])}...")
            print("LoadCallstack test passed!")
            return True
        else:
            print("No variables were created")
            return False
    except Exception as e:
        print(f"Error in LoadCallstack: {e}")
        return False

def test_coredump():
    """Test loading coredump data."""
    print("\nTesting LoadCoredump...")
    
    # Create a temporary file to simulate a coredump
    import tempfile
    with tempfile.NamedTemporaryFile(delete=False) as f:
        # Write ELF header magic bytes
        f.write(b"\x7F\x45\x4C\x46\x02")
        # Write some dummy data
        f.write(b"TESTCOREDUMP")
        temp_path = f.name
    
    try:
        # Create a Skrypt instance
        s = skrypt.Skrypt()
        
        # Load the coredump
        vars = s.LoadCoredump(temp_path)
        
        # Clean up
        os.unlink(temp_path)
        
        # Check if variables were created
        if vars:
            print(f"Created {len(vars)} variables")
            print(f"Variables: {', '.join(list(vars)[:5])}...")
            print("LoadCoredump test passed!")
            return True
        else:
            print("No variables were created")
            return False
    except Exception as e:
        print(f"Error in LoadCoredump: {e}")
        # Clean up
        try:
            os.unlink(temp_path)
        except:
            pass
        return False

def main():
    """Run all tests."""
    print("Testing Skrypt Python bindings for state loading capabilities")
    
    # Run all tests
    tests = [
        test_freesweeper_state,
        test_sudoku_state,
        test_cpp_code,
        test_callstack,
        test_coredump
    ]
    
    passed = 0
    for test in tests:
        if test():
            passed += 1
    
    # Print summary
    print(f"\nSummary: {passed}/{len(tests)} tests passed")
    
    return 0 if passed == len(tests) else 1

if __name__ == "__main__":
    sys.exit(main())
