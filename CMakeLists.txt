PROJECT(UART_Listen)

CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

# Set application name
SET(APP_NAME "uart_listen")

# Define compiler settings for each platform
set(AARCH64_C_COMPILER "aarch64-none-linux-gnu-gcc")
set(AARCH64_CXX_COMPILER "aarch64-none-linux-gnu-g++")
set(ARM32_C_COMPILER "arm-none-linux-gnueabihf-gcc")
set(ARM32_CXX_COMPILER "arm-none-linux-gnueabihf-g++")

# Common compiler flags
set(COMMON_FLAGS "-s -O3 -lrt")

# Function to create target for specific platform
function(build_platform PLATFORM C_COMPILER CXX_COMPILER)
    # Create build directory for this platform
    set(BUILD_DIR "${CMAKE_BINARY_DIR}/${PLATFORM}")
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin/${PLATFORM}")
    
    # Create output directory
    file(MAKE_DIRECTORY ${OUTPUT_DIR})
    
    # Configure compiler settings
    set(CMAKE_C_COMPILER ${C_COMPILER})
    set(CMAKE_CXX_COMPILER ${CXX_COMPILER})
    set(CMAKE_C_FLAGS "${COMMON_FLAGS}")
    set(CMAKE_CXX_FLAGS "${COMMON_FLAGS}")
    
    # Collect source files
    file(GLOB_RECURSE SRC_LIST 
        "${CMAKE_SOURCE_DIR}/src/*.c"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
    )
    
    # Create target with platform-specific name
    set(TARGET_NAME "${APP_NAME}_${PLATFORM}")
    add_executable(${TARGET_NAME} ${SRC_LIST})
    
    # Set output properties
    set_target_properties(${TARGET_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
        OUTPUT_NAME "${APP_NAME}"
    )
endfunction()

# Build for AARCH64
build_platform("aarch64" ${AARCH64_C_COMPILER} ${AARCH64_CXX_COMPILER})

# Build for ARM32
build_platform("arm" ${ARM32_C_COMPILER} ${ARM32_CXX_COMPILER})