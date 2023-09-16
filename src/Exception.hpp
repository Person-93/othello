#pragma once

#include "Othello.hpp"
#include <boost/exception/get_error_info.hpp>
#include <boost/exception/info.hpp>
#include <boost/throw_exception.hpp>
#include <log4cplus/loggingmacros.h>
#include <stdexcept>
#include <string>
#include <string_view>

namespace exception {
using Because = boost::error_info<struct cause, std::string>;
using WrappedExceptionType =
    boost::error_info<struct wrapped_exception_type, std::string>;
using WrappedExceptionMessage =
    boost::error_info<struct wrapped_exception_message, std::string>;
using Move = boost::error_info<struct move, std::pair<int, int>>;
using Board = boost::error_info<struct board_state, Othello::BoardState>;

class Exception : public virtual std::exception,
                  public virtual boost::exception {
public:
  const char *what() const noexcept override {
    const std::string *const info = boost::get_error_info<Because>(*this);
    return info ? info->c_str() : std::exception::what();
  }
};
} // namespace exception

#define THROW_EXCEPTION(exception)                                             \
  do {                                                                         \
    auto ex_ = exception;                                                      \
    LOG4CPLUS_ERROR(GetLogger(), "Exception thrown: " << ex_.what());          \
    BOOST_THROW_EXCEPTION(ex_);                                                \
  } while (false)

#define THROW_SIMPLE_EXCEPTION(message)                                        \
  THROW_EXCEPTION(exception::Exception{} << exception::Because((message)))
