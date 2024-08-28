authored by mohammad enaby
Open Library Book Management Application
This is a C++ application that interacts with the Open Library API to fetch, display, and manage a list of books. The application allows users to search for books by title, author, recommendation date, or ISBN, and view detailed information about each book. Users can also save their favorite books, view their search history, and interact with a user-friendly graphical interface.

Features
Book Search: Search for books by title, author, recommendation date, or ISBN.
Book Details: View detailed information about each book, including title, author, summary, and ratings.
Favorites: Save your favorite books to a local directory for easy access.
Search History: View and manage your previous searches.
User Interface: A graphical interface for easy interaction with the application.
Multithreading: Efficiently fetch and display data using C++ threading.
Sorting: Sort books by various criteria.
Installation:
Prerequisites
C++ compiler (e.g., g++, MSVC)
CMake
OpenSSL (for secure API requests)

Dependencies:
httplib for HTTP requests
nlohmann/json for JSON parsing
ImGui for the graphical interface

Usage
Search for Books: Use the search fields to find books by title, author, recommendation date, or ISBN.
View Book Details: Click on a book to see detailed information.
Save Favorites: Use the 'Add to Favorites' button to save books.
View Search History: Click on a search field to see previous searches.
Sort Books: Use the sort button to organize books by various criteria.
Project Structure
src/: Contains the source code files.
include/: Contains the header files.
libs/: External libraries like httplib, json, ImGui.
build/: Directory for the compiled binaries.
CMakeLists.txt: CMake configuration file.
API Integration
This project uses the Open Library API to fetch book data. The API provides access to a vast collection of books, including metadata and details.

Contributing
Contributions are welcome! Please fork the repository and submit a pull request with your changes. Ensure that your code follows the project's coding standards and includes appropriate documentation.
