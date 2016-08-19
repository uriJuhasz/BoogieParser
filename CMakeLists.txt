cmake_minimum_required(VERSION 3.6)
project(BoogieParser)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
        src/main.cpp
        src/util/Trie.cpp
        src/BoogieParser/BoogieLexer.cpp
        src/BoogieParser/ParserBase.cpp
        src/BoogieParser/Position.cpp)
add_executable(BoogieParser ${SOURCE_FILES})