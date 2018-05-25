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

#include "basic.h"
#include "watcher.hpp"

using namespace std;
using namespace boost::filesystem;

/*recursive_copy and recursive_delete functions use boost*/

/*void recursive_copy(path src, path dst)
{
  if (is_directory(src))
  {
    create_directories(dst);
    for (directory_entry& item : directory_iterator(src))
    {
      recursive_copy(item.path(), dst / item.path().filename());
    }
  } else if (is_regular_file(src))
  {
    copy(src, dst);
  } else
  {
    throw runtime_error(dst.generic_string() + " not dir or file");
  }
}

void recursive_delete(path dst)
{
  cout << "Deleting : " << endl;
  for (directory_entry& item : directory_iterator(dst))
  {
    cout << item.path() << endl;
    if (exists(item.path()))
    {
      if (is_directory(item.path()))
      {
        remove_all(item.path());
      } else if (is_regular_file(item.path()))
      {
        remove(item.path());
      }
    }
  }
}*/

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  string filename, line;
  string str[3];

  cout << "Enter the file name with ext: " << endl;
  cin >> filename;

  std::ifstream file(filename);  //неоднозначное определение, поэтому std::

  if (file.is_open()) {
    while (file.good()) {
      for (size_t i = 0; i < sizeof(str) / sizeof(string); i++) {
        getline(file, line);  //считываем файл построчно и строку записываем в
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

  //  recursive_delete(dst);
  //  recursive_copy(src, dst);
  Basic basic;
  basic.recursive_delete(watcher.dst);
  basic.recursive_copy(watcher.src, watcher.dst);

  /*Watch for changes in source dir*/

  QFileSystemWatcher qfsw;

  QObject::connect(&qfsw, &QFileSystemWatcher::directoryChanged, &watcher,
                   &Watcher::sdirChange);
  QObject::connect(&qfsw, &QFileSystemWatcher::fileChanged, &watcher,
                   &Watcher::sfileChange);

  qfsw.addPath((canonical(watcher.src).string()).c_str());
  for (auto &i : qfsw.directories()) {
    std::cout << "Watching for: " << i.toStdString() << std::endl;
  }

  /*removal through sec seconds*/

  QTimer timer;
  QObject::connect(&timer, &QTimer::timeout, &watcher, &Watcher::timersSlot);
  timer.setInterval(10 * 1000);
  timer.start();

  return app.exec();
}
