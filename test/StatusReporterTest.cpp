#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "StatusReporter.h"
#include "mockStorage.h"


using namespace ::testing;
using namespace book_library;
using namespace book_library::entities;

class StatusReporterTest : public ::testing::Test {
protected:
    std::shared_ptr<MockStorage> mockStorage = std::make_shared<MockStorage>();
    std::shared_ptr<MockBorrowManager> mockBorrowManager = std::make_shared<MockBorrowManager>();
    book_library::statistics::StatusReporter<MockStorage> statusReporter{mockStorage, mockBorrowManager};
};

TEST_F(StatusReporterTest, GetStatus) {
    EXPECT_CALL(*mockStorage, totalBooks()).WillOnce(Return(100));
    EXPECT_CALL(*mockBorrowManager, borrowedBook()).WillOnce(Return(30));

    auto report = statusReporter.getStatus();
    EXPECT_EQ(report["total"], 100);
    EXPECT_EQ(report["borrowed"], 30);
    EXPECT_EQ(report["available"], 70);
}

TEST_F(StatusReporterTest, UpdateStatistic) {
    entities::ISBNType isbn{"123456"};
    statusReporter.updateStatistic(isbn);

    auto leastPopularBookToRemove = statusReporter.getUnpopularBooks();
    EXPECT_EQ(leastPopularBookToRemove.value, isbn.value);
}

TEST_F(StatusReporterTest, RemoveFromStatistic) {
    entities::ISBNType isbn{"123456"};
    statusReporter.updateStatistic(isbn);
    statusReporter.removeFromStatistic(isbn);

    auto leastPopularBookToRemove = statusReporter.getUnpopularBooks();
    EXPECT_NE(leastPopularBookToRemove.value, isbn.value);
}