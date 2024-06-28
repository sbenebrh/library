#pragma once
#include <concepts>
#include <memory>
#include <optional>

#include "book.h"

namespace book_library {
template<typename T>
concept BookStorage = requires(T storage, std::unique_ptr<Book> book, entities::ISBNType isbn) {
    { storage.addBook(std::move(book)) } -> std::same_as<void>;
    { storage.removeBook(isbn) } -> std::same_as<std::shared_ptr<Book>>;
    { storage.lookupBook(isbn) } -> std::same_as<std::optional<std::shared_ptr<Book>>>;
    //other functions should be added also
};
}

