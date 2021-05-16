#include "C:\Factory\Common\all.h"

int main(int argc, char **argv)
{
	autoList_t *dirs;
	char *dir;
	uint index;

	LOGPOS();
	errorCase(!argIs("/Unsafe")); // 安全のため
	LOGPOS();
	dirs = lssDirs(nextArg());

	foreach(dirs, dir, index)
		if(getLocal(dir)[0] == '_') // ? "_orig" フォルダ etc.
			recurRemoveDirIfExist(dir);

	releaseDim(dirs, 1);
	LOGPOS();
}
