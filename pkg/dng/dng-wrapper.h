#pragma once

#include <stdint.h>

// Cannot include any dng_ header files, as they're pure c++ and break - this file needs to be basic C

#ifdef __cplusplus
extern "C" {
#endif
  // Some plain-old-data C objects, to help smuggle SDK C++ return
  // values over to Golang. We prefix the names with 'C', so things are
  // less confusing over in the .go code
  typedef struct cvec3 {
    double v[3];
  } CVec3;
  typedef struct cmat3 {
    double v[9];
  } CMat3;
  typedef struct crect {
    double v[4];
  } CRect;
  typedef struct curat {
    uint32_t v[3];
  } CURat;

  // The kinds of image you can pull out of a negative. (Stage 1
  // is the raw data; Stage 2 is linearized but not-yet-demosaic'ed,
  // so ignore for now).
  enum GO_ImageKind { ImageUndefined = 0, ImageStage3, ImageFinalRender };

  // Arguments passed in from Golang, to control how we load & develop the image.
  typedef struct cnegativeargs {
    int verbose;
    int white_balance_temp;
    int image_kind;
  } CNegativeArgs;

  // Our main types for holding pointers to the C++ objects we create
  // and refer to. We malloc these, and likely leak them
  typedef struct cnegative {
    void *negative;     // (void *) to a (dng_negative *)
    void *colorspec;    // (void *) to a (dng_color_spec *)
    void *image_final;  // (void *) to a (dng_image *) which is in fact a (dng_simple_image *)
    void *pixelbuffer;  // (void *) to a (dng_pixel_buffer *), for the chosen image_kind
  } CNegative;
  

  CNegative*    GO_Make(const char *filename, CNegativeArgs args);
  void          GO_Free(CNegative *cneg);

  void          GO_GetPixelRGB(CNegative *cneg, int x, int y, uint16_t *dst);
  CRect         GO_Bounds(CNegative *cneg);
  
  CVec3         GO_CameraWhite(CNegative *cneg);
  CMat3         GO_CameraToPCS(CNegative *cneg);

  const char*   GO_OriginalRawFileName(CNegative *cneg);

  CURat         GO_ExifExposureTime(CNegative *cneg);
  CURat         GO_ExifFNumber(CNegative *cneg);
  uint32_t      GO_ExifISO(CNegative *cneg);
  
#ifdef __cplusplus
}  // extern "C"
#endif
