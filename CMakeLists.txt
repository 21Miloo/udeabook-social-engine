cmake_minimum_required(VERSION 3.16)

project(udeabook LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(udeabook main.cpp
    DynamicList.h
    photo.h photo.cpp
    privacy.h
    album.h album.cpp
    menu.h menu.cpp
    resourcemeter.h resourcemeter.cpp
)

include(GNUInstallDirs)
install(TARGETS udeabook
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
