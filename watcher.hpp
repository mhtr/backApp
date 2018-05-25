#pragma once

#include <QObject>

#include <boost/filesystem.hpp>

#include "basic.h"

class Watcher : public QObject, private Basic {
  Q_OBJECT

 public:
  explicit Watcher(QObject* parent = Q_NULLPTR);

  int ttl;
  boost::filesystem::path dst;
  boost::filesystem::path src;

 public slots:
  void sdirChange(const QString& dir);
  void sfileChange(const QString& file);
  void timersSlot();
};
