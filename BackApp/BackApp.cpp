// BackApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	string filename, dst, src, ttl, str;
	
	cout << "Enter the file name with ext: " << endl;
	cin >> filename;

	ifstream file (filename);

	while (getline(file, str)) {
		
		
		// Обработка строки str
		cout << str << endl;
	}

	//cout << filename << endl;
	file.close();
	return 0;
}

