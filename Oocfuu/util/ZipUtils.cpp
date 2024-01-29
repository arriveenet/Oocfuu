#include "ZipUtils.h"
#include <assert.h>
#include <filesystem>
#include <iostream>

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

bool ZipFile::fetch()
{
	m_fileList.clear();

	char fileName[ZIP_FILE_NAME_MAX + 1];
	unz_file_info fileInfo;
	

	for (int err = unzGoToFirstFile(m_zipFile); err == UNZ_OK; err = unzGoToNextFile(m_zipFile)) {

		unzGetCurrentFileInfo(m_zipFile, &fileInfo, fileName, sizeof(fileName) - 1, nullptr, 0, nullptr, 0);

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

bool ZipFile::extract(const std::string& path)
{
	// ファイルリストを取得
	std::vector<std::string> fileList = getFileList();

	std::string dir = path.empty() ? "./" : "/";

	for (auto& file : fileList) {
		fs::path filePath = dir + file;
		
		// ファイル名を削除し、ディレクトリを取得
		fs::path directory = filePath;
		directory.remove_filename();

		// ディレクトリ階層を作成
		fs::create_directories(directory);

		// データを取得
		size_t size = 0;
		unsigned char* pData = getFileData(file, size);

		// ファイルを書き込む
		bool ret = write(filePath.string(), pData, size);
		delete[] pData;
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
	unsigned char* pBuffer = nullptr;
	size = 0;

	do {
		FileListContainer::const_iterator itr = m_fileList.find(fileName);
		if (itr == m_fileList.end())
			break;

		ZipEntryInfo fileInfo = itr->second;

		int ret = unzGoToFilePos(m_zipFile, &fileInfo.pos);
		if (ret != UNZ_OK)
			break;

		ret = unzOpenCurrentFile(m_zipFile);
		if (ret != UNZ_OK)
			break;

		pBuffer = new unsigned char[fileInfo.uncompressed_size];

		int readSize = unzReadCurrentFile(m_zipFile, pBuffer, static_cast<unsigned int>(fileInfo.uncompressed_size));

		if (readSize != fileInfo.uncompressed_size) {
			delete[] pBuffer;
			pBuffer = nullptr;
		}
		else {
			size = fileInfo.uncompressed_size;
		}

		unzCloseCurrentFile(m_zipFile);
	} while (false);

	return pBuffer;
}

bool ZipFile::write(const std::string& fileName, unsigned char* data, size_t size)
{
	FILE* pFile = nullptr;
	errno_t err = fopen_s(&pFile, fileName.c_str(), "wb");
	if (err != 0) {
		std::cerr << "ZipFile: " << fileName << "could not be opend." << std::endl;
		return false;
	}

	if (fwrite(data, size, 1, pFile) != 1) {
		fclose(pFile);
		std::cerr << "ZipFile: " << "Failed to write file." << std::endl;
		return false;
	}

	fclose(pFile);

	return true;
}
