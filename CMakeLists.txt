#EXECUTABLE_OUTPUT_PATH
#LIBRARY_OUTPUT_PATH
#PROJECT_BINARY_DIR
#PROJECT_SOURCE_DIR   
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (rpc)
ADD_CUSTOM_COMMAND(OUTPUT rpc.tab.hpp lex.yy.cpp rpc.tab.cpp
		COMMAND flex  -o parser/lex.yy.cpp rpc.l
		COMMAND bison -d -o parser/rpc.tab.cpp rpc.y
	)
SET(HEADER_FILES
	parser/DefType.h
	parser/Global.h
	parser/Program.h
	generator/As3Generator.h
	generator/CppGenerator.h
	generator/GoGenerator.h
	generator/CSharpGenerator.h
	generator/TSharpGenerator.h
	generator/Generator.h
	misc/md5.h
	parser/rpc.tab.hpp
	)
SET(SRC_FILES
	parser/DefType.cpp
	parser/Global.cpp
	parser/Program.cpp
	generator/As3Generator.cpp
	generator/CppGenerator.cpp
	generator/GoGenerator.cpp
	generator/CSharpGenerator.cpp
	generator/TSharpGenerator.cpp
	generator/Generator.cpp
	misc/md5.cpp
	parser/lex.yy.cpp 
	parser/rpc.tab.cpp
	main.cpp
	)
ADD_EXECUTABLE(rpc ${SRC_FILES} )
add_definitions(-std=c++14)
