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

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <rclcpp/rclcpp.hpp>

namespace protolink
{
namespace serial_protocol
{
template <typename Proto>
class Publisher
{
public:
  explicit Publisher(
    boost::asio::io_service & io_service, const std::string & device_file, const uint16_t baud_rate,
    const rclcpp::Logger & logger = rclcpp::get_logger("protolink_serial"))
  : logger(logger), serial_(io_service, device_file)
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

template <typename Proto>
class Subscriber
{
public:
  explicit Subscriber(
    boost::asio::io_service & io_service, const std::string & device_file, const uint16_t baud_rate,
    const rclcpp::Logger & logger = rclcpp::get_logger("protolink_serial"))
  : logger(logger), serial_(io_service, device_file)
  {
    serial_.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    constexpr int receive_size = 8;
    boost::asio::async_read(
      serial_, boost::asio::buffer(receive_data_), boost::asio::transfer_exactly(receive_size),
      boost::bind(
        &Subscriber::handler, this, boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
  }
  const rclcpp::Logger logger;

private:
  boost::asio::serial_port serial_;
  unsigned char receive_data_[64];
  void handler(const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/)
  {
    size_t len = 64;
    std::string s(reinterpret_cast<char const *>(receive_data_), len);
    Proto proto;
    proto.SerializeToString(&s);
  }
};
}  // namespace serial_protocol
}  // namespace protolink

#endif  // PROTOLINK__SERIAL_PROTOCOL_HPP_
