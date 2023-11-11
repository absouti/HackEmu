#pragma once

#include "base.h"

struct fTime {
	std::chrono::system_clock::time_point ATime; // Access Time
	std::chrono::system_clock::time_point MTime; // Modify Time
	std::chrono::system_clock::time_point CTime; // Change Time
};
enum class vfile_type {
	none,		//not used
	not_found,
	regular,
	directory,
	symlink,
};
struct block {
	size_t nodeNum;
	size_t filesize;
	void* charbuf;
	block();
	//~block();
};
struct vfile {
	std::string filename;
	vfile_type type;
	block* block;
	fTime time;
	vfile(std::string filename, vfile_type type);
	bool operator==(vfile& comparedF);
	//~vfile();
};
struct vdir {
	std::string dirName;
	size_t fileNum;
	std::list<vfile*> subFiles;
	std::list<vdir> subDirs;
	vdir();
};
class VFileSystem {
private:
	std::list<vfile> indexArea;
	std::list<block> dataArea;
	vdir rootDataArea;
public:
	VFileSystem();
	~VFileSystem();
	vfile& AddFile(std::string filename, vfile_type type, std::string path,
		void* Data, size_t data_len);
	bool DelFile(vfile file);
	bool Hardlink(block destNode, vfile file);
	vdir& getFileDirObj(vfile& file);
};