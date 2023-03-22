cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(dvrui VERSION 0.1.0 LANGUAGES CXX C)

set(TARGET_FILE "dvrui")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if (Qt5Widgets_FOUND)
	message(STATUS "Qt5Widgets_VERSION : ${Qt5Widgets_VERSION}")
	message(STATUS "Qt5Widgets_LIBRARIES : ${Qt5Widgets_LIBRARIES}")
	message(STATUS "Qt5Widgets_INCLUDE_DIRS : ${Qt5Widgets_INCLUDE_DIRS}")
	message(STATUS "Qt5Widgets_DEFINITIONS : ${Qt5Widgets_DEFINITIONS}")
	message(STATUS "Qt5Widgets_COMPILE_DEFINITIONS : ${Qt5Widgets_COMPILE_DEFINITIONS}")
else ()
	message(FATAL_ERROR "Qt5Widgets not found. Please install first.")
endif ()

file(GLOB UI_FILES "src/*.ui")
file(GLOB INC_FILES "src/*.h")
file(GLOB SRC_FILES "src/*.cpp")

qt5_wrap_ui(UI_HEADERS ${UI_FILES})

include_directories(
	"include"
)

link_directories(
	"libs"
)

message(STATUS "UI_FILES : ${UI_FILES}")
message(STATUS "UI_HEADERS : ${UI_HEADERS}")
message(STATUS "SRC_FILES : ${SRC_FILES}")

add_executable(${TARGET_FILE} 
  ${INC_FILES}
  ${UI_HEADERS}
  ${SRC_FILES}
  "dvrui.qrc"
)

target_link_libraries(${TARGET_FILE} 
	Qt5::Widgets
	asound
	osa
	tiv
	dvr
)
