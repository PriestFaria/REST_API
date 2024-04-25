cmake_minimum_required(VERSION 3.27)
project(rest)

set(CMAKE_CXX_STANDARD 20)
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

FetchContent_Declare(
        pugixml
        GIT_REPOSITORY	https://github.com/zeux/pugixml
        GIT_TAG 	f1d8c51ec8e05b571d7f420ce5cb1a6708c5e0ed #refs/tags/1.0.1
)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8) # Replace with your desired git commit from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)
add_executable(rest main.cpp)
FetchContent_MakeAvailable(pugixml)
target_link_libraries(rest PRIVATE pugixml)
target_link_libraries(rest PRIVATE cpr::cpr)








