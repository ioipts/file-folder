#include "axisfile.h"

int main(int argc,char** argv)
{	
	TIME t= getLastModifiedTime("sample.cpp");
	printf("last modified time: %u\n", t);

	makeFolder("./test/test1");
	makeFolder("./test/test2");

	std::vector<std::string> dir=listFolders(".");
	printf("num of folders: %d\n",dir.size());

	std::vector<std::string> files=listFiles(".");
	printf("num of files: %d\n",files.size());
	
	deleteFolder("test");
	return 0;
}
