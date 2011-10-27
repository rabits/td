project(td CXX)

cmake_minimum_required(VERSION 2.8)

set(TARGET_VERSION_MAJOR "0")
set(TARGET_VERSION_MINOR "1")
set(TARGET_VERSION_PATCH "0")

set(CONFIG_TD_FULLNAME "Total Destruction")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Choose the type of build, options are: Debug Release."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

message("Making ${CMAKE_BUILD_TYPE} configuration. Version: ${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}.${TARGET_VERSION_PATCH}")

#
# Adding sources
#

file(GLOB_RECURSE td_SRCS "src/*.cpp" "lib/*.cpp")

#
# Finding requirement libs
#

include(FindPkgConfig)

if(${PKG_CONFIG_FOUND})
    pkg_check_modules(OGRE OGRE>=1.7.3)
    pkg_check_modules(OIS OIS>=1.3.0)
    pkg_check_modules(BULLET bullet>=2.79)
    find_package( Boost 1.46.1 COMPONENTS filesystem system )
else()
    message(FATAL_ERROR "pkg-config NOT FOUND")
endif()

message("-------------------------------------------")

if(NOT OGRE_FOUND)
    message("OGRE NOT FOUND")
endif()

if(NOT OIS_FOUND)
    message("OIS NOT FOUND")
endif()

if(NOT BULLET_FOUND)
    message("bullet NOT FOUND")
endif()

if(NOT Boost_SYSTEM_FOUND)
    message("Boost system NOT FOUND")
endif()

if(NOT Boost_FILESYSTEM_FOUND)
    message("Boost filesystem NOT FOUND")
endif()

if(NOT (OGRE_FOUND AND OIS_FOUND AND BULLET_FOUND AND Boost_SYSTEM_FOUND AND Boost_FILESYSTEM_FOUND))
    message(FATAL_ERROR "Error: some libraries not found")
endif()

#
# Some options
#

# Paths
if(NOT CONFIG_TD_NAME)
    set(CONFIG_TD_NAME "td" CACHE STRING
        "Target binary and share install directory name"
        FORCE)
endif(NOT CONFIG_TD_NAME)
if(NOT CONFIG_PATH_BIN)
    set(CONFIG_PATH_BIN "games" CACHE PATH
        "Target path to install binary (CMAKE_INSTALL_PREFIX/games)"
        FORCE)
endif(NOT CONFIG_PATH_BIN)
if(NOT CONFIG_PATH_DATA)
    set(CONFIG_PATH_DATA "share/${CONFIG_TD_NAME}" CACHE PATH
        "Target path to install global data (CMAKE_INSTALL_PREFIX/share/${CONFIG_TD_NAME})"
        FORCE)
endif(NOT CONFIG_PATH_DATA)
if(NOT CONFIG_PATH_ETC)
    set(CONFIG_PATH_ETC "etc/${CONFIG_TD_NAME}" CACHE PATH
        "Target path to install global config (CMAKE_INSTALL_PREFIX/etc/${CONFIG_TD_NAME})"
        FORCE)
endif(NOT CONFIG_PATH_ETC)

# Options

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(CONFIG_DEBUG "Use force feedback for joysticks (ON)" ON)
else()
    option(CONFIG_DEBUG "Use force feedback for joysticks (OFF)" OFF)
endif()
option(CONFIG_JOYSTICK_USE_FORCEFEEDBACK "Use force feedback for joysticks (OFF)" OFF)
if(NOT CONFIG_JOYSTICK_MAX_NUMBER)
    set(CONFIG_JOYSTICK_MAX_NUMBER "4" CACHE PATH
        "Max number of joysticks (4)"
        FORCE)
endif(NOT CONFIG_JOYSTICK_MAX_NUMBER)

if(NOT CONFIG_OGRE_PLUGINS_PATH)
    set(CONFIG_OGRE_PLUGINS_PATH ${OGRE_LIBRARY_DIRS}/OGRE CACHE PATH
        "Max number of joysticks (${OGRE_LIBRARY_DIRS}/OGRE)"
        FORCE)
endif(NOT CONFIG_OGRE_PLUGINS_PATH)

#
# CPack
#

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "My own game")
set(CPACK_PACKAGE_VENDOR "Rabits")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
set(CPACK_PACKAGE_VERSION_MAJOR ${TARGET_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${TARGET_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${TARGET_VERSION_PATCH})
set(CPACK_SOURCE_IGNORE_FILES /build/;/.git/;/.gitignore)
set(CPACK_PACKAGE_INSTALL_DIRECTORY "TD ${TARGET_VERSION_MAJOR}.${TARGET_VERSION_MINOR}")
set(CPACK_GENERATOR TBZ2)
set(CPACK_SOURCE_GENERATOR TBZ2)
if(WIN32 AND NOT UNIX)
    # There is a bug in NSI that does not handle full unix paths properly. Make
    # sure there is at least one set of four (4) backlasshes.
    #SET(CPACK_PACKAGE_ICON "${CMake_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
    #SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\MyExecutable.exe")
    #SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} My Famous Project")
    #SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.my-project-home-page.org")
    #SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.my-personal-home-page.com")
    #SET(CPACK_NSIS_CONTACT "me@my-personal-home-page.com")
    #SET(CPACK_NSIS_MODIFY_PATH ON)
else(WIN32 AND NOT UNIX)
    #set(CPACK_STRIP_FILES "bin/MyExecutable")
    #set(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_PACKAGE_EXECUTABLES ${CONFIG_TD_NAME} "Total Destruction")

include(CPack)

#
# Adding binaries
#

message("Install to: ${CMAKE_INSTALL_PREFIX}")

add_executable(${CONFIG_TD_NAME} ${td_SRCS})

    set(TARGET_LD_FLAGS "${OGRE_LDFLAGS};${OIS_LDFLAGS};${BULLET_LDFLAGS}")
    message("Linked: ${TARGET_LD_FLAGS}")
    target_link_libraries(${CONFIG_TD_NAME} ${TARGET_LD_FLAGS} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

    set(TARGET_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/src;${CMAKE_CURRENT_BINARY_DIR}/config")
    set(SYSTEM_INCLUDE_DIRS "${OGRE_INCLUDE_DIRS};${OIS_INCLUDE_DIRS};${BULLET_INCLUDE_DIRS};${CMAKE_CURRENT_SOURCE_DIR}/lib")
    message("Included: ${TARGET_INCLUDE_DIRS} ${SYSTEM_INCLUDE_DIRS}")
    include_directories(include ${TARGET_INCLUDE_DIRS} SYSTEM ${SYSTEM_INCLUDE_DIRS})

#
# Seting CXX GCC flags
#

add_definitions( -std=c++0x -Wall )
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions( -Wextra -Weffc++ -Woverloaded-virtual -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code -O2 -g )
    find_program( CPPCHECK_EXEC "cppcheck" )
else()
    add_definitions( -s -O2 -DNDEBUG )
endif()

#
# Adding subdirs
#

#add_subdirectory(src)
#add_subdirectory(include)

#
# Writing config files
#

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake_in/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config/config.h)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake_in/config.xml.in ${CMAKE_CURRENT_BINARY_DIR}/config/config.xml)

#
# Adding Make targets
#

# install
install(TARGETS ${CONFIG_TD_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/${CONFIG_PATH_BIN})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/data DESTINATION ${CMAKE_INSTALL_PREFIX}/${CONFIG_PATH_DATA})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/share/users DESTINATION ${CMAKE_INSTALL_PREFIX}/${CONFIG_PATH_DATA})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config/config.xml DESTINATION ${CMAKE_INSTALL_PREFIX}/${CONFIG_PATH_ETC})

# Cppcheck execute if found and Debug mode
if(CPPCHECK_EXEC AND CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CPPCHECK_SRC ${td_SRCS})
    string(REPLACE ";" ";-I;" CPPCHECK_INCLUDE_PARAMS "${TARGET_INCLUDE_DIRS};${SYSTEM_INCLUDE_DIRS}")
    add_custom_target(cppcheck ALL ${CPPCHECK_EXEC} -f --enable=style --enable=all --verbose --template gcc -I ${CPPCHECK_INCLUDE_PARAMS} ${CMAKE_CURRENT_SOURCE_DIR}/src
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Analyzing source code by cppcheck" VERBATIM)
endif()

# Doxygen documentation
find_package(Doxygen)

if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake_in/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif()

# uninstall
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_in/uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/uninstall.cmake)
