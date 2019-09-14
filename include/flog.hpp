#ifndef OPCUA_CLIENT_LOG_H
#define OPCUA_CLIENT_LOG_H

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace ansiCode {
const std::string red("\033[0;31m");
const std::string reset("\x1b[0m");
const std::string bold("\u001b[1m");
const std::string underline("\u001b[4m");
const std::string reversed("\u001b[7m");
const std::string black("\u001b[30m");
const std::string green("\u001b[32m");
const std::string yellow("\u001b[33m");
const std::string blue("\u001b[34m");
const std::string magenta("\u001b[35m");
const std::string cyan("\u001b[36m");
const std::string white("\u001b[37m");
const std::string backgroundBlack("\u000b[40m");
const std::string backgroundRed("\u001b[41m");
const std::string backgroundGreen("\u001b[42m");
const std::string backgroundYellow("\u001b[43m");
const std::string backgroundBlue("\u001b[44m");
const std::string backgroundMagenta("\u001b[45m");
const std::string backgroundCyan("\u001b[46m");
const std::string backgroundWhite("\u001b[47m");
const std::string backgroundBrightBlack("\u001b[40;1m");
const std::string backgroundBrightRed("\u001b[41;1m");
const std::string backgroundBrightGreen("\u001b[42;1m");
const std::string backgroundBrightYellow("\u001b[43;1m");
const std::string backgroundBrightBlue("\u001b[44;1m");
const std::string backgroundBrightMagenta("\u001b[45;1m");
const std::string backgroundBrightCyan("\u001b[46;1m");
const std::string backgroundBrightWhite("\u001b[47;1m");
}  // namespace ansiCode

using boost::shared_ptr;

namespace flog {

enum severity_level { trace, debug, info, warning, error, critical };

std::ostream &operator<<(std::ostream &strm,
                                 severity_level level) {
  static const char *strings[] = {
      "trace", "debug", "info", "warning", "error", "critical"};
  if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
    strm << strings[level];
  else
    strm << static_cast<int>(level);

  return strm;
}

extern inline void init() {
  boost::shared_ptr<logging::core> core = logging::core::get();
  auto env = std::getenv("LOG");
  if (env) {
    auto log_level_env = std::string(env);
    if (log_level_env == "debug") {
      core->set_filter(expr::attr<severity_level>("Severity") >= debug);
    }
    if (log_level_env == "info") {
      core->set_filter(expr::attr<severity_level>("Severity") >= info);
    }
    if (log_level_env == "warning") {
      core->set_filter(expr::attr<severity_level>("Severity") >= warning);
    }
    if (log_level_env == "error") {
      core->set_filter(expr::attr<severity_level>("Severity") >= error);
    }
    if (log_level_env == "critical") {
      core->set_filter(expr::attr<severity_level>("Severity") >= critical);
    }
  }
  std::string timestamp = "%Y-%m-%d %H:%M:%S ";
  logging::add_console_log(
      std::clog,
      keywords::filter = expr::attr<severity_level>("Severity") == trace,
      keywords::format = expr::stream
                         << expr::format_date_time<boost::posix_time::ptime>(
                                "TimeStamp", timestamp)
                         << "|" << std::left << std::setw(16)
                         << expr::attr<std::string>("Channel") << "|"
                         << ansiCode::green << std::left << std::setw(8)
                         << expr::attr<severity_level>("Severity")
                         << ansiCode::reset << "|" << expr::message);
  logging::add_console_log(
      std::clog,
      keywords::filter = expr::attr<severity_level>("Severity") == debug,
      keywords::format = expr::stream
                         << expr::format_date_time<boost::posix_time::ptime>(
                                "TimeStamp", timestamp)
                         << "|" << std::left << std::setw(16)
                         << expr::attr<std::string>("Channel") << "|"
                         << ansiCode::cyan << std::left << std::setw(8)
                         << expr::attr<severity_level>("Severity")
                         << ansiCode::reset << "|" << expr::message);
  logging::add_console_log(
      std::clog,
      keywords::filter = expr::attr<severity_level>("Severity") == info,
      keywords::format =
          expr::stream << expr::format_date_time<boost::posix_time::ptime>(
                              "TimeStamp", timestamp)
                       << "|" << std::left << std::setw(16)
                       << expr::attr<std::string>("Channel") << "|" << std::left
                       << std::setw(8) << expr::attr<severity_level>("Severity")
                       << ansiCode::reset << "|" << expr::message);
  logging::add_console_log(
      std::clog,
      keywords::filter = expr::attr<severity_level>("Severity") == warning,
      keywords::format = expr::stream
                         << expr::format_date_time<boost::posix_time::ptime>(
                                "TimeStamp", timestamp)
                         << "|" << std::left << std::setw(16)
                         << expr::attr<std::string>("Channel") << "|"
                         << ansiCode::yellow << std::left << std::setw(8)
                         << expr::attr<severity_level>("Severity")
                         << ansiCode::reset << "|" << expr::message);
  logging::add_console_log(
      std::clog,
      keywords::filter = expr::attr<severity_level>("Severity") == error ||
                         expr::attr<severity_level>("Severity") == critical,
      keywords::format = expr::stream
                         << expr::format_date_time<boost::posix_time::ptime>(
                                "TimeStamp", timestamp)
                         << "|" << std::left << std::setw(16)
                         << expr::attr<std::string>("Channel") << "|"
                         << ansiCode::red << std::left << std::setw(8)
                         << expr::attr<severity_level>("Severity")
                         << ansiCode::reset << "|" << expr::message);
  logging::add_common_attributes();
}
}  // namespace logger

#endif  // OPCUA_CLIENT_LOG_H
