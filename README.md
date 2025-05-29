# Alif Image Processing Library

Alif Image processing library is available as a prebuilt library and as a CMSIS pack. Visit [releases](https://github.com/alifsemi/alif_image-processing-lib/releases) page.

## Supported features

Formats:
* ALPHA8
* ARGB8888
* ARGB4444
* ARGB1555
* RGBA8888
* RGBA4444
* RGBA5551
* BGR888
* RGB565
* YV12
* I420
* I422
* I444
* I400
* NV21
* NV12
* YUY2
* UYVY

Basic operations:
* Crop
* Flip
* Resize
* Rotate

Color correction:
* Color correction using a matrix
* White balance
* Lookup Table channel transformation

Bayer filters that can be decoded into RGB888 or BGR888 image:
* RGGB
* GRBG
* BGGR
* GBRG

## CMSIS package requirements

This CMSIS pack requires some packs to be installed and added to the project:
* [AlifSemiconductor::Dave2DDriver@1.0.1](https://github.com/alifsemi/alif_dave2d-driver)

## How to create and install CMSIS package

1. Make sure CMSIS Toolbox installed. Check `packchk` is available (add CMSIS Toolbox utils path to `PATH` if necessary):
```
packchk -V
```
2. Set `CMSIS_PACK_ROOT` environment variables to cmsis-packs installation directory:
```
export CMSIS_PACK_ROOT=<your pack root>
```
3. Go to the `cmsis/` directory in this repository:
```
cd cmsis
```
4. Run the generating script:
```
./gen_pack.sh
```
4. Install generated CMSIS pack:
```
cpackget add ./output/AlifSemiconductor.AIPL.1.1.1.pack
```

## How to build the library

### ARM Compiler

ARM Compiler can be obtained from the [ARM website](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded).
Make sure that ARM Compiler's `bin/` directory is added to `PATH` before proceeding.

### Build

1. Create and set up `aipl_config.h` and place it under `config/` directory. If the file is not provided `aipl_config_template.h` is copied.
2. Set up CMake:
```
cmake -B ./build/ -S . --toolchain ./cmake/toolchains/arm-armclang.cmake -DTARGET_CPU="cortex-m55" -DCMAKE_BUILD_TYPE=Release
```
3. Build with CMake:
```
cmake --build build/
```
4. `libaipl.a` can be found inside the `build/` directory.
