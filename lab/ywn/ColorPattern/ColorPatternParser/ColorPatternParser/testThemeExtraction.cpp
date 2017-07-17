/*
	Comparison of color theme extraction for paper
*/
#include "wSegment.h"
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>


void testCompareThemeExtraction()
{
	string filename = "data/themeExtract/200807041435588062.jpg";
}

// enum images in a filefold
bool string_end_with(const char* str, const char* part)
{
	size_t l1 = strlen(str);
	size_t l2 = strlen(part);
	if(l2 == 0)
		return true;
	if(l1 < l2)
		return false;
	return strcmp(str+l1-l2, part) == 0;
}

bool string_end_with(const wchar_t* str, const wchar_t* part)
{
	size_t l1 = wcslen(str);
	size_t l2 = wcslen(part);
	if(l2 == 0)
		return true;
	if(l1 < l2)
		return false;
	return wcscmp(str+l1-l2, part) == 0;
}

bool string_end_with(const string& str, const string& part)
{
	return string_end_with(str.c_str(),part.c_str());
}

vector<string> EnumImageFiles(const char* dirpath)
{
	namespace fs = boost::filesystem;
	fs::directory_iterator end_itr; // default construction yields past-the-end
	vector<string> names;
	for(fs::directory_iterator itr( dirpath ); itr != end_itr; ++itr)
	{
		if (is_directory(itr->status())) continue;
		string fname = itr->path().string();
		if(string_end_with(fname,".jpg")||string_end_with(fname,".png")||string_end_with(fname,".bmp"))
			names.push_back(fname);		
	}
	return names;
}

// extract theme for each image in a filefold
void batchThemeExtraction()
{
	char * RefDirname = "data/batchThemeExtract";
	vector<string> imgnames = EnumImageFiles(RefDirname);
	double matchCost, affCost;
	for (string s: imgnames)	
		cout << s << endl;
}