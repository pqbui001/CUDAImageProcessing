#include "stdafx.h"
#include "Utility.h"

using namespace std;

Utility::Utility()
{
	imagePath = "";
}

Utility::~Utility()
{
}

string Utility::GetImagePath(){
	return imagePath;
}

void Utility::SetImagePath(string path){
	imagePath = path;
}

string Utility::imagePath;
