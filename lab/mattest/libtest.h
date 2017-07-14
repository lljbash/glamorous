//
// MATLAB Compiler: 6.4 (R2017a)
// Date: Wed Jul 12 21:32:36 2017
// Arguments: "-B""macro_default""-W""cpplib:libtest""-T""link:lib""mattest.m"
//

#ifndef __libtest_h
#define __libtest_h 1

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

#ifdef EXPORTING_libtest
#define PUBLIC_libtest_C_API __global
#else
#define PUBLIC_libtest_C_API /* No import statement needed. */
#endif

#define LIB_libtest_C_API PUBLIC_libtest_C_API

#elif defined(_HPUX_SOURCE)

#ifdef EXPORTING_libtest
#define PUBLIC_libtest_C_API __declspec(dllexport)
#else
#define PUBLIC_libtest_C_API __declspec(dllimport)
#endif

#define LIB_libtest_C_API PUBLIC_libtest_C_API


#else

#define LIB_libtest_C_API

#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libtest_C_API 
#define LIB_libtest_C_API /* No special import/export declaration */
#endif

extern LIB_libtest_C_API 
bool MW_CALL_CONV libtestInitializeWithHandlers(
       mclOutputHandlerFcn error_handler, 
       mclOutputHandlerFcn print_handler);

extern LIB_libtest_C_API 
bool MW_CALL_CONV libtestInitialize(void);

extern LIB_libtest_C_API 
void MW_CALL_CONV libtestTerminate(void);



extern LIB_libtest_C_API 
void MW_CALL_CONV libtestPrintStackTrace(void);

extern LIB_libtest_C_API 
bool MW_CALL_CONV mlxMattest(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[]);


#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

/* On Windows, use __declspec to control the exported API */
#if defined(_MSC_VER) || defined(__BORLANDC__)

#ifdef EXPORTING_libtest
#define PUBLIC_libtest_CPP_API __declspec(dllexport)
#else
#define PUBLIC_libtest_CPP_API __declspec(dllimport)
#endif

#define LIB_libtest_CPP_API PUBLIC_libtest_CPP_API

#else

#if !defined(LIB_libtest_CPP_API)
#if defined(LIB_libtest_C_API)
#define LIB_libtest_CPP_API LIB_libtest_C_API
#else
#define LIB_libtest_CPP_API /* empty! */ 
#endif
#endif

#endif

extern LIB_libtest_CPP_API void MW_CALL_CONV mattest(int nargout, mwArray& c, const mwArray& a, const mwArray& b);

#endif
#endif
