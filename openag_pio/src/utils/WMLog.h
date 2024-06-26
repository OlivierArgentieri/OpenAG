#ifndef WMLOG_H
#define WMLOG_H

#include <WString.h>
#include <HardwareSerial.h>

#define LOG(MSG, LVL) WMLog::GetInstance().Log(MSG, WMLog::LogLevel::LVL);
#define LOG_INFO(MSG) WMLog::GetInstance().Log(MSG, WMLog::LogLevel::INFO);
#define LOG_DEBUG(MSG) WMLog::GetInstance().Log(MSG, WMLog::LogLevel::DEBUG);
#define LOG_WARNING(MSG) WMLog::GetInstance().Log(MSG, WMLog::LogLevel::WARNING);
#define LOG_ERROR(MSG) WMLog::GetInstance().Log(MSG, WMLog::LogLevel::ERROR);

class WMLog
{
public:
  /**
   * Public Enum
   */
	enum LogLevel : int {
		INFO = 0,
		DEBUG = 1,
		WARNING = 2,
		ERROR = 3,
	};

public:
  /**
   * Contructor
   */
  
  /** Default constructor */
  WMLog(const HardwareSerial& Serial);

public:
	/**
   * Public Methods
   */

	/** Log */
	void Log(const String& message, LogLevel level = LogLevel::INFO);

  /** Get Instance */
  // TODO defin macro for that
  static WMLog& GetInstance() {
    static WMLog instance(Serial);
    return instance;
  }

private:
  /**
   * private Property 
   */
  inline static HardwareSerial* serial = nullptr;

  /** List matching enum value and color value */
  inline static const String logLevelColors[] = {
    "\033[32m", // INFO
    "\033[0m",  // DEBUG
    "\033[33m", // WARNING
    "\033[31m", // ERROR
  };
  
  /** List matching enum value and name value */
  inline static const String logLevelnames[] = {
    "INFO",
    "DEBUG",
    "WARNING",
    "ERROR",
  };

private:
  /**
   * private methods
   */

  /** Get current time as hours:minutes:seconds */
  inline static String GetTime() {
    // Get current time
    unsigned long currentMillis = millis();

    // Convert to hours:minutes:seconds
    unsigned long seconds = currentMillis / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;

    // Format time
    String time = "";
    time += hours;
    time += ":";
    time += minutes % 60;
    time += ":";
    time += seconds % 60;

    return time;
  }
};

inline WMLog::WMLog(const HardwareSerial& Serial) {
	// store instance
	serial = const_cast<HardwareSerial*>(&Serial);
}

inline void WMLog::Log(const String& message, LogLevel level) {
  // Set color
  serial->print(WMLog::logLevelColors[level]);
  
  // Adding timestamp to message as current hours:minutes:seconds
  serial->print("[");
  serial->print(WMLog::GetTime());
  serial->print("] ");

  // Print and format message with level
  serial->print("[");
  serial->print(WMLog::logLevelnames[level]);
  serial->print("] ");

  //Print message
  serial->println(message);

  // Reset color
  serial->print("\033[0m");
}
#endif // WMLOG_H
