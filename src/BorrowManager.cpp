#include "BorrowManager.h"

namespace book_library{


bool BorrowManager::borrowBook(const entities::ISBNType isbn) {
    auto it = std::find(mBorrowed.begin(), mBorrowed.end(), isbn);
    if (it != mBorrowed.end()) {
        return false;
    }
    mBorrowed.push_back(isbn);
    return true; 
}

bool BorrowManager::returnBook(const entities::ISBNType isbn) {
    auto it = std::find(mBorrowed.begin(), mBorrowed.end(), isbn);

    if (it == mBorrowed.end()) {
        return false;
    }

    mBorrowed.erase(it);
    return true; 
}

bool BorrowManager::isBorrowed(const entities::ISBNType isbn){
    auto it = std::find(mBorrowed.begin(), mBorrowed.end(), isbn);
    return it != mBorrowed.end();


}

size_t BorrowManager::borrowedBook() const
{
    return mBorrowed.size();
}

}
