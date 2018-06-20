#include <QtCore/QCoreApplication>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QTextStream>
#include <QString>
#include <QFile>

#include <filesystem>
#include <fstream>
#include <iostream>

#include "watcher.hpp"
#include "custom.hpp"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    QCoreApplication app(argc, argv);

    QTextStream stream(stdin);
    QString filename;
    std::cout << "Enter the file name with ext: ";
    stream >> filename;

    Custom custom(filename);
    std::filesystem::path src = custom.parseSrc();
    std::filesystem::path dst = custom.parseDst();
    int ttl = custom.parseTtl();

    Watcher watcher(src, dst, ttl);

    std::cout << "Source: " << src << std::endl;
    std::cout << "Destination: " << dst << std::endl;
    std::cout << "TTL: " << ttl << " sec" << std::endl;

    watcher.recursive_delete(dst);
    watcher.recursive_copy(src, dst);

    watcher.addWatchPath(QString::fromStdWString(src.wstring()));

    QTimer::singleShot(ttl * 1000, &watcher, &Watcher::timersSlot);
    return app.exec();
}
