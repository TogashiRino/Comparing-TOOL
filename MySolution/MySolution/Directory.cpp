#include "stdafx.h"
#include "Directory.h"


void Directory::intiate()
{
	int index = 1;
	for (auto & p : fs::recursive_directory_iterator(path)) {

		Directory node;
		node.path = p;
		fs::path dummy = p;
		node.name = dummy.filename().string();
		auto ftime = fs::last_write_time(p);
		node.cftime = decltype(ftime)::clock::to_time_t(ftime);
		if (fs::is_regular_file(p)) {
			node.index = index++;
			node.type = 'f';
			node.size = fs::file_size(p);
		}

		else if (fs::is_directory(p)) {
			node.type = 'd';
			node.index = 0;
			node.size = calculateDirectorySize(p);
		}
		subDir.push_back(node);


	}

}

void Directory::view()
{

	//fs::path x = p;
	//string y = x.string();
	cout << "NO.  Name                 Size                       Modified" << endl;
	for (std::vector<Directory>::iterator it = subDir.begin(); it != subDir.end(); ++it) {
		if (it->type == 'f') {
			cout << it->index << " ";
		}
		std::time_t cf = it->cftime;
		char *date = std::asctime(std::localtime(&cf));
		date[strlen(date) - 1] = '\0'; //to stop date from printing endl after it

		cout << "    " << it->name << "                 " << it->size << "               " << date << endl;



	}


}


Directory::Directory(fs::path p)
{
	path = p;
}

void Directory::setPath(fs::path p)
{
	path = p;
}

fs::path Directory::getPath()
{
	return path;
}

double Directory::calculateDirectorySize(fs::path pa)
{

	double size = 0;
	for (auto& cur : fs::recursive_directory_iterator(pa)) {
		if (fs::is_regular_file(cur)) {
			size += fs::file_size(cur);
		}
	}

	return size;
}

fs::path Directory::getChosenPath(int index)
{
	for (std::vector<Directory>::iterator it = subDir.begin(); it != subDir.end(); ++it) {
		if (it->index == index) {
			return it->path;
		}

	}
	return "-1";
}
std::vector<std::string> Directory::split(std::string str, std::string sep) { //function that I got that acts like split in java--a/x/z-> split on slash >> a,x,z
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}

void Directory::setSimplePathVector()
{
	int dif = chosenPath.length() - path.string().length();

	string sub = chosenPath.substr(path.string().length() + 1, dif);
	//don't forget to change it to \\ if its in testing and / if its manual testing 
	simplePath = split(sub, "\\");
}

void Directory::setChosenPath(string p)
{

	chosenPath = p;
}

void Directory::printSimplePathVector()
{
	for (std::vector<string>::iterator it = simplePath.begin(); it != simplePath.end(); ++it) {
		cout << *it << endl;
	}
}


void Directory::writeTOXMl(string fileName, bool r)
{
	XMLDocument xmlDoc;
	XMLNode * pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertFirstChild(pRoot);
	XMLElement * pElement;
	XMLElement * item;

	for (unsigned i = 0; i < simplePath.size(); i++) {

		item = xmlDoc.NewElement("item");
		int indx = searchbyName(simplePath[i]);
		//	cout << indx << endl;
		Directory x = subDir[indx];
		std::time_t cf = x.cftime;
		char *date = std::asctime(std::localtime(&cf));
		date[strlen(date) - 1] = '\0';
		if (i == simplePath.size() - 1) {
			if (r == true) {
				item->SetAttribute("Result", "Matched");
			}
			else {
				item->SetAttribute("Result", " Not Matched");
			}
		}
		else {
			item->SetAttribute("Result", "");
		}

		pRoot->InsertEndChild(item);
		pElement = xmlDoc.NewElement("left");
		if (x.type == 'f') {
			pElement->SetAttribute("Type", "File");
		}
		else if (x.type == 'd') {
			pElement->SetAttribute("Type", "Dir");
		}
		pElement->SetAttribute("Name", x.name.c_str());
		pElement->SetAttribute("Size", x.size);
		pElement->SetAttribute("Modified", date);

		item->InsertEndChild(pElement);

	}
	xmlDoc.SaveFile(fileName.c_str());

}

int Directory::searchbyName(string name)
{

	for (unsigned i = 0; i < subDir.size(); i++) {
		if (subDir[i].name == name) {
			return i;
		}
	}
	return -1;
}



Directory::Directory()
{
}


Directory::~Directory()
{
}
