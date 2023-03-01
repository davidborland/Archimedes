cmake_minimum_required(VERSION 3.0)

project(Archimedes)

#######################################
# Include Qt
#######################################

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5GUI)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CPP_FILES *.cpp)

qt5_wrap_ui(UISrcs ${UI_FILES} )

#######################################
# Include OpenGL
#######################################
find_package(OpenGL)

#######################################
# Include HDAL (Novint Falcon library)
#######################################

# $ENV{NOVINT_DEVICE_SUPPORT} doesn't seem to be working as advertised.
# Need to set this manually
FIND_PATH( HDAL_ROOT_DIR $ENV{NOVINT_DEVICE_SUPPORT}/include/hdl/hdl.h )

INCLUDE_DIRECTORIES( ${HDAL_ROOT_DIR}/include )
LINK_DIRECTORIES( ${HDAL_ROOT_DIR}/lib )

SET(HDAL_LIB hdl.lib)

#######################################
# Create executable
#######################################

add_executable(Archimedes MACOSX_BUNDLE ${CPP_FILES} ${UISrcs} ${QT_WRAP})
qt5_use_modules(Archimedes Widgets Gui OpenGL)
target_link_libraries(Archimedes ${QT_LIBRARIES} ${OPENGL_LIBRARIES} ${HDAL_LIB})
