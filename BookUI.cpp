#include "BookUI.h"
#include "BookManager.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>

namespace fs = std::filesystem;
std::vector<std::pair<std::string, std::string>> books; 
std::vector<std::pair<std::string, std::string>> sorted_books_name;
std::vector<std::pair<std::string, std::string>> fav_books;
BookUI::BookUI(BookManager& manager) : manager(manager) {}
void BookUI::render() {
    ImGui::Begin("Book Management App", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Set title with larger font
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 textSize = ImGui::CalcTextSize("Open Library Management");
    ImGui::SetCursorPosX((windowSize.x - textSize.x) * 0.5f); // Center horizontally
    ImGui::TextColored(ImVec4(1.0f, 0.84f, 0.0f, 1.0f), "Open Library Management");
    ImGui::SetCursorPosX(0); // Reset cursor position
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f); // Black background

    ImGui::Separator();

    // Begin Columns
    ImGui::Columns(3, "SearchAndHistoryAndFavColumns", true);

    // Left Column: Search Input Fields
    ImGui::Checkbox("Search by ISBN", &searchByIsbn);
    ImGui::InputTextWithHint("##isbn", "Enter ISBN:", isbnBuffer, IM_ARRAYSIZE(isbnBuffer), searchByIsbn ? ImGuiInputTextFlags_CharsDecimal : ImGuiInputTextFlags_ReadOnly);

    ImGui::Checkbox("Search by Name", &searchByName);
    ImGui::InputTextWithHint("##name", "Enter Book Name:", nameBuffer, IM_ARRAYSIZE(nameBuffer), searchByName ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly);

    ImGui::Checkbox("Search by Author", &searchByAuthor);
    ImGui::InputTextWithHint("##author", "Enter Author:", authorBuffer, IM_ARRAYSIZE(authorBuffer), searchByAuthor ? ImGuiInputTextFlags_None : ImGuiInputTextFlags_ReadOnly);

    ImGui::Checkbox("Search by Year", &searchByYear);
    ImGui::InputTextWithHint("##year", "Enter Year:", yearBuffer, IM_ARRAYSIZE(yearBuffer), searchByYear ? ImGuiInputTextFlags_CharsDecimal : ImGuiInputTextFlags_ReadOnly);

    // Move to the next column
    ImGui::NextColumn();

    // Right Column: History Buttons
    if (ImGui::Button("Library History")) {
        show_history.store(!show_history.load());
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear History")) {
        clear_history.store(true);
    }

    if (clear_history.load()) {
        manager.clearHistory();
        clear_history.store(false); // Reset the flag after clearing history
    }

    if (show_history.load()) {
        if (!manager.getBooksHistory().empty()) {
            for (const auto& historyItem : manager.getBooksHistory()) {
                // Change button color to white and text color to black
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White button
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Black text

                if (ImGui::Button(historyItem.first.c_str())) {
                    selectedBook = historyItem.first;
                    selectedBookIsbn = historyItem.second;
                    manager.selectBook(selectedBookIsbn); // Use selectedBookIsbn instead of selectedBook
                    manager.fetchBookDetails(selectedBookIsbn); // Fetch details for the selected book
                    showBookDetails = true; // Show details for the selected book
                }

                // Revert back to default colors
                ImGui::PopStyleColor(2);
            }
        }
        else {
            ImGui::Text("No history available.");
        }
    }

    ImGui::NextColumn();

    // Right Column: History Buttons
    if (ImGui::Button("Favourite")) {
        show_fav.store(!show_fav.load());
    }


    if (show_fav.load()) {
        if (!fav_books.empty()) {
            for (const auto& FavItem : fav_books) {
                // Change button color to white and text color to black
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // White button
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Black text

                if (ImGui::Button(FavItem.first.c_str())) {
                    selectedBook = FavItem.first;
                    selectedBookIsbn = FavItem.second;
                    manager.selectBook(selectedBookIsbn); // Use selectedBookIsbn instead of selectedBook
                    manager.fetchBookDetails(selectedBookIsbn); // Fetch details for the selected book
                    showBookDetails = true; // Show details for the selected book
                }

                // Revert back to default colors
                ImGui::PopStyleColor(2);
            }
        }
        else {
            ImGui::Text("No history available.");
        }
    }
    ImGui::Columns(1); // End columns

    ImGui::Separator();
    ImGui::Text("Actions");

    if (ImGui::Button("Search")) {
        books.clear();
        sorted_books_name.clear();
        progress = 0.0f;
        manager.FetchBookList(
            searchByName ? nameBuffer : "",
            searchByAuthor ? authorBuffer : "",
            searchByYear ? yearBuffer : ""
        );

        showBookDetails = false; // Hide details when performing a new search
        selectedBook.clear();    // Clear selected book
    }
    books = manager.getDisplayedBooks();
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        isbnBuffer[0] = '\0';
        nameBuffer[0] = '\0';
        authorBuffer[0] = '\0';
        yearBuffer[0] = '\0';
        searchByIsbn = searchByName = searchByAuthor = searchByYear = false;
        manager.clear();
        showBookDetails = false; // Hide details when clearing
        selectedBook.clear();    // Clear selected book
        sorted_books_name.clear();
    }
  
    ImGui::Separator();
    ImGui::Text("filter");
    // Custom styling for the arrow button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Black background
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));    // White text

    // Render the arrow button
    ImGui::SameLine();
    if (ImGui::ArrowButton("filter", ImGuiDir_Down)) {
        show_sort_name.store(!show_sort_name); 
    }

    // Restore previous style
    ImGui::PopStyleColor(2);

    if (show_sort_name) {
        if (ImGui::Button("Sort A to Z")) {
            sort_name_button.store(!sort_name_button.load());
        }
    }

    

    if (manager.isUpdatingBooks) {
        ImGui::Text("Getting book list...");
        if (!wasUpdating) {
            progress = 0.0f;
            wasUpdating = true;
        }

        if (progress < 1.0f) {
            progress += 0.01f;
        }
        else if (progress == 1.0f) {
            progress = 0.0f;
        }
        else {
            progress = 1.0f;
        }

        ImGui::ProgressBar(progress, ImVec2(0, 2));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    if (sort_name_button) {
        sorted_books_name = manager.sortDisplayedBooksByName(books);
        sort_name_button.store(!sort_name_button.load());
    }

    ImGui::Text("Books:");
    ImGui::BeginChild("BooksList", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

    if (!sorted_books_name.empty()) {
        DisplayBooks(sorted_books_name);
    }

    else if (!books.empty()) {
        DisplayBooks(books);
    }
    else {
        ImGui::Text("No books to display.");
    }

    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Text("Book Details");

    if (showBookDetails && !selectedBook.empty()) {
        ImGui::BeginChild("BookDetails", ImVec2(0, 300), true);

        ImGui::Text("Title: %s", selectedBook.c_str());
        ImGui::Separator();

        std::string bookDetails = manager.getBookDetails();
        if (manager.isFetchingDetails) {
            ImGui::Text("Getting Book Details...");
            if (!wasFetching) {
                progress = 0.0f;
                wasFetching = true;
            }

            if (progress < 1.0f) {
                progress += 0.01f;
            }
            else {
                progress = 1.0f;
            }

            ImGui::ProgressBar(progress, ImVec2(0, 2));
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        ImGui::TextWrapped("%s", bookDetails.c_str());
        ImGui::Separator();

        ImGui::Text("JSON Data:");
        ImGui::SameLine();
        if (ImGui::Button("Copy Details")) {
            ImGui::SetClipboardText(manager.getBookDetails().c_str());
        }
        ImGui::SameLine();
        if (ImGui::Button("Add to Favourite")) {
            saveBookDetailsAsFavourite();
            fav_books.push_back({ selectedBook,selectedBookIsbn });
        }

        std::string bookDataOutput = manager.getRawJsonData();
        ImGui::InputTextMultiline("##jsonData", &bookDataOutput[0], bookDataOutput.size(), ImVec2(-FLT_MIN, 200), ImGuiInputTextFlags_ReadOnly);

        ImGui::EndChild();
    }
    ImGui::Separator();
    ImGui::Text("status");
    renderNotificationSection();
    if (existing_Error) {
        showNotificationMessage(manager.getErrorType(), 2000);
    }
    ImGui::End(); // End the main window
}


void BookUI::showNotificationMessage(const std::string& message, int duration_ms) {
    notificationMessage = message;
    notificationStartTime = std::chrono::steady_clock::now();
    showNotification = true;
}

std::string sanitizeFilename(const std::string& filename) {
    std::string sanitized = filename;
    std::replace(sanitized.begin(), sanitized.end(), ' ', '_');
    sanitized.erase(std::remove_if(sanitized.begin(), sanitized.end(),
        [](char c) { return !std::isalnum(c) && c != '_'; }),
        sanitized.end());
    return sanitized;
}

void BookUI::saveBookDetailsAsFavourite() {
    std::string bookTitle = selectedBook;

    if (bookTitle.empty()) {
        showNotificationMessage("No book selected.", 3000);
        return;
    }

    fs::path favDir = fs::current_path() / "favourite";
    fs::path favFile = favDir / (sanitizeFilename(bookTitle) + ".json");

    if (!fs::exists(favDir)) {
        fs::create_directory(favDir);
    }

    if (fs::exists(favFile)) {
        showNotificationMessage("This book is already in your favourites.", 3000);
        return;
    }

    nlohmann::json bookJson;
    bookJson["title"] = bookTitle;
    bookJson["details"] = manager.getBookDetails();
    bookJson["json_data"] = manager.getRawJsonData();

    std::ofstream file(favFile);
    if (file.is_open()) {
        file << bookJson.dump(4, '\t');
        file.close();
        showNotificationMessage("Book details saved to favourites.", 3000);
    }
    else {
        showNotificationMessage("Failed to save book details.", 3000);
    }
}

void BookUI::renderNotificationSection() {
    if (showNotification) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));  // Red text color
        ImGui::TextWrapped("%s", notificationMessage.c_str());
        ImGui::PopStyleColor();

        if (std::chrono::steady_clock::now() - notificationStartTime > std::chrono::milliseconds(3000)) {
            showNotification = false;
        }
    }
}

void BookUI::DisplayBooks(const std::vector<std::pair<std::string, std::string>>& books) {
    // Define a fixed width for buttons
    float buttonWidth = ImGui::GetContentRegionAvail().x; // Adjust if needed

    for (const auto& book : books) {
        ImGui::PushID(book.first.c_str());

        // Set the button color style (optional, if you want custom styling)
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));   // Black background
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));    // White text

        // Render the button with a fixed width
        if (ImGui::Button(book.first.c_str(), ImVec2(buttonWidth, 0))) {
            if (selectedBook != book.first) {
                selectedBook = book.first;  // Set the selected book
                selectedBookIsbn = book.second;
                manager.selectBook(selectedBook);
                manager.fetchBookDetails(selectedBookIsbn); // Fetch details for the selected book
                manager.addSearchToHistory(book.first, book.second);
                showBookDetails = true; // Show details for the selected book
            }
            else {
                showBookDetails = !showBookDetails; // Toggle visibility if the same book is clicked
            }
        }

        // Restore previous style
        ImGui::PopStyleColor(2); // Pop both the Button and Text style colors

        ImGui::PopID();
    }
}
