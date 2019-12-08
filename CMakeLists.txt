cmake_minimum_required(VERSION 3.11)

project(gen_proj)

add_executable(gen_proj main.cpp files.hpp)

set(Boost_USE_STATIC_LIBS ON)
add_subdirectory(convenience)
target_link_libraries(gen_proj PRIVATE convenience)

install(
	TARGETS gen_proj
	DESTINATION bin
)

if (WIN32)
    set(CPACK_GENERATOR WIX)
    set(CPACK_WIX_UPGRADE_GUID 13e4fd96-92fe-46f2-a32b-187268225ed6)
    set(CPACK_WIX_PATCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/wix_patch.xml")
endif()

include(CPack)
