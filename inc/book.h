#pragma once
#include "entities.h"

#include <string>

namespace book_library
{
struct Book
{
    entities::TitleType mTitle;
    entities::AuthorType mAuthor;
    entities::ISBNType mISBN;
    entities::YearType mPublishedYear;

    Book(entities::TitleType title, entities::AuthorType author, entities::ISBNType isbn, entities::YearType year):
    mTitle{title},
    mAuthor{author},
    mISBN{isbn},
    mPublishedYear{year}
    {}
};

}