#include <gtest/gtest.h>
#include <thread>
#include "BookLibrary.h"
#include "bookRepository.h"
#include "BorrowManager.h"
#include "subscriber.h"

using namespace book_library;
using namespace book_library::entities;


void addBooks(Subscriber<BookLibrary<storage::BookRepository>>& subscriber, int start, int end) {
    for (int i = start; i <= end; ++i) {
        auto book = std::make_unique<Book>(TitleType{"Title" + std::to_string(i)}, AuthorType{"Author" + std::to_string(i)}, ISBNType{std::to_string(i)}, YearType{"2020"});
        subscriber.addBook(std::move(book));
    }
}

void borrowBooks(Subscriber<BookLibrary<storage::BookRepository>>& subscriber, int start, int end) {
    for (int i = start; i <= end; ++i) {
        subscriber.borrowBook(entities::ISBNType{std::to_string(i)});
    }
}

void returnBooks(Subscriber<BookLibrary<storage::BookRepository>>& subscriber, int start, int end) {
    for (int i = start; i <= end; ++i) {
        subscriber.returnBook(entities::ISBNType{std::to_string(i)});
    }
}

TEST(ConcurrencyTest, ConcurrentOperations) {
    auto bookRepository = std::make_shared<storage::BookRepository>();
    auto library = std::make_shared<BookLibrary<storage::BookRepository>>(bookRepository, 100);

    // Subscriber interacts with the library
    Subscriber<BookLibrary<storage::BookRepository>> subscriber(library);

    std::thread t1(addBooks, std::ref(subscriber), 1, 50);
    std::thread t2(addBooks, std::ref(subscriber), 51, 100);
    std::thread t3(borrowBooks, std::ref(subscriber), 1, 50);
    std::thread t4(returnBooks, std::ref(subscriber), 1, 50);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    auto report = library->getStatusReport();
    EXPECT_EQ(report["total"], 100);
    EXPECT_EQ(report["borrowed"], 0);  
}