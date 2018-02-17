#include "ApprovalTests.hpp"
#include "Catch.hpp"
/**
 * In clion, you can run this with ctrl+r
 * You might need to add a Catch Runtime to see it in the unit test window
 * Run > edit configurations > + > Catch > Target=Playground_Tests
 */
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



