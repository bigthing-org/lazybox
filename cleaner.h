#ifndef CLEANER_H
#define CLEANER_H

#include "commonheader.h"
#include "plugin.h"
#include "config.h"
#include "utils.h"

// 该文件实现清理模块



// 记录文件信息
class QCleanFileInfo : QFile
{
public:
    int cleantype;      // 清理类型
    bool deletef;       // 是否默认删除
    QString info;   // 文件信息

public:
    QCleanFileInfo(const QFile& file, bool deletef, int cleantype, const QString& info)
        : QFile(file), deletef(deletef), cleantype(cleantype), info(info) {}
};

// 清理者基类
class QCleaner : LazyBoxPlugin
{
public:
    QCleaner() { }

    // 安装插件
    virtual bool Init() { this->description = "系统垃圾"; }

    // 卸载插件
    virtual void Uninit() {}

    // 插件id
    virtual void GetPluginId() {}

    // 实现该类型扫描
    virtual void Analysis() = 0;

    // 实现删除
    virtual void Delete() { }

    qint64 GetTotalSize()
    {
        return totalsize;
    }

    QString GetDescription()
    {
        return description;
    }

private:
    QVector<QCleanFileInfo>& targets;
    bool inited;
    QString description;// 说明
    qint64 totalsize;// 总空间大小
    qint64 removesize;// 成功清理的的空间大小

private:
    // 从配置中获取所有匹配目录
    virtual void GetDirsFromConfig(cJSON* rootjson, std::vector<std::string>& output)
    {
        std::vector<std::string> roots;
        std::vector<std::string> relatives;
        {
            std::string type;
            std::string relative;
            if (cJSON_HasObjectItem(rootjson, "type"))
            {
                type = cJSON_GetObjectItem(rootjson, "type")->valuestring;
            }
            if (cJSON_HasObjectItem(rootjson, "relative"))
            {
                relative = cJSON_GetObjectItem(rootjson, "relative")->valuestring;
            }
            if (cJSON_HasObjectItem(rootjson, "root"))
            {
                // root = cJSON_GetObjectItem(rootjson, "root")->valueint;
            }

            // 解析根路径
            if (type == "sysdir")
            {
#if defined(Q_OS_WIN)
                std::string locatestr = cJSON_GetObjectItem(rootjson, "locate")->valuestring;
                if (locatestr == "windowsdir") //
                {
                    char path[MAX_PATH];
                    if (0 != GetWindowsDirectoryA(path, MAX_PATH))
                    {
                        roots.push_back(std::string(path));
                    }

                }
                else if (locatestr == "systemdir") //
                {
                    char path[MAX_PATH];
                    if (0 != GetWindowsDirectoryA(path, MAX_PATH))
                    {
                        std::string tpath = path;
                        roots.push_back(std::string(tpath + "/" + "System32"));
                        roots.push_back(std::string(tpath + "/" + "SysWOW64"));
                    }
                }
#endif
            }
#if defined(Q_OS_WIN)
            else if (type == "csidl")
            {
                // 获取所有用户路径
                std::string path_for_current_user = Utils::GetWinDir(cJSON_GetObjectItem(rootjson, "locate")->valueint);
                std::string curuser = "\\" + Utils::GetCurrentUserName(); // 防止误匹配
                std::string::size_type p = path_for_current_user.find(curuser);
                if (curuser != "\\" && p != std::string::npos)
                {
                    for (const std::string& user : Utils::GetAllUsersNames())
                    {
                        std::string t = path_for_current_user;
                        roots.push_back(t.replace(p + 1, curuser.length() - 1, user));
                    }
                }
            }
            else if (type == "alldrive")
            {
                std::vector<std::string> localdrives = Utils::GetLocalDrives();
                roots.insert(roots.end(), localdrives.begin(), localdrives.end());
            }
#endif
            else if (type == "env")
            {
                char* env = getenv((const char*)cJSON_GetObjectItem(rootjson, "locate")->valuestring);
                if (env != 0)
                {
                    std::string envstr = env;
                    std::vector<std::string> tp;
#if defined(Q_OS_WIN)
                    if (envstr.find(';') != std::string::npos)
                    {
                        Utils::SplitString(envstr, tp, ";");
                    }
                    else
                    {
                        tp.push_back(envstr);
                    }
#else
                    if(envstr.find(":") != -1)
                    {
                        Utils::SplitString(envstr, tp, ":");
                    }
                    else
                    {
                        tp.push_back(envstr);
                    }
#endif
                    for (std::string& t : tp)
                    {
                        if (*t.rbegin() == '/' || *t.rbegin() == '\\')
                        {
                            t.pop_back();
                        }
                    }
                    roots.insert(roots.end(), tp.begin(), tp.end());
                }
            }

            // 解析相对路径
            if (relative.find('|') != std::string::npos)
            {
                Utils::SplitString(relative, relatives, "|");
            }
            else
            {
                relatives.push_back(relative);
            }
        }
        for (const std::string& ro : roots)
        {
            for (const std::string& re : relatives)
            {
                output.push_back(ro + "/" + re);
            }
        }
    }
};

class QSystemCleaner : public QCleaner
{
public:
    virtual void Analysis()
    {
        /*
        data.clear();
        totalsize = 0;
        removesize = 0;
        // 解析配置文件
        cJSON* cleaner = Config::GetInstance()->GetConfigForType(PLUGIN_TYPE_CLEANER);
        if (cleaner == 0)
        {
            return;
        }

        for (const std::string& target : targets)
        {
            if (!cJSON_HasObjectItem(cleaner, target.c_str()))
            {
                continue;
            }
            cJSON* cleaner_target = cJSON_GetObjectItem(cleaner, target.c_str());
#if defined(Q_OS_WIN)
            const char* os = "win";
#else
            const char* os = "os";
#endif
            if (!cJSON_HasObjectItem(cleaner_target, os))
            {
                continue;
            }
            cJSON* cleaner_target_for_os = cJSON_GetObjectItem(cleaner_target, os);
            int itemnum = cJSON_GetArraySize(cleaner_target_for_os);
            for (int i = 0; i < itemnum; i++)
            {
                cJSON* item = cJSON_GetArrayItem(cleaner_target_for_os, i);
                if (item == 0)
                {
                    continue;
                }
                std::string filter = ".*";
                std::string describ = "";
                if (cJSON_HasObjectItem(item, "filter"))
                {
                    filter = cJSON_GetObjectItem(item, "filter")->valuestring;
                }
                if (cJSON_HasObjectItem(item, "describ"))
                {
                    describ = cJSON_GetObjectItem(item, "describ")->valuestring;
                }
                std::vector<std::string> targetdirs;
                GetDirsFromConfig(item, targetdirs);
                std::vector<std::string> targetfiles;
                for (const std::string& targetdir : targetdirs)
                {
#ifdef DEBUG
                    printf("%s\n", targetdir.c_str());
#endif
                    Utils::ListFiles(targetdir, targetfiles, filter, false);
                    // Find all files / folders / subfolders
                    for (std::string& targetfile : targetfiles)
                    {
                        FileInfo fi;
                        fi.path = targetfile;
                        fi.deletef = true;
                        fi.cleantype = CLEAN_TYPE_SYSTEMP;
                        fi.size = Utils::GetFileSize(targetfile);
                        fi.info = describ;
                        totalsize += fi.size;
                        data.push_back(fi);
                    }
                }
            }
        }
        */
    }
};

#endif // CLEANER_H
