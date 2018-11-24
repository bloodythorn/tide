#include "tide/engine/logger.hpp"

#include <codecvt> // String Conversions
#include <iomanip> // TODO
#include <iostream>// wcerr

namespace tide { namespace Engine { namespace Log {

std::shared_ptr<Log::LogStats> Log::LogStats::m_instance;

unsigned int GetMilliseconds(
  std::chrono::time_point<std::chrono::system_clock> p_time) {

  auto elapsed = p_time - std::chrono::time_point<std::chrono::system_clock>();
  return std::chrono::duration_cast<std::chrono::milliseconds>(
      elapsed -
      std::chrono::duration_cast<std::chrono::seconds>(elapsed)).count();
}

std::wstring GetMilliString(unsigned int p_ms) {
  std::wstringstream ws;
  ws << p_ms;
  std::wstring output;
  for(size_t i = 0; i < (3 - ws.str().size()); ++i) {
    output += L"0";
  }
  return output + ws.str();
}

std::wstring GetStamp(const LogLevel& p_lvl) {
  std::wstringstream ws;
  ws << WGetNowTimeString()
     << L"[" << levelStrings[p_lvl] << L"]" << L":";
  return ws.str();
}

Logger Log(const std::wstring& p_log) { return Logger(p_log); }

Logger Log(const std::wstring& p_log, const LogLevel& p_lvl) {
  return Logger(p_log, p_lvl);
}

std::wstring ToWideString(const std::string& p_in) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.from_bytes(p_in);
}

std::string ToNarrowString(std::wstring& p_in) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
  return conv.to_bytes(p_in);
}

std::wstring WGetTimeString(
  std::chrono::time_point<std::chrono::system_clock> p_time) {
  auto time = std::chrono::system_clock::to_time_t(p_time);
  std::wostringstream ws;
  std::tm tmPut = *std::localtime(&time);
  ws << std::put_time(&tmPut, L"%FT%T") << "."
     << GetMilliString(GetMilliseconds(p_time));
  return ws.str();
}

std::wstring WGetNowTimeString() {
  return WGetTimeString(std::chrono::system_clock::now());
}

Logger::Logger(const std::wstring& p_log)
: m_log(p_log), m_level(DEF_LOG_LEVEL) { }

Logger::Logger(const std::wstring& p_log, const LogLevel& p_lvl)
: m_log(p_log), m_level(p_lvl) { }

Logger::Logger(const Logger& p_logger)
: m_log(p_logger.m_log), m_level(p_logger.m_level) {
  *this << p_logger.rdbuf();
}

Logger::Logger(Logger&& p_logger) noexcept
: m_log(p_logger.m_log), m_level(p_logger.m_level) {
  *this << p_logger.rdbuf();
}

Logger::~Logger() {
  if(m_level >= LogStats::GetInstance().GetInstance().getReportLevel(m_log))
    std::wcerr
      << GetStamp(m_level)
      << m_log << " | "
      << ((this->str().size() > 0) ? this->str() : EMPTY_LOG_MSG)
      << std::endl;
}

LogStats::LogStats() { }

LogStats::~LogStats() { }

LogStats& LogStats::GetInstance() {
  if(!m_instance) m_instance.reset(new LogStats);
  return *m_instance;
}

LogLevel LogStats::getReportLevel(
  const std::wstring& p_log) const {
  return (m_rLevels.find(p_log) == m_rLevels.end())
    ? DEF_LOG_RLEVEL : m_instance->m_rLevels[p_log];
}

LogStats& LogStats::setReportLevel(
  const std::wstring& p_log, const LogLevel& p_lvl) {
  m_instance->m_rLevels[p_log] = p_lvl;
  return *this;
}

}/*Log*/}/*Engine */}/*tide*/