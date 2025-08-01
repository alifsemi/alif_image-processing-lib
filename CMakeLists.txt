cmake_minimum_required(VERSION 3.28)

project(aipl VERSION 1.2.0 LANGUAGES C)

# Add library sources
add_library(aipl STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_color_formats.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_image.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_error.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_dave2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_crop.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/dave2d/aipl_crop_dave2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_crop_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_flip.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/dave2d/aipl_flip_dave2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_flip_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_flip_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_resize.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/dave2d/aipl_resize_dave2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_resize_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_resize_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_rotate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/dave2d/aipl_rotate_dave2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_rotate_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_rotate_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_color_correction.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_color_correction_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_color_correction_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_white_balance.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_white_balance_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_white_balance_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_lut_transform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_lut_transform_default.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_lut_transform_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/aipl_color_conversion.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/dave2d/aipl_color_conversion_dave2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/helium/aipl_color_conversion_helium.c
    ${CMAKE_CURRENT_SOURCE_DIR}/source/default/aipl_color_conversion_default.c
)

# Directly use config template if no config was provided
if (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/config/aipl_config.h)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/aipl_config_template.h DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/config)
    file(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/config/aipl_config_template.h ${CMAKE_CURRENT_SOURCE_DIR}/config/aipl_config.h)
endif()

# Add include directories
target_include_directories(aipl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/default
    ${CMAKE_CURRENT_SOURCE_DIR}/include/dave2d
    ${CMAKE_CURRENT_SOURCE_DIR}/include/helium
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/external/include
    ${CMAKE_CURRENT_SOURCE_DIR}/config
)

# Set the compiler flags
target_compile_options(aipl PUBLIC
    "-mcpu=${TARGET_CPU}"
    "-mfloat-abi=hard"
    "-mlittle-endian"
    "-std=c99"
    "-fdata-sections"
    "-fshort-enums"
    "-Wall"
)
