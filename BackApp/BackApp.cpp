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

void recursive_copy(const path &src, const path &dst)
{
	if (exists(dst)) {
		throw runtime_error(dst.generic_string() + " exists");
	}

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


int main()
{
	string filename, line, dst, src, ttl;
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

	src = str[1].substr(str[1].find('=') + 1, str[1].size() - 1);
	dst = str[0].substr(str[0].find('=') + 1, str[0].size() - 1);
	ttl = str[2].substr(str[2].find('=') + 1, str[2].size() - 1);

	path dst_p(str[0].substr(str[0].find('=') + 1, str[0].size() - 1));
	path src_p(str[1].substr(str[1].find('=') + 1, str[1].size() - 1));

	cout << "Source: " << src_p << endl;
	cout << "Destination: " << dst_p << endl;

	recursive_copy(src_p, dst_p);
	return 0;
}

