#include <QtCore/QCoreApplication>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "watcher.hpp"

using namespace std;
using namespace boost::filesystem;

int
main(int argc, char* argv[])
{
  setlocale(LC_ALL, "");
  QCoreApplication app(argc, argv);
  string filename, line;
  string str[3];

  //  cout << "Enter the file name with ext: " << endl;
  //  cin >> filename;
  filename = "config.txt";

  std::ifstream file(filename); //неоднозначное определение, поэтому std::

  if (file.is_open()) {
    while (file.good()) {
      for (size_t i = 0; i < sizeof(str) / sizeof(string); i++) {
        getline(file, line); //считываем файл построчно и строку записываем в
                             //переменную line
        str[i] = line;
      }
    }
    file.close();
  } else {
    cout << "Unable to open file\n";
    std::terminate();
  }

  Watcher watcher;
  watcher.ttl =
    atoi((str[2].substr(str[2].find('=') + 1, str[2].size() - 1)).c_str());
  watcher.dst = str[0].substr(str[0].find('=') + 1, str[0].size() - 1);
  watcher.src = str[1].substr(str[1].find('=') + 1, str[1].size() - 1);

  cout << "Source: " << watcher.src << endl;
  cout << "Destination: " << watcher.dst << endl;
  cout << "TTL: " << watcher.ttl << " sec" << endl;

  watcher.recursive_delete(watcher.dst);
  watcher.recursive_copy(watcher.src, watcher.dst);

  watcher.addWatchPath(QString::fromStdWString(watcher.src.wstring()));

  QTimer::singleShot(watcher.ttl * 1000, &watcher, &Watcher::timersSlot);
  return app.exec();
}
