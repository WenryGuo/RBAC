#define CROW_MAIN
#include "crow.h"
#include<mysql_driver.h>
#include<mysql_connection.h>
#include<cppconn/statement.h>
#include<cppconn/resultset.h>
#include<iostream>
#include<string>
std::shared_ptr<sql::Connection> get_mysql_connection(){
    try{
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
         // 获取 MySQL 驱动
         driver = sql::mysql::get_mysql_driver_instance();
         // 建立连接
         con = driver->connect("tcp://127.0.0.1:3306", "root", "abc@123"); 
         con->setSchema("RBAC");
         return std::shared_ptr<sql::Connection>(con);
     } catch (sql::SQLException &e) {
         std::cerr << "Error connecting to MySQL: " << e.what() << std::endl;
         return nullptr;
     }
}
int main(){
    crow::SimpleApp app; // 创建Crow Web应用
    // 定义路由
    CROW_ROUTE(app, "/")([](){
        return "Hello, Crow!\n";
    });
    CROW_ROUTE(app, "/json")([](){
        // 获取数据库连接
        auto con = get_mysql_connection();
        if(!con){
            crow::json::wvalue data;
            data["error"] = "Failed to connect to database\n";
            return data;
        }
        // 查询所有用户
        std::shared_ptr<sql::Statement> stmt(con->createStatement());
        std::shared_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT username FROM users"));
        crow::json::wvalue data;
        int i = 0;
        while(res->next()){
            data["users"][i++]=res->getString("username");
        }
        // data["message"] = "Hello, JSON";
        return data;
    });
    // 监听 8080 端口
    app.port(8080).multithreaded().run();
}