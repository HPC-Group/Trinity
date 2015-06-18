#ifndef FileFinder__
#define FileFinder__

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#else
#include <dirent.h>
#endif


namespace Core{

	class FileFinder{
		public:
			static FileFinder& getInstance(){
				static FileFinder instance;
				return instance;
			}

			std::string findFilePath(std::vector<std::string> searchPathList, std::string filename){
				std::string currentPath = "";
				for(std::string s : searchPathList){
					currentPath = s+filename;
#ifdef _WIN32
					//std::replace(currentPath.begin(), currentPath.end(), '/', '\\');
#endif
					std::ifstream ifs;
 					ifs.open (currentPath, std::ifstream::in);
  					if(ifs.good())
  						return currentPath;
				}
				currentPath = "null";
				return currentPath;
			}

			int readFiles(std::string dir, std::vector<std::string> &files)
            {
#ifdef _WIN32

				//std::replace(dir.begin(), dir.end(), '/', '\\');

				char search_path[200];
				sprintf(search_path, "%s*.*", dir.c_str());
				WIN32_FIND_DATA fd; 

				wchar_t* wString = new wchar_t[4096];
				MultiByteToWideChar(CP_ACP, 0, search_path, -1, wString, 4096);

				HANDLE hFind = ::FindFirstFile(wString, &fd);
				if(hFind != INVALID_HANDLE_VALUE) { 
					do { 
						// read all (real) files in current folder
						// , delete '!' read other 2 default folder . and ..
						if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {

							char ch[260];
							char DefChar = ' ';
							WideCharToMultiByte(CP_ACP, 0, fd.cFileName, -1, ch, 260, &DefChar, NULL);

							std::string s(ch);
							files.push_back(dir+s);
							
						}
					}while (::FindNextFile(hFind, &fd));
					::FindClose(hFind);
				}
				return 0;
#else
				DIR *dp;
				struct dirent *dirp;
				if ((dp = opendir(dir.c_str())) == NULL) {
					return -1;
				}

				while ((dirp = readdir(dp)) != NULL) {
					files.push_back(std::string(dir + dirp->d_name));
				}
				closedir(dp);
				return 0;
#endif
            }

			int readFilesWithEnding(std::string dir, std::vector<std::string> &files, std::string ending)
			{
#ifdef _WIN32
				//std::replace(dir.begin(), dir.end(), '/', '\\');

				char search_path[200];
				sprintf(search_path, "%s*.*", dir.c_str());
				WIN32_FIND_DATA fd; 
				wchar_t* wString = new wchar_t[4096];
				MultiByteToWideChar(CP_ACP, 0, search_path, -1, wString, 4096);

				HANDLE hFind = ::FindFirstFile(wString, &fd);
				if(hFind != INVALID_HANDLE_VALUE) { 
					do { 
						// read all (real) files in current folder
						// , delete '!' read other 2 default folder . and ..
						if(! (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {

							char ch[260];
							char DefChar = ' ';
							WideCharToMultiByte(CP_ACP, 0, fd.cFileName, -1, ch, 260, &DefChar, NULL);

							std::string s(ch);
							if (s.find(ending) != std::string::npos){
								files.push_back(dir+s);
							}
						}
					}while(::FindNextFile(hFind, &fd)); 
					::FindClose(hFind); 
				} 
#else
				DIR *dp;
				struct dirent *dirp;
				if((dp  = opendir(dir.c_str())) == NULL) {
					return -1;
				}

				while ((dirp = readdir(dp)) != NULL) {
					std::string cur(dirp->d_name);
					if(cur.find(ending) != std::string::npos)
						files.push_back(std::string(dir+dirp->d_name));
				}
				closedir(dp);
				
#endif
				return 0;
            }

		private:
			FileFinder(){};
			~FileFinder(){}
	};


};

#endif