#pragma once

#include <string>
namespace book_library::entities
{

template <typename T>
struct Entity 
{
    T value;
    bool operator< (const Entity& other) const {
        return value < other.value;
    }
    bool operator== (const Entity& other) const {
        return value == other.value;
    }
};

struct TitleType: Entity<std::string> {};
struct AuthorType: Entity<std::string> {};
struct ISBNType: Entity<std::string> {};
struct YearType: Entity<std::string> {};

struct TitleTypeHash {
    std::size_t operator()(const TitleType& title) const {
        return std::hash<std::string>()(title.value);
    }
};

struct ISBNTypeHash {
    std::size_t operator()(const ISBNType& isbn) const {
        return std::hash<std::string>()(isbn.value);
    }
};

}