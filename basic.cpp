#include <iostream>

#include "basic.h"

using namespace std;

void Basic::recursive_copy(path src, path dst)
{
    if (is_directory(src))
    {
      create_directories(dst);
      for (directory_entry& item : directory_iterator(src))
      {
        recursive_copy(item.path(), dst / item.path().filename());
      }
    } else if (is_regular_file(src))
    {
      copy(src, dst);
    } else
    {
      throw runtime_error(dst.generic_string() + " not dir or file");
    }
}

void Basic::recursive_delete(path dst)
{
    cout << "Deleting : " << endl;
    for (directory_entry& item : directory_iterator(dst))
    {
      cout << item.path() << endl;
      if (exists(item.path()))
      {
        if (is_directory(item.path()))
        {
          remove_all(item.path());
        } else if (is_regular_file(item.path()))
        {
          remove(item.path());
        }
      }
    }
}
