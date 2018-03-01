#include "ApprovalTests.hpp"
#include "Catch.hpp"
#include <ostream>

TEST_CASE("DemoSimpleTestCase")
{
    Approvals::verify("Some\nMulti-line\noutput");
}

struct Demo
{
    int value1;
    int value2;

    friend std::ostream& operator<<(std::ostream& os, const Demo& obj)
    {
        return os
            << "value1: " << obj.value1
            << ", value2: " << obj.value2;
    }
};

TEST_CASE("DemoCombinationTestCase")
{
    std::vector<int> evens{ 2, 4, 6, 8, 10 };
    std::vector<int> odds{ 1, 3, 5, 7, 9 };
    CombinationApprovals::verifyAllCombinations<
        std::vector<int>, std::vector<int>, Demo>(
            [](int i, int j) { return Demo{ i, j }; },
            evens, odds);
}

