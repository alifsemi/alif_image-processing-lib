set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER armclang)
set(CMAKE_CXX_COMPILER armclang)

set(CMAKE_ASM_COMPILER armasm)
set(CMAKE_LINKER armlink)

set(CMAKE_C_FLAGS "--target=arm-arm-none-eabi -mthumb")
set(CMAKE_CXX_FLAGS "--target=arm-arm-none-eabi -mthumb")

set(CMAKE_AR armar)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
