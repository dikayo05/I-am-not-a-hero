cmake_minimum_required(VERSION 3.28)
project(IAmNotAHero VERSION 1.0.5 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SFML via FetchContent ---
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# --- Source and Executable ---
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(main ${SOURCES})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

# --- Copy Assets After Build ---
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:main>/assets
)

# --- Start Installation and Packages Section ---

# --- Installation Rules ---
include(GNUInstallDirs)

install(TARGETS main
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY assets
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${CMAKE_SOURCE_DIR}/assets/icon.ico
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# --- CPack Configuration ---

# Set Your Own Game Version
set(GAME_VERSION "${PROJECT_VERSION}")

# Installer file name that you have full control over
set(CPACK_PACKAGE_NAME "IAmNotAHero")
set(CPACK_PACKAGE_VERSION "${GAME_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-win64")

# NSIS Installer Settings
set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_INSTALLER_ICON "${CMAKE_SOURCE_DIR}/assets/icon.ico")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/assets/icon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/assets/icon.ico")
set(CPACK_PACKAGE_CONTACT "yonandaputra05@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "IAmNotAHero - A 2D Adventure Game using SFML")

# Shortcut and Search
set(CPACK_CREATE_DESKTOP_LINKS "IAmNotAHero")
set(CPACK_NSIS_MENU_LINKS "bin/main.exe" "IAmNotAHero")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_NSIS_PACKAGE_NAME "IAmNotAHero")

# Icon and path
set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
set(CPACK_NSIS_INSTALLED_ICON_NAME "icon.ico")

# Registry and PATH
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MODIFY_PATH ON)

include(CPack)
# --- End of Installation and Packages Section ---