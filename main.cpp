//
// Created by jiangqi on 2023-11-05.
//
#include <iostream>

#include "crow_all.h"
#include "CLITools.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load("index.html");
        crow::mustache::context ctx ({{"uptime", CLITools::execute("uptime")}});
        return page.render(ctx);
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

    app.port(18080).run();

    return 0;
}
