#pragma once
#include <string>
#include <memory>
#include <optional>
#include <iostream>


#include "book.h"
#include "entities.h"

namespace book_library {

template <typename Library>
class Subscriber {
public:
    Subscriber(std::shared_ptr<Library> library);

    void addBook(std::unique_ptr<Book> book);
    std::optional<std::shared_ptr<Book>> removeBook(const entities::ISBNType isbn);
    std::optional<std::shared_ptr<Book>> removeBook(const entities::TitleType title);

    std::optional<std::shared_ptr<Book>> lookupBook(const entities::ISBNType isbn);
    std::optional<std::shared_ptr<Book>> lookupBook(const entities::TitleType title);

    std::optional<std::shared_ptr<Book>> borrowBook(const entities::ISBNType isbn);
    void returnBook(const entities::ISBNType isbn);

private:
    std::shared_ptr<Library> mLibrary;
};



template <typename Library>
Subscriber<Library>::Subscriber(std::shared_ptr<Library> library)
    : mLibrary(library) {}

template <typename Library>
void Subscriber<Library>::addBook(std::unique_ptr<Book> book) {
    mLibrary->addBook(std::move(book));
    std::cout << "Book added to the library." << std::endl;
}

template <typename Library>
std::optional<std::shared_ptr<Book>> Subscriber<Library>::removeBook(const entities::ISBNType isbn) {
    const auto& book = mLibrary->removeBook(isbn);
    if (book.has_value()) {
        std::cout << "Book removed from the library." << std::endl;
    } 
    else {
        std::cout << "The book is currently borrowed or does not exist." << std::endl;
    }
    return book;
}

template <typename Library>
std::optional<std::shared_ptr<Book>> Subscriber<Library>::removeBook(const entities::TitleType title) {
    auto book = mLibrary->removeBook(title);
    if (book.has_value()) {
        std::cout << "Book removed from the library." << std::endl;
    } else {
        std::cout << "The book is currently borrowed or does not exist." << std::endl;
    }
    return book;
}

template <typename Library>
std::optional<std::shared_ptr<Book>> Subscriber<Library>::lookupBook(const entities::ISBNType isbn) {
    auto book = mLibrary->lookupBook(isbn);
    if (book.has_value()) {
        std::cout << "Book found: " << book.value()->mTitle.value << std::endl;
    } else {
        std::cout << "Book not found." << std::endl;
    }
    return book;
}

template <typename Library>
std::optional<std::shared_ptr<Book>> Subscriber<Library>::lookupBook(const entities::TitleType title) {
    auto book = mLibrary->lookupBook(title);
    if (book.has_value()) {
        std::cout << "Book found: " << book.value()->mTitle.value << std::endl;
    } else {
        std::cout << "Book not found." << std::endl;
    }
    return book;
}

template <typename Library>
std::optional<std::shared_ptr<Book>> Subscriber<Library>::borrowBook(const entities::ISBNType isbn) {
    auto book = mLibrary->borrowBook(isbn);
    if (book.has_value()) {
        std::cout << "Book borrowed: " << book.value()->mTitle.value << std::endl;
    } else {
        std::cout << "The book is currently borrowed or does not exist." << std::endl;
    }
    return book;
}

template <typename Library>
void Subscriber<Library>::returnBook(const entities::ISBNType isbn) {
    mLibrary->returnBook(isbn);
    std::cout << "Book returned to the library." << std::endl;
}



}