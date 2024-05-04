cmake_minimum_required(VERSION 3.18)

set(EXECUTABLE_NAME npuzzle)

project(${EXECUTABLE_NAME} VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
# Set for a Debug Build
# set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_AUTOMOC on) # codice generato prima della compilazione
set(CMAKE_AUTORCC on) # resource compiler (immagini icone, etc.)
set(CMAKE_AUTOUIC on) # NON parsa il file xml per generare l'UI



find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_subdirectory(gui)
add_subdirectory(heuristics)
add_subdirectory(utils)

qt_add_executable(
	${EXECUTABLE_NAME}
	main.cpp
	resources/styleResources.qrc
	${GUILIB_SRCS}
	${HEURISTICSLIB_SRCS}
	${UTILS_SRCS}
)

target_link_libraries(
	${EXECUTABLE_NAME}
	PRIVATE
	Qt6::Widgets
	Qt6::Core
)

target_include_directories(
	${EXECUTABLE_NAME}
	PRIVATE
	${Qt6Widgets_INCLUDE_DIRS}
	"./gui/"
	"./gui/gui_utils"
	"./gui/gui_state"
	"./heuristics"
	"./utils"
)

# per portabilità
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
