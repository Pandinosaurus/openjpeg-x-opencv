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


FILE(GLOB dlls 
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ocv331/msvc2015_${EX_PLATFORM_NAME}/Release/opencv_core331.dll"
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ocv331/msvc2015_${EX_PLATFORM_NAME}/Release/opencv_imgproc331.dll"
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ocv331/msvc2015_${EX_PLATFORM_NAME}/Release/opencv_imgcodecs331.dll"
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ocv331/msvc2015_${EX_PLATFORM_NAME}/Release/opencv_highgui331.dll"
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ojpg230/msvc2015_${EX_PLATFORM_NAME}/Release/*.dll"
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ojpg230/msvc2015_${EX_PLATFORM_NAME}/Release/*.lib")

FILE(GLOB platformsplugins
	  "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/qt510/msvc2015_${EX_PLATFORM_NAME}/plugins/platforms/qwindows.dll")

add_executable(${PROJECT_NAME}
               ${SRC}
	       ${RESSOURCES})

target_link_libraries(${PROJECT_NAME}
                      ${OpenCV_LIBS} 
		      #${OPENJPEG_LIBRARIES}
		      "${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/ojpg230/msvc2015_${EX_PLATFORM_NAME}/Release/*.lib")

# Instruct CMake to use NSIS with CPACK to create an intaller
SET(CPACK_GENERATOR NSIS)
SET(CPACK_PACKAGE_NAME "OpenJpegTrial")
SET(CPACK_PACKAGE_VENDOR "CMake.org")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "OpenJpegTrial - Because we need it")
SET(CPACK_PACKAGE_VERSION "0.0.1")
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "1")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY ${PROJECT_NAME})
SET(CPACK_NSIS_MODIFY_PATH ON)

# TARGETS are executables, they should be located on the side of the dlls
install(TARGETS OpenJpegTrial    
       
	RUNTIME DESTINATION bin
    
	COMPONENT app)

# Install VC redist (see on visual studio website)
install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/../redistributables/vc2015/vc_redist_2015_${EX_PLATFORM_NAME}.exe
	DESTINATION tmp
	COMPONENT redistributables)	

# Our dll
install(FILES ${dlls}
	DESTINATION bin
	COMPONENT dependencies)


INCLUDE(CPack) #should be at the end
cpack_add_install_type(Full DISPLAY_NAME "Install everything")
cpack_add_install_type(Extra DISPLAY_NAME "Install lib for developers")
cpack_add_component(app
  		    INSTALL_TYPES Full)
cpack_add_component(redistributables
  		    INSTALL_TYPES Full)
cpack_add_component(dependencies 
		    INSTALL_TYPES Full)
#cpack_add_component(lib
#  		    INSTALL_TYPES Full Extra)     #Define that component "lib" is installed when selecting "Full" and "Extra" type


#Add postbuild commands
#L'avantage, c'est quand on nettoie ça nettoie vraiment tout ce qu'il faut
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