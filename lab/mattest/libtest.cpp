//
// MATLAB Compiler: 6.4 (R2017a)
// Date: Wed Jul 12 21:32:36 2017
// Arguments: "-B""macro_default""-W""cpplib:libtest""-T""link:lib""mattest.m"
//

#include <stdio.h>
#define EXPORTING_libtest 1
#include "libtest.h"

static HMCRINSTANCE _mcr_inst = NULL;


#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultPrintHandler(const char *s)
{
  return mclWrite(1 /* stdout */, s, sizeof(char)*strlen(s));
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

#ifdef __cplusplus
extern "C" {
#endif

static int mclDefaultErrorHandler(const char *s)
{
  int written = 0;
  size_t len = 0;
  len = strlen(s);
  written = mclWrite(2 /* stderr */, s, sizeof(char)*len);
  if (len > 0 && s[ len-1 ] != '\n')
    written += mclWrite(2 /* stderr */, "\n", sizeof(char));
  return written;
}

#ifdef __cplusplus
} /* End extern "C" block */
#endif

/* This symbol is defined in shared libraries. Define it here
 * (to nothing) in case this isn't a shared library. 
 */
#ifndef LIB_libtest_C_API
#define LIB_libtest_C_API /* No special import/export declaration */
#endif

LIB_libtest_C_API 
bool MW_CALL_CONV libtestInitializeWithHandlers(
    mclOutputHandlerFcn error_handler,
    mclOutputHandlerFcn print_handler)
{
    int bResult = 0;
  if (_mcr_inst != NULL)
    return true;
  if (!mclmcrInitialize())
    return false;
    {
        mclCtfStream ctfStream = 
            mclGetEmbeddedCtfStream((void *)(libtestInitializeWithHandlers));
        if (ctfStream) {
            bResult = mclInitializeComponentInstanceEmbedded(   &_mcr_inst,
                                                                error_handler, 
                                                                print_handler,
                                                                ctfStream);
            mclDestroyStream(ctfStream);
        } else {
            bResult = 0;
        }
    }  
    if (!bResult)
    return false;
  return true;
}

LIB_libtest_C_API 
bool MW_CALL_CONV libtestInitialize(void)
{
  return libtestInitializeWithHandlers(mclDefaultErrorHandler, mclDefaultPrintHandler);
}

LIB_libtest_C_API 
void MW_CALL_CONV libtestTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_libtest_C_API 
void MW_CALL_CONV libtestPrintStackTrace(void) 
{
  char** stackTrace;
  int stackDepth = mclGetStackTrace(&stackTrace);
  int i;
  for(i=0; i<stackDepth; i++)
  {
    mclWrite(2 /* stderr */, stackTrace[i], sizeof(char)*strlen(stackTrace[i]));
    mclWrite(2 /* stderr */, "\n", sizeof(char)*strlen("\n"));
  }
  mclFreeStackTrace(&stackTrace, stackDepth);
}


LIB_libtest_C_API 
bool MW_CALL_CONV mlxMattest(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "mattest", nlhs, plhs, nrhs, prhs);
}

LIB_libtest_CPP_API 
void MW_CALL_CONV mattest(int nargout, mwArray& c, const mwArray& a, const mwArray& b)
{
  mclcppMlfFeval(_mcr_inst, "mattest", nargout, 1, 2, &c, &a, &b);
}

