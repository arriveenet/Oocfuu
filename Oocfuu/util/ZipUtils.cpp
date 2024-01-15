#include "ZipUtils.h"
#include <filesystem>

#define ZIP_FILE_NAME_MAX	256

namespace fs = std::filesystem;

struct ZipEntryInfo {
	unz_file_pos pos;
	uLong uncompressed_size;
};

ZipFile::ZipFile(const std::string& zipFile)
{
	m_zipFile = unzOpen(zipFile.c_str());
}

ZipFile::~ZipFile()
{
	if (m_zipFile != nullptr) {
		unzClose(m_zipFile);
		m_zipFile = nullptr;
	}
}

bool ZipFile::load()
{
	m_fileList.clear();

	char fileName[ZIP_FILE_NAME_MAX + 1];
	unz_file_info64 fileInfo;
	

	for (int err = unzGoToFirstFile(m_zipFile); err == UNZ_OK; err = unzGoToNextFile(m_zipFile)) {

		unzGetCurrentFileInfo64(m_zipFile, &fileInfo, fileName, sizeof(fileName) - 1, nullptr, 0, nullptr, 0);

		unz_file_pos filePos;
		int posErr = unzGetFilePos(m_zipFile, &filePos);
		if (posErr == UNZ_OK) {
			std::string currentFileName = fileName;
			ZipEntryInfo entry = {};
			entry.pos = filePos;
			entry.uncompressed_size = static_cast<uLong>(fileInfo.uncompressed_size);
			
			m_fileList[currentFileName] = entry;
		}
	}

	return true;
}

std::vector<std::string> ZipFile::getFileList()
{
	std::vector<std::string> files;

	FileListContainer::const_iterator itr = m_fileList.begin();
	for (; itr != m_fileList.end(); itr++) {
		const std::string& filename = itr->first;
		if (!fs::is_directory(fs::path(filename))) {
			files.push_back(filename);
		}
	}

	return files;
}

unsigned char* ZipFile::getFileData(const std::string& fileName, size_t& size)
{
	return nullptr;
}
