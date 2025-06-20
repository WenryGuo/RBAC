#define CROW_MAIN
#include "crow.h"
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include "bcrypt/BCrypt.hpp"

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
        res.set_header("Access-Control-Allow-Headers", "content-Type, authorization");
        res.set_header("Access-Control-Allow-Credentials", "true");
        res.set_header("Vary", "Origin"); // 避免浏览器缓存问题

        if (req.method == "OPTIONS"_method)
        {
            res.code = 204;
            res.end();
            return;
        }
    }

    void after_handle(crow::request &, crow::response &res, context &)
    {
        res.set_header("Access-Control-Allow-Origin", "http://localhost:5173");
        res.set_header("Access-Control-Allow-Credentials", "true");
        res.set_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
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

    CROW_ROUTE(app, "/register").methods("POST"_method)([](const crow::request &req)
                                                        {
                            std::cout<<"in here...\n";
                            crow::json::wvalue data;
                             auto body = crow::json::load(req.body);
                             if (!body || !body.has("username") || !body.has("email") || !body.has("password")) {
                                 data["400"] = "参数错误";
                                return data;
                             }
                         
                             std::string username = body["username"].s();
                             std::string email = body["email"].s();
                             std::string password = body["password"].s();
                         
                             std::string hashed = BCrypt::generateHash(password);
                         
                             auto con = get_mysql_connection();
                             if (!con) {data["500"] = "数据库连接失败";
                                        return data;}
                         
                             try {
                                 std::shared_ptr<sql::PreparedStatement> stmt(
                                     con->prepareStatement("INSERT INTO users (username, email, password_hash) VALUES (?, ?, ?)"));
                                 stmt->setString(1, username);
                                 stmt->setString(2, email);
                                 stmt->setString(3, hashed);
                                 stmt->execute();
                             } catch (sql::SQLException &e) {
                                 if (e.getErrorCode() == 1062) {
                                     data["409"] = "用户名或邮箱已存在";
                                    return data;
                                 }
                                 std::cerr << "SQL Error: " << e.what() << std::endl;
                                 data["500"] = "注册失败";
                                return data;
                             }
                         
                             crow::json::wvalue result;
                             result["message"] = "注册成功";
                             return result; });

    CROW_ROUTE(app, "/login").methods("POST"_method)([](const crow::request &req)
                                                     {
                            crow::json::wvalue data;
                            auto body = crow::json::load(req.body);
                            if (!body || !body.has("username") || !body.has("password")) {
                                data["400"] = "Invalid request";
                                return data;
                            }
                        
                            std::string username = body["username"].s();
                            std::string password = body["password"].s();
                        
                            auto con = get_mysql_connection();
                            if (!con) {data["500"] = "DB error";return data;}
                        
                            try {
                                // std::shared_ptr<sql::Statement> stmt(con->createStatement());
                                std::shared_ptr<sql::PreparedStatement> stmt(con->prepareStatement(
                                    "SELECT id, password_hash FROM users WHERE username = ?"  // 使用 ? 占位符
                                ));
                                stmt->setString(1, username);
                                std::shared_ptr<sql::ResultSet> res(stmt->executeQuery());
                                
                                // std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
                        
                                if (res->next()) {
                                    std::string hash = res->getString("password_hash");
                        
                                    // 使用 bcrypt 验证密码
                                    if (BCrypt::validatePassword(password, hash)) {
                                        crow::json::wvalue result;
                                        result["message"] = "Login successful";
                                        result["user_id"] = res->getInt("id");
                                        return result;
                                    } else {
                                        data["401"] = "Invalid password";
                                        return data;
                                    }
                                } else {
                                    data["404"] = "User not found";
                                    return data;
                                }
                            } catch (const sql::SQLException& e) {
                                std::cerr << "SQL Exception: " << e.what() << std::endl;
                                data["500"] = "SQL error";
                                return data;
                            } });

    CROW_ROUTE(app, "/json")([]()
                             {
        crow::json::wvalue data;
        // 获取数据库连接
        auto con = get_mysql_connection();
        if(!con){
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

    
    CROW_ROUTE(app, "/roles")([]()
                             {
        crow::json::wvalue data;
        // 获取数据库连接
        auto con = get_mysql_connection();
        if(!con){
            data["error"] = "Failed to connect to database";
            return data;
        }
        try{
        // 查询所有用户
        std::shared_ptr<sql::Statement> stmt(con->createStatement());
        std::shared_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT username FROM roles"));
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
        return data;});
    // 监听 8080 端口
    app.port(8080).multithreaded().run();
}
