cmake_minimum_required( VERSION 3.29 )

set( PROJECT_NAME "Weekly-Report-Engine" )
project( ${PROJECT_NAME} LANGUAGES CXX )
set( TARGET ${PROJECT_NAME} )

# Set source and include paths
set( INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include/" )
set( SOURCE_PATH "${PROJECT_SOURCE_DIR}/src/" )

# Recursively find project files
file( GLOB_RECURSE HPPS "${INCLUDE_PATH}/*.hpp" )
file( GLOB_RECURSE INLS "${INCLUDE_PATH}/*.inl" )
file( GLOB_RECURSE CPPS "${SOURCE_PATH}/*.cpp"  )

# Link sources to library
add_executable( ${TARGET} ${CPPS} ${INLS} ${HPPS} )

# Set C++17 standard for this target
target_compile_features( ${TARGET} PUBLIC cxx_std_17 )

# Include header file directory to target
target_include_directories( ${TARGET} PUBLIC $<BUILD_INTERFACE:${INCLUDE_PATH}> $<INSTALL_INTERFACE:include/> )