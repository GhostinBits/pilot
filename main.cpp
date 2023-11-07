//
// Created by jiangqi on 2023-11-05.
//
#include <iostream>
#include "crow_all.h"
#include "CLITools.h"

int main() {
    crow::App<crow::CookieParser> app;

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);


        if (1) {  // need to check credentials here
            ctx.set_cookie("loggedIn", "true").path("/").max_age(1800).httponly();
            res.redirect("/");
            res.end();
        }
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::GET)([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        if (!(ctx.get_cookie("loggedIn") == "true")) {
            auto page = crow::mustache::load("login.html");
            res.write(page.render_string());
            res.end();
        }
        else {
            res.redirect("/");
            res.end();
        }
    });

    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([&](const crow::request& req, crow::response& res){
        crow::mustache::context pageCtx ({{"uptime", CLITools::execute("uptime")}});
        auto& ctx = app.get_context<crow::CookieParser>(req);
        std::string loggedin = ctx.get_cookie("loggedIn");
        if (!(ctx.get_cookie("loggedIn") == "true")) {
            res.redirect("/login");
            res.end();
        }
        else {
            auto page = crow::mustache::load("index.html");
            res.write(page.render_string(pageCtx));
            res.end();
        }
    });

    CROW_ROUTE(app, "/settings").methods(crow::HTTPMethod::GET)([](){
        auto page = crow::mustache::load("settings.html");
        crow::mustache::context ctx ({{"networkInfo", CLITools::getWirelessInterfaces()}});
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/settings").methods(crow::HTTPMethod::POST)([](const crow::request& req){

        // building a url, the ip can be arbitrary
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);
        CLITools::updateWireless(qs.get("ssid"), qs.get("passphrase"));
        auto page = crow::mustache::load("settings.html");
        crow::mustache::context ctx ({{"networkInfo", CLITools::getWirelessInterfaces()}});
        return page.render(ctx);
    });

    CROW_ROUTE(app, "/uptime")([](){
        return CLITools::execute("uptime");
    });

    CROW_ROUTE(app, "/logout")([&](const crow::request& req, crow::response& res){
        auto& ctx = app.get_context<crow::CookieParser>(req);
        crow::query_string qs = crow::query_string("http://0.0.0.0/?" + req.body);

        ctx.set_cookie("loggedIn", "false").path("/").max_age(1800).httponly();
        res.redirect("/login");
        res.end();
    });

    app.port(18080).run();

    return 0;
}
