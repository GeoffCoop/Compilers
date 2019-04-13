cmake_minimum_required(VERSION 2.8)
project(cpsl)

find_package(BISON)
find_package(FLEX)

bison_target(Parser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
flex_target(Scanner scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
add_flex_bison_dependency(Scanner Parser)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/Code
)

set(EXTRA_COMPILE_FLAGS "-g3 -std=c++14")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")

set (cpsl_srcs
	main.cpp
	${BISON_Parser_OUTPUTS}
	${FLEX_Scanner_OUTPUTS}
)

set(frontend_srcs
	Code/FrontEnd.hpp
	Code/FrontEnd.cpp
)

list(APPEND cpsl_srcs ${frontend_srcs})

set(code_srcs
	Code/SymbolTable.hpp
	Code/SymbolTable.cpp
	Code/Symbol.hpp
	Code/Type.hpp
	Code/Type.cpp
	Code/LValue.hpp
	Code/LValue.cpp
	Code/Program.hpp
	Code/ThreeAddressInstruction.hpp
	Code/ThreeAddressInstruction.cpp
	Code/StringTable.hpp
	Code/StringTable.cpp
	Code/RegAllocation.hpp
)

list(APPEND cpsl_srcs ${code_srcs})

set(statement_srcs
	Code/Statements/AssignStatement.hpp
	Code/Statements/ForStatement.hpp
	Code/Statements/IfStatement.hpp
	Code/Statements/ProcCall.hpp
	Code/Statements/ReadStatement.hpp
	Code/Statements/ReturnStatement.hpp
	Code/Statements/RepeatStatement.hpp
	Code/Statements/Statement.hpp
	Code/Statements/StopStatement.hpp
	Code/Statements/WhileStatement.hpp
	Code/Statements/WriteStatement.hpp
)

list(APPEND cpsl_srcs ${statement_srcs})

set(expression_srcs
	Code/Expressions/add.hpp
	Code/Expressions/and.hpp
	Code/Expressions/charExpression.hpp
	Code/Expressions/chr.hpp
	Code/Expressions/const.hpp
	Code/Expressions/div.hpp
	Code/Expressions/eq.hpp
	Code/Expressions/Expression.hpp
	Code/Expressions/Expression.cpp
	Code/Expressions/functionCall.hpp
	Code/Expressions/gte.hpp
	Code/Expressions/gt.hpp
	Code/Expressions/intExpression.hpp
	Code/Expressions/lte.hpp
	Code/Expressions/lt.hpp
	Code/Expressions/lvalueexpr.hpp
	Code/Expressions/mod.hpp
	Code/Expressions/mult.hpp
	Code/Expressions/ne.hpp
	Code/Expressions/not.hpp
	Code/Expressions/ord.hpp
	Code/Expressions/or.hpp
	Code/Expressions/pred.hpp
	Code/Expressions/stringExpression.hpp
	Code/Expressions/sub.hpp
	Code/Expressions/succ.hpp
	Code/Expressions/umin.hpp
)

list(APPEND cpsl_srcs ${expression_srcs})

source_group("MyCpsl" FILES ${cpsl_srcs})

add_executable(cpsl ${cpsl_srcs})
target_link_libraries(cpsl ${FLEX_LIBRARIES} ${BISON_LIBRARIES})
