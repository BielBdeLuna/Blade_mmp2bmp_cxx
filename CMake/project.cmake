#	this file is GPLv3

set( SRC_DIR ${PROJECT_BASE}/src )
set( SYS_DIR ${SRC_DIR}/sys )
set( FRMW_DIR ${SRC_DIR}/framework )

set( BIN_RELEASE_DIR ${PROJECT_BASE}/release )


option( DEBUG_BUILD "build as debug" ON )
option( SYSTEM_NATIVE "build native to the system" ON )

#g++ flags

set( GXX_STANDARD "-std=c++20 " )
set( GXX_ARCHITECTURE "-march=native " )


set( CMAKE_CXX_FLAGS 
			${GXX_STANDARD}
			"-fpermissive "
	 )


if( SYSTEM_NATIVE )
	message( STATUS "setting up a native build.\n" )
	set( CMAKE_CXX_FLAGS ${GXX_ARCHITECTURE} )
endif()

if( DEBUG_BUILD )
	message( STATUS "setting up a debug build.\n" )
	set( CMAKE_BUILD_TYPE Debug )
endif()


# HEADERS
set( TARGET_HEADERS
			${SYS_DIR}/error.hxx
			${FRMW_DIR}/bitmap.hxx
	 )
	 
# SOURCES	 
set( TARGET_SOURCES
			${SYS_DIR}/error.cxx
			${FRMW_DIR}/bitmap.cxx
			${SYS_DIR}/main.cxx
	 )

# TARGET
set( EXE_NAME MMPdumper )
add_executable( ${EXE_NAME} ${TARGET_HEADERS} ${TARGET_SOURCES} )

set_target_properties( ${EXE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_RELEASE_DIR} )