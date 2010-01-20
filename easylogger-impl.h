namespace easylogger {

	bool Logger::IsLevel(LogLevel level) const {
		return _level <= level || (_parent != 0 && _parent->IsLevel(level));
	}

	_private::LogSink Logger::Log(LogLevel level, const char* file,
			unsigned int line, const char* func) {
		return _private::LogSink(this, level, file, line, func);
	}

	::std::ostream& Logger::Stream(::std::ostream& stream) {
		_stream = &stream;
		return *_stream;
	}

	const ::std::string& Logger::Format(const ::std::string& format) {
		return _format = format;
	}

	void Logger::WriteLog(LogLevel level, Logger* logger, const char* file,
			unsigned int line, const char* func, const char* message) {
		if (_level <= level && _stream != 0) {
			const char* cptr = _format.c_str();
			while (*cptr != 0) {
				if (*cptr == '%') {
					switch (*++cptr) {
					// % at end of stream
					case 0:
						*_stream << '%' << ::std::endl;
						return;
					// %% - literal escape
					case '%':
						*_stream << '%';
						break;
					// %F - file name
					case 'F':
						*_stream << file;
						break;
					// %C - line counter
					case 'C':
						if (line != 0) {
							*_stream << line;
						} else {
							*_stream << '?';
						}
						break;
					// %P - function name
					case 'P':
						*_stream << func;
						break;
					// %N - logger name
					case 'N':
						*_stream << logger->Name();
						break;
					// %L - log level
					case 'L':
						switch (level) {
						case LEVEL_TRACE: *_stream << "TRACE"; break;
						case LEVEL_DEBUG: *_stream << "DEBUG"; break;
						case LEVEL_INFO: *_stream << "INFO"; break;
						case LEVEL_WARNING: *_stream << "WARNING"; break;
						case LEVEL_ERROR: *_stream << "ERROR"; break;
						case LEVEL_FATAL: *_stream << "FATAL"; break;
						default: *_stream << "UNKNOWN"; break;
						}
						break;
					// %M - message
					case 'S':
						*_stream << message;
						break;
					}
				} else {
					*_stream << *cptr;
				}

				++cptr;
			}
			*_stream << ::std::endl;
		}
		if (_parent != 0) {
			_parent->WriteLog(level, logger, file, line, func, message);
		}
	}

	_private::Tracer::Tracer(Logger& logger, const char* file,
			unsigned int line, const char* func, const char* name) :
			_logger(logger), _file(file), _func(func), _name(name) {
		_private::LogSink sink(_logger.Log(LEVEL_TRACE, _file, line, _func));
		sink.Stream() << "Entering " << _name;
	}

	_private::Tracer::~Tracer() {
		_private::LogSink sink(_logger.Log(LEVEL_TRACE, _file, 0, _func));
		sink.Stream() << "Exiting " << _name;
	}

	_private::LogSink::~LogSink() {
		_logger->WriteLog(_level, _logger, _file, _line, _func,
				_os.str().c_str());
	}

} // namespace easylogger
