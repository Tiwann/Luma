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

function(add_shader_target TARGET)
    cmake_parse_arguments(
            ARG
            ""
            "PROFILE"
            "SOURCES;OUTPUT_TYPE;VERTEX;FRAG;COMPUTE;HULL"
            ${ARGN}
    )

    if(NOT ARG_SOURCES)
        message(FATAL_ERROR
                "add_shader_target(${TARGET}): SOURCES is required"
        )
    endif()

    if(NOT ARG_OUTPUT_TYPE)
        message(FATAL_ERROR
                "add_shader_target(${TARGET}): OUTPUT_TYPE is required. Can be either spirv, dxil or glsl"
        )
    endif ()

    foreach(_type IN LISTS ARG_OUTPUT_TYPE)
        if(NOT "${_type}" MATCHES "^(spirv|dxil|glsl)$")
            message(FATAL_ERROR
                    "add_shader_target(${TARGET}): Unknown OUTPUT_TYPE '${_type}'. Can be spirv, dxil, or glsl."
            )
        endif ()
    endforeach ()

    find_program(SLANGC_EXECUTABLE slangc REQUIRED)

    set(SHADER_OUTPUTS)

    foreach(SHADER IN LISTS ARG_SOURCES)

        if(NOT IS_ABSOLUTE "${SHADER}")
            set(SHADER "${CMAKE_CURRENT_SOURCE_DIR}/${SHADER}")
        endif()

        get_filename_component(SHADER_NAME "${SHADER}" NAME)

        foreach(OUT_TYPE IN LISTS ARG_OUTPUT_TYPE)
            if("${OUT_TYPE}" STREQUAL "spirv")
                set(OUTPUT_EXT "spv")
            elseif("${OUT_TYPE}" STREQUAL "dxil")
                set(OUTPUT_EXT "dxil")
            else()
                set(OUTPUT_EXT "${OUT_TYPE}")
            endif ()

            if(ARG_PROFILE)
                set(PROFILE_FLAG -profile "${ARG_PROFILE}")
            elseif("${OUT_TYPE}" STREQUAL "dxil")
                set(PROFILE_FLAG -profile sm_6_0)
            else()
                set(PROFILE_FLAG "")
            endif ()

            # Vertex shader
            if(ARG_VERTEX)
                set(OUTPUT "${SHADER}.${ARG_VERTEX}.${OUTPUT_EXT}")

                add_custom_command(
                        OUTPUT "${OUTPUT}"
                        COMMAND "${SLANGC_EXECUTABLE}"
                        "${SHADER}"
                        -entry "${ARG_VERTEX}"
                        -stage vertex
                        -target "${OUT_TYPE}"
                        ${PROFILE_FLAG}
                        -o "${OUTPUT}"
                        DEPENDS "${SHADER}"
                        COMMENT "Compiling vertex shader ${SHADER_NAME} (${OUT_TYPE})"
                        VERBATIM
                )

                list(APPEND SHADER_OUTPUTS "${OUTPUT}")
            endif()

            # Fragment shader
            if(ARG_FRAG)
                set(OUTPUT "${SHADER}.${ARG_FRAG}.${OUTPUT_EXT}")

                add_custom_command(
                        OUTPUT "${OUTPUT}"
                        COMMAND "${SLANGC_EXECUTABLE}"
                        "${SHADER}"
                        -entry "${ARG_FRAG}"
                        -stage fragment
                        -target "${OUT_TYPE}"
                        ${PROFILE_FLAG}
                        -o "${OUTPUT}"
                        DEPENDS "${SHADER}"
                        COMMENT "Compiling fragment shader ${SHADER_NAME} (${OUT_TYPE})"
                        VERBATIM
                )

                list(APPEND SHADER_OUTPUTS "${OUTPUT}")
            endif()

            # Compute shader
            if(ARG_COMPUTE)
                set(OUTPUT "${SHADER}.${ARG_COMPUTE}.${OUTPUT_EXT}")

                add_custom_command(
                        OUTPUT "${OUTPUT}"
                        COMMAND "${SLANGC_EXECUTABLE}"
                        "${SHADER}"
                        -entry "${ARG_COMPUTE}"
                        -stage compute
                        -target "${OUT_TYPE}"
                        ${PROFILE_FLAG}
                        -o "${OUTPUT}"
                        DEPENDS "${SHADER}"
                        COMMENT "Compiling compute shader ${SHADER_NAME} (${OUT_TYPE})"
                        VERBATIM
                )

                list(APPEND SHADER_OUTPUTS "${OUTPUT}")
            endif()

            # Hull shader
            if(ARG_HULL)
                set(OUTPUT "${SHADER}.${ARG_HULL}.${OUTPUT_EXT}")

                add_custom_command(
                        OUTPUT "${OUTPUT}"
                        COMMAND "${SLANGC_EXECUTABLE}"
                        "${SHADER}"
                        -entry "${ARG_HULL}"
                        -stage hull
                        -target "${OUT_TYPE}"
                        ${PROFILE_FLAG}
                        -o "${OUTPUT}"
                        DEPENDS "${SHADER}"
                        COMMENT "Compiling hull shader ${SHADER_NAME} (${OUT_TYPE})"
                        VERBATIM
                )

                list(APPEND SHADER_OUTPUTS "${OUTPUT}")
            endif()
        endforeach ()
    endforeach()

    add_custom_target("${TARGET}"
            DEPENDS ${SHADER_OUTPUTS}
    )
endfunction()