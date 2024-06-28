#pragma once
#include <memory>
#include <optional>
#include <mutex>

#include "book.h"
#include "bookStorage.h"
#include "BorrowManager.h"
#include "StatusReporter.h"

#include <iostream>


namespace book_library
{
template <BookStorage Storage/*, RecommandationSystem RecSystem*/>
class BookLibrary
{
public:
    BookLibrary(std::shared_ptr<Storage> storage, size_t capacity = 100);
    
    //API
    void addBook(std::unique_ptr<Book> book);
    std::optional<std::shared_ptr<Book>> removeBook(const entities::ISBNType isbn);
    std::optional<std::shared_ptr<Book>> removeBook(const entities::TitleType title);

    std::optional<std::shared_ptr<Book>> lookupBook(const entities::ISBNType isbn) ;
    std::optional<std::shared_ptr<Book>> lookupBook(const entities::TitleType title);

    std::optional<std::shared_ptr<Book>> borrowBook(const entities::ISBNType isbn);
    void returnBook(const entities::ISBNType isbn);

    statistics::Report getStatusReport();  //TOOD: convert to report class

private:

    void removeUnpopularBookIfNeeded();

private: 
    std::shared_ptr<Storage> mStorage;
    std::shared_ptr<BorrowManager> mBorrowManager;
    statistics::StatusReporter<Storage> mStatusReporter;
    size_t mCapacity;
    std::mutex libraryMutex;

};

template<BookStorage Storage>
BookLibrary<Storage>::BookLibrary(std::shared_ptr<Storage> storage, size_t capacity): mStorage(std::move(storage)) 
, mBorrowManager(std::make_shared<BorrowManager>()),
mStatusReporter(mStorage, mBorrowManager),
mCapacity(capacity)
{
}

template<BookStorage Storage>
void BookLibrary<Storage>::addBook(std::unique_ptr<Book> book) {
    std::lock_guard<std::mutex> lock(libraryMutex);
    removeUnpopularBookIfNeeded();
    auto title = book->mTitle;
    mStorage->addBook(std::move(book));

    const auto& bookAfterStore = mStorage->lookupBook(title);
    if (bookAfterStore.has_value())
    {
        mStatusReporter.updateStatistic(bookAfterStore.value()->mISBN);
    }
}

template<BookStorage Storage>
std::optional<std::shared_ptr<Book>> BookLibrary<Storage>::removeBook(const entities::ISBNType isbn){

    std::lock_guard<std::mutex> lock(libraryMutex);
    const auto& book = mStorage->lookupBook(isbn);
    if(book.has_value()) {
        if(! mBorrowManager->isBorrowed(isbn))
        {
            mStatusReporter.removeFromStatistic(isbn);
            return  mStorage->removeBook(isbn);
        }
    }

    return std::nullopt;

}

template<BookStorage Storage>
std::optional<std::shared_ptr<Book>> BookLibrary<Storage>::removeBook(const entities::TitleType title){
    std::lock_guard<std::mutex> lock(libraryMutex);
    const auto& book = mStorage->lookupBook(title);
    if(book.has_value()) {
        std::cout << "has_value" <<std::endl;
        if(! mBorrowManager->isBorrowed(book.value()->mISBN))
        {
            mStatusReporter.removeFromStatistic(book.value()->mISBN);
            return  mStorage->removeBook(title);
        }
            
    }

    return std::nullopt;
}

template<BookStorage Storage>
std::optional<std::shared_ptr<Book>> BookLibrary<Storage>::lookupBook(const entities::ISBNType isbn) 
{
    return mStorage->lookupBook(isbn);
}

template<BookStorage Storage>
std::optional<std::shared_ptr<Book>> BookLibrary<Storage>::lookupBook(const entities::TitleType title) 
{
    return mStorage->lookupBook(title);
}


template<BookStorage Storage>
std::optional<std::shared_ptr<Book>> BookLibrary<Storage>::borrowBook(const entities::ISBNType isbn)
{
    std::lock_guard<std::mutex> lock(libraryMutex);
    const auto& book = mStorage->lookupBook(isbn);
    if(book.has_value())
    {
        if(mBorrowManager->borrowBook(book.value()->mISBN))
        {
            return book.value();
        }
    }
    return std::nullopt;
}

template<BookStorage Storage>
void BookLibrary<Storage>::returnBook(const entities::ISBNType isbn)
{
    mBorrowManager->returnBook(isbn);
}

template<BookStorage Storage>
statistics::Report BookLibrary<Storage>::getStatusReport()
{
    return mStatusReporter.getStatus();
}

template <BookStorage Storage>
void BookLibrary<Storage>::removeUnpopularBookIfNeeded() {
    if (mStorage->totalBooks() >= mCapacity) {
        auto leastPopularBook = mStatusReporter.getUnpopularBooks();
        mStorage->removeBook(leastPopularBook);
    }
}



}
