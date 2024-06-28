#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "BookLibrary.h"
#include "mockStorage.h"

using namespace ::testing;
using namespace book_library;
using namespace book_library::entities;

class BookLibraryTest : public ::testing::Test {
protected:
    std::shared_ptr<MockStorage> mockStorage = std::make_shared<MockStorage>();
    BookLibrary<MockStorage> library{mockStorage, 100};
};

TEST_F(BookLibraryTest, AddBook) {
    auto book = std::make_unique<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});
    EXPECT_CALL(*mockStorage, addBook(_)).Times(1);

    library.addBook(std::move(book));
}

TEST_F(BookLibraryTest, RemoveBookByISBN) {
    entities::ISBNType isbn{"123"};
    
    auto book = std::make_shared<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});
    EXPECT_CALL(*mockStorage, lookupBook(isbn)).WillOnce(Return(std::make_optional(book)));

    EXPECT_CALL(*mockStorage, removeBook(isbn)).WillOnce(Return (std::make_shared<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"})));

    auto bookREmoved = library.removeBook(isbn);
    EXPECT_TRUE(bookREmoved.has_value());
}

TEST_F(BookLibraryTest, RemoveBookByTitle) {
    entities::TitleType title{"Title"};
    auto book = std::make_shared<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});    EXPECT_CALL(*mockStorage, lookupBook(title)).WillOnce(Return(std::make_optional(book)));
    EXPECT_CALL(*mockStorage, removeBook(title)).WillOnce(Return(book));

    auto removedBook = library.removeBook(title);
    EXPECT_TRUE(removedBook.has_value());
}

TEST_F(BookLibraryTest, LookupBookByISBN) {
    entities::ISBNType isbn{"123"};
    auto book = std::make_shared<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});    
    EXPECT_CALL(*mockStorage, lookupBook(isbn)).WillOnce(Return(book));

    auto foundBook = library.lookupBook(isbn);
    EXPECT_TRUE(foundBook.has_value());
}

TEST_F(BookLibraryTest, LookupBookByTitle) {
    entities::TitleType title{"Title"};
    auto book = std::make_shared<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});    
    EXPECT_CALL(*mockStorage, lookupBook(title)).WillOnce(Return(std::make_optional(book)));

    auto foundBook = library.lookupBook(title);
    EXPECT_TRUE(foundBook.has_value());
}

TEST_F(BookLibraryTest, BorrowBook) {
    entities::ISBNType isbn{"123"};
    auto book = std::make_shared<Book>(TitleType{"Title"}, AuthorType{"Author"}, ISBNType{"123"}, YearType{"2001"});    
    EXPECT_CALL(*mockStorage, lookupBook(isbn)).WillOnce(Return(std::make_optional(std::make_unique<Book>(*book))));

    auto borrowedBook = library.borrowBook(isbn);
    EXPECT_TRUE(borrowedBook.has_value());
}


TEST_F(BookLibraryTest, GetStatusReport) {
    EXPECT_CALL(*mockStorage, totalBooks()).WillOnce(Return(100));

    auto report = library.getStatusReport();
    EXPECT_EQ(report["total"], 100);
    EXPECT_EQ(report["borrowed"], 0);
    EXPECT_EQ(report["available"], 100);
}
