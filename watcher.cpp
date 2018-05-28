#include <QDebug>
#include <QDir>
#include <QTimer>

#include <codecvt>
#include <iostream>

#include <boost/exception/all.hpp>

#include "watcher.hpp"

Watcher::Watcher(QObject* parent)
  : QObject(parent)
  , ttl(0)
  , dst("")
  , src("")
{
  connect(&_sysWatcher,
          &QFileSystemWatcher::directoryChanged,
          this,
          &Watcher::sdirChange);
}

void
Watcher::sdirChange(const QString& path)
{
  qDebug() << "Directory updated: " << path;

  // Compare the latest contents to saved contents for the dir updated to find
  // out the difference(change)

  QStringList currEntryList = _currContents[path];
  const QDir dir(path);

  QStringList newEntryList = dir.entryList(
    QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

  QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);

  QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

  // Files that have been added
  QSet<QString> newFiles = newDirSet - currentDirSet;
  QStringList newFile = newFiles.toList();

  // Files that have been removed
  QSet<QString> deletedFiles = currentDirSet - newDirSet;
  QStringList deleteFile = deletedFiles.toList();

  // Update the current set
  _currContents[path] = newEntryList;

  if (!newFile.isEmpty() && !deleteFile.isEmpty()) {
    // File/Dir is renamed

    if (newFile.count() == 1 && deleteFile.count() == 1) {
      qDebug() << "File Renamed from " << newFile.first() << " to "
               << deleteFile.first();
    }
  }

  else {
    // New File/Dir Added to Dir
    if (!newFile.isEmpty()) {
      qDebug() << "New Files/Dirs added: " << newFile;

      foreach (QString file, newFile) {
        // Handle Operation on new files.....
        QString from = QString("%1\\%2").arg(path).arg(file);
        _sysWatcher.addPath(from);

        auto to = from.toStdWString();
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        to.replace(0, src.size(), converter.from_bytes(dst.string()));

        recursive_copy(from.toStdWString(), to);
      }
    }

    // File/Dir is deleted from Dir

    if (!deleteFile.isEmpty()) {
      qDebug() << "Files/Dirs deleted: " << deleteFile;
      foreach (QString file, deleteFile) {
        // Handle operation of each deleted file....
      }
    }
  }
}

void
Watcher::timersSlot()
{
  recursive_delete(dst);
  std::cout << "TTL is out\n" << std::endl;
  std::exit(0);
}

void
Watcher::addWatchPath(QString path)
{
  qDebug() << "Add to watch: " << path;

  _sysWatcher.addPath(path);
  for (auto& item : recursive_directory_iterator(path.toStdWString())) {
    _sysWatcher.addPath(QString::fromStdWString(item.path().wstring()));
  }

  // Save the list of current contents if the added path is a directory

  QFileInfo f(path);

  if (f.isDir()) {
    const QDir dirw(path);
    _currContents[path] = dirw.entryList(
      QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
  }
}

/* to do :
 * add TTL for each file
void
Watcher::endOfttl()
{
  QTimer::singleShot(ttl * 1000, this, &Watcher::timersSlot);
}*/

void
Watcher::recursive_copy(path src, path dst)
{
  try {
    if (is_directory(src)) {
      create_directories(dst);
      for (auto& from : recursive_directory_iterator(src)) {
        auto to = from.path().string();
        to.replace(0, src.size(), dst.string());
        if (is_directory(from)) {
          create_directories(to);
        } else {
          copy_file(from, to);
        }
      }
    } else {
      copy_file(src, dst);
    }
  } catch (...) {
    std::cout << boost::current_exception_diagnostic_information() << std::endl;
  }
}

void
Watcher::recursive_delete(path dst)
{
  std::cout << "Deleting : " << std::endl;
  for (directory_entry& item : directory_iterator(dst)) {
    std::cout << item.path() << std::endl;
    if (exists(item.path())) {
      if (is_directory(item.path())) {
        remove_all(item.path());
      } else if (is_regular_file(item.path())) {
        remove(item.path());
      }
    }
  }
}
