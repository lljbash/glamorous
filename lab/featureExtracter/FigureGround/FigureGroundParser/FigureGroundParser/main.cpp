#include "FigureGround.h"
#include <string>

using namespace std;

// need *_RCC.png in the same folder
int main(int argc, char *argv[])
{
	string srcpic = argv[1];
	FigureGround fg;
	fg.CalParam(srcpic);
	
	return 0;
}