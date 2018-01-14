#include "AFMySQLPlugin.h"
#include "AFCMySQLModule.h"
//#include "AFCAsycMySQLModule.h"
#include "SDK/Interface/AFIPluginManager.h"

#ifdef ARK_DYNAMIC_PLUGIN

ARK_EXPORT void DllStartPlugin(AFIPluginManager* pm)
{
    CREATE_PLUGIN(pm, AFMySQLPlugin)

};

ARK_EXPORT void DllStopPlugin(AFIPluginManager* pm)
{
    DESTROY_PLUGIN(pm, AFMySQLPlugin)
};

#endif


//////////////////////////////////////////////////////////////////////////

int AFMySQLPlugin::GetPluginVersion()
{
    return 0;
}

const std::string AFMySQLPlugin::GetPluginName()
{
    return GET_CLASS_NAME(AFMySQLPlugin)
}

void AFMySQLPlugin::Install()
{
    REGISTER_MODULE(pPluginManager, AFIMySQLModule, AFCMySQLModule)
    //REGISTER_MODULE(pPluginManager, NFIAsyMysqlModule, NFCAsyMysqlModule)
}

void AFMySQLPlugin::Uninstall()
{
    //UNREGISTER_MODULE(pPluginManager, NFIAsyMysqlModule, NFCAsyMysqlModule)
    UNREGISTER_MODULE(pPluginManager, AFIMySQLModule, AFCMySQLModule)
}