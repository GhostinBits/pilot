#include <iostream>

#include "crow_all.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        auto page = crow::mustache::load_text("index.html");
        return page;
    });

    CROW_ROUTE(app, "/settings")([](){
        auto page = crow::mustache::load_text("settings.html");
        return page;
    });

    app.port(18080).run();

    return 0;
}
