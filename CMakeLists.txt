#EXECUTABLE_OUTPUT_PATH
#LIBRARY_OUTPUT_PATH
#PROJECT_BINARY_DIR
#PROJECT_SOURCE_DIR   
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT (rpc)
ADD_CUSTOM_COMMAND(OUTPUT rpc.tab.hpp lex.yy.cpp rpc.tab.cpp
		COMMAND flex  -olex.yy.cpp rpc.l
		COMMAND bison -d -o rpc.tab.cpp rpc.y
	)
SET(HEADER_FILES
	parser/DefType.h
	parser/Global.h
	parser/Program.h
	generator/As3Generator.h
	generator/CppGenerator.h
	generator/Generator.h
	rpc.tab.hpp
	md5.h
	)
SET(SRC_FILES
	parser/DefType.cpp
	parser/Global.cpp
	parser/Program.cpp
	generator/As3Generator.cpp
	generator/CppGenerator.cpp
	generator/Generator.cpp
	lex.yy.cpp 
 	rpc.tab.cpp
	md5.cpp
	main.cpp
	)
ADD_EXECUTABLE(rpc ${SRC_FILES} )
