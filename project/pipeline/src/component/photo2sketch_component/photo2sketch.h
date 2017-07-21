#ifndef __PYX_HAVE__photo2sketch_component__photo2sketch
#define __PYX_HAVE__photo2sketch_component__photo2sketch


#ifndef __PYX_HAVE_API__photo2sketch_component__photo2sketch

#ifndef __PYX_EXTERN_C
  #ifdef __cplusplus
    #define __PYX_EXTERN_C extern "C"
  #else
    #define __PYX_EXTERN_C extern
  #endif
#endif

__PYX_EXTERN_C DL_IMPORT(void) photo2sketch(char const *, char const *);

#endif /* !__PYX_HAVE_API__photo2sketch_component__photo2sketch */

#if PY_MAJOR_VERSION < 3
PyMODINIT_FUNC initphoto2sketch(void);
#else
PyMODINIT_FUNC PyInit_photo2sketch(void);
#endif

#endif /* !__PYX_HAVE__photo2sketch_component__photo2sketch */
