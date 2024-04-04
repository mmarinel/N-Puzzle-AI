cmake_minimum_required(VERSION 3.18)

project(npuzzle VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_AUTOMOC on) # codice generato prima della compilazione
set(CMAKE_AUTORCC on) # resource compiler (immagini icone, etc.)
set(CMAKE_AUTOUIC on) # NON parsa il file xml per generare l'UI

find_package(Qt6 REQUIRED COMPONENTS Qml)

add_executable(
	npuzzle
	main.cpp
)

target_link_libraries(npuzzle PRIVATE Qt6::Qml)

# per portabilità
set_target_properties(npuzzle PROPERTIES
	WIN32_EXECUTABLE ON
	MACOSX_BUNDLE ON
)
