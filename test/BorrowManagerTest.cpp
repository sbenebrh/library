#include <gtest/gtest.h>
#include "BorrowManager.h"

using namespace book_library;
using namespace book_library::entities;


class BorrowManagerTest : public ::testing::Test {
protected:
    BorrowManager borrowManager;

    ISBNType createISBN(const std::string& value) {
        ISBNType isbn;
        isbn.value = value;
        return isbn;
    }
};

TEST_F(BorrowManagerTest, BorrowBookSuccess) {
    ISBNType isbn = createISBN("123456");
    EXPECT_TRUE(borrowManager.borrowBook(isbn));
    EXPECT_TRUE(borrowManager.isBorrowed(isbn));
}

TEST_F(BorrowManagerTest, BorrowBookFailure) {
    ISBNType isbn = createISBN("123456");
    borrowManager.borrowBook(isbn);
    EXPECT_FALSE(borrowManager.borrowBook(isbn)); 
}

TEST_F(BorrowManagerTest, ReturnBookSuccess) {
    ISBNType isbn = createISBN("123456");
    borrowManager.borrowBook(isbn);
    EXPECT_TRUE(borrowManager.returnBook(isbn));
    EXPECT_FALSE(borrowManager.isBorrowed(isbn));
}

TEST_F(BorrowManagerTest, ReturnBookFailure) {
    ISBNType isbn = createISBN("123456");
    EXPECT_FALSE(borrowManager.returnBook(isbn)); 
}

TEST_F(BorrowManagerTest, IsBorrowed) {
    ISBNType isbn = createISBN("123456");
    EXPECT_FALSE(borrowManager.isBorrowed(isbn)); 
    borrowManager.borrowBook(isbn);
    EXPECT_TRUE(borrowManager.isBorrowed(isbn)); 
}

TEST_F(BorrowManagerTest, BorrowedBookCount) {
    EXPECT_EQ(borrowManager.borrowedBook(), 0); 

    ISBNType isbn1 = createISBN("123456");
    ISBNType isbn2 = createISBN("789012");
    borrowManager.borrowBook(isbn1);
    EXPECT_EQ(borrowManager.borrowedBook(), 1); 

    borrowManager.borrowBook(isbn2);
    EXPECT_EQ(borrowManager.borrowedBook(), 2); 

    borrowManager.returnBook(isbn1);
    EXPECT_EQ(borrowManager.borrowedBook(), 1); 
}
