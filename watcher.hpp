#pragma once

#include <QFileSystemWatcher>
#include <QMap>
#include <QObject>

#include <boost/filesystem.hpp>

using namespace boost::filesystem;

class Watcher : public QObject
{
  Q_OBJECT

public:
  explicit Watcher(QObject* parent = Q_NULLPTR);

  void addWatchPath(QString path);
  void recursive_copy(path src, path dst);
  void recursive_delete(path dst);

  int ttl;
  boost::filesystem::path dst;
  boost::filesystem::path src;

public slots:
  void sdirChange(const QString& dir);
  void timersSlot();

private:
  QMap<QString, QStringList> _currContents;
  QFileSystemWatcher _sysWatcher;
  inline void endOfttl();
};
