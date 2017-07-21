//
// MATLAB Compiler: 6.4 (R2017a)
// Date: Fri Jul 21 18:40:53 2017
// Arguments:
// "-B""macro_default""-W""cpplib:libinkdisp""-T""link:lib""inkdisp3.m""gptoolbo
// x/imageprocessing/lischinski.m""-d""libdisp"
//

#include <stdio.h>
#define EXPORTING_libinkdisp 1
#include "libinkdisp.h"

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
#ifndef LIB_libinkdisp_C_API
#define LIB_libinkdisp_C_API /* No special import/export declaration */
#endif

LIB_libinkdisp_C_API 
bool MW_CALL_CONV libinkdispInitializeWithHandlers(
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
            mclGetEmbeddedCtfStream((void *)(libinkdispInitializeWithHandlers));
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

LIB_libinkdisp_C_API 
bool MW_CALL_CONV libinkdispInitialize(void)
{
  return libinkdispInitializeWithHandlers(mclDefaultErrorHandler, mclDefaultPrintHandler);
}

LIB_libinkdisp_C_API 
void MW_CALL_CONV libinkdispTerminate(void)
{
  if (_mcr_inst != NULL)
    mclTerminateInstance(&_mcr_inst);
}

LIB_libinkdisp_C_API 
void MW_CALL_CONV libinkdispPrintStackTrace(void) 
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


LIB_libinkdisp_C_API 
bool MW_CALL_CONV mlxInkdisp3(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "inkdisp3", nlhs, plhs, nrhs, prhs);
}

LIB_libinkdisp_C_API 
bool MW_CALL_CONV mlxLischinski(int nlhs, mxArray *plhs[], int nrhs, mxArray *prhs[])
{
  return mclFeval(_mcr_inst, "lischinski", nlhs, plhs, nrhs, prhs);
}

LIB_libinkdisp_CPP_API 
void MW_CALL_CONV inkdisp3(int nargout, mwArray& img2, const mwArray& img, const mwArray& 
                           lambda, const mwArray& alpha, const mwArray& canny)
{
  mclcppMlfFeval(_mcr_inst, "inkdisp3", nargout, 1, 4, &img2, &img, &lambda, &alpha, &canny);
}

LIB_libinkdisp_CPP_API 
void MW_CALL_CONV lischinski(int nargout, mwArray& A, const mwArray& L, const mwArray& 
                             varargin)
{
  mclcppMlfFeval(_mcr_inst, "lischinski", nargout, 1, -2, &A, &L, &varargin);
}

