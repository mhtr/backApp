// BackApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <tchar.h>
#include <boost\filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

void recursive_copy(path src, path dst)
{
	if (is_directory(src)) {
		create_directories(dst);
		for (directory_entry& item : directory_iterator(src)) {
			recursive_copy(item.path(), dst / item.path().filename());
		}
	}
	else if (is_regular_file(src)) {
		copy(src, dst);
	}
	else {
		throw runtime_error(dst.generic_string() + " not dir or file");
	}
}

void recursive_delete(path dst)
{
	if (is_directory(dst)) {
		/*for (directory_entry& item : directory_iterator(dst)) {
			recursive_delete(dst / item.path().filename());
		}*/
		boost::filesystem::remove(dst);
	}
	else if (is_regular_file(dst)) {
		remove(dst);
	}
	else {
		throw runtime_error(dst.generic_string() + " not dir or file");
	}
}

int main()
{
	string filename, line;
	int ttl;
	string str[3];
	
	cout << "Enter the file name with ext: " << endl;
	cin >> filename;

	std::ifstream file (filename);  //неоднозначное определение, поэтому std::

	if (file.is_open())
	{
		while (file.good())
		{
			for (int i = 0; i < sizeof(str)/sizeof(string); i++)
			{
				getline(file, line);	//считываем файл построчно и строку записываем в переменную line
				str[i] = line;
			}
		}
		file.close();
	}
	else cout << "Unable to open file";

	ttl = atoi((str[2].substr(str[2].find('=') + 1, str[2].size() - 1)).c_str());
	path dst(str[0].substr(str[0].find('=') + 1, str[0].size() - 1));
	path src(str[1].substr(str[1].find('=') + 1, str[1].size() - 1));

	cout << "Source: " << src << endl;
	cout << "Destination: " << dst << endl;
	cout << "TTL: " << ttl << " sec" << endl;
	
	recursive_delete(dst);
	recursive_copy(src, dst);
	for (int i = 1; i <= ttl; i++)
	{
		
		boost::detail::Sleep(1000);
		cout << ".";
		
	}

	system("pause");
	return 0;
}

