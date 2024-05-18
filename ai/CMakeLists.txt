
add_subdirectory(heuristics)

set(
	AI_SRCS
	${HEURISTICS_SRCS}
	"${CMAKE_CURRENT_LIST_DIR}/ai_utils.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/Agent.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/Agent.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Node.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Node.hpp"
	"${CMAKE_CURRENT_LIST_DIR}/Problem.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/Problem.hpp"
	PARENT_SCOPE
)
