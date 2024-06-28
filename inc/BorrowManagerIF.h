#pragma once
#include <memory>
#include <unordered_map>
#include <vector>

#include "entities.h"

namespace book_library
{
class BorrowManagerIF {
public:
    [[nodiscard]] virtual bool borrowBook(const entities::ISBNType isbn) = 0;
    virtual bool returnBook(const entities::ISBNType isbn) = 0;
    [[nodiscard]] virtual bool isBorrowed(const entities::ISBNType isbn) = 0;

    virtual size_t borrowedBook() const = 0;
};
}