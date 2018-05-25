#pragma once

#include "boost/filesystem.hpp"

using namespace boost::filesystem;

class Basic
{

public:
    void recursive_copy(path src, path dst);
    void recursive_delete(path dst);
};
