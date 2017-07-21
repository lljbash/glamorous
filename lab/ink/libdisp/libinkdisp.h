//
// MATLAB Compiler: 6.4 (R2017a)
// Date: Fri Jul 21 18:40:53 2017
// Arguments:
// "-B""macro_default""-W""cpplib:libinkdisp""-T""link:lib""inkdisp3.m""gptoolbo
// x/imageprocessing/lischinski.m""-d""libdisp"
//

#ifndef __libinkdisp_h
#define __libinkdisp_h 1

#if defined(__cplusplus) && !defined(mclmcrrt_h) && defined(__linux__)
#  pragma implementation "mclmcrrt.h"
#endif
#include "mclmcrrt.h"
#include "mclcppclass.h"
#ifdef __cplusplus
extern "C" {
#endif

#if defined(__SUNPRO_CC)
/* Solaris shared libraries use __global, rather than mapfiles
 * to define the API exported from a shared library. __global is
 * only necessary when building the library -- files including
 * this header file to use the library do not need the __global
 * declaration; hence the EXPORTING_<library> logic.
 */

#ifdef EXPORTING_libinkdisp
#define PUBLIC_libinkdisp_C_API __global
#else
#define PUBLIC_libinkdisp_C_API /* No import statement needed. */
#endif

#define LIB_libinkdisp_C_API PUBLIC_libinkdisp_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libinkdisp
#define PUBLIC_libinkdisp_C_API __declspec(dllexport)
#else
#define PUBLIC_libinkdisp_C_API __declspec(dllimport)
#endif

#define LIB_libinkdisp_C_API PUBLIC_libinkdisp_C_API


#else

#define LIB_libinkdisp_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libinkdisp_C_API 
#define LIB_libinkdisp_C_API /* No special import/export declaration */
#endif

extern LIB_libinkdisp_C_API 
bool MW_CALL_CONV libinkdispInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libinkdisp_C_API 
bool MW_CALL_CONV libinkdispInitialize(void);

extern LIB_libinkdisp_C_API 
void MW_CALL_CONV libinkdispTerminate(void);



extern LIB_libinkdisp_C_API 
void MW_CALL_CONV libinkdispPrintStackTrace(void);

extern LIB_libinkdisp_C_API 
bool MW_CALL_CONV mlxInkdisp3(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);

extern LIB_libinkdisp_C_API 
bool MW_CALL_CONV mlxLischinski(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libinkdisp
#define PUBLIC_libinkdisp_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libinkdisp_CPP_API __declspec(dllimport)
#endif

#define LIB_libinkdisp_CPP_API PUBLIC_libinkdisp_CPP_API

#else

#if !defined(LIB_libinkdisp_CPP_API)
#if defined(LIB_libinkdisp_C_API)
#define LIB_libinkdisp_CPP_API LIB_libinkdisp_C_API
#else
#define LIB_libinkdisp_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libinkdisp_CPP_API void MW_CALL_CONV inkdisp3(int nargout, mwArray& img2, const mwArray& img, const mwArray& lambda, const mwArray& alpha, const mwArray& canny);

extern LIB_libinkdisp_CPP_API void MW_CALL_CONV lischinski(int nargout, mwArray& A, const mwArray& L, const mwArray& varargin);

#endif
#endif
