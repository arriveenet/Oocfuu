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
	bool extract(const std::string& path);

	std::vector<std::string> getFileList();
	unsigned char* getFileData(const std::string& fileName, size_t& size);

private:
	bool write(const std::string& fileName, unsigned char* data, size_t size);

private:
	ZipFile();

	unzFile m_zipFile;
	typedef std::unordered_map<std::string, struct ZipEntryInfo> FileListContainer;
	FileListContainer m_fileList;
};
