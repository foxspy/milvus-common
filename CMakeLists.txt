# Copyright (C) 2019-2020 Zilliz. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under the License
# is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
# or implied. See the License for the specific language governing permissions and limitations under the License

cmake_minimum_required( VERSION 3.18 )
project(milvus_common CXX C)

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED on )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

set(MILVUS_COMMON_WORKSPACE ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${MILVUS_COMMON_WORKSPACE}/include)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR}/)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

find_package(OpenMP REQUIRED)

find_package(folly REQUIRED)
include_directories(${folly_INCLUDE_DIRS})

find_package(nlohmann_json REQUIRED)
find_package(glog REQUIRED)
find_package(prometheus-cpp REQUIRED)

if(NOT WITH_CUVS)
  find_package(fmt REQUIRED)
endif()
if(NOT WITH_LIGHT)
  find_package(opentelemetry-cpp REQUIRED)
endif()

list(APPEND COMMON_LINKER_LIBS Boost::boost)
list(APPEND COMMON_LINKER_LIBS glog::glog)
list(APPEND COMMON_LINKER_LIBS nlohmann_json::nlohmann_json)
list(APPEND COMMON_LINKER_LIBS prometheus-cpp::core prometheus-cpp::push)
list(APPEND COMMON_LINKER_LIBS fmt::fmt-header-only)
list(APPEND COMMON_LINKER_LIBS Folly::folly)
list(APPEND COMMON_LINKER_LIBS atomic)
if(NOT WITH_LIGHT)
  list(APPEND COMMON_LINKER_LIBS opentelemetry-cpp::opentelemetry_trace)
  list(APPEND COMMON_LINKER_LIBS
       opentelemetry-cpp::opentelemetry_exporter_ostream_span)
  list(APPEND COMMON_LINKER_LIBS
       opentelemetry-cpp::opentelemetry_exporter_jaeger_trace)
  list(APPEND COMMON_LINKER_LIBS
       opentelemetry-cpp::opentelemetry_exporter_otlp_grpc)
  list(APPEND COMMON_LINKER_LIBS
       opentelemetry-cpp::opentelemetry_exporter_otlp_http)
endif()

file(GLOB_RECURSE SRC_FILES src/*.cpp src/*.cc)
add_library(milvus-common SHARED ${SRC_FILES})

list(APPEND milvus-common_LINKER_LIBS ${COMMON_LINKER_LIBS})

target_link_libraries(milvus-common PUBLIC ${COMMON_LINKER_LIBS})

install(TARGETS milvus-common
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY "${MILVUS_COMMON_WORKSPACE}/include/"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/include")
