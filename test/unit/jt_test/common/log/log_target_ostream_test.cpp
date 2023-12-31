#include <log/log_entry.hpp>
#include <log/log_level.hpp>
#include <log/log_target_ostream.hpp>
#include <gtest/gtest.h>
#include <sstream>

TEST(LogTargetOstreamTest, LogsExpectedString)
{
    std::stringstream ss;
    jt::LogTargetOstream target { ss };

    auto const logMessage = "abcd";
    target.log(jt::LogEntry { logMessage, "time", jt::LogLevel::LogLevelAction, {} });

    auto const expectedLogEntry = logMessage + std::string("\n");
    ASSERT_EQ(ss.str(), expectedLogEntry);
}

TEST(LogTargetOstreamTest, SetLevelSetsLevel)
{
    std::stringstream ss;
    jt::LogTargetOstream target { ss };

    ASSERT_NO_THROW(target.setLogLevel(jt::LogLevel::LogLevelFatal));
}
