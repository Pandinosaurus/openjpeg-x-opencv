CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT( OpenJpegTrial )

# Check system type by counting bytes
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
    SET( EX_PLATFORM "64" )
    SET( EX_PLATFORM_NAME "x64" )
else()
    MESSAGE( "32 bits compiler detected" )
    SET( EX_PLATFORM "86" )
    SET( EX_PLATFORM_NAME "x86" )
endif()

# Instruct CMake to prepare files for c++11
SET(CMAKE_CXX_FLAGS "-std=c++11")

# Find includes in corresponding build directories
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find useful libs
FIND_PACKAGE( OpenCV )
FIND_PACKAGE( OpenJPEG )

# !!!!!!!!! You may need to change the include directories according to your install of OpenCV an OpenJPEG !!!!!!!!!
# !!!!!!!!! Here, we demonstrate OPENJPEG_INCLUDE_DIRS macro AND absolute path !!!!!!!!!
#Includes are used at compilation time
#At runtime, dlls are searched if no static lib are embedded
#if dlls not found, search the path
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${OPENJPEG_INCLUDE_DIRS}
		    "D:/lib/openjpeg/build${EX_PLATFORM}/src/lib/openjp2"
		    "D:/lib/openjpeg/build${EX_PLATFORM}/src/bin/common"
	  	    "D:/lib/openjpeg/src/lib/"
		    "D:/lib/openjpeg//src/bin/")

FILE(GLOB SRC	
	  "*.cpp"
)

add_executable(${PROJECT_NAME}
               ${SRC})

# !!!!!!!!! You may need to change the paths according to your install of OpenCV an OpenJPEG !!!!!!!!!
# !!!!!!!!! Here, we demonstrate OPENJPEG macro - doesn't work on my setup - and absolute path examples !!!!!!!!!
target_link_libraries(${PROJECT_NAME}
                      ${OpenCV_LIBS} 
		      #${OPENJPEG_LIBRARIES}
		      "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ojpg230/msvc2015_${EX_PLATFORM_NAME}/Release/*.lib")


#Add postbuild commands
# !!!!!!!!! You may need to change the paths according to your install of OpenCV an OpenJPEG !!!!!!!!!
# !!!!!!!!! Here, we demonstrate OPENJPEG_INCLUDE_DIRS macro AND absolute path !!!!!!!!!
#For OpenCV dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
"${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ocv331/msvc2015_${EX_PLATFORM_NAME}/Release/"
"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
#For OpenJpeg dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
"${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ojpg230/msvc2015_${EX_PLATFORM_NAME}/Release/"
"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)
