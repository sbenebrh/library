#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "bookRepository.h"

using namespace book_library;
using namespace book_library::storage;
using namespace book_library::entities;


class BookRepositoryTest : public ::testing::Test {
protected:
    BookRepository bookRepository;

    std::unique_ptr<Book> createBook(const std::string& title, const std::string& author, const std::string& isbn, std::string year) {
        return std::make_unique<Book>(TitleType{title}, AuthorType{author}, ISBNType{isbn}, YearType{year});
    }
};

TEST_F(BookRepositoryTest, AddBook) {
    auto book = createBook("Harry Potter", "J K Rowling", "123456", "2001");
    bookRepository.addBook(std::move(book));

    ISBNType isbn{"123456"};
    TitleType title{"Harry Potter"};

    auto foundBookByISBN = bookRepository.lookupBook(isbn);
    auto foundBookByTitle = bookRepository.lookupBook(title);

    ASSERT_TRUE(foundBookByISBN.has_value());
    ASSERT_TRUE(foundBookByTitle.has_value());
    EXPECT_EQ(foundBookByISBN.value()->mTitle.value, title.value);
    EXPECT_EQ(foundBookByTitle.value()->mTitle.value, title.value);
}


TEST_F(BookRepositoryTest, RemoveBookByISBN) {
    auto book = createBook("Harry Potter", "J K Rowling", "123456", "2001");
    bookRepository.addBook(std::move(book));

    ISBNType isbn{"123456"};
    TitleType title{"Harry Potter"};
    auto removedBook = bookRepository.removeBook(isbn);

    ASSERT_TRUE(removedBook);
    EXPECT_EQ(removedBook->mTitle.value, title.value);
    EXPECT_FALSE(bookRepository.lookupBook(isbn).has_value());
}

TEST_F(BookRepositoryTest, RemoveBookByTitle) {
    auto book = createBook("Harry Potter", "J K Rowling", "123456", "2001");
    bookRepository.addBook(std::move(book));

    TitleType title{"Harry Potter"};
    auto removedBook = bookRepository.removeBook(title);

    ASSERT_TRUE(removedBook);
    EXPECT_EQ(removedBook->mTitle.value, title.value);
    EXPECT_FALSE(bookRepository.lookupBook(title).has_value());
}

TEST_F(BookRepositoryTest, LookupBook) {
    auto book = createBook("Harry Potter", "J K Rowling", "123456", "2001");
    bookRepository.addBook(std::move(book));

    ISBNType isbn{"123456"};
    TitleType title{"Harry Potter"};

    auto foundBookByISBN = bookRepository.lookupBook(isbn);
    auto foundBookByTitle = bookRepository.lookupBook(title);

    ASSERT_TRUE(foundBookByISBN.has_value());
    ASSERT_TRUE(foundBookByTitle.has_value());
    EXPECT_EQ(foundBookByISBN.value()->mTitle.value, title.value);
    EXPECT_EQ(foundBookByTitle.value()->mTitle.value, title.value);
}

TEST_F(BookRepositoryTest, TotalBooks) {
    EXPECT_EQ(bookRepository.totalBooks(), 0);

    auto book1 = createBook("Harry Potter", "J K Rowling", "123456", "2001");
    auto book2 = createBook("Harry Potter 2", "J K Rowling", "789012", "2002");
    bookRepository.addBook(std::move(book1));
    bookRepository.addBook(std::move(book2));

    EXPECT_EQ(bookRepository.totalBooks(), 2);

    ISBNType isbn{"123456"};
    bookRepository.removeBook(isbn);
    EXPECT_EQ(bookRepository.totalBooks(), 1);
}