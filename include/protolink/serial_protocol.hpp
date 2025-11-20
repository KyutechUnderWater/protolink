// Copyright 2024 OUXT Polaris.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef PROTOLINK__SERIAL_PROTOCOL_HPP_
#define PROTOLINK__SERIAL_PROTOCOL_HPP_

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <rclcpp/rclcpp.hpp>

namespace protolink
{
namespace serial_protocol
{
/**
 * @brief Publisher with serial
 */
template <typename Proto>
class Publisher
{
public:
  explicit Publisher(
    boost::asio::io_context & io_context, const std::string & device_file, const uint32_t baud_rate,
    const rclcpp::Logger & logger = rclcpp::get_logger("protolink_serial_pub"))
  : logger(logger), serial_(io_context, device_file)
  {
    serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
  }

  const rclcpp::Logger logger;

  void send(const Proto & message)
  {
    std::string encoded_text = "";
    message.SerializeToString(&encoded_text);
    sendEncodedText(encoded_text);
  }

private:
  boost::asio::serial_port serial_;
  void sendEncodedText(const std::string & encoded_text)
  {
    boost::asio::write(serial_, boost::asio::buffer(encoded_text));
  }
};

/**
 * @brief Subscriber with serial
 */
template <typename Proto, int ReadChunkSize = 64>
class Subscriber
{
public:
  explicit Subscriber(
    boost::asio::io_context & io_context, const std::string & device_file, const uint32_t baud_rate,
    std::function<void(const Proto &)> callback,
    const rclcpp::Logger & logger = rclcpp::get_logger("protolink_serial_sub"))
  : logger(logger), serial_(io_context, device_file), callback_(callback)
  {
    serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    serial_.async_read_some(
      boost::asio::buffer(receive_data_),
      boost::bind(
        &Subscriber::handler, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  }
  const rclcpp::Logger logger;

private:
  boost::asio::serial_port serial_;
  std::function<void(Proto)> callback_;
  boost::array<char, ReceiveBufferSize> receive_data_;
  void handler(const boost::system::error_code & error, size_t bytes_transferred)
  {
    if (error != boost::system::errc::success) {
      RCLCPP_ERROR_STREAM(
        logger, "Error code : " << error.value() << "\nError Message : " << error.message());
      return;
    }
    std::string data(receive_data_.data(), bytes_transferred);
    Proto proto;
    proto.ParseFromString(data);
    callback_(proto);
  }
};
}  // namespace serial_protocol
}  // namespace protolink

#endif  // PROTOLINK__SERIAL_PROTOCOL_HPP_

