# Get source files
FILE(
    GLOB_RECURSE SRC_LIST 
    ./*.c
    ./*.cpp
)

# Create platform-specific target name
set(TARGET_NAME "${APP_NAME}_${PLATFORM}")

# Create executable
ADD_EXECUTABLE(${TARGET_NAME} ${SRC_LIST})

# Set C and CXX compiler for this target
set(CMAKE_C_COMPILER ${TARGET_NAME}_C_COMPILER)
set(CMAKE_CXX_COMPILER ${TARGET_NAME}_CXX_COMPILER)

# Link libraries
TARGET_LINK_LIBRARIES(
    ${TARGET_NAME}
    rt
)