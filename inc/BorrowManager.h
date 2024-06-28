#pragma once

#include <memory>
#include<unordered_map>
#include<vector>

#include "BorrowManagerIF.h"
#include "entities.h"

namespace book_library
{
class BorrowManager: public BorrowManagerIF {
public:
[no_discard] bool borrowBook(const entities::ISBNType isbn) override;
bool returnBook(const entities::ISBNType isbn) override;
[no_discard] bool isBorrowed(const entities::ISBNType isbn) override;

size_t borrowedBook() const override;

private:
std::vector<entities::ISBNType > mBorrowed;
};
} 
