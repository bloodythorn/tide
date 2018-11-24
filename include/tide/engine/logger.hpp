#ifndef LOG_HPP
#define LOG_HPP

#include <chrono>        // Clock Functions
#include <memory>        // shared ptr
#include <sstream>       // wstringstream
#include <string>        // string/wstring
#include <unordered_map> // duh
#include <vector>        // vector<>

namespace tide { namespace Engine { namespace Log {

/* Namespace Data Members */

  /** @brief Empty Log Message
    *
    *  Default Empty Log.
    *
    */
  static const std::wstring EMPTY_LOG_MSG = L"---";

  /** @brief Log Levels
    *
    * Enumeration for log levels.
    *
    */
  enum LogLevel { Debug, Info, Warn, Error, Fatal };

  /** @brief Default Log Level
    *
    *  Initial Log Level settings.
    *
    */
  static const LogLevel DEF_LOG_LEVEL = Debug;
  static const LogLevel DEF_LOG_RLEVEL = Debug;

  /** @brief Log Level Strings
    *
    *  String counterparts to log level enumerations.
    *
    */
  static const std::vector<std::wstring> levelStrings = {
    L"Debug", L"Info", L"Warn", L"Error", L"Fatal"
  };

  /** @brief Log Config
    *
    *  Log name used if no log name is defined.
    *
    */
  static const std::wstring DEF_LOG_NAME{L"Log"};

  /* Namespace Functions */

  /** @brief Extracts Milliseconds from Time
    *
    *  This function will return how many milliseconds have elapsed since the
    *  time_point given. If the time_point is in the future the result is
    *  undefined.
    *
    *  @param p_time Time Point
    *  @return Milliseconds elapsed in an unsigned int
    *
    */
  unsigned int GetMilliseconds(
    std::chrono::time_point<std::chrono::system_clock>);

  /** @brief Formats Milliseconds string */
  std::wstring GetMilliString(unsigned int);

  std::wstring GetStamp(const LogLevel&);

  std::wstring ToWideString(std::string);
  std::string ToNarrowString(std::wstring);

  /** @brief Converts time point to string */
  std::wstring WGetTimeString(
    std::chrono::time_point<std::chrono::system_clock> p_time);

  /** @brief Retrieves String of Current Time */
  std::wstring WGetNowTimeString();

  /** @brief Logger Object */
  class Logger : public std::wostringstream {
  public:

    explicit
    Logger(const std::wstring&);
    Logger(const std::wstring&, const LogLevel&);
    ~Logger();
    Logger(const Logger&);
    Logger(Logger&&) noexcept;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

  private:

    /* Data Members */
    const std::wstring m_log;
    const LogLevel     m_level;
  };

  /** @brief Logger creation function */
  Logger Log(const std::wstring&);
  Logger Log(const std::wstring&, const LogLevel&);

  /* Class to hold all state data, instantiates on first use */
  class LogStats {
  public:

    /** Destructor */
    ~LogStats();

    /* Member Functions */

    /** @brief Getter for Instance */
    static LogStats& GetInstance();

    /** @brief Loglevel Getter/Setters */
    LogLevel getReportLevel(const std::wstring&) const;
    LogStats& setReportLevel(const std::wstring&, const LogLevel&);

  private:

    /** Default Constructor */
    LogStats();

    /** Copy Constructor */
    LogStats(const LogStats&) = delete;

    /** Move Constructor */
    LogStats(LogStats&) noexcept = delete;

    /** Copy Assignment Operator */
    LogStats& operator=(const LogStats&) = delete;

    /** Move Assignment Operator */
    LogStats& operator=(LogStats&&) noexcept = delete;

    /* Data Members */
    static std::shared_ptr<LogStats> m_instance;
    std::unordered_map<std::wstring, LogLevel> m_rLevels;
  };
}/*Log*/}/*Engine*/}/*tide*/

#define LOG_STAT     tide::Engine::Log::LogStats::GetInstance()
#define LOG_DEBUG(a) tide::Engine::Log::Log(a, tide::Engine::Log::Debug)
#define LOG_INFO(a)  tide::Engine::Log::Log(a, tide::Engine::Log::Info)
#define LOG_WARN(a)  tide::Engine::Log::Log(a, tide::Engine::Log::Warn)
#define LOG_ERROR(a) tide::Engine::Log::Log(a, tide::Engine::Log::Error)
#define LOG_FATAL(a) tide::Engine::Log::Log(a, tide::Engine::Log::Fatal)

#endif //TLOG_HPP
