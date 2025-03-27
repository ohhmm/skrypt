
set(deplist math)

# Define common source files
set(COMMON_SOURCES
    skrypt.cpp
    state_loading.cpp
)

# Build the library without Python bindings
lib(${deplist} SOURCES ${COMMON_SOURCES})
