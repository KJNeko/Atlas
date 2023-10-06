


file(GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/atlas-updater/*.cpp)

add_executable(AtlasUpdater ${SRC_FILES})
target_include_directories(AtlasUpdater PUBLIC ${CMAKE_SOURCE_DIR}/atlas-updater)