#pragma once

#include "config.hpp"
#include <QString>

class Custom : public Config
{
public:
    Custom(QString filename);
    std::filesystem::path  parseDst() const override;
    std::filesystem::path  parseSrc() const override;
    int parseTtl() const override;

private:
    QString filename;
};
