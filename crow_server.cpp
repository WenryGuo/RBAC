#define CROW_MAIN
#include "crow.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <iostream>
#include <string>
struct CORS
{
    struct context
    {
    };
    void before_handle(crow::request &req, crow::response &res, context &)
    {
        res.set_header("Access-Control-Allow-Origin", "http://localhost:5173");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        // 直接处理 OPTIONS 请求
        if (req.method == "OPTIONS"_method) {
            res.code = 204;
            res.end();
            // 重点：提前 return，跳过后续处理
            throw crow::response(); 
        }
    }
    void after_handle(crow::request &, crow::response &res, context &) {
        // 确保非预检响应也设置 header
        res.set_header("Access-Control-Allow-Origin", "http://localhost:5173");
    }
};
std::shared_ptr<sql::Connection> get_mysql_connection()
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;
        // 获取 MySQL 驱动
        driver = sql::mysql::get_mysql_driver_instance();
        // 建立连接
        con = driver->connect("tcp://127.0.0.1:3306", "root", "abc@123");
        con->setSchema("RBAC");
        return std::shared_ptr<sql::Connection>(con);
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "Error connecting to MySQL: " << e.what() << std::endl;
        return nullptr;
    }
}
int main()
{
    crow::App<CORS> app;
    // crow::SimpleApp app; // 创建Crow Web应用
    // 定义路由

    CROW_ROUTE(app, "/")([]()
                         { return "Hello, Crow!\n"; });

    CROW_ROUTE(app, "/json")([]()
                             {
        crow::json::wvalue data;
        // 获取数据库连接
        auto con = get_mysql_connection();
        if(!con){
            crow::json::wvalue data;
            data["error"] = "Failed to connect to database";
            return data;
        }
        try{
        // 查询所有用户
        std::shared_ptr<sql::Statement> stmt(con->createStatement());
        std::shared_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT username FROM users"));
        crow::json::wvalue::list users_list;
        int i = 0;
        while(res->next()){
          users_list.push_back(crow::json::wvalue(res->getString("username")));
        }
        data["users"] = std::move(users_list);
    }catch(sql::SQLException &e){
        std::cerr << "SQL error: " << e.what() << std::endl;
        data["error"] = "SQL query failed";
    }
        // data["message"] = "Hello, JSON";
        // crow::response resp(data);
        // resp.add_header("Content-Type", "application/json");
        return data; });
    // 监听 8080 端口
    app.port(8080).multithreaded().run();
}