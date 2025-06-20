cmake_minimum_required(VERSION 3.10)
project(crow_server)

set(CMAKE_CXX_STANDARD 17)

# 定义可执行目标
add_executable(crow_server crow_server.cpp)

# 添加Crow头文件路径
target_include_directories(crow_server PRIVATE ${CMAKE_SOURCE_DIR}/Crow/include) # 添加头文件
target_include_directories(crow_server PRIVATE ${CMAKE_SOURCE_DIR}/asio/asio) # 添加asio头文件,Crow依赖这个库
target_include_directories(crow_server PRIVATE /usr/include/cppconn)
target_include_directories(crow_server PRIVATE /usr/local/include)

# 添加pthread库支持
target_link_libraries(crow_server pthread)
# 链接 MySQL Connector库
target_link_libraries(crow_server mysqlcppconn bcrypt)
