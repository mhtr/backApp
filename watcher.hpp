#pragma once

#include <QFileSystemWatcher>
#include <QMap>
#include <QObject>

#include <filesystem>

class Watcher : public QObject
{
    Q_OBJECT

public:
    Watcher(std::filesystem::path src, std::filesystem::path dst, int ttl, QObject *parent = Q_NULLPTR);
    virtual ~Watcher() = default;

    void addWatchPath(QString path);
    void recursive_copy(std::filesystem::path src, std::filesystem::path dst);
    void recursive_delete(std::filesystem::path dst);



    public
slots:
    void sdirChange(const QString &dir);
    void timersSlot();

private:

    int ttl;
    std::filesystem::path dst;
    std::filesystem::path src;

    QMap<QString, QStringList> _currContents;
    QFileSystemWatcher _sysWatcher;
    inline void endOfttl();
};
