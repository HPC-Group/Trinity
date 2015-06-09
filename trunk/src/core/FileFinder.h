#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>

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
                DIR *dp;
                struct dirent *dirp;
                if((dp  = opendir(dir.c_str())) == NULL) {
                    return -1;
                }

                while ((dirp = readdir(dp)) != NULL) {
                    files.push_back(std::string(dir+dirp->d_name));
                }
                closedir(dp);
                return 0;
            }

            int readFilesWithEnding(std::string dir, std::vector<std::string> &files, std::string ending)
            {
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
                return 0;
            }

		private:
			FileFinder(){};
			~FileFinder(){}
	};


};
