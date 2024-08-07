#	this file is GPLv3

# Blade Of Darkness texture exporter

cmake_minimum_required( VERSION 3.7 )
message( STATUS "\t\t..::Blade Of Darkness texture dumper::..\n" )

set( PROJECT_NAME MMPdumper )

project( ${PROJECT_NAME} VERSION 1.0 )

set( PROJECT_BASE ${CMAKE_CURRENT_LIST_DIR} )
set( CMAKE_DIR ${PROJECT_BASE}/CMake )

include( ${CMAKE_DIR}/project.cmake )
