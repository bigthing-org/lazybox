#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "../commonheader.h"
#include "../cleaner.h"

// 迅雷看看清理
class ThunderCleaner : Cleaner
{
public:
    // 实现该类型扫描
    virtual void Analysis(std::vector<FileInfo>& targets)
    {
        std::vector<std::string> pathset;
        std::string desktop_dir = Utils::GetWinDir(CSIDL_COMMON_DESKTOP);
        std::string cid_store_path;

        // 特殊文件
        cid_store_path = Utils::GetUpDir(desktop_dir) + "\\Thunder Network\\cid_store.dat";
        if (Utils::FileExist(cid_store_path))
        {
            pathset.push_back(cid_store_path);
        }
        cid_store_path = Utils::GetUpDir(desktop_dir) + "\\Thunder Network\\DownloadSDK\\Profiles\\cid_store.dat";
        if (Utils::FileExist(cid_store_path))
        {
            pathset.push_back(cid_store_path);
        }

        // 下载文件
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("D:\\thunder\\Profiles\\TaskDb.dat");
        if (db.open())
        {
            QSqlQuery query;
            query.prepare("select SavePath, Name from TaskBase");
            if (query.exec())
            {
                while (query.next())
                {
                    std::string path = query.value("SavePath").toString().toStdString().c_str();
                    std::string file = query.value("Name").toString().toStdString().c_str();
                    std::string wholepath = path + file;
                    pathset.push_back(wholepath.c_str());
                }
            }
        }
        for (const std::string& p : pathset)
        {
            FileInfo fi(p, true, CLEAN_TYPE_DOWNLOADER, "上传下载记录");
            targets.push_back(fi);
        }
    }

    virtual void Delete(std::string data)
    {
        // 清空TaskDb.dat
        // 清空cid_store.dat
        // 删除下载文件
    }

    virtual std::vector<std::string> GetThunderDir() // 获取所有可找到的迅雷安装目录
    {
        std::vector<std::string> outlist;
#if defined(Q_OS_WIN)
        std::set<std::string> findlist;
        // 搜索本地路径
        for (std::string& drive : Utils::GetLocalDrives())
        {
            const char* suffix[] =
            {
                "\\Program Files\\Thunder Network\\Thunder",
                "\\Program Files\\Thunder Network\\Thunder9",
                "\\Program Files (x86)\\Thunder Network\\Thunder",
                "\\Program Files (x86)\\Thunder Network\\Thunder9"
            };
            for (int i = 0; i < ARRLEN(suffix); i++)
            {
                std::string path = drive + suffix[i];
                if (Utils::FileExist(path))
                {
                    findlist.insert(path);
                }
            }
        }
       // 搜索注册表
        HKEY hkey1;
        RegOpenKeyA(HKEY_CLASSES_ROOT, "thunder\\DefaultIcon", &hkey1);
        if (hkey1 != 0)
        {
            char path[MAX_PATH];
            long size = sizeof(path);
            RegQueryValueA(hkey1, "", path, &size);
            std::string thunderpath = path;
            std::string::size_type pos = thunderpath.find("\\Program\\Thunder.exe");
            if (pos != std::string::npos)
            {
                findlist.insert(thunderpath.substr(0, pos));
            }
        }
        for (const std::string& path : findlist)
        {
            outlist.push_back(path);
        }
#endif
        return outlist;
    }

    virtual std::vector<std::string> GetThunderDownloadDir() // 获取所有可找到的迅雷下载目录
    {
        std::vector<std::string> outlist;
#if defined(Q_OS_WIN)
        std::set<std::string> findlist;

        for (std::string& path : GetThunderDir())
        {
            std::string inipath1 = path + "\\Profiles\\config.ini";
            std::string inipath2 = path + "\\Profiles\\newtask.ini";
            do {
                tinyxml2::XMLDocument doc;
                doc.LoadFile(inipath1.c_str());
                if (doc.Error())
                {
                    break;
                }
                tinyxml2::XMLElement* profile = doc.RootElement();
                if (profile == 0)
                {
                    break;
                }
                tinyxml2::XMLElement* section = profile->FirstChildElement("section");
                while (section != 0)
                {
                    if (!strcmp(section->Attribute("id"), "ReservedSettings"))
                    {
                        tinyxml2::XMLElement* key = section->FirstChildElement("key");
                        while (key != 0)
                        {
                            if (!strcmp(key->Attribute("id"), "InitPath"))
                            {
                                std::string val = key->GetText();
                                if (val != "")
                                {
                                    if (val != "" && val[val.length() - 1] == '\\')
                                    {
                                        val = val.substr(0, val.length() - 1);
                                        findlist.insert(val);
                                    }
                                }
                            }
                            key = key->NextSiblingElement();
                        }
                    }
                    else if (!strcmp(section->Attribute("id"), "TaskDefaultSettings"))
                    {
                        tinyxml2::XMLElement* key = section->FirstChildElement("key");
                        while (key != 0)
                        {
                            if (!strcmp(key->Attribute("id"), "DefaultPath"))
                            {
                                std::string val = key->GetText();
                                if (val != "" && val[val.length() - 1] == '\\')
                                {
                                    val = val.substr(0, val.length() - 1);
                                    findlist.insert(val);
                                }
                            }
                            key = key->NextSiblingElement();
                        }
                    }
                    else if (!strcmp(section->Attribute("id"), "PathAndCategory"))
                    {
                        tinyxml2::XMLElement* key = section->FirstChildElement("key");
                        while (key != 0)
                        {
                            if (!strcmp(key->Attribute("id"), "LastUsedPath"))
                            {
                                std::string val = key->GetText();
                                if (val != "")
                                {
                                    if (val != "" && val[val.length() - 1] == '\\')
                                    {
                                        val = val.substr(0, val.length() - 1);
                                        findlist.insert(val);
                                    }
                                }
                            }
                            key = key->NextSiblingElement();
                        }
                    }
                    section = section->NextSiblingElement();
                }
            } while (false);

            /*
                // config.ini为utf8编码，newtask.ini为gb2312编码
                char DefaultPath[MAX_PATH], LastUsedPath[MAX_PATH];
                GetPrivateProfileStringA("TaskDefaultSettings", "DefaultPath", "", DefaultPath, MAX_PATH, inipath2.c_str());
                findlist.insert(DefaultPath);
                GetPrivateProfileStringA("PathAndCategory", "LastUsedPath", "", LastUsedPath, MAX_PATH, inipath2.c_str());
                findlist.insert(LastUsedPath);
            */
        }

        for (const std::string& path : findlist)
        {// 迅雷路径不能包含系统路径
            if (path != "" && path.find("Windows") == std::string::npos)
            {
                outlist.push_back(path); // 由于编码问题，仍然有重复
            }
        }
#endif
        return outlist;
    }
};

#endif // DOWNLOADER_H
