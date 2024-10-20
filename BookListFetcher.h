#ifndef BOOKLISTFETCHER_H
#define BOOKLISTFETCHER_H

#include <string>
#include <vector>
#include <mutex>
#include <atomic>

class BookListFetcher {
public:
    BookListFetcher(const std::string& name, const std::string& author, const std::string& year);
    void fetchBookList();
    std::vector<std::pair<std::string, std::string>> getDisplayedBooks() const;
    bool exist_error() const; // Method to check if no books were found
    std::string errorType() const;

private:
    std::string name;
    std::string author;
    std::string year;

    std::vector<std::pair<std::string, std::string>> displayedBooks;
    mutable std::mutex mtx;
    std::atomic<bool> errorFlag{ false }; // Atomic boolean for status
    std::string error;
};

#endif // BOOKLISTFETCHER_H
