#include <QDebug>
#include <QDir>
#include <QTimer>

#include <codecvt>
#include <iostream>
#include <filesystem>

#include "watcher.hpp"

using namespace std::filesystem;

Watcher::Watcher(path src, path dst, int ttl, QObject *parent) : QObject(parent), ttl(ttl), dst(dst), src(src)
{
    connect(&_sysWatcher, &QFileSystemWatcher::directoryChanged, this,
            &Watcher::sdirChange);
}

void Watcher::sdirChange(const QString &path)
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

    // New File/Dir Added to Dir
    if (!newFile.isEmpty())
    {
        qDebug() << "New Files/Dirs added: " << newFile;

        foreach (QString file, newFile)
        {
            // Handle Operation on new files.....
            std::filesystem::path from = path.toStdWString() / file.toStdWString();
            _sysWatcher.addPath(QString::fromStdWString(from.wstring()));

            auto to = from.wstring();
            to.replace(0, src.wstring().size(), dst.wstring());

            recursive_copy(from, to);
        }



    }
}

void Watcher::timersSlot()
{
    recursive_delete(dst);
    std::cout << "TTL is out\n" << std::endl;
    std::exit(0);
}

void Watcher::addWatchPath(QString path)
{
    qDebug() << "Add to watch: " << path;

    _sysWatcher.addPath(path);

    for (auto &item : recursive_directory_iterator(path.toStdWString()))
    {
        _sysWatcher.addPath(QString::fromStdWString(item.path().wstring()));
    }

    // Save the list of current contents if the added path is a directory

    QFileInfo f(path);

    if (f.isDir())
    {
        const QDir dirw(path);
        _currContents[path] = dirw.entryList(
                                  QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    }
}

void Watcher::recursive_copy(path src, path dst)
{

    if (is_directory(src))
    {
        create_directories(dst);

        for (auto &from : recursive_directory_iterator(src))
        {
            auto to = from.path().string();
            to.replace(0, src.wstring().size(), dst.string());

            if (is_directory(from))
            {
                create_directories(to);
            }
            else
            {
                copy_file(from, to);
            }
        }
    }
    else
    {
        copy_file(src, dst);
    }
}


void Watcher::recursive_delete(path dst)
{
    std::cout << "Deleting : " << std::endl;

    for (auto &item : directory_iterator(dst))
    {
        std::cout << item.path() << std::endl;

        if (exists(item.path()))
        {
            if (is_directory(item.path()))
            {
                remove_all(item.path());
            }
            else if (is_regular_file(item.path()))
            {
                remove(item.path());
            }
        }
    }
}
