// BackApp.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	string filename, line;
	//char dst, src, ttl;
	string str[3];
	
	cout << "Enter the file name with ext: " << endl;
	cin >> filename;

	ifstream file (filename);

	if (file.is_open())
	{
		while (file.good())
		{
			for (int i = 0; i < sizeof(str)/sizeof(string); i++)
			{
				getline(file, line);
				str[i] = line;
				cout << str[i] << endl;
			}
			
			
		}
		file.close();
	}


	else cout << "Unable to open file";
	cout << str[2];
	cin;

	return 0;
}

