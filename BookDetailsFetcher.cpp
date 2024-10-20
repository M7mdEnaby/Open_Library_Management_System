#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "BookDetailsFetcher.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>

BookDetailsFetcher::BookDetailsFetcher(const std::string& isbn)
    : isbn(isbn) {}

void BookDetailsFetcher::fetchBookDetails() {
    try {
        httplib::SSLClient cli("openlibrary.org");

        // Construct the URL for fetching book details
        std::string url = "/api/books?bibkeys=ISBN:" + isbn + "&format=json&jscmd=data";
        auto res = cli.Get(url.c_str());

        std::lock_guard<std::mutex> lock(mtx); // Lock the mutex once here

        if (res) {
            if (res->status == 200) {
                nlohmann::json jsonData = nlohmann::json::parse(res->body);
                rawJsonData = jsonData.dump(4);  // Format the JSON data with indentation (4 spaces)

                // Assign the entire JSON string to bookDetails for detailed display                
                if (jsonData.contains("ISBN:" + isbn) && jsonData["ISBN:" + isbn].is_object()) {
                    auto& book = jsonData["ISBN:" + isbn];
                    std::string title = book.contains("title") && book["title"].is_string() ? book["title"].get<std::string>() : "N/A";
                    std::string author = book.contains("authors") && book["authors"].is_array() ? book["authors"][0]["name"].get<std::string>() : "N/A";
                    std::string summary = book.contains("notes") && book["notes"].is_string() ? book["notes"].get<std::string>() : "No notes available";
                    std::string publish_date = book.contains("publish_date") && book["publish_date"].is_string() ? book["publish_date"].get<std::string>() : "No information available";
                    std::string number_of_pages = book.contains("number_of_pages") && book["number_of_pages"].is_number() ? std::to_string(book["number_of_pages"].get<int>()) : "N/A";
                    std::string url = book.contains("url") && book["url"].is_string() ? book["url"].get<std::string>() : "No url available";
                    // Format the details for display
                    bookDetails = "Title: " + title + "\n"
                        "Author: " + author + "\n"
                        "notes: " + summary + "\n"
                        "Publish_date: " + publish_date + "\n"
                        "Number_of_pages: " + number_of_pages + "\n"
                        "URL:" + url + "\n";
                }
            }
            else {
                rawJsonData = "Failed to fetch book data, status code: " + std::to_string(res->status);
                bookDetails = rawJsonData;
            }
        }
        else {
            auto err = res.error();
            bookDetails = "Failed to connect to the server. Error: " + httplib::to_string(err);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception occurred: " << e.what() << std::endl;
        bookDetails = "An exception occurred: " + std::string(e.what());
    }
}




std::string BookDetailsFetcher::getBookDetails() const {
    std::lock_guard<std::mutex> lock(mtx);
    return bookDetails;
}

std::string BookDetailsFetcher::getRawJsonData() const {
    std::lock_guard<std::mutex> lock(mtx);
    return rawJsonData;
}
