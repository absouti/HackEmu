#include "vfs.h"

size_t block_num = 0;

vdir::vdir() {
	this->fileNum = 0;
}
block::block() {
	this->nodeNum = block_num;
	block_num += 1;
}
vfile::vfile(std::string filename, std::string type) {
	this->filename = filename;
	this->type = type;
}
VFileSystem::VFileSystem() {
	//Read storge
	//...

	this->rootDataArea.dirName = "/";
}
VFileSystem::~VFileSystem() {
	//Write storge
	//...
}
vfile& VFileSystem::AddFile(std::string filename, std::string type, void* Data, size_t data_len) {
	// check data_len(optical)
	
	
	// create block
	block tmpBlock;
	tmpBlock.filesize = data_len;
	tmpBlock.charbuf = Data;
	this->dataArea.push_back(tmpBlock);

	// create vfile
	vfile tmpFile(filename, type);
	tmpFile.block = &dataArea.back();
	this->indexArea.push_back(tmpFile);

	// add file to dir
	this->rootDataArea.subFiles.push_back(&this->indexArea.back());
	this->rootDataArea.fileNum += 1;

	// return file point
	return this->indexArea.back();
}
bool VFileSystem::DelFile(vfile file) {
	
	return false;
}
bool VFileSystem::Hardlink(block destNode, vfile file) {
	return false;
}
vdir& VFileSystem::getFileDirObj(vfile& file) {
	for (auto itr = this->rootDataArea.subFiles.begin();
		itr != this->rootDataArea.subFiles.end(); itr++) {
		if (&file == *itr) {
			return this->rootDataArea;
		}
	}
	for (auto itd = this->rootDataArea.subDirs.begin();
		itd != this->rootDataArea.subDirs.end(); itd++) {
		for (auto itf = this->rootDataArea.subDirs.begin();
			itf != this->rootDataArea.subDirs.end(); itf++) {

		}
	}
}
bool vfile::operator==(vfile& comparedF) {
	//ÔÝÊ±ÆúÓÃ
	if (this->block != comparedF.block)
		return false;
	if (this->filename.compare(comparedF.filename))
		return false;
}