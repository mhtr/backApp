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
//#include <string>

#include "watcher.hpp"

using namespace std;
using namespace std::filesystem;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    QCoreApplication app(argc, argv);

    QTextStream stream(stdin);
    QString filename;
    std::cout << "Enter the file name with ext: ";
    stream >> filename;
//    filename = "conf.txt";

    Watcher watcher;

    QFile file(filename);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        stream.setDevice(&file);

        while (!stream.atEnd())
        {
            QString line = stream.readLine().trimmed();

            if (line.startsWith("dst="))
            {
                watcher.dst = line.remove(0, 4).toStdWString();
            }
            else if (line.startsWith("src="))
            {
                watcher.src = line.remove(0, 4).toStdWString();
            }
            else if (line.startsWith("ttl="))
            {
                watcher.ttl = line.remove(0, 4).toInt();
            }
        }

        file.close();
    }
    else
    {
        std::cout << "Unable to open file!" << std::endl;
        std::exit(-1);
    }

    cout << "Source: " << watcher.src << endl;
    cout << "Destination: " << watcher.dst << endl;
    cout << "TTL: " << watcher.ttl << " sec" << endl;

    watcher.recursive_delete(watcher.dst);
    watcher.recursive_copy(watcher.src, watcher.dst);

    watcher.addWatchPath(QString::fromStdWString(watcher.src.wstring()));

    QTimer::singleShot(watcher.ttl * 1000, &watcher, &Watcher::timersSlot);
    return app.exec();
}
