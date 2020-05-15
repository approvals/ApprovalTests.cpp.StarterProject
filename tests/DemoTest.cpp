#define APPROVAL_TESTS_HIDE_DEPRECATED_CODE 1

#include "ApprovalTests.hpp"
#include "catch2/catch.hpp"
#include <ostream>

using namespace ApprovalTests;

TEST_CASE("DemoSimpleTestCase")
{
    Approvals::verify("Some\nMulti-line\noutput");
}

struct Demo
{
    int value1;
    int value2;

    // See https://github.com/approvals/ApprovalTests.cpp/blob/master/doc/ToString.md#top
    friend std::ostream& operator<<(std::ostream& os, const Demo& obj)
    {
        return os << "value1: " << obj.value1 << ", value2: " << obj.value2;
    }
};

TEST_CASE("DemoCombinationTestCase")
{
    // See https://github.com/approvals/ApprovalTests.cpp/blob/master/doc/TestingCombinations.md#top
    std::vector<int> evens{2, 4, 6, 8, 10};
    std::vector<int> odds{1, 3, 5, 7, 9};
    CombinationApprovals::verifyAllCombinations(
        [](int i, int j) {
            return Demo{i, j};
        },
        evens,
        odds);
}
