#include <catch.hpp>
#include <Utility/Log.hpp>
#include <iostream>

TEST_CASE("Log check", "[Log]") {
    SECTION("Log setup.") {
        REQUIRE(Log().SetupStream(Log::DEFAULT, &std::cout));
        REQUIRE(Log().SetupStream(Log::INFO, &std::cout));
        REQUIRE(Log().SetupStream(Log::WARNING, &std::cout));
        REQUIRE(Log().SetupStream(Log::ERR, &std::cerr));
        REQUIRE(Log().SetupStream(Log::DEBUG, &std::cout));
    }

    SECTION("Setup all logs at once.") {
        REQUIRE(Log().SetupStreams(&std::cout, &std::cout, &std::cout, &std::cerr, &std::cout ));
    }

    SECTION("Try to setup log with wrong value.") {
        // Set to number of channels (which is one too many).
        REQUIRE_FALSE(Log().SetupStream(Log::NUMBER_OF_CHANNELS, &std::cout));
        
        // Random value.
        REQUIRE_FALSE(Log().SetupStream((Log::Channel)128, &std::cout));
        
        // Edge cases
        // Will not work.
        REQUIRE_FALSE(Log().SetupStream((Log::Channel)(Log::NUMBER_OF_CHANNELS + 1), &std::cout));
        REQUIRE_FALSE(Log().SetupStream((Log::Channel)(-1), &std::cout));

        // Will work but is extremely unecessary.
        REQUIRE(Log().SetupStream((Log::Channel)(Log::NUMBER_OF_CHANNELS - 1), &std::cout));
        REQUIRE(Log().SetupStream((Log::Channel)(0), &std::cout));
        
    }

    SECTION("Throwing exceptions with log") {
        REQUIRE_THROWS( (Log(Log::ERR) << "Test\n") );
    }

}