function(luma_generate_export_header)
    cmake_parse_arguments(ARG "" "TARGET;MODULE_NAME" "" ${ARGN})

    if(NOT ARG_TARGET OR NOT ARG_MODULE_NAME)
        message(FATAL_ERROR "luma_generate_export_header: TARGET and MODULE_NAME are required.")
    endif()

    string(TOUPPER ${ARG_MODULE_NAME} ARG_MODULE_NAME)
    string(SUBSTRING ${ARG_MODULE_NAME} 0 1 _first)
    string(SUBSTRING ${ARG_MODULE_NAME} 1 -1 _rest)
    string(TOLOWER   ${_rest} _rest)
    set(FOLDER_NAME "${_first}${_rest}")

    set(OUTPUT_DIR  "${CMAKE_CURRENT_BINARY_DIR}/Include/Luma/${FOLDER_NAME}")
    set(OUTPUT_FILE "${OUTPUT_DIR}/Export.h")

    set(HEADER_TEMPLATE "${CMAKE_SOURCE_DIR}/CMake/ModuleExport.h.in")

    set(MODULE_NAME ${ARG_MODULE_NAME})
    configure_file("${HEADER_TEMPLATE}" "${OUTPUT_FILE}" @ONLY)

    target_include_directories(${ARG_TARGET} PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Include)

    message(STATUS "[Luma] Generated export header for module ${MODULE_NAME} → ${OUTPUT_FILE}")
endfunction()