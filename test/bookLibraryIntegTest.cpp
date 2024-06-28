#include <gtest/gtest.h>
#include "BookLibrary.h"
#include "bookRepository.h"
#include "BorrowManager.h"

using namespace book_library;
using namespace book_library::entities;

class BookLibraryIntegrationTest : public ::testing::Test {
protected:
    std::shared_ptr<storage::BookRepository> bookRepository = std::make_shared<storage::BookRepository>();
    BookLibrary<storage::BookRepository> library{bookRepository, 100};
};

TEST_F(BookLibraryIntegrationTest, AddAndLookupBook) {
    auto book = std::make_unique<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});
    library.addBook(std::move(book));

    auto foundBook = library.lookupBook(entities::ISBNType{"123"});
    EXPECT_TRUE(foundBook.has_value());
    EXPECT_EQ(foundBook.value()->mTitle.value, "Title");
}

TEST_F(BookLibraryIntegrationTest, BorrowAndReturnBook) {
    auto book = std::make_unique<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});
    library.addBook(std::move(book));

    auto borrowedBook = library.borrowBook(entities::ISBNType{"123"});
    EXPECT_TRUE(borrowedBook.has_value());
    EXPECT_EQ(borrowedBook.value()->mTitle.value, "Title");

    library.returnBook(entities::ISBNType{"123"});
    auto bookAfterReturn = library.lookupBook(entities::ISBNType{"123"});
    EXPECT_TRUE(bookAfterReturn.has_value());
}