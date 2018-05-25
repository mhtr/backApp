#include <iostream>

#include "watcher.hpp"

Watcher::Watcher(QObject *parent) : QObject(parent), ttl(0), dst(""), src("") {}

void Watcher::sdirChange(const QString &dir) { std::cout << dir.toStdString(); }

void Watcher::sfileChange(const QString &file) {
  std::cout << file.toStdString();
}

void Watcher::timersSlot() {
  std::cout << "Timer\n";
  recursive_delete(dst);
}
