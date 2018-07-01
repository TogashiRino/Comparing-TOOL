#pragma once
#include "stdafx.h"
#include<iostream>
#include<algorithm>
#include <fstream> 
#include<string>
#include <experimental/filesystem>
#include"tinyxml2.h"
using namespace tinyxml2;
namespace fs = std::experimental::filesystem;
using namespace std;
class Directory
{
private:
	/*
	I could have used structure and just have here the vector and path but it would create an issue with the whole
	intializiation and just not worth it :D
	*/
	vector<Directory> subDir;
	string name;
	fs::path path;
	char type; //f stands for file d stands for directory;
	double size;
	int index;
	std::time_t cftime;
	string chosenPath; //say you have the selected at D:/UnitTesting/sample files/Sample 2/ code.cpp its only the path sample 2/code.cpp like in the xml file 
	vector<string> simplePath;

public:
	void intiate(); //add subdirectories and files to subDire viector
	void view();//prints the structure
	Directory(fs::path p);
	void setPath(fs::path p);
	fs::path getPath();
	double calculateDirectorySize(fs::path pa);
	fs::path getChosenPath(int index);
	void setSimplePathVector();
	void setChosenPath(string p);
	void printSimplePathVector();
	vector<std::string> split(std::string str, std::string sep);
	void writeTOXMl(string fileName,bool result);
	int searchbyName(string name);
	Directory();
	~Directory();
};

