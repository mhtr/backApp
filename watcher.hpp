#pragma once

#include <QFileSystemWatcher>
#include <QMap>
#include <QObject>

#include <filesystem>

using namespace std::filesystem;

// using namespace std::filesystem;

class Watcher : public QObject
{
    Q_OBJECT

public:
    explicit Watcher(QObject *parent = Q_NULLPTR);

    void addWatchPath(QString path);
    void recursive_copy(path src, path dst);
    void recursive_delete(path dst);

    int ttl;
    path dst;
    path src;

    public
slots:
    void sdirChange(const QString &dir);
    void timersSlot();

private:
    QMap<QString, QStringList> _currContents;
    QFileSystemWatcher _sysWatcher;
    inline void endOfttl();
};
