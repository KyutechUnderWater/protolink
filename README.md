# protolink

## Introduction

Automatic Protocol Buffers generation tool to link ROS2 and microcontroller. The protocol buffer developed by google is used to provide ros2-independent communication.

## Features

- Generate .proto file from ROS2 message
- Generate C++ protocol buffer API from .proto file
- Generate ANSI-C protocol buffer API from .proto file using Nanopb
- Generate function to interconversion between ROS2 message and Protocol Buffers

## Requirements

- ROS2 (humble or latest)
- Python3

## Installation

Install dependencies

```bash
$ sudo apt update
$ sudo apt install protobuf-compiler libpaho-mqtt-dev libpaho-mqttpp-dev cmake ninja-build

```

Clone repository

```bash
$ git clone https://github.com/OUXT-Polaris/protolink.git
$ cd protolink
```

## How to Using

When using geographic_msgs/GeoPose

Add denendent packages to package.xml

```xml
# <depend> .....
<depend>geographic_msgs</depend>
```

Add task to CMakeLists.txt

```txt
# include(cmake/add_protolink_message.cmake)....
add_protolink_message_from_ros_message("geographic_msgs" "GeoPose")
```

Generate .proto file and API

```bash
$ cmake -S . -B build -G "Ninja"
$ cmake --build build
```

Generated API file path

- C++ API: build/...
- ANSI-C API: build/nanopb-gen/...
- proto file: build/proto_file/...
