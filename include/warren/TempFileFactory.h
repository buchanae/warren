#ifndef WARREN_TEMPFILEFACTORY_H
#define WARREN_TEMPFILEFACTORY_H

#include <string>
#include <vector>

using std::string;
using std::vector;

#define DEFAULT_TMP_DIR "/tmp"
#define DEFAULT_TMP_PREFIX "Warren"

struct TempFileFactory
{
    string TMP_DIR;
    string PREFIX;

    TempFileFactory (void)
    {
        TMP_DIR = DEFAULT_TMP_DIR;
        PREFIX = DEFAULT_TMP_PREFIX;
    }

    bool make (string& path)
    {
        // open temp. file
        path = TMP_DIR + "/" + PREFIX + "-XXXXXX";
        vector<char> dst_path(path.begin(), path.end());
        dst_path.push_back('\0');

        int fd = mkstemp(&dst_path[0]);
        if (fd != -1)
        {
            path.assign(dst_path.begin(), dst_path.end() - 1);
            close(fd);
            return true;
        }
        else return false;
    }
};

#endif
