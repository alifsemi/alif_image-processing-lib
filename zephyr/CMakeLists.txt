if (CONFIG_AIPL)
  set(AIPL_DIR ${ZEPHYR_AIPL_MODULE_DIR})

  zephyr_interface_library_named(AIPL)
  zephyr_library()
  zephyr_include_directories(
    ${AIPL_DIR}/include
    ${AIPL_DIR}/include/default
    ${AIPL_DIR}/include/dave2d
    ${AIPL_DIR}/include/helium
    ${AIPL_DIR}/zephyr
  )

  zephyr_library_sources(
    ${AIPL_DIR}/source/aipl_color_formats.c
    ${AIPL_DIR}/source/aipl_image.c
    ${AIPL_DIR}/source/aipl_error.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_AIPL_DAVE2D_ACCELERATION
    ${AIPL_DIR}/source/aipl_dave2d.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_AIPL_BASIC
    ${AIPL_DIR}/source/aipl_crop.c
    ${AIPL_DIR}/source/dave2d/aipl_crop_dave2d.c
    ${AIPL_DIR}/source/default/aipl_crop_default.c
    ${AIPL_DIR}/source/aipl_flip.c
    ${AIPL_DIR}/source/dave2d/aipl_flip_dave2d.c
    ${AIPL_DIR}/source/default/aipl_flip_default.c
    ${AIPL_DIR}/source/helium/aipl_flip_helium.c
    ${AIPL_DIR}/source/aipl_resize.c
    ${AIPL_DIR}/source/dave2d/aipl_resize_dave2d.c
    ${AIPL_DIR}/source/default/aipl_resize_default.c
    ${AIPL_DIR}/source/helium/aipl_resize_helium.c
    ${AIPL_DIR}/source/aipl_rotate.c
    ${AIPL_DIR}/source/dave2d/aipl_rotate_dave2d.c
    ${AIPL_DIR}/source/default/aipl_rotate_default.c
    ${AIPL_DIR}/source/helium/aipl_rotate_helium.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_AIPL_COLOR_CORRECTION
    ${AIPL_DIR}/source/aipl_color_correction.c
    ${AIPL_DIR}/source/default/aipl_color_correction_default.c
    ${AIPL_DIR}/source/helium/aipl_color_correction_helium.c
    ${AIPL_DIR}/source/aipl_white_balance.c
    ${AIPL_DIR}/source/default/aipl_white_balance_default.c
    ${AIPL_DIR}/source/helium/aipl_white_balance_helium.c
    ${AIPL_DIR}/source/aipl_lut_transform.c
    ${AIPL_DIR}/source/default/aipl_lut_transform_default.c
    ${AIPL_DIR}/source/helium/aipl_lut_transform_helium.c
  )

  zephyr_library_sources_ifdef(
    CONFIG_AIPL_COLOR_CONVERSION
    ${AIPL_DIR}/source/aipl_color_conversion.c
    ${AIPL_DIR}/source/dave2d/aipl_color_conversion_dave2d.c
    ${AIPL_DIR}/source/helium/aipl_color_conversion_helium.c
    ${AIPL_DIR}/source/default/aipl_color_conversion_default.c
  )

endif()
