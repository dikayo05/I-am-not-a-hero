cmake_minimum_required(VERSION 3.28)
project(IAmNotAHero LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(main ${SOURCES})

target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio)

# Copy assets
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:main>/assets
)

# --- Start Installation and Packages Section ---

# 1. Include the module for standard installation
include(GNUInstallDirs)

# 2. Rules for installing the Target (your .exe file)
#    This will install "main.exe" into the "bin" folder inside the installation directory
install(TARGETS main
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 3. Rules for installing the Assets folder
# This will install the entire "assets" folder into the "bin" folder
# so the location becomes "bin/assets/..."
# This is IMPORTANT so that your relative path "assets/images/..." keeps working.
install(DIRECTORY assets
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 4. Include CPack and SFML's CPack module
# (This module exists because you are using FetchContent)
include(CPack)

# 5. (Optional) Settings for the NSIS installer
set(CPACK_GENERATOR "NSIS")
set(CPACK_PACKAGE_NAME "IAmNotAHero")
set(CPACK_NSIS_INSTALLER_ICON "${CMAKE_SOURCE_DIR}/assets/icon.ico")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/assets/icon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/assets/icon.ico")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_NSIS_MENU_LINKS "bin/main.exe" "IAmNotAHero")
set(CPACK_CREATE_DESKTOP_LINKS "IAmNotAHero")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "yonandaputra05@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "IAmNotAHero - A 2D Adventure Game using SFML")

# --- End of Installation and Packages Section ---