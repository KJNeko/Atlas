
file(GLOB_RECURSE TESTS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_test(AtlasTests ${TESTS} ${SOURCES} ${UI_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/../atlas/resources.qrc ${APP_ICON_RESOURCE_WINDOWS})
target_include_directories(AtlasTests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/atlas)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/catch2)
target_link_libraries(
    AtlasTests
        PRIVATE
            Qt6::Core Qt6::Widgets Qt6::Concurrent Qt6::Network Qt6::Charts Qt6::Test
            SQLite::SQLite3 fmt::fmt spdlog::spdlog Catch2::Catch2WithMain TracyClient lz4
)
set_target_properties(AtlasTests PROPERTIES COMPILE_FLAGS ${FGL_FLAGS})
if (WIN32)
    target_compile_definitions(AtlasTests PRIVATE UNICODE=1)
endif ()
