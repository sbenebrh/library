#include <iostream>
#include <memory>
#include <string>
#include "BookLibrary.h"
#include "bookRepository.h"
#include "subscriber.h"
#include "bookLoader.h"

using namespace book_library;
using namespace book_library::entities;

void printMenu() {
    std::cout << "Book Library Menu:" << std::endl;
    std::cout << "1. Add a book" << std::endl;
    std::cout << "2. Remove a book by ISBN" << std::endl;
    std::cout << "3. Remove a book by Title" << std::endl;
    std::cout << "4. Lookup a book by ISBN" << std::endl;
    std::cout << "5. Lookup a book by Title" << std::endl;
    std::cout << "6. Borrow a book" << std::endl;
    std::cout << "7. Return a book" << std::endl;
    std::cout << "8. Get library status report" << std::endl;
    std::cout << "9. Load books from a file" << std::endl;
    std::cout << "10. Exit" << std::endl;
}

int main() {
    auto bookRepository = std::make_shared<storage::BookRepository>();
    auto library = std::make_shared<BookLibrary<storage::BookRepository>>(bookRepository, 100);
    Subscriber<BookLibrary<storage::BookRepository>> subscriber(library);

    std::cout << "Do you want to add books from a config file: "<<std::endl;
    int choice = 0;
    while (choice != 10) {
        printMenu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string title, author, isbn, year;

                std::cout << "Enter book title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                std::cout << "Enter book author: ";
                std::getline(std::cin, author);
                std::cout << "Enter book ISBN: ";
                std::getline(std::cin, isbn);
                std::cout << "Enter book year: ";
                std::cin >> year;

                auto book = std::make_unique<Book>(TitleType{title},AuthorType{author},ISBNType{isbn}, YearType{year});
                subscriber.addBook(std::move(book));
                break;
            }
            case 2: {
                std::string isbn;
                std::cout << "Enter book ISBN: ";
                std::cin.ignore();
                std::getline(std::cin, isbn);
                subscriber.removeBook(entities::ISBNType{isbn});
                break;
            }
            case 3: {
                std::string title;
                std::cout << "Enter book title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                subscriber.removeBook(entities::TitleType{title});
                break;
            }
            case 4: {
                std::string isbn;
                std::cout << "Enter book ISBN: ";
                std::cin.ignore();
                std::getline(std::cin, isbn);
                subscriber.lookupBook(entities::ISBNType{isbn});
                break;
            }
            case 5: {
                std::string title;
                std::cout << "Enter book title: ";
                std::cin.ignore();
                std::getline(std::cin, title);
                subscriber.lookupBook(entities::TitleType{title});
                break;
            }
            case 6: {
                std::string isbn;
                std::cout << "Enter book ISBN: ";
                std::cin.ignore();
                std::getline(std::cin, isbn);
                subscriber.borrowBook(entities::ISBNType{isbn});
                break;
            }
            case 7: {
                std::string isbn;
                std::cout << "Enter book ISBN: ";
                std::cin.ignore();
                std::getline(std::cin, isbn);
                subscriber.returnBook(entities::ISBNType{isbn});
                break;
            }
            case 8: {
                auto report = library->getStatusReport();
                std::cout << "Library Status Report:" << std::endl;
                for (const auto& [key, value] : report) {
                    std::cout << key << ": " << value << std::endl;
                }
                break;
            }
            case 9: {
                std::string filename;
                std::cout << "Enter the configuration file name: ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                loadBooksFromFile(filename, library);
                break;
            }
            case 10: {
                std::cout << "Exiting..." << std::endl;
                break;
            }
            default: {
                std::cout << "Invalid choice, please try again." << std::endl;
                break;
            }
        }
    }

    return 0;
}