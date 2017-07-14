#include "wSegment.h"
#include "ColorAttribute.h"
#include "ClassifyColor.h"
#include <cstdlib>
#include <sstream>
#include <string.h>

using std::string;


int main(int argc, char* argv[])
{
	string srcpic = argv[1];
	string desfile = argv[2];
	
	ColorAttribute ca(srcpic);
	ca.getColorPattern(srcpic, desfile);
	return 0;
}

