#include <iostream>
#include <map>
#include <vector>
#include "error.hpp"

namespace thallium
{
	VMException::VMException(const char *s) : _swhat(s) {}

	const char* VMException::what() const noexcept
	{
		return _swhat;
	}

#ifdef _WIN32
	const std::array<std::string, 2> errortime_match =
	{
		{"prerun",
		 "runtime"}
	};

	const std::array<std::string, 3> errortype_match =
	{
		{"internal",
		 "error",
		 "warning",
		 "note"}
	};
#else
	const std::array<std::string, 2> errortime_match =
	{
		{"\x1B[90m[prerun] \x1B[39m",
		 "\x1B[90m[runtime] \x1B[39m"}
	};

	const std::array<std::string, 4> errortype_match =
	{
		{"\x1B[91minternal:\x1B[39m",
		 "\x1B[91merror:\x1B[39m",
		 "\x1B[93mwarning:\x1B[39m",
		 "\x1B[94mnote:\x1B[39m"}
	};
#endif

	// Make sure there are as much array entries as enum entries
	static_assert(errortime_match.size() == static_cast<size_t>(TimeOfError::_total), "TimeOfError enum / string array size mismatch");
	static_assert(errortype_match.size() == static_cast<size_t>(ErrorType::_total), "ErrorType enum / string array size mismatch");

	std::string errortime_string(TimeOfError etime)
	{
		return errortime_match[static_cast<size_t>(etime)];
	}

	std::string errortype_string(ErrorType etype)
	{
		return errortype_match[static_cast<size_t>(etype)];
	}

	bool is_fatal(const ErrorType etype)
	{
		return etype == ErrorType::Internal || etype == ErrorType::Fatal;
	}

	bool tassert(const bool condition, const TimeOfError etime, const ErrorType etype, const std::string estring, const bool ignore_fatal)
	{
		if (!condition && !ignore_fatal)
		{
			error(etime, etype, estring.c_str());
		}

		return condition;
	}

	void error(const TimeOfError etime, const ErrorType etype, const std::string estring, const bool ignore_fatal)
	{
		const std::string etype_str = errortype_string(etype);
		std::cout << errortime_string(etime) << errortype_string(etype) << ' ' << estring << std::endl;
		if (is_fatal(etype) && !ignore_fatal)
		{
			throw VMException(estring.c_str());
		}
	}
}
