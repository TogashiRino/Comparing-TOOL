// MySolution.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <ctime>
#include<iostream>
#include<algorithm>
#include <fstream> 
#include<string>
#include <experimental/filesystem>
#include"tinyxml2.h"
#include "Directory.h"
using namespace tinyxml2;



namespace fs = std::experimental::filesystem;
using namespace std;
//funciton that comapres two files and takes the pathes of these files 
bool ArefilesTheSame(fs::path p1, fs::path p2)
{

	ifstream file1, file2;
	ifstream::pos_type size1, size2;

	file1.open(p1, ios_base::binary);
	if (!file1.is_open()) {
		cout << "failed to open the first file" << endl;
		return false;
	}
	else {
		cout << "First File opened successfully" << endl;

		file1.seekg(0, ios::end);
		size1 = file1.tellg();
		file1.seekg(0, ios::beg);
	}

	file2.open(p2, ios_base::binary);
	if (!file2.is_open()) {
		cout << "failed to open the second file" << endl;
		return false;
	}
	else {
		cout << "Second File opened successfully" << endl;

		file2.seekg(0, ios::end);
		size2 = file2.tellg();
		file2.seekg(0, ios::beg);
		//if they both don't have the same files and its a binary comparison then obviously something is different 
		if (size1 != size2) {
			return false;
		}
		//otherwise i take chunks of bytes of both to make it faster than comparing all
		static const size_t BLOCKSIZE = 4096;
		size_t remaining = size1;
		while (remaining)
		{
			char buffer1[BLOCKSIZE], buffer2[BLOCKSIZE];
			size_t size = std::min(BLOCKSIZE, remaining);
			file1.read(buffer1, size);
			file2.read(buffer2, size);

			if (0 != memcmp(buffer1, buffer2, size))
				return false;

			remaining -= size;
		}
		file1.close();
		file2.close();
		return true;

	}
}

/*
hello welcome to my code :D  these are few notes to help you understand better and also save your time
3 things I want to address :
-the print structure function doesn't look like what is in the assignment where you compare
both contents side by side ,I print the contents of the firth path then the second  and it isn't very neat but its efficient enough,

-I just wanna mention that I used class because you said its mandatory and ofc to show you that using class isn't an issue but
i didn't use struct which i hope you figure out is less of an issue
-if line 51 in path.cpp gives you an error I solved this issue by going to project properties ,c/c++,preprocesser and added _CRT_SECURE_NO_WARNINGS,
it turns out to be a known issue

Finally: You will find some comments to help you out with the code knowing you prob won't need it,Enjoy :)).



*/

int main()
{
	
	/*
	Example that helps in making the xml 

	Directory x("D:/UnitTesting/sample files/Sample 1");
	x.intiate();
	x.view();
	x.setChosenPath("D:/UnitTesting/sample files/Sample 1/Code/main.cpp");
	x.setSimplePathVector();
	x.printSimplePathVector();
	cout << x.searchbyName("Code");
	x.writeTOXMl("D:/file.xml", true);
	int wait;
	cin >> wait;
*/
	



	while (true) {
		string p1, p2;
		bool check;
		cout << "Enter First Path: ";
		getline(cin, p1);
		while (!fs::exists(p1)) {//checks if the path does exist 
			cout << "The first path you entered doesn't Exist, Please try again \n" << "Enter First Path: ";
			getline(cin, p1);
		}
		cout << "Enter Second Path: ";
		getline(cin, p2);
		while (!fs::exists(p2)) {
			cout << "The second path you entered doesn't Exist, Please try again \n" << "Enter Second Path: ";
			getline(cin, p2);
		}
		Directory x(p1);
		x.intiate();//adds all files of the first path,recursively
		x.view();//prints the structure of the first path
		Directory y(p2);
		y.intiate();
		y.view();
		cout << "Enter File Index Number to compare:- ";
		int choice;
		cin >> choice;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		fs::path first, second;
		first = x.getChosenPath(choice);
		second = y.getChosenPath(choice);
		
		if (first.string() != "-1"&&second.string() != "-1") {
			x.setChosenPath(first.string());
			y.setChosenPath(second.string());
			x.setSimplePathVector();
			y.setSimplePathVector();
		//	x.printSimplePathVector();
		//	y.printSimplePathVector();
			cout << first << "             " << second << endl;
			 check = ArefilesTheSame(first, second);
			if (check) {
				cout << "Matched \n";
			}
			else {
				cout << "Not Matched \n";
			}
		}

		cout << "Export the results to XML file, Enter file name ";
		string fileName;
		getline(cin, fileName);
		while (!fs::exists(fileName)) {
			cout << "No such file, Please Try again \n" << "Export the results to XML file, Enter file name ";
			getline(cin, fileName);
		}
		x.writeTOXMl(fileName, check);


		cout << "Try Again?[y/n]: ";

		char response;
		cin >> response;
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (response == 'n' || response == 'N') {
			break;
		}

	}

	
	return 0;
}

