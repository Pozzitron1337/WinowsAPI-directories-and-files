#include <Windows.h>
#include <list>
#include <string>
#include <regex>
#include <malloc.h>

#define SIZE_BUF 255

inline void CreateDirectories();
DWORD GetFileSize(LPSTR);
LPSTR FindFirstDirRecursive(LPSTR,const char*);
LPSTR FindFirstFileRecursive(LPSTR, const char*);
LPSTR Read_File(LPSTR);
bool Write_File(LPSTR, std::string);
bool ChangeFileDate(LPSTR, SYSTEMTIME, SYSTEMTIME, SYSTEMTIME);
bool ChandeFileSize(LPSTR, LONG);
std::list<std::string> FindFilesInDirectory(LPSTR dir, std::string);
std::list<std::string> FindFilesInDirectoryByRegex(LPSTR dir, std::string);
std::list<std::string> FindDirectoriesRecursive(LPSTR, std::string);
void RecursiveBypass(LPSTR);
void Copy(LPSTR, LPSTR);

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow){
	LPCSTR root = "D:/PROJECTS/C++Projects/WinAPIdirectoriesandfiles/root";
	LPCSTR bin = "D:/DOWNLOAD";
	CreateDirectory(root, NULL);
	CreateDirectories();
	RecursiveBypass((LPSTR)root);
	LPSTR file23path = FindFirstDirRecursive((LPSTR) root, "FILE23");
	LPSTR file22path = FindFirstDirRecursive((LPSTR)root, "FILE22");
	SetCurrentDirectory(file22path);
	HANDLE file23 = CreateFile("FILE23",GENERIC_WRITE,FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	SetCurrentDirectory(file23path);
	HANDLE hFileNew = CreateFile("test.txt", GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ,NULL, CREATE_NEW,FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFileNew);
	CloseHandle(file23);
	
	HANDLE test = CreateFile("test.txt", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	DWORD testsize=GetFileSize(test, NULL);
	CloseHandle(test);
	
	LPSTR testpath = FindFirstFileRecursive((LPSTR)root, "test.txt");
	
	//Copy((LPSTR)bin,file23path);

	LPSTR imagepath = FindFirstFileRecursive(file23path, "1.jpg");//"D:\PROJECTS\C++Projects\WinAPIdirectoriesandfiles\root\FILE21\FILE22\FILE23\1.jpg";
	DWORD imageSize = GetFileSize(imagepath);
	std::list<std::string> files=FindFilesInDirectoryByRegex(file23path, ".*t\\..*"); //(.+[t])(\\.)(.+)
	
	LPSTR imageread = Read_File(imagepath);
	const std::string text = "FI7316";
	Write_File((LPSTR)imagepath ,text);
	
	SYSTEMTIME st1 = { 0 };
	st1.wDay = 5;
	st1.wDayOfWeek = 3;
	st1.wHour = 4;
	st1.wMilliseconds = 9;
	st1.wMinute = 30;
	st1.wMonth = 1;
	st1.wSecond = 10;
	st1.wYear = 2000;
	SYSTEMTIME st2 = { 0 };
	st2.wDay = 5;
	st2.wDayOfWeek = 3;
	st2.wHour = 4;
	st2.wMilliseconds = 9;
	st2.wMinute = 30;
	st2.wMonth = 1;
	st2.wSecond = 10;
	st2.wYear = 2001;
	ChangeFileDate(imagepath, st1, { 0 }, st2);
	ChandeFileSize(imagepath, imageSize);
	SetFileAttributes(imagepath, FILE_ATTRIBUTE_READONLY);//~FILE_ATTRIBUTE_READONLY to check off the radio button.
	
	return 0;
}

std::list<std::string> FindFilesInDirectoryByRegex(LPSTR dir, std::string reg) {
	reg = "(" + reg + ")";
	std::list<std::string> files;
	files = FindFilesInDirectory(dir, "*");
	std::list<std::string> result;
	std::regex regular(reg);
	std::cmatch m;
	SetCurrentDirectory(dir);
	std::list<std::string> fileNames;
	WIN32_FIND_DATA FindFiles;
	HANDLE hFind = FindFirstFile("*", &FindFiles);
	if (hFind == INVALID_HANDLE_VALUE)
		return fileNames;
	do {
		if (std::regex_search(FindFiles.cFileName, m, regular))
			fileNames.push_back(FindFiles.cFileName);
	} while (FindNextFile(hFind, &FindFiles));
	return fileNames;
}
std::list<std::string> FindFilesInDirectory(LPSTR dir, std::string file) {
	SetCurrentDirectory(dir);
	LPSTR tmp = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
	GetCurrentDirectory(SIZE_BUF, tmp);
	lstrcat(tmp, "\\");
	lstrcat(tmp, file.c_str());
	std::list<std::string> fileNames;
	WIN32_FIND_DATA FindFiles;
	HANDLE hFind = FindFirstFile(file.c_str(), &FindFiles);
	if (hFind == INVALID_HANDLE_VALUE)
		return fileNames;
	do {
		fileNames.push_back(FindFiles.cFileName);
	} while (FindNextFile(hFind, &FindFiles));

	return fileNames;
}
std::list<std::string> FindDirectoriesRecursive(LPSTR root,std::string dirName){
	SetCurrentDirectory(root);
	static std::list<std::string> result;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile("*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return result;
	do {
		LPSTR strtmp = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
		GetCurrentDirectory(SIZE_BUF, strtmp);
		lstrcat(strtmp, (LPSTR)"\\");
		lstrcat(strtmp, FindFileData.cFileName);
		LPSTR strtmp_ = (LPSTR)calloc(strlen(strtmp) + 1, sizeof(WCHAR));
		lstrcat(strtmp_, strtmp);
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) {
			if (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)))
			{
				FindDirectoriesRecursive(strtmp_, dirName);
				SetCurrentDirectory(root);
				if (!strcmp(dirName.c_str(), FindFileData.cFileName)) {
					LPSTR temp = (LPSTR)calloc(strlen(strtmp) + 1, sizeof(WCHAR));
					GetCurrentDirectory(SIZE_BUF, temp);
					lstrcat(temp, "\\");
					lstrcat(temp, FindFileData.cFileName);
					result.push_back(temp);
					free(temp);
				}
			}
		}
		free(strtmp);
	} while (FindNextFile(hFind, &FindFileData));
	return result;
}
LPSTR FindFirstDirRecursive(LPSTR root, const char* search) {//returns path on first dir if it exists
	SetCurrentDirectory(root);
	static LPSTR result = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile("*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return NULL;
	do {
		LPSTR strtmp = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
		GetCurrentDirectory(SIZE_BUF, strtmp);
		lstrcat(strtmp, (LPSTR)"\\");
		lstrcat(strtmp, FindFileData.cFileName);
		LPSTR strtmp_ = (LPSTR)calloc(strlen(strtmp) + 1, sizeof(WCHAR));
		lstrcat(strtmp_, strtmp);
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) {
			if (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)))
			{
				FindFirstDirRecursive(strtmp_, search);
				SetCurrentDirectory(root); //<- не забыть!!!!
				if (!strcmp(search, FindFileData.cFileName)) {
					result = strtmp_;
					return result;
				}
			}
		}
		free(strtmp);
		free(strtmp_);
	} while (FindNextFile(hFind, &FindFileData));
	return result;
}
LPSTR FindFirstFileRecursive(LPSTR root,const char* search) {//returns path on first file if it exists
	SetCurrentDirectory(root);
	static LPSTR result = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile("*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return NULL;
	do {
		LPSTR strtmp = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
		GetCurrentDirectory(SIZE_BUF, strtmp);
		lstrcat(strtmp, (LPSTR)"\\");
		lstrcat(strtmp, FindFileData.cFileName);
		LPSTR strtmp_ = (LPSTR)calloc(strlen(strtmp) + 1, sizeof(WCHAR));
		lstrcat(strtmp_, strtmp);
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) {
			if (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)))
			{
				FindFirstFileRecursive(strtmp_, search);
				SetCurrentDirectory(root);
				
			}
			else {
				if (!strcmp(search, FindFileData.cFileName)) {
					result = strtmp_;
					return result;
				}
				
				
			}
		}
		free(strtmp);
		free(strtmp_);
	} while (FindNextFile(hFind, &FindFileData));
	return result;
}
LPSTR Read_File(LPSTR filePath) {
	HANDLE hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	OVERLAPPED olf = { 0 };
	LARGE_INTEGER li = { 0 };
	li.QuadPart = 1;
	olf.Offset = 0;// li.LowPart;
	olf.OffsetHigh = li.HighPart;
	LPSTR buffer = (CHAR*)calloc(SIZE_BUF + 1, sizeof(CHAR));
	DWORD iNumRead = 0;
	ReadFile(hFile, buffer, SIZE_BUF, &iNumRead, &olf);
	CloseHandle(hFile);
	return buffer;
}
DWORD GetFileSize(LPSTR filePath) {
	std::string path = filePath;
	std::string fileName;
	for (int i = path.length() - 1; path[i] != '\\'&&path[i] != '/'; i--) {
		fileName = path[i] + fileName;
		path = path.substr(0, path.length() - 1);
	}
	path = path.substr(0, path.length() - 1);
	SetCurrentDirectory(path.c_str());
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	DWORD hFileSize = GetFileSize(hFile, NULL);
	CloseHandle(hFile);
	return hFileSize;
}
bool Write_File(LPSTR filePath, std::string data) {
	HANDLE hFile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	OVERLAPPED olf = { 0 };
	LARGE_INTEGER li = { 0 };
	li.QuadPart = 1;
	olf.Offset = 0xFFFFFFFF;
	olf.OffsetHigh = 0xFFFFFFFF;
	LPSTR str = const_cast<char *>(data.c_str());
	lstrcat(str, "\r\n");
	DWORD iNumWrite = 0;
	if (!WriteFile(hFile, str, strlen(str), &iNumWrite, &olf)) {
		CloseHandle(hFile);
		return false;
	}
	if (olf.Internal == -1 && GetLastError()) {
		CloseHandle(hFile);
		return false;
	}
	else {
		CloseHandle(hFile);
		return true;
	}
}
bool ChangeFileDate(LPSTR dir,SYSTEMTIME stCreateTime,SYSTEMTIME stLastAccessTime,SYSTEMTIME stLastWriteTime){
	std::string path=dir;
	std::string fileName;
	for (int i = path.length() - 1; path[i] != '\\'&&path[i]!='/'; i--) {
		fileName = path[i] + fileName;
		path = path.substr(0, path.length() - 1);
	}
	path = path.substr(0,path.length()-1);
	SetCurrentDirectory(path.c_str());
	HANDLE handleFile = CreateFile(fileName.c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	FILETIME ftCreateTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	GetFileTime(handleFile, &ftCreateTime, &ftLastAccessTime, &ftLastWriteTime);
	SystemTimeToFileTime(&stCreateTime, &ftCreateTime);
	SystemTimeToFileTime(&stLastAccessTime, &ftLastAccessTime);
	SystemTimeToFileTime(&stLastWriteTime, &ftLastWriteTime);
	SetFileTime(handleFile, &ftCreateTime, &ftLastAccessTime, &ftLastWriteTime);
	CloseHandle(handleFile);
	return true;
}
bool ChandeFileSize(LPSTR filePath, LONG newSize) {
	std::string path = filePath;
	std::string fileName;
	for (int i = path.length() - 1; path[i] != '\\'&&path[i] != '/'; i--) {
		fileName = path[i] + fileName;
		path = path.substr(0, path.length() - 1);
	}
	path = path.substr(0, path.length() - 1);
	SetCurrentDirectory(path.c_str());
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;
	SetFilePointer(hFile, newSize, NULL, FILE_BEGIN);
	SetEndOfFile(hFile);
	CloseHandle(hFile);
	return true;
}
void RecursiveBypass(LPSTR root) {
	SetCurrentDirectory(root);
	static LPSTR result = NULL;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile("*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do {
		LPSTR strtmp = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
		GetCurrentDirectory(SIZE_BUF, strtmp);
		lstrcat(strtmp, (LPSTR)"\\");
		lstrcat(strtmp, FindFileData.cFileName);
		LPSTR strtmp_ = (LPSTR)calloc(strlen(strtmp) + 1, sizeof(WCHAR));
		lstrcat(strtmp_, strtmp);
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) {
			if (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)))
			{
				/*
				BUSINES LOGIC
				*/
				RecursiveBypass(strtmp_);
				SetCurrentDirectory(root);
			}
		}
		free(strtmp);
		free(strtmp_);
	} while (FindNextFile(hFind, &FindFileData));
}
void Copy(LPSTR from, LPSTR to) {
	SetCurrentDirectory(from);
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile("*.*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do {
		LPSTR tmpFrom = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
		GetCurrentDirectory(SIZE_BUF,tmpFrom);
		lstrcat(tmpFrom, (LPSTR)"\\");
		lstrcat(tmpFrom, FindFileData.cFileName);
		LPSTR tmpFrom_ = (LPSTR)calloc(strlen(tmpFrom) + 1, sizeof(WCHAR));
		lstrcat(tmpFrom_, tmpFrom);
		LPSTR tmpTo = (LPSTR)calloc(SIZE_BUF + 1, sizeof(WCHAR));
		if (strcmp(FindFileData.cFileName, ".") && strcmp(FindFileData.cFileName, "..")) {
			if (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)))
			{
				lstrcpy(tmpTo, to);
				lstrcat(tmpTo, (LPSTR)"\\");
				lstrcat(tmpTo, FindFileData.cFileName);
				CreateDirectory(tmpTo,NULL);
				Copy(tmpFrom_,tmpTo);
				SetCurrentDirectory(from);
			}
			else {
				lstrcpy(tmpTo, to);
				lstrcat(tmpTo, (LPSTR)"\\");
				lstrcat(tmpTo, FindFileData.cFileName);
				CopyFile(tmpFrom,tmpTo, FALSE);	
			}
		}
		free(tmpFrom);
		free(tmpFrom_);
		free(tmpTo);
	} while (FindNextFile(hFind, &FindFileData));
}
inline void CreateDirectories() {

	LPCSTR dir11 = "D:/PROJECTS/C++Projects/WinAPIdirectoriesandfiles/root/FILE11";
	LPCSTR dir21 = "D:/PROJECTS/C++Projects/WinAPIdirectoriesandfiles/root/FILE21";
	LPCSTR dir22 = "D:/PROJECTS/C++Projects/WinAPIdirectoriesandfiles/root/FILE21/FILE22";
	LPCSTR dir23 = "D:/PROJECTS/C++Projects/WinAPIdirectoriesandfiles/root/FILE21/FILE22/FILE23";

	CreateDirectory(dir11, NULL);
	CreateDirectory(dir21, NULL);
	CreateDirectory(dir22, NULL);
	CreateDirectory(dir23, NULL);
}