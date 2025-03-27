
set(deplist math)

# Define common source files
set(COMMON_SOURCES
    skrypt.cpp
    state_loading.cpp
)

# Check for Python bindings option
if(OPENMIND_BUILD_PYTHON_BINDINGS)
    # Add Python bindings source file
    set(COMMON_SOURCES ${COMMON_SOURCES} python_bindings.cpp)
    
    # Find Python
    find_package(Python COMPONENTS Interpreter Development)
    
    # Find Boost Python
    find_package(Boost COMPONENTS python)
    
    if(Python_FOUND AND Boost_FOUND)
        # Add Python and Boost to dependencies
        set(deplist ${deplist} Python::Python Boost::python)
        
        # Define Python module flag
        add_definitions(-DSKRYPT_PYTHON_BINDINGS)
    else()
        message(STATUS "Python or Boost::python not found, disabling Python bindings")
    endif()
endif()

# Make TBB optional
option(SKRYPT_USE_TBB "Use TBB for parallel processing" OFF)
if(SKRYPT_USE_TBB)
    find_package(TBB)
    if(TBB_FOUND)
        set(deplist ${deplist} TBB::tbb)
        add_definitions(-DSKRYPT_USE_TBB)
    else()
        message(STATUS "TBB not found, disabling TBB support")
    endif()
endif()

# Build the library
lib(${deplist} SOURCES ${COMMON_SOURCES})
