#ifndef CONFIG_H
#define CONFIG_H

#include "utils.h"

class Config
{
public:
    static Config* GetInstance()
    {
        static Config conf;
        return &conf;
    }

    QJsonObject GetConfigForType(int type)
    {
        QJsonDocument d = QJsonDocument::fromJson(config.toUtf8());
        QJsonObject root = d.object();
        switch (type)
        {
            case PLUGIN_TYPE_CLEANER:
            {
                if (root.contains("cleaner"))
                {
                    return root.value("cleaner").toObject();
                }
            }
        }
        return QJsonObject();
    }

    void Update()
    {
        QFile configfile("config.json");
        if (!configfile.exists())
        {
            configfile.setFileName("../config.json");
        }
        if (!configfile.exists())
        {
            return;
        }
        configfile.open(QIODevice::ReadOnly | QIODevice::Text);
        config = configfile.readAll();
        configfile.close();
    }

private:
    Config(const Config&);
    Config& operator= (const Config&);
    Config()
    {
        Update();
    }

private:
    QString config;
};

#endif // CONFIG_H
