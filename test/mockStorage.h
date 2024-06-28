#pragma once

#include <gmock/gmock.h>
#include "entities.h"
#include "bookStorage.h"
#include "BorrowManager.h"
using namespace book_library;

class MockStorage {
public:
    MOCK_METHOD(void, addBook, (std::unique_ptr<book_library::Book>), ());
    MOCK_METHOD(std::shared_ptr<book_library::Book>, removeBook, (const entities::ISBNType), ());
    MOCK_METHOD(std::shared_ptr<book_library::Book>, removeBook, (const entities::TitleType), ());
    MOCK_METHOD(std::optional<std::shared_ptr<book_library::Book>>, lookupBook, (const entities::TitleType), ());
    MOCK_METHOD(std::optional<std::shared_ptr<book_library::Book>>, lookupBook, (const entities::ISBNType), ());
    MOCK_METHOD(size_t, totalBooks, (), (const));
};

class MockBorrowManager: public BorrowManagerIF {
public:
    MOCK_METHOD(bool, borrowBook, (const entities::ISBNType isbn), (override));
    MOCK_METHOD(bool, returnBook, (const entities::ISBNType isbn), (override));
    MOCK_METHOD(bool, isBorrowed, (const entities::ISBNType isbn), (override));
    MOCK_METHOD(size_t, borrowedBook, (), (const, override));
};