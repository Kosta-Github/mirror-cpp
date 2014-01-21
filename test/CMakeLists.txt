cmake_minimum_required(VERSION 2.8)

project(mirror-cpp CXX)
message("configure: mirror-cpp unittests")

# configure catch
include_directories("${HTTP_CPP_3RD_PARTY_DIR}/catch/include")
add_definitions(-DCATCH_CONFIG_PREFIX_ALL)

add_executable(
	mirror_unittests
	main.cpp
	mirror_unittests.cpp
)
	 
target_link_libraries(
	mirror_unittests
	mirror-cpp
)

add_test(
	NAME mirror_unittests
	COMMAND mirror_unittests
)
