#ifndef PLUGIN_H
#define PLUGIN_H



// 所有插件的基类
class LazyBoxPlugin {
public:
    virtual bool Init() = 0;
    virtual void Uninit() = 0;
    virtual void GetPluginId() = 0;
};

#endif // PLUGIN_H
