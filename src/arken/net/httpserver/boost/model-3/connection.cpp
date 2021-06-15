//
// connection.cpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// https://stackoverflow.com/questions/11878091/delayed-action-using-boostdeadline-timer

#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "request_handler.hpp"
#include <arken/base>
#include <arken/net/httphandle.h>

using arken::net::HttpHandle;


namespace http {
namespace server3 {

connection::connection(boost::asio::io_service& io_service,
    request_handler& handler)
  : strand_(io_service),
    socket_(io_service),
    deadline_(io_service),
    microtime(0),
    request_handler_(handler)
{
}

boost::asio::ip::tcp::socket& connection::socket()
{
  return socket_;
}

void connection::start()
{
  socket_.async_read_some(boost::asio::buffer(buffer_),
      strand_.wrap(
        boost::bind(&connection::handle_read, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)));
}

void connection::handle_largefile(const boost::system::error_code& e)
{
  if( (os::microtime() - microtime) > 1 ) {
    std::string result = HttpHandle::sync(data_.c_str(), data_.size());
    boost::asio::async_write(socket_, boost::asio::buffer(result),
        boost::bind(&connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error));
  }
}

void connection::handle_read(const boost::system::error_code& e,
    std::size_t bytes_transferred)
{

  if (!e)
  {
    if( microtime == 0 && bytes_transferred < 4096 ) {
      std::string result = HttpHandle::sync(buffer_.data(), bytes_transferred);
      boost::asio::async_write(socket_, boost::asio::buffer(result),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    } else {
      data_.append(buffer_.data(), bytes_transferred);
      microtime = os::microtime();
      deadline_.cancel();
      deadline_.expires_from_now( boost::posix_time::seconds( 1 ));
      deadline_.async_wait( boost::bind( &connection::handle_largefile, this, boost::asio::placeholders::error));
      socket_.async_read_some(boost::asio::buffer(buffer_),
          strand_.wrap(
            boost::bind(&connection::handle_read, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)));
    }
    /*
    boost::tribool result;
    boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
        request_, buffer_.data(), buffer_.data() + bytes_transferred);

    if (result)
    {
      //request_handler_.handle_request(request_, reply_);
      std::cout << buffer_.data() << std::endl;
      std::cout << "processado requisicao bytes_transferred " << bytes_transferred << std::endl;
      std::string result = HttpHandle::sync(buffer_.data(), bytes_transferred);
      boost::asio::async_write(socket_, boost::asio::buffer(result),
          boost::bind(&connection::handle_write, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else if (!result)
    {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
    }
    else
    {
      std::cout << "aqui indefinido " << bytes_transferred << std::endl;
      socket_.async_read_some(boost::asio::buffer(buffer_),
          strand_.wrap(
            boost::bind(&connection::handle_read, shared_from_this(),
              boost::asio::placeholders::error,
              boost::asio::placeholders::bytes_transferred)));
    }

  */
  } else {
      reply_ = reply::stock_reply(reply::bad_request);
      boost::asio::async_write(socket_, reply_.to_buffers(),
          strand_.wrap(
            boost::bind(&connection::handle_write, shared_from_this(),
              boost::asio::placeholders::error)));
  }


  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.
}

void connection::handle_write(const boost::system::error_code& e)
{
  if (!e)
  {
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
  }

  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}

} // namespace server3
} // namespace http

