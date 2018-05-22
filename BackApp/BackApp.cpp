// BackApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
#include <tchar.h>
#include <boost\filesystem.hpp>
#include <boost\system\error_code.hpp>
#include <QtCore/qfilesystemwatcher.h>
#include <QtCore/qobject.h>
#include <QtCore/qstring.h>

using namespace std;
using namespace boost::filesystem;

class Watcher : public QObject
{
	Q_OBJECT

public slots:
	void sdirChange(const QString &dir) {
		cout << "Changed\n";
	}
	void sfileChange(const QString &file) {
		cout << "Changed\n";
	}

};

void watch(path src) {
	Watcher watcher;
	QFileSystemWatcher qfsw;

	QObject::connect(&qfsw, &QFileSystemWatcher::directoryChanged, &watcher,
		&Watcher::sdirChange);
	QObject::connect(&qfsw, &QFileSystemWatcher::fileChanged, &watcher,
		&Watcher::sfileChange);

	qfsw.addPath((canonical(src).string()).c_str());
}


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

void recursive_delete(path dst) {
	cout << "Deleting : " << endl;
	for (directory_entry& item : directory_iterator(dst)) {
		cout << item.path() << endl;
		if (exists(item.path())) {
			if (is_directory(item.path())) {
				remove_all(item.path());
			}
			else if (is_regular_file(item.path())) {
				remove(item.path());
			}
		}
	}
}


int main()
{
	string filename, line;
	int ttl;
	string str[3];


	cout << "Enter the file name with ext: " << endl;
	cin >> filename;

	std::ifstream file(filename);  //неоднозначное определение, поэтому std::

	if (file.is_open())
	{
		while (file.good())
		{
			for (int i = 0; i < sizeof(str) / sizeof(string); i++)
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

	//recursive_delete(dst);
	recursive_copy(src, dst);
	recursive_delete(dst);
	//watch(src);

	

	system("pause");
	return 0;
}

