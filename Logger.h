#ifndef LOGGER_H_
#define LOGGER_H_

#include <cstdarg>
#include <string>

#include <syslog.h>

/// A class to log data and events as a helper info for debugging purpose.
class Logger
{
public:
	static void init(const char* ident, int option = 0, int facility = LOG_DAEMON)
	{
		openlog(ident, option, facility);
	}

	static void debug(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_DEBUG, (std::string("<debug> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void info(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_INFO, (std::string("<info> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void notice(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_NOTICE, (std::string("<notice> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void warning(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_WARNING, (std::string("<warning> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void error(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_ERR, (std::string("<error> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void critical(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_CRIT, (std::string("<critical> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void alert(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_ALERT, (std::string("<alert> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void emergency(const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(LOG_EMERG, (std::string("<emergency> ") + format).c_str(), ap);
		va_end(ap);
	}

	static void log(int priority, const char* format, ...)
	{
		va_list ap;
		va_start(ap, format);
		vsyslog(priority, format, ap);
		va_end(ap);
	}
};


#endif
