// easylogger - Simple "good enough" C++ logging framework
// 
// Copyright (C) 2010 Sean Middleditch <sean@middleditch.us>
//
// janssonxx is free software; you can redistribute it and/or modify
// it under the terms of the MIT license. See LICENSE for details.

#if !defined(EASYLOGGER_H)
#define EASYLOGGER_H

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

namespace easylogger {

	class Logger;

	enum LogLevel {
		LEVEL_TRACE,
		LEVEL_DEBUG,
		LEVEL_INFO,
		LEVEL_WARNING,
		LEVEL_ERROR,
		LEVEL_FATAL
	};

	class LogSink {
	public:
		LogSink(Logger* logger, LogLevel level, const char* file,
				unsigned int line, const char* func) : _logger(logger),
				_level(level), _file(file), _line(line), _func(func) {}

		LogSink(const LogSink& sink) : _logger(sink._logger),
				_level(sink._level), _file(sink._file), _line(sink._line),
				_func(sink._func) {}

		::std::ostream& Stream() { return _os; }

		inline ~LogSink();
		
	private:
		::std::ostringstream _os;

		Logger* _logger;

		LogLevel _level;

		const char* _file;

		unsigned int _line;

		const char* _func;
	};

	class Logger {
	public:
		Logger(const ::std::string& name) : _name(name), _parent(0),
				_level(LEVEL_INFO), _stream(&::std::cout),
				_format("[%F:%C %P] %N %L: %S") {}

		Logger(const ::std::string& name, Logger& parent) : _name(name),
				_parent(&parent), _level(LEVEL_INFO), _stream(0),
				_format("[%F:%C %P] %N %L: %S") {}

		~Logger() {}

		const ::std::string& Name() const { return _name; }

		LogLevel Level() const { return _level; }

		LogLevel Level(LogLevel level) { return _level = level; }

		inline bool IsLevel(LogLevel level) const;

		inline LogSink Log(LogLevel level, const char* file, unsigned int line,
				const char* func);

		inline void Log(LogLevel level, const char* file, unsigned int line,
				const char* func, const char* message);

		::std::ostream& Stream() const { return *_stream; }

		inline ::std::ostream& Stream(::std::ostream& stream);

		const ::std::string& Format() const { return _format; }

		const ::std::string& Format(const ::std::string& format);

		void Flush() { _stream->flush(); }

		void WriteLog(LogLevel level, Logger* logger, const char* file,
				unsigned int line, const char* func, const char* message);
	
	private:
		inline void Log(LogLevel level, Logger* logger, const char* file,
				unsigned int line, const char* func, const char* message);

		::std::string _name;

		Logger* _parent;

		LogLevel _level;

		::std::ostream* _stream;

		::std::string _format;
	};

	class Tracer {
	public:
		inline Tracer(Logger& logger, const char* file, unsigned int line,
				const char* func, const char* name);

		inline ~Tracer();

	private:
		Logger& _logger;

		const char* _file;

		const char* _func;

		const char* _name;
	};

} // namespace easylogger

template <typename T>
::easylogger::LogSink& operator<<(::easylogger::LogSink& sink, const T& val) {
	sink.Stream() << val;
	return sink;
}

#define EASY_LOG(logger, level, message) do{ \
		if ((logger).IsLevel((level))) { \
			do { \
				::easylogger::LogSink _easy_sink((logger).Log(level, __FILE__, __LINE__, __FUNCTION__)); \
				_easy_sink << message; \
			} while(0); \
			if ((level) == ::easylogger::LEVEL_FATAL) { \
				(logger).Flush(); \
				std::abort(); \
			} \
		} \
	}while(0)

#define LOG_TRACE(logger, message) EASY_LOG((logger), ::easylogger::LEVEL_TRACE, message)
#define LOG_DEBUG(logger, message) EASY_LOG((logger), ::easylogger::LEVEL_DEBUG, message)
#define LOG_INFO(logger, message) EASY_LOG((logger), ::easylogger::LEVEL_INFO, message)
#define LOG_WARNING(logger, message) EASY_LOG((logger), ::easylogger::LEVEL_WARNING, message)
#define LOG_ERROR(logger, message) EASY_LOG((logger), ::easylogger::LEVEL_ERROR, message)
#define LOG_FATAL(logger, message) EASY_LOG((logger), ::easylogger::LEVEL_FATAL, message)


#if !defined(NDEBUG)
# define EASY_ASSERT(logger, expr, msg) do{ \
		if (!(expr)) { \
			EASY_LOG((logger), ::easylogger::LEVEL_FATAL, "ASSERTION FAILED: " #expr ": " msg); \
		} \
	}while(0)
#else
# define EASY_ASSERT(logger, expr, msg) do{ }while(0)
#endif

#define ASSERT_NOTNULL(logger, expr, msg) EASY_ASSERT((logger), (expr) != NULL, msg)
#define ASSERT_EQ(logger, lhs, rhs, msg) EASY_ASSERT((logger), (lhs) == (rhs), msg)
#define ASSERT_NE(logger, lhs, rhs, msg) EASY_ASSERT((logger), (lhs) != (rhs), msg)
#define ASSERT_TRUE(logger, expr) EASY_ASSERT((logger), (lhs) == true, msg)
#define ASSERT_FALSE(logger, expr, msg) EASY_ASSERT((logger), (lhs) != false, msg)

#define EASY_TRACE(logger, name) ::easylogger::Tracer easy_trace_ ## name((logger), __FILE__, __LINE__, __FUNCTION__, #name)

#include "easylogger-impl.h"

#endif
