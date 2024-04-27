#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <cstdlib>
#include <pugixml.hpp>

int main() {
//    const std::string fromDate = "2023-09-01"; // Start date of the desired period
//    const std::string toDate = "2023-09-10";   // End date of the desired period
//    const std::string valutaCode = "R01235";   // Code for the currency (e.g., US Dollar is R01235)
//
//
//
//    // Send a POST request
//    cpr::Response r = cpr::Post(cpr::Url{"/DailyInfoWebServ/DailyInfo.asmx HTTP/1.1"},
//                                cpr::Header{{"Content-Type", "application/soap+xml; charset=utf-8"},
//                                            {"Accept", "text/xml"}},
//                                cpr::Body{R"(
//<?xml version="1.0" encoding="utf-8"?>
//<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
//                 xmlns:xsd="http://www.w3.org/2001/XMLSchema"
//                 xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
//  <soap12:Body>
//    <GetCursDynamic xmlns="http://web.cbr.ru/">
//      <FromDate>2022-02-01</FromDate>
//      <ToDate>2022-02-03</ToDate>
//      <ValutaCode>R01235</ValutaCode>
//    </GetCursDynamic>
//  </soap12:Body>
//</soap12:Envelope>
//)"});
//
//    r.status_code;                  // 200
//    r.header["content-type"];// application/json; charset=utf-8
//    std::cout << r.text;                         // JSON text string
//    std::ofstream in;
//    in.open("example.xml");
//    in << r.text;
//    in.close();


    const std::string fromDate = "2023-09-01"; // Start date of the desired period
    const std::string toDate = "2023-09-10";   // End date of the desired period
    const std::string valutaCode = "R01235";   // Code for the currency (e.g., US Dollar is R01235)

    std::ostringstream soapRequestStream;
    soapRequestStream << R"(<?xml version="1.0" encoding="utf-8"?>)"
                         R"(<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance")"
                         R"( xmlns:xsd="http://www.w3.org/2001/XMLSchema")"
                         R"( xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">)"
                         R"(<soap12:Body>)"
                         R"(<GetCursDynamic xmlns="http://web.cbr.ru/">)"
                         R"(<FromDate>)" << fromDate << R"(</FromDate>)"
                                                        R"(<ToDate>)" << toDate << R"(</ToDate>)"
                                                                                   R"(<ValutaCode>)" << valutaCode << R"(</ValutaCode>)"
                                                                                                                      R"(</GetCursDynamic>)"
                                                                                                                      R"(</soap12:Body>)"
                                                                                                                      R"(</soap12:Envelope>)";

    // Generate the full SOAP request body with actual values
    std::string soapRequest = soapRequestStream.str();

    // Send a POST request
    cpr::Response r = cpr::Post(cpr::Url{"https://www.cbr.ru/DailyInfoWebServ/DailyInfo.asmx"},
                                cpr::Header{{"Content-Type", "application/soap+xml; charset=utf-8"},
                                            {"Accept", "text/xml"}},
                                cpr::Body{soapRequest});

    // Check the response
    if (r.status_code == 200) {
        std::cout << "Success:\n" << r.text << std::endl;
    } else {
        std::cerr << "Error: " << r.status_code << std::endl;
        if (!r.error.message.empty()) {
            std::cerr << "Detailed error: " << r.error.message << std::endl;
        }
    }

    // Save the response to a file if it was successful
    if (r.status_code == 200) {
        std::ofstream out("response.xml");
        out << r.text;
        out.close();
    }


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

    std::cout << "\nYA HOROSHIY";


    return 0;
}