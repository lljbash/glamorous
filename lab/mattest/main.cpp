// Include the C++ shared library header
#include "libtest.h"

int run_main(int argc, char **argv)
{
  // Set up the application state for the MATLAB Runtime instance created in the application.
  if (!mclInitializeApplication(NULL,0))
    {
      std::cerr << "could not initialize the application properly"
                << std::endl;
    	  return -1;
    }

  // Load the required MATLAB code into the MATLAB Runtime.
  if( !libtestInitialize() )
    {
      std::cerr << "could not initialize the library properly"
                << std::endl;
	     return -1;
    }

  try
    {
      // Create input data
      double data[] = {1,2,3,4,5,6,7,8,9};
      mwArray in1(3, 3, mxDOUBLE_CLASS, mxREAL);
      mwArray in2(3, 3, mxDOUBLE_CLASS, mxREAL);
      in1.SetData(data, 9);
      in2.SetData(data, 9);

      // Create output array
      mwArray out;

      // Call the library function
      mattest(1, out, in1, in2);

      std::cout << "The value of added matrix is:" << std::endl;
      std::cout << out << std::endl;
    }

  // Catch the MATLAB generated mwException
  catch (const mwException& e)
    {
      std::cerr << e.what() << std::endl;
      return -2;
    }

  // Catch any other exceptions that may be thrown
  catch (...)
    {
      std::cerr << "Unexpected error thrown" << std::endl;
      return -3;
    }

  // Release the resources used by the generated MATLAB code
  libtestTerminate();

  // Release all state and resources used by the MATLAB Runtime for the application
  mclTerminateApplication();
  return 0;
}

int main()
{
  // Initialize the MATLAB Runtime
  mclmcrInitialize();

  // Create a new thread and run the MATLAB generated code in it.
  return mclRunMain((mclMainFcnType)run_main,0,NULL);
}
