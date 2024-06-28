#pragma once
#include <unordered_map>
#include <mutex>
#include <optional>

#include "book.h"
#include "entities.h"

namespace book_library::storage
{
class BookRepository {
public:
    void addBook(std::unique_ptr<Book> book);
    std::shared_ptr<Book> removeBook(const entities::ISBNType isbn);
    std::shared_ptr<Book> removeBook(const entities::TitleType title);

    std::optional<std::shared_ptr<Book>> lookupBook(const entities::TitleType title) ;
    std::optional<std::shared_ptr<Book>> lookupBook(const entities::ISBNType isbn)  ;

    size_t totalBooks() const;

private:
    std::unordered_map<entities::ISBNType, std::shared_ptr<Book>, entities::ISBNTypeHash> mBooksByISBN;
    std::unordered_map<entities::TitleType, std::shared_ptr<Book>, entities::TitleTypeHash> mBooksByTitle;
    std::mutex repositoryMutex;
};


}
