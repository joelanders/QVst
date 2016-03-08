project(adelay)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${VST_INCLUDE})

set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} /DEF:${CMAKE_CURRENT_SOURCE_DIR}/src/adelay.def"
)

file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SRC})
target_link_libraries(${PROJECT_NAME} ${VST_LIBS})
