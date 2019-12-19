#include "ApprovalTests.hpp"
#include "catch.hpp"

using namespace ApprovalTests;

// All these tests should pass on your system.
// Please note that you will need a Diff Tool installed:
//  https://github.com/approvals/ApprovalTests.cpp/blob/master/doc/Reporters.md#supported-diff-tools

TEST_CASE("ApprovingText")
{
    Approvals::verify("Approval Tests can verify text via the golder master method ");
}

TEST_CASE("ApprovingWithReporters")
{
    // you can alter this to make it launch
    Approvals::verify("Reporters launch only on failure, by default they are the DiffReporter", DiffReporter());
}

TEST_CASE("Approval Is just an assertion library, you don't need to use it.")
{
    REQUIRE(std::string("No Approvals, only Catch") == "No Approvals, only Catch");
}
