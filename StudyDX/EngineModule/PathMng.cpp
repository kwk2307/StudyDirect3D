#include "Precompiled.h"

#include <filesystem>
namespace fs = std::filesystem;

PathMng::PathMng()
	:_ContentPath()
{
}

PathMng::~PathMng()
{
}

void PathMng::init()
{	
	std::string path = fs::current_path().parent_path().string();
	path += "\\Content\\";

}
