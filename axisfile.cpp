#include "axisfile.h"

bool isFileExists(const char* filename)
{
	FILE* f = FOPEN(filename, "rb");
	if (!f) return false;
	FCLOSE(f);
	return true;
}

#if defined(_MSC_VER)

bool isFolderExists(const char* dir)
{
	struct stat sb;
	return ((stat(dir, &sb) == 0) && ((sb.st_mode & _S_IFMT) == _S_IFDIR));
}

std::vector<std::string> listFolders(const char* path)
{
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::string onlypath(path);
	std::vector<std::string> res = {};

	if (onlypath[onlypath.size() - 1] != '\\') onlypath = onlypath + "\\";
	onlypath=onlypath+"*";
	hFind = FindFirstFileA(onlypath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)	//not found
	{
		return {};
	}
	do
	{
		int lenpath = (int)onlypath.size();
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(ffd.cFileName, ".") != 0) && (strcmp(ffd.cFileName, "..") != 0))
		{
			res.push_back(std::string(ffd.cFileName));
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
	return res;
}

bool makeFolder(const char* path)
{
	std::string tmp(path);
	if (tmp[tmp.size() - 1] != '\\') tmp=tmp+"\\";
	const char* p = tmp.c_str();
	char* d = (char*)p;
	bool res = true;
	while (d != NULL)
	{
		d = strstr(d, "\\");
		if (d != NULL)
		{
			*d = 0;
			int len = (int)tmp.size();
			res&=CreateDirectoryA(p, NULL);
			*d = '\\';
			d++;
		}
	}
	return res;
}

bool deleteFolder(const char* path)
{
	HANDLE hFind;
	WIN32_FIND_DATAA ffd;
	struct stat sb;

	std::string onlypath(path);
	if (onlypath[onlypath.size() - 1] != '\\') onlypath = onlypath + "\\";
	if (!(stat((char*)onlypath.c_str(), &sb) == 0 && ((sb.st_mode & _S_IFMT) == _S_IFDIR))) return false;
	hFind = FindFirstFileA((onlypath+"*").c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) return false;
	do
	{
		int lenpath = (int)onlypath.size();
		if ((ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(ffd.cFileName, ".") != 0) && (strcmp(ffd.cFileName, "..") != 0))
		{
			if (!deleteFolder((onlypath+ffd.cFileName).c_str()))
			{
				FindClose(hFind);
				return false;
			}
		} else if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(ffd.cFileName, ".") != 0) && (strcmp(ffd.cFileName, "..") != 0))
		{
			if (!DeleteFileA((onlypath+ffd.cFileName).c_str()))
			{
				FindClose(hFind);
				return false;
			}
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
	return (RemoveDirectoryA(onlypath.c_str()) == true);
}

std::vector<std::string> listFiles(const char* path)
{
	WIN32_FIND_DATAA ffd;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::string onlypath(path);
	std::vector<std::string> res = {};

	if (onlypath[onlypath.size() - 1] != '\\') onlypath = onlypath + "\\";
	onlypath = onlypath + "*";
	hFind = FindFirstFileA(onlypath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFind)	//not found
	{
		return {};
	}
	do
	{
		int lenpath = (int)onlypath.size();
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strcmp(ffd.cFileName, ".") != 0) && (strcmp(ffd.cFileName, "..") != 0))
		{
			res.push_back(std::string(ffd.cFileName));		
		}
	} while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
	return res;
}

bool deleteFile(const char* file)
{
	return (remove(file)==0);
}

bool copyFile(const char* filename, const char* newfilename)
{
	return CopyFileA(filename, newfilename, false);
}

bool moveFile(const char* filename, const char* newfilename)
{
	return MoveFileA(filename, newfilename);
}

TIME getLastModifiedTime(const char* filename)
{
	FILETIME creationtime;
	FILETIME lastaceesstime;
	FILETIME lastwritetime;
	WORD FatDate = 0;
	WORD FatTime = 0;

	HANDLE h = CreateFileA(filename,
		FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	BOOL res = GetFileTime(h, &creationtime, &lastaceesstime, &lastwritetime);
	FileTimeToLocalFileTime(&lastwritetime, &lastwritetime);
	FileTimeToDosDateTime(&lastwritetime, &FatDate, &FatTime);
	CloseHandle(h);
	struct tm time;
	time.tm_mday = FatDate & (1 + 2 + 4 + 8 + 16);
	time.tm_mon = ((FatDate & (32 + 64 + 128 + 256)) >> 5)-1;
	time.tm_year = ((FatDate & (512 + 1024 + 2048 + 4096 + 8192 + 16384 + 32768)) >> 9) +80;
	time.tm_hour = ((FatTime & (2048 + 4096 + 8192 + 16384 + 32768)) >> 11);
	time.tm_min = (FatTime & (32 + 64 + 128 + 256 + 512 + 1024)) >> 5;
	time.tm_sec = FatTime & (1 + 2 + 4 + 8 + 16);
	return mktime(&time);
}

bool setLastModifiedTime(const char* filename, TIME t)
{
	bool res = false;
	SYSTEMTIME thesystemtime;
	memset(&thesystemtime, 0, sizeof(SYSTEMTIME));
	tm* tmp = gmtime(&t);			//need to be UTC
	thesystemtime.wDay = tmp->tm_mday;
	thesystemtime.wMonth = tmp->tm_mon + 1;
	thesystemtime.wYear = tmp->tm_year + 1900;
	thesystemtime.wHour = tmp->tm_hour;
	thesystemtime.wMinute = tmp->tm_min;
	thesystemtime.wSecond = tmp->tm_sec;
	thesystemtime.wMilliseconds = 0;

	FILETIME thefiletime;
	SystemTimeToFileTime(&thesystemtime, &thefiletime);
	HANDLE h = CreateFileA(filename,
		FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	res = SetFileTime(h, (LPFILETIME)NULL, (LPFILETIME)NULL, &thefiletime);
	CloseHandle(h);
	return res;
}

#else 

bool isFolderExists(const char* path)
{
	DIR* dp;
	if ((dp = opendir(path)) == NULL) {
		return false;
	}
	closedir(dp);
	return true;
}

std::vector<std::string> listFolders(const char* path)
{
	char cwd[1024];
	struct dirent* dbuf;
	DIR* dp;
	struct stat stbuf;
	std::string onlypath(path);
	std::vector<std::string> res = {};

	if (onlypath[onlypath.size() - 1] != '/') onlypath = onlypath + "/";
	dp = opendir(onlypath.c_str());						//may access denied
	if (dp == NULL) { return {}; }
	if (getcwd(cwd, sizeof(cwd)) == NULL) return {};
	if (chdir(onlypath.c_str()) < 0) { return {}; }
	while ((dbuf = readdir(dp))) {
		if (!strcmp(dbuf->d_name, ".") || !strcmp(dbuf->d_name, "..")) continue;
		if (stat(dbuf->d_name, &stbuf) < 0) { continue; }
		switch (stbuf.st_mode & S_IFMT) {
		case S_IFDIR:
		{
			res.push_back(dbuf->d_name);
		}
		break;
		}
	}
	closedir(dp);
	(void)chdir(cwd);
	return res;
}

bool makeFolder(const char* path)
{
	std::string tmp(path);
	if (tmp[tmp.size() - 1] != '/') tmp = tmp + "/";
	const char* p = tmp.c_str();
	char* d = (char*)p;
	bool res = true;
	while (d != NULL)
	{
		d = strstr(d, "/");
		if (d != NULL)
		{
			*d = 0;
			res&=mkdir(tmp.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			*d = '/';
			d++;
		}
	}
	return res;
}

bool deleteFolder(const char* path)
{
	char cwd[1024];
	struct dirent* dbuf;
	DIR* dp;
	struct stat stbuf;
	std::string onlypath(path);

	if (onlypath[onlypath.size() - 1] != '/') onlypath = onlypath + "/";
	dp = opendir(onlypath.c_str());						//may access denied
	if (dp == NULL) { return false; }
	if (getcwd(cwd, sizeof(cwd)) == NULL) return false;
	if (chdir(onlypath.c_str()) < 0) { return false; }
	while ((dbuf = readdir(dp))) {
		if (!strcmp(dbuf->d_name, ".") || !strcmp(dbuf->d_name, "..")) continue;
		if (stat(dbuf->d_name, &stbuf) < 0) { continue; }
		if (S_ISDIR(stbuf.st_mode)) {
			if (!deleteFolder((onlypath + dbuf->d_name).c_str())) {
				closedir(dp);
				(void)chdir(cwd);
				return false;
			}
		} else
		if (S_ISREG(stbuf.st_mode)) {
			if (remove((onlypath + dbuf->d_name).c_str()) != 0) {
				closedir(dp);
				(void)chdir(cwd);
				return false;
			}
		} 
	}
	closedir(dp);
	int res=rmdir(onlypath.c_str());
	(void)chdir(cwd);
	return (res==0);
}

std::vector<std::string> listFiles(const char* path)
{
	char cwd[1024];
	struct dirent* dbuf;
	DIR* dp;
	struct stat stbuf;
	std::string onlypath(path);
	std::vector<std::string> res = {};

	if (onlypath[onlypath.size() - 1] != '/') onlypath = onlypath + "/";
	dp = opendir(onlypath.c_str());						//may access denied
	if (dp == NULL) { return {}; }
	if (getcwd(cwd, sizeof(cwd)) == NULL) return {};
	if (chdir(onlypath.c_str()) < 0) { return {}; }
	while ((dbuf = readdir(dp))) {
		if (!strcmp(dbuf->d_name, ".") || !strcmp(dbuf->d_name, "..")) continue;
		if (stat(dbuf->d_name, &stbuf) < 0) { continue; }
		if (S_ISREG(stbuf.st_mode)) {
			res.push_back(dbuf->d_name);
		}
	}
	closedir(dp);
	(void)chdir(cwd);
	return res;
}

bool copyFile(const char* filename, const char* newfilename)
{	//too slow
	FILE* from;
	FILE* to;
	int ch;

	if ((from = fopen(filename, "rb+")) == NULL) return false;

	if ((to = fopen(newfilename, "wb")) == NULL) {
		fclose(from);
		return false;
	}

	while (!feof(from)) {
		ch = fgetc(from);
		//if(ferror(from)) {
		// }
		if (!feof(from)) fputc(ch, to);
	}
	fclose(from);
	fclose(to);
	return true;
}

bool deleteFile(const char* file)
{
	return (remove(file) == 0);
}

bool moveFile(const char* filename, const char* newfilename)
{
	return (rename(filename, newfilename) == 0);
}

TIME getLastModifiedTime(const char* filename)
{
	struct stat attr;
	stat(filename, &attr);
	return attr.st_mtime;
}

bool setLastModifiedTime(const char* filename, TIME t)
{
	struct stat attr;
	struct utimbuf new_times;

	stat(filename, &attr);
	new_times.actime = attr.st_atime; 
	new_times.modtime = t;    
	return (utime(filename, &new_times)==0);
}

#endif

