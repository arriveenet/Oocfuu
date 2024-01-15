#pragma once
#include <minizip/unzip.h>
#include <string>
#include <vector>
#include <unordered_map>

struct ZipEntryInfo;

class ZipFile {
public:
	ZipFile(const std::string& zipFile);
	virtual ~ZipFile();

	bool load();
	
	std::vector<std::string> getFileList();
	unsigned char* getFileData(const std::string& fileName, size_t& size);

private:
	ZipFile();

	unzFile m_zipFile;
	typedef std::unordered_map<std::string, struct ZipEntryInfo> FileListContainer;
	FileListContainer m_fileList;
};
