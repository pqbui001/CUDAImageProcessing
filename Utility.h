#pragma once
#include <string>

using namespace std;

static class Utility
{
public:
	Utility();
	static string GetImagePath();
	static void SetImagePath(string path);
	~Utility();
private:
	static string imagePath;
};