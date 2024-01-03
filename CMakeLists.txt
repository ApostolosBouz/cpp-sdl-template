cmake_minimum_required(VERSION 3.21)

# Configuration bug on Apple, that requires this check before 'project' command.
if (APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
  set(CMAKE_OSX_ARCHITECTURES "$(ARCHS_STANDARD)")
endif ()

# Change the first parameter (name of your .exe) and last parameter (in DESCRIPTION "" quotes) to whatever.
project(SDL_Game
        VERSION 0.0.1
        LANGUAGES CXX
        DESCRIPTION "Windows SDL / C++ Template")

# Setting C++ Standards and disabling extensions for all targets
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a debug condiguration if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

# https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Available.md

set(msvc_warnings
  /W4 # Baseline reasonable warnings
  /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
  /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
  /w14263 # 'function': member function does not override any base class virtual member function
  /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
  /w14287 # 'operator': unsigned/negative constant mismatch
  /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
  /w14296 # 'operator': expression is always 'boolean_value'
  /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
  /w14545 # expression before comma evaluates to a function which is missing an argument list
  /w14546 # function call before comma missing argument list
  /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
  /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
  /w14555 # expression has no effect; expected expression with side- effect
  /w14619 # pragma warning: there is no warning number 'number'
  /w14640 # Enable warning on thread un-safe static member initialization
  /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
  /w14905 # wide string literal cast to 'LPSTR'
  /w14906 # string literal cast to 'LPWSTR'
  /w14928 # illegal copy-initialization; more than one user-defined conversion has been implicitly LearnSDL.exelied
  /permissive- # standards conformance mode for MSVC compiler.
  )
set(clang_warnings
  -Wall
  -Wextra # reasonable and standard
  -Wshadow # warn the user if a variable declaration shadows one from a parent context
  -Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual destructor. This helps catch hard to track down memory errors
  -Wcast-align # warn for potential performance problem casts
  -Wunused # warn on anything being unused
  -Woverloaded-virtual # warn if you overload (not override) a virtual function
  -Wpedantic # warn if non-standard C++ is used
  -Wconversion # warn on type conversions that may lose data
  -Wsign-conversion # warn on sign conversions
  -Wnull-dereference # warn if a null dereference is detected
  -Wformat=2 # warn on security issues around functions that format output (ie printf)
  )
set(gcc_warnings
  ${clang_warnings}
  -Wmisleading-indentation # warn if indentation implies blocks where blocks
  #  Clang in Windows issues (must not use for now):
  # -Wduplicated-branches # warn if if / else branches have duplicated code
  # -Wduplicated-cond # warn if if / else chain has duplicated conditions
  # -Wlogical-op # warn about logical operations being used where bitwise were
  # -Wuseless-cast # warn if you perform a cast to the same type
  )

if (MSVC)
  set(PROJECT_WARNINGS ${msvc_warnings})
elseif (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  set(PROJECT_WARNINGS ${clang_warnings})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(PROJECT_WARNINGS ${gcc_warnings})
else ()
  message(AUTHOR_WARNING "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
endif ()

# Setting Output Directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Enable project folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

# Game's 3rd party dependencies
add_subdirectory(extern)

# Game's entry point file
set(entry_point "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# Game's header files
file(GLOB game_source_files
"${CMAKE_CURRENT_SOURCE_DIR}/src/Private/*.cpp")

# Game's header files
file(GLOB game_header_files
"${CMAKE_CURRENT_SOURCE_DIR}/src/Public/*.hpp")

# Game's includes root location
set(game_header_dir "${CMAKE_CURRENT_SOURCE_DIR}/src/Public")

# Game's icon and various resource files
file(GLOB game_resource_files
"${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.ico"
"${CMAKE_CURRENT_SOURCE_DIR}/Resources/*.rc")

# Game's configuration and various files
file(GLOB game_config_files
"${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt"
"${CMAKE_CURRENT_SOURCE_DIR}/README.md"
"${CMAKE_CURRENT_SOURCE_DIR}/.gitignore"
"${CMAKE_CURRENT_SOURCE_DIR}/.gitattributes"
"${CMAKE_CURRENT_SOURCE_DIR}/.gitmodules")

# Game's filter groups for Visual Studio
source_group("Sources"          FILES  ${entry_point})
source_group("Sources/Private"  FILES  ${game_source_files})
source_group("Sources/Public"   FILES  ${game_header_files})
source_group("Configuration"    FILES  ${game_config_files})
source_group("Resources"        FILES  ${game_resource_files})

if(WIN32)
  add_executable(${PROJECT_NAME} WIN32
        ${entry_point}
        ${game_source_files}
        ${game_header_files}
        ${game_resource_files}
  )
endif()

if(APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${entry_point}
        ${game_source_files}
        ${game_header_files}
        ${game_resource_files}
  )
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${game_header_dir})

# Add the libraries to your target
target_link_libraries(${PROJECT_NAME}
  PRIVATE
      SDL2::SDL2-static
      SDL2_image::SDL2_image-static
      SDL2_mixer
      SDL2_ttf
      SDL2_net
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2main)
endif()

# Compile options
target_compile_options(${PROJECT_NAME} PRIVATE ${PROJECT_WARNINGS})

# Set LearnSDL.exe as the Startup project on Visual Studio
if(MSVC)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    # Tells Visual Studio to set the working directory to the directory where the target's executable will be outputted.
    # That should be the same location where the assets will be copied (see custom commands at the end).
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
endif()

# Copy assets/ with its contents, to CMAKE_RUNTIME_OUTPUT_DIRECTORY
add_custom_command(
    TARGET ${PROJECT_NAME} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)