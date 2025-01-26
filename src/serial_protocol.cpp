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

#include <boost/bind.hpp>
#include <conversion_std_msgs__String.hpp>
#include <protolink/serial_protocol.hpp>
#include <thread>

namespace protolink
{
namespace serial_protocol
{
Subscriber::Subscriber(
  boost::asio::io_service & io_service, const std::string & device_file, const uint16_t baud_rate,
  const rclcpp::Logger & logger)
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

void Subscriber::handler(const boost::system::error_code & /*error*/, size_t /*bytes_transferred*/)
{
  size_t len = 64;
  std::string s(reinterpret_cast<char const *>(receive_data_), len);
  protolink__std_msgs__String::std_msgs__String proto;
  proto.SerializeToString(&s);
}
}  // namespace serial_protocol
}  // namespace protolink
