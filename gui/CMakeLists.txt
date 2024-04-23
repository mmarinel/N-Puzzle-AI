
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED on)

# set(CMAKE_AUTOMOC on) # codice generato prima della compilazione
# set(CMAKE_AUTORCC on) # resource compiler (immagini icone, etc.)
# set(CMAKE_AUTOUIC on) # NON parsa il file xml per generare l'UI

# find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(
	GUILIB_SRCS
	"${CMAKE_CURRENT_LIST_DIR}/Window.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/Window.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Content.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/Content.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/MenuView.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/MenuView.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Page.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/SolveView.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/SolveView.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/BoardView.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/BoardView.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/TileView.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/TileView.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/gui_utils/gui_utils.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/gui_utils/gui_utils.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/gui_utils/CustomDialog.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/gui_utils/CustomDialog.cpp"
	PARENT_SCOPE
)

# add_library(
# 	${GUILIB_NAME}
# 	STATIC
# 	${GUILIB_SRCS}
# )

# target_link_libraries(
# 	${GUILIB_NAME}
# 	PRIVATE
# 	Qt6::Widgets
# )

# target_include_directories(
# 	${GUILIB_NAME}
# 	PUBLIC
# 	${CMAKE_CURRENT_SOURCE_DIR}
# )
