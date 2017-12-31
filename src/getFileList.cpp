
#include <boost/filesystem.hpp>
#include "getFileList.h"

using namespace std;

std::vector<std::string> getFileList(const std::string& path)
{
    vector<string> m_file_list;
    if (!path.empty())
    {
        namespace fs = boost::filesystem;

        fs::path apk_path(path);
        fs::recursive_directory_iterator end;


        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            const fs::path cp = (*i);
            m_file_list.push_back(cp.string());
        }
    }
    return m_file_list;
}
