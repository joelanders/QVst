if(VST_CONSOLE)
    add_definitions(-DQVST_CONSOLE)
endif()

# Parse target source code
include(parse_src)

# Build static library
# add_library(${PROJECT_NAME} SHARED ${SRC_CPP} ${SRC_C} ${SRC_H} ${SRC_H_PRIVATE} ${SRC_RES} ${QVST_PLUGIN_DLLMAIN_SRC})
# target_link_libraries(${PROJECT_NAME} ${DEPENDS})

set(ALL_SOURCES ${SRC_CPP} ${SRC_C} ${SRC_H} ${SRC_H_PRIVATE} ${SRC_RES} ${QVST_PLUGIN_DLLMAIN_SRC})
smtg_add_vst3plugin(${PROJECT_NAME} ${ALL_SOURCES})

include(SMTG_Bundle)
smtg_set_bundle(${PROJECT_NAME}
    BUNDLE_IDENTIFIER "com.steinberg.vst3.${PROJECT_NAME}"
    INFOPLIST "${CMAKE_CURRENT_LIST_DIR}/resource/Info.plist" PREPROCESS
)

# Plugin DLL interface definitions
set(CMAKE_SHARED_LINKER_FLAGS 
    "${CMAKE_SHARED_LINKER_FLAGS} -exported_symbols_list:${QVST_PLUGIN_DLL_DEF}"
)

# Link with dependencies
include(link_libs)

# Process unit tests
include(parse_tests)

# Install this shared library
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${INSTALL_BIN_PATH}
        LIBRARY DESTINATION ${INSTALL_LIB_PATH}
        COMPONENT plugins)

