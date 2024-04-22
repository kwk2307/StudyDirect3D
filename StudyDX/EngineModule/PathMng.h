#pragma once

class PathMng
{
public:
	static PathMng* GetInstance() {
		static PathMng mgr;
			return &mgr;
	}
private:
	PathMng();
	~PathMng();

private:
	std::string _ContentPath;
public:
	void init();
	const std::string& GetContentPath() { return _ContentPath; }
};

