#include "C:\Factory\Common\all.h"

int main(int argc, char **argv)
{
	autoList_t *paths;
	char *path;
	uint index;

	errorCase(!argIs("/Unsafe")); // 安全のため

	paths = lss(nextArg());
	reverseElements(paths);

	foreach(paths, path, index)
		if(getLocal(path)[0] == '_')
			recurRemovePathIfExist(path);

	// g
}
