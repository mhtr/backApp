#pragma once

#include <filesystem>

class Config
{
public:
    virtual ~Config() = default;

    virtual std::filesystem::path parseDst() const = 0;
    virtual std::filesystem::path parseSrc() const = 0;
    virtual int parseTtl() const = 0;
};
