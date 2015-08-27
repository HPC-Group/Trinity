#include "FileTools.h"

#include <iterator> // back_inserter
#include <algorithm> // transform
 

#ifndef DETECTED_OS_WINDOWS
#include <regex.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#define LARGE_STAT(name,buffer) stat(name,buffer)
#else
#include <shlwapi.h>
#include <sys/stat.h>
#pragma warning (disable : 4996)
#define LARGE_STAT(name,buffer) _stat64(name,buffer)
#endif


using namespace std;
using namespace Core::StringTools;

namespace Core {
  namespace IO {
    namespace FileTools {

      string GetFromResourceOnMac(const string& strFileName) {
    #ifdef DETECTED_OS_APPLE2
      CFStringRef cfFilename = CFStringCreateWithCString(kCFAllocatorDefault, RemoveExt(GetFilename(strFileName)).c_str(), CFStringGetSystemEncoding());
      CFStringRef cfExt = CFStringCreateWithCString(kCFAllocatorDefault, GetExt(GetFilename(strFileName)).c_str(), CFStringGetSystemEncoding());

      CFURLRef    imageURL = CFBundleCopyResourceURL( CFBundleGetMainBundle(), cfFilename, cfExt, NULL );
      if (imageURL == NULL) return "";
      CFStringRef macPath = CFURLCopyFileSystemPath(imageURL, kCFURLPOSIXPathStyle);
      const char *pathPtr = CFStringGetCStringPtr(macPath, CFStringGetSystemEncoding());
      if (macPath != 0 && pathPtr != 0) {
        string result = pathPtr;
        return result;
      } else return strFileName;
    #else
      return strFileName;
    #endif

  }

      bool HasAbsPath(const std::string& path) {
#ifdef DETECTED_OS_WINDOWS
        return (path.size() > 2  && path[1] == ':');
#else
        return (path.size() > 0  && path[0] == '/');
#endif
      }
      
      string Abs2RelPath(const std::string& sourcePath,
                         const std::string& targetFile) {
        string source = GetPath(CanonicalizePath(sourcePath));
        string target = GetPath(CanonicalizePath(targetFile));
        
        if (!HasAbsPath(target) || !HasAbsPath(source)) return targetFile;
        
        // do the computation with linux-style slashes
        ReplaceAll(source, "\\", "/");
        ReplaceAll(target, "\\", "/");
        
        vector<string> source_list = Tokenize(source, PM_CUSTOM_DELIMITER, '/');
        vector<string> target_list = Tokenize(target, PM_CUSTOM_DELIMITER, '/');
        
#ifdef DETECTED_OS_WINDOWS
        // on windows both must be on the same drive otherwise the result
        // is simply the target
        if (ToLowerCase(source_list[0]) != target_list[0]) {
          ReplaceAll(target, "/", "\\");
          return target;
        }
#endif
        
        size_t i = 0;
        while (source_list.size() > i && target_list.size() > i) {
#ifdef DETECTED_OS_WINDOWS
          if (ToLowerCase(source_list[i]) != ToLowerCase(target_list[i])) {
            break;
          }
#else
          if (source_list[i] != target_list[i]) {
            break;
          }
#endif
          i++;
        }
        
        std::string result;
        for (size_t j = i; j < source_list.size();j++) {
          result += "../";
        }
        
        for (size_t j = i; j < target_list.size();j++) {
          result += target_list[j] + "/";
        }
        result += GetFilename(targetFile);

#ifdef DETECTED_OS_WINDOWS
        ReplaceAll(result, "/", "\\");
#endif
        
        return result;
      }

      
      bool GetFileStats(const string& strFileName, LARGE_STAT_BUFFER& stat_buf) {
        return (LARGE_STAT( strFileName.c_str(), &stat_buf) >= 0);
      }
      
      bool GetFileStats(const wstring& wstrFileName, LARGE_STAT_BUFFER& stat_buf) {
        string strFileName(wstrFileName.begin(), wstrFileName.end());
        return (LARGE_STAT( strFileName.c_str(), &stat_buf) >= 0);
      }
      
      bool FileExists(const wstring& wstrFileName) {
        LARGE_STAT_BUFFER stat_buf;
        return GetFileStats(wstrFileName, stat_buf);
      }
      
      bool FileExists(const string& strFileName) {
        LARGE_STAT_BUFFER stat_buf;
        return GetFileStats(strFileName, stat_buf);
      }
      
      string GetExt(const string& fileName) {
        size_t indexDot = fileName.find_last_of(".");
        size_t indexSlash = std::max<int>(int(fileName.find_last_of("\\")),int(fileName.find_last_of("/")));
        if (indexDot == string::npos || (indexSlash != string::npos && indexDot < indexSlash)) return "";
        
        string ext = fileName.substr(indexDot+1);
        return ext;
      }
      
      wstring GetExt(const wstring& fileName) {
        size_t indexDot = fileName.find_last_of(L".");
        size_t indexSlash = std::max<int>(int(fileName.find_last_of(L"\\")),int(fileName.find_last_of(L"/")));
        if (indexDot == wstring::npos || (indexSlash != wstring::npos && indexDot < indexSlash)) return L"";
        
        wstring ext = fileName.substr(indexDot+1);
        return ext;
      }
      
      string GetPath(const string& fileName) {
        string path = fileName.substr(0,std::max<int>(int(fileName.find_last_of("\\")),int(fileName.find_last_of("/")))+1);
        if(path.empty()) { path = "./"; }
        return path;
      }
      
      wstring GetPath(const wstring& fileName) {
        wstring path = fileName.substr(0,std::max<int>(int(fileName.find_last_of(L"\\")),int(fileName.find_last_of(L"/")))+1);
        if(path.empty()) { path = L"./"; }
        return path;
      }
      
      std::string GetFilename(const std::string& fileName) {
        size_t index = std::max<int>(int(fileName.find_last_of("\\")),int(fileName.find_last_of("/")))+1;
        string name = fileName.substr(index,fileName.length()-index);
        return name;
      }
      
      std::wstring GetFilename(const std::wstring& fileName) {
        size_t index = std::max<int>(int(fileName.find_last_of(L"\\")),int(fileName.find_last_of(L"/")))+1;
        wstring name = fileName.substr(index,fileName.length()-index);
        return name;
      }
      
      std::string AssembleFullFilename(const std::string& dir, const std::string& name) {
        if (dir.empty()) return name;
        std::string result = dir;
#ifdef DETECTED_OS_WINDOWS
        ReplaceAll(result, "/", "\\");
        if (dir[dir.length()-1] != '\\')
          return result + name;
        else
          return result + "\\" + name;
#else
        ReplaceAll(result, "\\", "/");
        if (dir[dir.length()-1] != '/')
          return result + name;
        else
          return result + "/" + name;
#endif
      }

      std::wstring AssembleFullFilename(const std::wstring& dir, const std::wstring& name) {
        if (dir.empty()) return name;
        std::wstring result = dir;
#ifdef DETECTED_OS_WINDOWS
        ReplaceAll(result, L"/", L"\\");
        if (dir[dir.length()-1] != wchar_t('\\'))
          return result + name;
        else
          return result + L"\\" + name;
#else
        ReplaceAll(result, L"\\", L"/");
        if (dir[dir.length()-1] != wchar_t('/'))
          return result + name;
        else
          return result + L"/" + name;
#endif
      }
      
#ifdef DETECTED_OS_WINDOWS
# define MAX_PATH_LENGTH MAX_PATH
#else
# define MAX_PATH_LENGTH PATH_MAX
#endif
      
      std::string CanonicalizePath(const std::string& _path) {
        std::string path = _path;
#ifdef DETECTED_OS_WINDOWS
        ReplaceAll(path, "/", "\\"); // make sure all slashes are of the same type
        wchar_t resolved[MAX_PATH_LENGTH];
        wchar_t wide[MAX_PATH_LENGTH];
        mbstowcs(wide, path.c_str(), MAX_PATH_LENGTH);
        if(PathCanonicalize(resolved, wide) == FALSE)
#else
          ReplaceAll(path, "\\", "/"); // make sure all slashes are of the same type
          char resolved[MAX_PATH_LENGTH];
        if(realpath(path.c_str(), resolved) == NULL)
#endif
        {
          std::ostringstream fn;
//          fn << "realpath " << path;
//          perror(fn.str().c_str());
          return path; // need to return *something*.
        }
#ifdef DETECTED_OS_WINDOWS
        char buffer[MAX_PATH_LENGTH];
        const wchar_t *res_ptr = resolved;
        mbstate_t mbs = {0};
        wcsrtombs(buffer, &res_ptr, MAX_PATH_LENGTH, &mbs);
        return std::string(buffer);
#else
        return std::string(resolved);
#endif
      }
      
      string FindPath(const string& fileName, const string& path) {
        string searchFile;
        string slash = "";
        
        if (fileName[0] != '/' && fileName[0] != '\\' && path[path.length()-1] != '/' && path[path.length()-1] != '\\') {
          slash = "/";
        }
        
        
        // search in the given path
        searchFile = path + slash + fileName;
        
        // search in the given path
        searchFile = path + fileName;
        if (FileExists(searchFile))  return searchFile;
        
        // search in the current directory
        searchFile = "./" + fileName;
        if (FileExists(searchFile)) return searchFile;
        
        // search in the parent directory
        searchFile = "../" + fileName;
        if (FileExists(searchFile)) return searchFile;
        
        return "";
      }
      
      wstring FindPath(const wstring& fileName, const wstring& path) {
        wstring searchFile;
        wstring slash = L"";
        
        if (fileName[0] != wchar_t('/') && fileName[0] != wchar_t('\\') &&
            path[path.length()-1] != wchar_t('/') && path[path.length()-1] != wchar_t('\\')) {
          
          slash = L"/";
        }
        
        
        // search in the given path
        searchFile = path + slash + fileName;
        if (FileExists(searchFile))  return searchFile;
        
        // search in the current directory
        searchFile = L".\\" + fileName;
        if (FileExists(searchFile)) return searchFile;
        
        // search in the parent directory
        searchFile = L"..\\" + fileName;
        if (FileExists(searchFile)) return searchFile;
        
        return L"";
      }
      
      
      std::string  RemoveExt(const std::string& fileName) {
        size_t indexDot = fileName.find_last_of(".");
        size_t indexSlash = std::max<int>(int(fileName.find_last_of("\\")),int(fileName.find_last_of("/")));
        if (indexDot == string::npos || (indexSlash != string::npos && indexDot < indexSlash)) return fileName;
        
        return fileName.substr(0,indexDot);
      }
      
      std::wstring  RemoveExt(const std::wstring& fileName) {
        size_t indexDot = fileName.find_last_of(L".");
        size_t indexSlash = std::max<int>(int(fileName.find_last_of(L"\\")),int(fileName.find_last_of(L"/")));
        if (indexDot == wstring::npos || (indexSlash != wstring::npos && indexDot < indexSlash)) return fileName;
        
        return fileName.substr(0,indexDot);
      }
      
      
      string  ChangeExt(const string& fileName, const std::string& newext) {
        return RemoveExt(fileName)+ "." + newext;
      }
      
      wstring ChangeExt(const std::wstring& fileName, const std::wstring& newext) {
        return RemoveExt(fileName)+ L"." + newext;
      }
      
      string  CheckExt(const string& fileName, const std::string& newext) {
        string currentExt = GetExt(fileName);
#ifdef DETECTED_OS_WINDOWS  // do a case insensitive check on windows systems
        if (ToLowerCase(currentExt) != ToLowerCase(newext))
#else
          if (currentExt != newext)
#endif
            return fileName + "." + newext;
          else
            return fileName;
      }
      
      wstring CheckExt(const std::wstring& fileName, const std::wstring& newext) {
        wstring currentExt = GetExt(fileName);
#ifdef DETECTED_OS_WINDOWS  // do a case insensitive check on windows systems
        if (ToLowerCase(currentExt) != ToLowerCase(newext))
#else
          if (currentExt != newext)
#endif
            return fileName + L"." + newext;
          else
            return fileName;
      }
      
      string  AppendFilename(const string& fileName, const string& tag) {
        return RemoveExt(fileName) + tag + "." + GetExt(fileName);
      }
      
      wstring AppendFilename(const wstring& fileName, const wstring& tag) {
        return RemoveExt(fileName) + tag + L"." + GetExt(fileName);
      }
      
      string  AppendFilename(const string& fileName, const int iTag) {
        return AppendFilename(fileName, ToString(iTag));
      }
      
      wstring AppendFilename(const wstring& fileName, const int iTag) {
        return AppendFilename(fileName, ToWString(iTag));
      }
      
      vector<wstring> GetSubDirList(const wstring& dir) {
        vector<wstring> subDirs;
        wstring rootdir;
        
#ifdef DETECTED_OS_WINDOWS
        wstringstream s;
        if (dir == L"") {
          WCHAR path[4096];
          GetCurrentDirectoryW(4096, path);
          s << path << L"/";
        } else {
          s << dir << L"/";
        }
        
        rootdir = s.str();
        
        WIN32_FIND_DATAW FindFileData;
        HANDLE hFind;
        
        hFind=FindFirstFileW((rootdir + L"*.*").c_str(), &FindFileData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
          do {
            wstring wstrFilename = FindFileData.cFileName;
            if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && wstrFilename != L"." && wstrFilename != L"..") {
              subDirs.push_back(wstrFilename);
            }
          }while ( FindNextFileW(hFind, &FindFileData) );
        }
        
        FindClose(hFind);
#else
        if (dir == L"") {
          rootdir = L"./";
        } else {
          rootdir = dir + L"/";
        }
        
        string strDir(rootdir.begin(), rootdir.end());
        
        DIR* dirData=opendir(strDir.c_str());
        
        if (dirData != NULL) {
          struct dirent *inode;
          
          while ((inode=readdir(dirData)) != NULL) {
            string strFilenameLocal = inode->d_name;
            wstring wstrFilename(strFilenameLocal.begin(), strFilenameLocal.end());
            string strFilename = strDir + strFilenameLocal;
            
            struct ::stat st;
            if (::stat(strFilename.c_str(), &st) != -1)
              if (S_ISDIR(st.st_mode) && strFilenameLocal != "." && strFilenameLocal != "..") {
                subDirs.push_back(wstrFilename);
              }
          }
          closedir(dirData);
        }
#endif
        
        vector<wstring> completeSubDirs(subDirs.size());
        for (size_t i = 0;i<subDirs.size();i++) {
          completeSubDirs.push_back(rootdir+subDirs[i]);
        }
        
        return completeSubDirs;
      }
      
      
      vector<string> GetSubDirList(const string& dir) {
        vector<string> subDirs;
        string rootdir;
        
#ifdef DETECTED_OS_WINDOWS
        stringstream s;
        if (dir == "") {
          CHAR path[4096];
          GetCurrentDirectoryA(4096, path);
          s << path << "/";
        } else {
          s << dir << "/";
        }
        
        rootdir = s.str();
        
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind;
        
        hFind=FindFirstFileA((rootdir + "*.*").c_str(), &FindFileData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
          do {
            string strFilename = FindFileData.cFileName;
            if( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && strFilename != "." && strFilename != "..") {
              subDirs.push_back(strFilename);
            }
          }while ( FindNextFileA(hFind, &FindFileData) );
        }
        
        FindClose(hFind);
#else
        if (dir == "") {
          rootdir = "./";
        } else {
          rootdir = dir + "/";
        }
        
        DIR* dirData=opendir(rootdir.c_str());
        
        if (dirData != NULL) {
          struct dirent *inode;
          
          while ((inode=readdir(dirData)) != NULL) {
            string strFilenameLocal = inode->d_name;
            string strFilename = rootdir + strFilenameLocal;
            
            struct ::stat st;
            if (::stat(strFilename.c_str(), &st) != -1)
              if (S_ISDIR(st.st_mode) && strFilenameLocal != "." && strFilenameLocal != "..") {
                subDirs.push_back(strFilenameLocal);
              }
          }
          closedir(dirData);
        }
#endif
        
        vector<string> completeSubDirs(subDirs.size());
        for (size_t i = 0;i<subDirs.size();i++) {
          completeSubDirs.push_back(rootdir+subDirs[i]);
        }
        return completeSubDirs;
      }
      
      vector<wstring> GetDirContents(const wstring& dir,
                                     const wstring& fileName,
                                     const wstring& ext) {
        vector<wstring> files;
        wstringstream s;
        
#ifdef DETECTED_OS_WINDOWS
        wstring wstrDir;
        if (dir == L"") {
          WCHAR path[4096];
          GetCurrentDirectoryW(4096, path);
          s << path << L"/" << fileName << L"." << ext;
          wstrDir = wstring(path);
        } else {
          s << dir << L"/" << fileName << L"." << ext;
          wstrDir = dir;
        }
        
        WIN32_FIND_DATAW FindFileData;
        HANDLE hFind;
        
        
        hFind=FindFirstFileW(s.str().c_str(), &FindFileData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
          do {
            if( 0 == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
              files.push_back(wstrDir + L"/" + wstring(FindFileData.cFileName));
            }
          }while ( FindNextFileW(hFind, &FindFileData) );
        }
        
        FindClose(hFind);
#else
        wstring wstrDir;
        if (dir == L"") {
          wstrDir = L"./";
        } else {
          wstrDir = dir + L"/";
        }
        string strDir(wstrDir.begin(), wstrDir.end());
        
        
        // filter files via regexpr
        string regExpr = "^";
        regex_t preg;
        if (fileName != L"")  {
          string strFileName(fileName.begin(), fileName.end());
          regExpr = regExpr + strFileName;
          // replace * by .* and ? by .
          ReplaceAll(regExpr, "*", ".*");
          ReplaceAll(regExpr, "?", ".");
        }
        if (ext != L"")  {
          if (fileName == L"") regExpr = regExpr + ".*";
          
          string tmpext(ext.begin(), ext.end());
          // replace * by .* and ? by .
          ReplaceAll(tmpext, "*", ".*");
          ReplaceAll(tmpext, "?", ".");
          
          // append dot and extension to regular expression
          regExpr = regExpr + "\\." + tmpext + "$";
        }
        if (regcomp(&preg, regExpr.c_str(), REG_EXTENDED | REG_NOSUB) != 0) return files;
        
        
        DIR* dirData=opendir(strDir.c_str());
        
        if (dirData != NULL) {
          struct dirent *finfo;
          
          while ((finfo=readdir(dirData)) != NULL) {
            string strFilename = finfo->d_name;
            wstring wstrFilename(strFilename.begin(), strFilename.end());
            strFilename = strDir + strFilename;
            
            struct ::stat st;
            if (::stat(strFilename.c_str(), &st) != -1)
              if (!S_ISDIR(st.st_mode) && !regexec(&preg, finfo->d_name, size_t(0), NULL, 0)) {
                files.push_back(wstrFilename);
              }
          }
          closedir(dirData);
        }
        regfree(&preg);
#endif
        
        return files;
      }
      
      vector<string> GetDirContents(const string& dir,
                                    const string& fileName,
                                    const string& ext) {
        vector<string> files;
        
        stringstream s;
        
        
#ifdef DETECTED_OS_WINDOWS
        WIN32_FIND_DATAA FindFileData;
        HANDLE hFind;
        
        string strDir;
        if (dir == "") {
          char path[4096];
          GetCurrentDirectoryA(4096, path);
          s << path << "/" << fileName << "." << ext;
          strDir = string(path);
        } else {
          s << dir << "/" << fileName << "." << ext;
          strDir = dir;
        }
        
        hFind=FindFirstFileA(s.str().c_str(), &FindFileData);
        
        if (hFind != INVALID_HANDLE_VALUE) {
          do {
            if( 0 == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) {
              files.push_back(strDir + "/" + string(FindFileData.cFileName));
            }
          }while ( FindNextFileA(hFind, &FindFileData) );
        }
        
        FindClose(hFind);
#else
        string strDir;
        if (dir == "") {
          strDir = "./";
        } else {
          strDir = dir + "/";
        }
        
        // filter files via regexpr
        string regExpr = "^";
        regex_t preg;
        if (fileName != "")  {
          regExpr = regExpr + fileName;
          // replace * by .* and ? by .
          ReplaceAll(regExpr, "*", ".*");
          ReplaceAll(regExpr, "?", ".");
        }
        if (ext != "")  {
          if (fileName == "") regExpr = regExpr + ".*";
          
          string tmpext = ext;
          // replace * by .* and ? by .
          ReplaceAll(tmpext, "*", ".*");
          ReplaceAll(tmpext, "?", ".");
          
          // append dot and extension to regular expression
          regExpr = regExpr + "\\." + tmpext + "$";
        }
        if (regcomp(&preg, regExpr.c_str(), REG_EXTENDED | REG_NOSUB) != 0) return files;
        
        DIR* dirData=opendir(strDir.c_str());
        
        if (dirData != NULL) {
          struct dirent *finfo;
          
          while ((finfo=readdir(dirData)) != NULL) {
            string strFilename = finfo->d_name;
            strFilename = strDir + strFilename;
            
            struct ::stat st;
            if (::stat(strFilename.c_str(), &st) != -1) {
              if (!S_ISDIR(st.st_mode) &&
                  !regexec(&preg, finfo->d_name, size_t(0), NULL, 0)) {
                files.push_back(std::string(strFilename.c_str()));
              }
            }
          }
          closedir(dirData);
        }
        regfree(&preg);
#endif
        
        return files;
      }
      
      std::string  FindNextSequenceName(const std::string& strFilename) {
        std::string dir = GetPath(strFilename);
        std::string fileName = RemoveExt(GetFilename(strFilename));
        std::string ext = GetExt(strFilename);
        
        return FindNextSequenceName(fileName, ext, dir);
      }
      
      std::wstring FindNextSequenceName(const std::wstring& wStrFilename) {
        std::wstring dir = GetPath(wStrFilename);
        std::wstring fileName = RemoveExt(GetFilename(wStrFilename));
        std::wstring ext = GetExt(wStrFilename);
        
        return FindNextSequenceName(fileName, ext, dir);
      }
      
      // Functor to identify the numeric ID appended to a given filename.
      template <typename T> struct fileNumber : public std::unary_function<T, size_t> {
        size_t operator()(const T& filename) const {
          // get just the filename itself, without extension or path information.
          T fn = RemoveExt(GetFilename(filename));
          
          if (fn.length() == 0) return 0;
          
          // Find where the numbers start.
          typename T::const_iterator numerals = fn.end()-1;
          while (numerals != fn.begin() && ::isdigit(*(numerals-1))) --numerals;
          
          
          // the string should only contain the numbers or a _ otherweise the
          // filename in question was just a prefix to another longer filename
          size_t iNonNumeralCharCount = fn.length()-T(&*numerals).length();
          if (iNonNumeralCharCount > 1) return 0;
          if (iNonNumeralCharCount > 0) {
            T leadingCharacter = fn.substr(0,1);
            std::string tmp(leadingCharacter.begin(), leadingCharacter.end());
            if (tmp != "_") return 0;
          }
          
          
          // convert it to a size_t and return that.
          size_t retval = 0;
          FromString(retval, T(&*numerals));
          return retval;
        }
      };
      
      // Given a filename model and a directory, identify the next filename in the
      // sequence.  Sequences start at 0 and increment.
      string FindNextSequenceName(const string& fileName, const string& ext,
                                  const string& dir) {
        stringstream out;
        vector<string> files = GetDirContents(dir, fileName+"*", ext);
        
        // chop of original filename (in case it also ends with numbers)
        for (auto i=files.begin(); i < files.end(); ++i) {
          std::string tmp = GetFilename(*i);
          (*i) = tmp.substr(fileName.length(), tmp.length()-fileName.length());
        }
        
        // Get a list of all the trailing numeric values.
        std::vector<size_t> values;
        values.reserve(files.size());
        std::transform(files.begin(), files.end(), std::back_inserter(values),
                       fileNumber<std::string>());
        
        // No files in the dir?  Default to 1.
        if(values.empty()) {
          out << dir << fileName << "_" << 1 << "." << ext;
        } else {
          // Otherwise, the next number is the current max + 1.
          size_t max_val = *(std::max_element(values.begin(), values.end()));
          out << dir << fileName << "_" << max_val+1 << "." << ext;
        }
        
        return out.str();
      }
      
      wstring  FindNextSequenceName(const wstring& fileName, const wstring& ext, const wstring& dir) {
        wstringstream out;
        vector<wstring> files = GetDirContents(dir, fileName+L"*", ext);
        
        // Get a list of all the trailing numeric values.
        std::vector<size_t> values;
        values.reserve(files.size());
        std::transform(files.begin(), files.end(), std::back_inserter(values),
                       fileNumber<std::wstring>());
        
        // No files in the dir?  Default to 0.
        if(values.empty()) {
          out << dir << fileName << 0 << L"." << ext;
        } else {
          // Otherwise, the next number is the current max + 1.
          size_t max_val = *(std::max_element(values.begin(), values.end()));
          out << dir << fileName << max_val+1 << L"." << ext;
        }
        
        return out.str();
      }
      
      
      uint32_t FindNextSequenceIndex(const string& fileName, const string& ext, const string& dir) {
        vector<string> files = GetDirContents(dir, fileName+"*", ext);
        
        uint32_t iMaxIndex = 0;
        for (size_t i = 0; i<files.size();i++) {
          string curFilename = RemoveExt(files[i]);
          uint32_t iCurrIndex = uint32_t(atoi(curFilename.substr(fileName.length()).c_str()));
          iMaxIndex = (iMaxIndex <= iCurrIndex) ? iCurrIndex+1 : iMaxIndex;
        }
        
        return iMaxIndex;
      }
      
      uint32_t FindNextSequenceIndex(const wstring& fileName, const wstring& ext, const wstring& dir) {
        vector<wstring> files = GetDirContents(dir, fileName+L"*", ext);
        
        uint32_t iMaxIndex = 0;
        for (size_t i = 0; i<files.size();i++) {
          wstring wcurFilename = RemoveExt(files[i]);
          string curFilename(wcurFilename.begin(), wcurFilename.end());
          uint32_t iCurrIndex = uint32_t(atoi(curFilename.substr(fileName.length()).c_str()));
          iMaxIndex = (iMaxIndex <= iCurrIndex) ? iCurrIndex+1 : iMaxIndex;
        }
        
        return iMaxIndex;
      }
      
#define INFO_BUFFER_SIZE 32767
      bool GetHomeDirectory(std::string& path) {
#ifdef DETECTED_OS_WINDOWS
        char infoBuf[INFO_BUFFER_SIZE];
        DWORD dwRet = GetEnvironmentVariableA("HOMEDRIVE", infoBuf, INFO_BUFFER_SIZE);
        if (dwRet) {
          path = std::string(infoBuf);
        } else {
          return false;
        }
        dwRet = GetEnvironmentVariableA("HOMEPATH", infoBuf, INFO_BUFFER_SIZE);
        if (dwRet) {
          path = path + std::string(infoBuf);
          return true;
        } else {
          return false;
        }
#else
        // first try the HOME env. var.
        const char *strHomeFromEnv = getenv("HOME");
        // check if that was defined
        if (strHomeFromEnv) {
          path = std::string(strHomeFromEnv);
          return true;
        } else {
          // if not read the homedir from the passwd file
          const struct passwd *userpwuid = getpwuid(geteuid());
          if (userpwuid) {
            path = std::string( userpwuid->pw_dir );
            return true;
          } else return false;
        }
#endif
      }
      
      bool GetHomeDirectory(std::wstring& path) {
#ifdef DETECTED_OS_WINDOWS
        WCHAR infoBuf[INFO_BUFFER_SIZE];
        DWORD dwRet = GetEnvironmentVariableW(L"HOMEDRIVE", infoBuf, INFO_BUFFER_SIZE);
        if (dwRet) {
          path = std::wstring(infoBuf);
        } else {
          return false;
        }
        dwRet = GetEnvironmentVariableW(L"HOMEPATH", infoBuf, INFO_BUFFER_SIZE);
        if (dwRet) {
          path = path + std::wstring(infoBuf);
          return true;
        } else {
          return false;
        }
#else
        // too lazy to find the unicode version for linux and mac
        std::string astrPath;
        if (!GetHomeDirectory(astrPath)) return false;
        path = std::wstring( astrPath.begin(), astrPath.end());
        return true;
#endif
      }
    }
  }
}

/*
 The MIT License
 
 Copyright (c) 2014 HPC Group, Univeristy Duisburg-Essen
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */
