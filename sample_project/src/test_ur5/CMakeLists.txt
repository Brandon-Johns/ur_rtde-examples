# Mid/Low-level CMake project file
#	Project specific logic: include source and define output
cmake_minimum_required (VERSION 3.8)

####################################################################################
# All dependancies
##########################################
set(LIBRARIES
	${URRTDE_Lib}
)

set(LIBRARIES_DIR 
)

set(INCLUDES_DIR 
)

set(INCLUDES_LOCAL_DIR 
	${CMAKE_CURRENT_LIST_DIR} # .h files in the same directory as the target .cpp file
)

####################################################################################
# Build
##########################################
# Source files containing main()
#	set(BJ_Sources <exe1> [exe2] ...)
set(BJ_Sources "MoveSpeedj" "MoveToHome" "MoveServoj")

# All other .cpp files
set(BJ_Dependencies "BJ_Kill.cpp")

foreach(BJ_Source ${BJ_Sources})
	# Choose Output filename
	set(BJ_ExeName "${BJ_Source}")

	# Libraries to link against - Directories
	if(WIN32)
		#Nothing
	else() 
		link_directories(${LIBRARIES_DIR})
	endif()

	# Link source files to output file (<exeName> <source1.c> [source2.c|.h] ...)
	add_executable(${BJ_ExeName} ${BJ_Source}.cpp ${BJ_Dependencies})

	# Directories to include
	target_include_directories(${BJ_ExeName} PRIVATE ${INCLUDES_DIR} ${INCLUDES_LOCAL_DIR})

	# Libraries to link against - Libraries
	target_link_libraries(${BJ_ExeName} PUBLIC ${LIBRARIES})
endforeach()
