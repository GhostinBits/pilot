//
// Created by jiangqi on 2023-11-05.
//
#include <iostream>
#include "crow_all.h"
#include "util.h"
#include "Session.h"
#include "UserMan.h"

int main() {
    crow::App<crow::CookieParser> app;
    Session session;
    UserMan userMan(&session);

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        // building a url, the ip can be arbitrary
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);

        if (session.authenticate(qs.get("username"), qs.get("password"))) {
            ctx.set_cookie("loggedIn", "true").path("/").max_age(1800).httponly();
            res.redirect("/");
            res.end();
        }
        else {
            crow::mustache::context pageCtx ({{"error", "Wrong credentials. Please try again."}});
            auto page = crow::mustache::load("login.html");
            res.write(page.render_string(pageCtx));
            res.end();
        }
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::GET)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        if (!(ctx.get_cookie("loggedIn") == "true") || !session.loginStatus()) {
            crow::mustache::context pageCtx ({{"error", ""}});
            auto page = crow::mustache::load("login.html");
            res.write(page.render_string(pageCtx));
            res.end();
        }
        else {
            res.redirect("/");
            res.end();
        }
    });

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){

        auto& ctx = app.get_context<crow::CookieParser>(req);
        std::string loggedin = ctx.get_cookie("loggedIn");
        if (!(ctx.get_cookie("loggedIn") == "true") || !session.loginStatus()) {
            res.redirect("/login");
            res.end();
        }

        crow::mustache::context pageCtx ({{"uptime", util::execute("uptime")}});
        auto page = crow::mustache::load("index.html");
        res.write(page.render_string(pageCtx));
        res.end();
    });

    CROW_ROUTE(app, "/settings").methods(crow::HTTPMethod::GET)([&](const crow::request& req, crow::response& res){

        auto& ctx = app.get_context<crow::CookieParser>(req);
        std::string loggedin = ctx.get_cookie("loggedIn");
        if (!(ctx.get_cookie("loggedIn") == "true") || !session.loginStatus()) {
            res.redirect("/login");
            res.end();
        }

        crow::mustache::context pageCtx ({{"networkInfo", util::getWirelessInterfaces()}});
        auto page = crow::mustache::load("settings.html");
        res.write(page.render_string(pageCtx));
        res.end();
    });

    CROW_ROUTE(app, "/settings").methods(crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){

        auto& ctx = app.get_context<crow::CookieParser>(req);
        std::string loggedin = ctx.get_cookie("loggedIn");
        if (!(ctx.get_cookie("loggedIn") == "true") || !session.loginStatus()) {
            res.redirect("/login");
            res.end();
        }

        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);
        util::updateWireless(qs.get("ssid"), qs.get("passphrase"));
        crow::mustache::context pageCtx ({{"networkInfo", util::getWirelessInterfaces()}});
        auto page = crow::mustache::load("settings.html");
        res.write(page.render_string(pageCtx));
        res.end();
    });

    CROW_ROUTE(app, "/uptime")([](){
        return util::execute("uptime");
    });

    CROW_ROUTE(app, "/logout")([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);

        ctx.set_cookie("loggedIn", "false").path("/").max_age(1800).httponly();
        res.redirect("/login");
        res.end();
    });

    CROW_ROUTE(app, "/run").methods(crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);
        res.write(util::execute_timeout(qs.get("command"), 3));
        res.end();
    });

    CROW_ROUTE(app, "/terminal").methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){

        auto& ctx = app.get_context<crow::CookieParser>(req);
        std::string loggedin = ctx.get_cookie("loggedIn");
        if (!(ctx.get_cookie("loggedIn") == "true") || !session.loginStatus()) {
            res.redirect("/login");
            res.end();
        }

        auto page = crow::mustache::load("terminal.html");
        res.write(page.render_string());
        res.end();
    });

    CROW_ROUTE(app, "/users").methods(crow::HTTPMethod::GET)([&](const crow::request& req, crow::response& res){

        auto& ctx = app.get_context<crow::CookieParser>(req);
        std::string loggedin = ctx.get_cookie("loggedIn");
        if (!(ctx.get_cookie("loggedIn") == "true") || !session.loginStatus()) {
            res.redirect("/login");
            res.end();
        }

        crow::mustache::context pageCtx ({{"userTable", userMan.getUsers()}});
        auto page = crow::mustache::load("users.html");
        res.write(page.render_string(pageCtx));
        res.end();
    });

    CROW_ROUTE(app, "/deleteuser").methods(crow::HTTPMethod::GET)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);
        userMan.deleteUser(req.url_params.get("username"));
        res.redirect("/users");
        res.end();
    });

    CROW_ROUTE(app, "/adduser").methods(crow::HTTPMethod::GET)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        // building a url, the ip can be arbitrary
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);
        userMan.addUser(req.url_params.get("username"), req.url_params.get("password"));
        res.redirect("/users");
        res.end();
    });

    app.port(18080).run();

    return 0;
}
