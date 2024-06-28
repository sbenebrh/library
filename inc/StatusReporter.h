#pragma once

#include <unordered_map>

#include "entities.h"
#include "bookStorage.h"
#include "memory"
#include "BorrowManagerIF.h"


namespace book_library::statistics
{

struct Report: std::unordered_map<std::string, size_t>
{};


template< BookStorage Storage>
class StatusReporter
{
public:
StatusReporter(std::weak_ptr<Storage> storage, std::weak_ptr<BorrowManagerIF> borrowManager);
Report getStatus() ;
entities::ISBNType getUnpopularBooks();
void updateStatistic(entities::ISBNType);
void removeFromStatistic(entities::ISBNType isbn);

private:
std::unordered_map<entities::ISBNType, size_t, entities::ISBNTypeHash> mBorrowedCounter;
std::weak_ptr<Storage> mStorage;
std::weak_ptr<BorrowManagerIF> mBorrowManager;

};

template< BookStorage Storage>
StatusReporter<Storage>::StatusReporter(std::weak_ptr<Storage> storage, std::weak_ptr<BorrowManagerIF> borrowManager)
: mStorage(storage)
, mBorrowManager(borrowManager)
{}

template<BookStorage Storage>
Report StatusReporter<Storage>::getStatus()  
{
    Report report;

    auto totalBooks = mStorage.lock()->totalBooks();
    report["total"] = totalBooks;

    auto borrowedBooks = mBorrowManager.lock()->borrowedBook();
    report["borrowed"] = borrowedBooks;

    auto availableBooks = totalBooks - borrowedBooks;
    report["available"] = availableBooks;

    return report;
}

template<BookStorage Storage>
void StatusReporter<Storage>::updateStatistic(entities::ISBNType isbn)
{
    mBorrowedCounter[isbn] +=1;
}

template<BookStorage Storage>
void StatusReporter<Storage>::removeFromStatistic(entities::ISBNType isbn)
{
    auto toRemove = mBorrowedCounter.find(isbn);
    if( toRemove != mBorrowedCounter.end())
    {
        mBorrowedCounter.erase(toRemove);
    }
    
}

template<BookStorage Storage>
entities::ISBNType StatusReporter<Storage>::getUnpopularBooks()
{
    auto leastPopularBookToRemove = std::min_element(
        mBorrowedCounter.begin(), mBorrowedCounter.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;
        });
    entities::ISBNType isbn;
    if(leastPopularBookToRemove != mBorrowedCounter.end())
    {
        isbn = leastPopularBookToRemove->first;
        mBorrowedCounter.erase(leastPopularBookToRemove);
    }


    return isbn;

        
}

}
