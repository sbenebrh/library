#pragma once
#include <string>
#include <memory>
#include <fstream>
#include <sstream>

#include "BookLibrary.h"
#include "entities.h"

namespace book_library {

template <typename Library>
void loadBooksFromFile(const std::string& filename, std::shared_ptr<Library>& lib) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string title, author, isbn, year;

        if (std::getline(iss, title, ',') &&
            std::getline(iss, author, ',') &&
            std::getline(iss, isbn, ',') &&
            std::getline(iss, year, ',')) {

            auto book = std::make_unique<Book>(entities::TitleType{title}, entities::AuthorType{author}, entities::ISBNType{isbn}, entities::YearType{year});
            lib->addBook(std::move(book));
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();
}
}


//example of config file format

/*
Harry Potter and the Philosopher's Stone,J.K. Rowling,9780747532699,1997
The Hobbit,J.R.R. Tolkien,9780345339683,1937
1984,George Orwell,9780451524935,1949
To Kill a Mockingbird,Harper Lee,9780060935467,1960
The Great Gatsby,F. Scott Fitzgerald,9780743273565,1925
*/