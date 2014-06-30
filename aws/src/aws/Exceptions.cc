#include <aws/Exceptions.hh>

#include <elle/printf.hh>

namespace aws
{
  AWSException::AWSException(std::string const& operation,
                             std::string const& url,
                             unsigned int attempt,
                             std::unique_ptr<elle::Exception> inner)
    : Super(elle::sprintf("AWS error: '%s' on '%s' (attempt %s): %s",
                          operation, url, attempt, inner->what()))
    , _attempt(attempt)
    , _operation(operation)
    , _url(url)
  {
    inner_exception(std::move(inner));
  };

  RequestError::RequestError(std::string const& message,
                             boost::optional<reactor::http::StatusCode> http_status,
                             boost::optional<std::string> const& error_code)
    : elle::Exception(message)
    , _http_status(http_status)
    , _error_code(error_code)
  {}

  TransientError::TransientError(
    std::string const& message,
    boost::optional<reactor::http::StatusCode> http_status,
    boost::optional<std::string> const& error_code
    )
  : RequestError(message, http_status, error_code)
  {}

  CredentialsExpired::CredentialsExpired(
    std::string const& message,
    boost::optional<reactor::http::StatusCode> http_status,
    boost::optional<std::string> const& error_code
    )
  : RequestError(message, http_status, error_code)
  {}

  CredentialsNotValid::CredentialsNotValid(
    std::string const& message,
    boost::optional<reactor::http::StatusCode> http_status,
    boost::optional<std::string> const& error_code
    )
  : RequestError(message, http_status, error_code)
  {}

  CorruptedData::CorruptedData(
    std::string const& message,
    boost::optional<reactor::http::StatusCode> http_status,
    boost::optional<std::string> const& error_code
    )
  : RequestError(message, http_status, error_code)
  {}

  FileNotFound::FileNotFound(
    std::string const& message,
    boost::optional<reactor::http::StatusCode> http_status,
    boost::optional<std::string> const& error_code
    )
  : RequestError(message, http_status, error_code)
  {}
}
