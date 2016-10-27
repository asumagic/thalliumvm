#ifndef THALLIUMVM_ERROR_HPP
#define THALLIUMVM_ERROR_HPP

#include <string>

namespace thallium
{
	/**
	 * ThalliumVM exception class
	 *
	 * Thrown by tassert on fatal errors.
	 */
	class VMException : public std::exception
	{
	public:
		/**
		 * Default constructor of VMException
		 * \param s what() string
		 */
		VMException(const char* s);

		/**
		 * Returns the exception string for the VM exception.
		 * \return Exception string
		 */
		const char* what() const throw() override;

	private:
		const char* _swhat;
	};

	/**
	 * Enum defining the time of an error
	 */
	enum class TimeOfError
	{
		/**
		 * Pre-runtime
		 */
		Preload,

		/**
		 * ThalliumVM program runtime
		 */
		Runtime,

		_total
	};

	/**
	 * Enum defining the type of an error
	 */
	enum class ErrorType
	{
		/**
		 * Internal VM fatal error
		 */
		Internal,

		/**
		 * Fatal error
		 */
		Fatal,

		/**
		 * Warning
		 */
		Warning,

		_total
	};

	/**
	 * Returns a string for the given time of error
	 * \param etime Time of error
	 * \return Error time stirng
	 */
	std::string errortime_string(TimeOfError etime);

	/**
	 * Returns a string for the given error type
	 * \param etype Error type
	 * \return Error type string
	 */
	std::string errortype_string(ErrorType etype);

	/**
	 * Determines whether an error type is fatal or not.
	 * \param etype Error type to check
	 * \return Fatal
	 */
	bool is_fatal(const ErrorType etype);

	/**
	 * ThalliumVM assertion
	 * \param condition If the condition is false, the assertion will fail.
	 * \param etime Time of error (pre-run, runtime)
	 * \param etype Severity
	 * \param estring Error string
	 * \return condition
	 */
	bool tassert(const bool condition,
				  const TimeOfError etime, const ErrorType etype,
				  const std::string estring);

	/**
	 * ThalliumVM error output
	 * \param etime Time of error (pre-run, runtime)
	 * \param etype Severity
	 * \param estring Error string
	 */
	void error(const TimeOfError etime, const ErrorType etype,
				const std::string estring);
}

#endif