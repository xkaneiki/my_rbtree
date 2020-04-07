cmake_minimum_required(VERSION 3.10)

project(myrbtree)

add_library(myrbtree myrbtree.c)
target_include_directories(myrbtree 
    PUBLIC ${PROJECT_SOURCE_DIR}
)

add_executable(main main.c)

target_link_libraries(main
    PRIVATE myrbtree
)
