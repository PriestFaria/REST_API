#include <iostream>
#include <fstream>
#include <cpr/cpr.h>
#include <pugixml.hpp>
#include <sstream>


struct Args {
    std::string vNom;
    std::string vName;
    std::string date;
    std::string toDate;
    std::string fromDate;
    std::string filePath;
};


int parseXML() {
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
}


int requestForDateToDate(const std::string &valutaCode, const std::string &fromDate, const std::string &toDate) {
//    const std::string fromDate = "2023-09-01"; // Start date of the desired period
//    const std::string toDate = "2023-09-10";   // End date of the desired period
//    const std::string valutaCode = "R01235";   // Code for the currency (e.g., US Dollar is R01235)

    std::ostringstream soapRequestStream;
    soapRequestStream << R"(<?xml version="1.0" encoding="utf-8"?>)"
                         R"(<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance")"
                         R"( xmlns:xsd="http://www.w3.org/2001/XMLSchema")"
                         R"( xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">)"
                         R"(<soap12:Body>)"
                         R"(<GetCursDynamic xmlns="http://web.cbr.ru/">)"
                         R"(<FromDate>)" << fromDate << R"(</FromDate>)"
                                                        R"(<ToDate>)" << toDate << R"(</ToDate>)"
                                                                                   R"(<ValutaCode>)" << valutaCode
                      << R"(</ValutaCode>)"
                         R"(</GetCursDynamic>)"
                         R"(</soap12:Body>)"
                         R"(</soap12:Envelope>)";

    std::string soapRequest = soapRequestStream.str();

    cpr::Response r = cpr::Post(cpr::Url{"https://www.cbr.ru/DailyInfoWebServ/DailyInfo.asmx"},
                                cpr::Header{{"Content-Type", "application/soap+xml; charset=utf-8"},
                                            {"Accept",       "text/xml"}},
                                cpr::Body{soapRequest});

    if (r.status_code == 200) {
//        std::cout << "Success:\n" << r.text << std::endl;
    } else {
        std::cerr << "Error: " << r.status_code << std::endl;
        if (!r.error.message.empty()) {
            std::cerr << "Detailed error: " << r.error.message << std::endl;
        }
        return 1;
    }

    if (r.status_code == 200) {
        std::ofstream out("response.xml");
        out << r.text;
        out.close();
        return 0;
    }
}


std::string getValParentCode(const std::string &vName, const std::string &filePath) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if (!result) {
        std::cerr << "XML [" << filePath << "] parsed with errors, error description is: "
                  << result.description() << std::endl;
        return "";
    }

    pugi::xml_node valCurs = doc.child("ValCurs");

    for (pugi::xml_node valute: valCurs.children("Valute")) {
        if (valute.child("CharCode").text().get() == vName) {
            return valute.attribute("ID").value();
        }
    }

    return "";
}


void getDatafromDateToDateXML(const std::string &filePath, int nom) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(filePath.c_str());

    if (!result) {
        std::cerr << "XML [" << filePath << "] parsed with errors, error description is: "
                  << result.description() << std::endl;
        return;
    }

    pugi::xml_node diffgram = doc.child("soap:Envelope")
            .child("soap:Body")
            .child("GetCursDynamicResponse")
            .child("GetCursDynamicResult")
            .child("diffgr:diffgram");

    pugi::xml_node valuteData = diffgram.child("ValuteData");

    for (pugi::xml_node valuteCursDynamic: valuteData.children("ValuteCursDynamic")) {
        std::string cursDate = valuteCursDynamic.child("CursDate").text().as_string();
        double vCurs = valuteCursDynamic.child("Vcurs").text().as_double();

        std::cout << "Дата: " << cursDate << ", Объем валюты: " << vCurs * nom << std::endl;
    }
}


std::string getProperDateFormat(const std::string &date) {
    if (date.length() != 10) {
        std::cerr << "Invalid date format: " << date << std::endl;
        return "";
    }

    std::string day = date.substr(0, 2);
    std::string month = date.substr(3, 2);
    std::string year = date.substr(6, 4);

    return year + "-" + month + "-" + day;
}


int main(int argc, char *argv[]) {

    Args args;

    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        size_t start = arg.find("--");
        size_t equal = arg.find('=');
        std::string key;
        std::string value;
        if (start == 0 && equal != std::string::npos) {
            key = arg.substr(2, equal - 2); // Обрезаем "--" и "="
            value = arg.substr(equal + 1);
        }

        if (key == "Vnom") {
            args.vNom = value;
        } else if (key == "Vname") {
            args.vName = value;
        } else if (key == "Date") {
            args.date = value;
        } else if (key == "File") {
            args.filePath = value;
        } else if (key == "DateDate") {
            size_t dash = value.find('-');
            if (dash != std::string::npos) {
                args.fromDate = value.substr(0, dash);
                args.toDate = value.substr(dash + 1);
            }
        }
    }

    // Отладочный вывод для проверки результатов
    std::cout << "Vnom: " << args.vNom << std::endl;
    std::cout << "Vname: " << args.vName << std::endl;
    std::cout << "Date: " << args.date << std::endl;
    std::cout << "FromDate: " << args.fromDate << std::endl;
    std::cout << "ToDate: " << args.toDate << std::endl;
    std::cout << "File: " << args.filePath << std::endl;

    std::string vCode = getValParentCode(args.vName, "valutes_codes.xml");

    if (args.date != "")
        args.date = getProperDateFormat(args.date);
    else {
        args.toDate = getProperDateFormat(args.toDate);
        args.fromDate = getProperDateFormat(args.fromDate);
    }
    std::cout << "vCode: " << vCode << std::endl;
    requestForDateToDate(vCode, args.fromDate, args.toDate);
    getDatafromDateToDateXML("response.xml", std::stoi(args.vNom));
    return 0;
}

