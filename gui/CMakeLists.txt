
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED on)

# set(CMAKE_AUTOMOC on) # codice generato prima della compilazione
# set(CMAKE_AUTORCC on) # resource compiler (immagini icone, etc.)
# set(CMAKE_AUTOUIC on) # NON parsa il file xml per generare l'UI

# find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(
	GUILIB_SRCS
	"./gui/Window.hpp"
	"./gui/Window.cpp"
	"./gui/Content.hpp"
	"./gui/Content.cpp"
	"./gui/MenuView.hpp"
	"./gui/MenuView.cpp"
	"./gui/Page.hpp"
	"./gui/SolveView.hpp"
	"./gui/SolveView.cpp"
	"./gui/BoardView.hpp"
	"./gui/BoardView.cpp"
	"./gui/TileView.hpp"
	"./gui/TileView.cpp"
	"./gui/utils/utils.hpp"
	"./gui/utils/utils.cpp"
	"./gui/utils/CustomDialog.hpp"
	"./gui/utils/CustomDialog.cpp"
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
