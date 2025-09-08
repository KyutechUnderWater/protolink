# Install Protocol Buffer

## Build and Install abseil-cpp

Clone abseil-cpp from GitHub repository.

```bash
git clone https://github.com/abseil/abseil-cpp.git && cd abseil-cpp
```

Building and Installing to /usr/local

```bash
cmake -S . -B build  -DCMAKE_INSTALL_PREFIX=/usr/local -DABSL_ENABLE_INSTALL=ON  -DCMAKE_POSITION_INDEPENDENT_CODE=ON
cmake --build build --target install --parallel 20
```

## Build and Install Protobuf

Clone protobuf from GitHub repository

```bash
git clone -b v31.1 https://github.com/protocolbuffers/protobuf.git && cd protobuf
git submodule update --init --recursive
```

Building and Installing to /usr/local

```bash
cmake -S . -B build   -DCMAKE_INSTALL_PREFIX=/usr/local   -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_POSITION_INDEPENDENT_CODE=ON
cmake --build build --parallel 20
cmake --install build
```
