cmake_minimum_required(VERSION 3.6)
project(nlutooling)

set(CMAKE_CXX_STANDARD 14)
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

if (EXISTS '/usr/local/Cellar/nlohmann_json/2.1.0/include')
  include_directories('/usr/local/Cellar/nlohmann_json/2.1.0/include')
endif()

set(SOURCE_FILES main.cpp nlu_client.cpp nlu_client.h)
find_package(Boost 1.63.0 COMPONENTS system filesystem program_options REQUIRED)
find_package(CURL)

IF(CURL_FOUND)
    INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
ENDIF(CURL_FOUND)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

add_executable(nlutooling ${SOURCE_FILES})

if(Boost_FOUND)

    target_link_libraries(nlutooling ${Boost_LIBRARIES})

endif()

if(CURL_FOUND)
    target_link_libraries(nlutooling ${CURL_LIBRARIES})
endif()
