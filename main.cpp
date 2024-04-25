#include <iostream>
#include <pugixml.hpp>
#include <cpr/cpr.h>
#include <fstream>
#include <cstdlib>
int main() {
//    cpr::Response r = cpr::Get(cpr::Url{"https://www.cbr.ru/scripts/XML_daily.asp?date_req=02/03/2002"},
//                               cpr::Authentication{"user", "pass", cpr::AuthMode::BASIC},
//                               cpr::Parameters{{"anon", "true"}, {"key", "value"}}, cpr::Header{ { "content-type", "application/json; charset=utf-8" } });
//    r.status_code;                  // 200
//    r.header["content-type"];// application/json; charset=utf-8
//    std::cout << r.text;                         // JSON text string
//    std::ofstream in;
//    in.open("example.xml");
//    in << r.text;
//    in.close();

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("example.xml");
    if (!result) {
        std::cerr << "XML parsing error: " << result.description() << std::endl;
        return -1;
    }

    pugi::xml_node valCurs = doc.child("ValCurs");
    if (!valCurs) {
        std::cerr << "Invalid XML format: <ValCurs> element not found" << std::endl;
        return -1;
    }

    for (pugi::xml_node valute: valCurs.children("Valute")) {
        std::string id = valute.attribute("ID").as_string();
        std::string name = valute.child("Name").text().as_string();

        std::cout << "Valute ID: " << id << ", Name: " << name << std::endl;
    }




    return 0;
}
