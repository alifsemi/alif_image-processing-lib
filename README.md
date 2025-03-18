# Alif Image Processing Library

    Basic operations:
      * Crop
      * Flip
      * Resize
      * Rotate
    Color correction:
      * Color correction using a matrix
      * White balance
      * Lookup Table channel transformation
    Supported formats:
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
    Supported bayer filters that can be decoded into RGB888 or BGR888 image:
      * RGGB
      * GRBG
      * BGGR
      * GBRG

## Requirements

This CMSIS pack requires some packs to be installed and added to the project:
* [AlifSemiconductor::Dave2DDriver@1.0.1](https://github.com/alifsemi/alif_dave2d-driver)

## How to create and install CMSIS-Pack

1. Make sure CMSIS Toolbox installed. Check `packchk` is available (add CMSIS Toolbox utils path to `PATH` if necessary).
2. Set `CMSIS_PACK_ROOT` environment variables to cmsis-packs installation directory.
3. Run `./gen_pack.sh` script
4. Install generated CMSIS pack by following command:
`cpackget add ./output/AlifSemiconductor.AIPL.1.1.0.pack`
