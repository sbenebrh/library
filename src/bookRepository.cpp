#include "bookRepository.h"

namespace book_library::storage{

void BookRepository::addBook(std::unique_ptr<Book> book)
{
    std::lock_guard<std::mutex> lock(repositoryMutex);
    entities::ISBNType isbn{book->mISBN};
    entities::TitleType title{book->mTitle};
    auto sharedBook = std::shared_ptr<Book>(std::move(book));
    mBooksByISBN[isbn] = sharedBook;
    mBooksByTitle[title] = sharedBook;
}

std::shared_ptr<Book> BookRepository::removeBook(const entities::ISBNType isbn)
{
    std::lock_guard<std::mutex> lock(repositoryMutex);
    auto it = mBooksByISBN.find(isbn);
    if (it != mBooksByISBN.end()) {
        
        auto book = it->second;
        mBooksByTitle.erase(entities::TitleType{book->mTitle});
        mBooksByISBN.erase(it->first);
        return book;
    }
    return nullptr;
}

std::shared_ptr<Book> BookRepository::removeBook(const entities::TitleType title) {

    std::lock_guard<std::mutex> lock(repositoryMutex);
    auto it = mBooksByTitle.find(title);
    if (it != mBooksByTitle.end()) {
        auto book = it->second;
        mBooksByISBN.erase(entities::ISBNType{book->mISBN});
        mBooksByTitle.erase(it);
        return book;
    }
    return nullptr;
}

std::optional<std::shared_ptr<Book>> BookRepository::lookupBook(const entities::TitleType title) {
    std::lock_guard<std::mutex> lock(repositoryMutex);
    auto it = mBooksByTitle.find(title);
    if (it != mBooksByTitle.end()) {
        return it->second;
    }
    return std::nullopt; 
}

std::optional<std::shared_ptr<Book>> BookRepository::lookupBook(const  entities::ISBNType isbn) {
    std::lock_guard<std::mutex> lock(repositoryMutex);
    auto it = mBooksByISBN.find(isbn);
    if (it != mBooksByISBN.end()) {
        return it->second;
    }
    return std::nullopt;
}

size_t BookRepository::totalBooks() const
{
    return mBooksByISBN.size();
}

}
