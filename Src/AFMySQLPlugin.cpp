#include "AFMySQLPlugin.h"
#include "AFCMySQLModule.h"
//#include "AFCAsycMySQLModule.h"
#include "SDK/Interface/AFIPluginManager.h"

#ifdef ARK_DYNAMIC_PLUGIN

ARK_DLL_PLUGIN_ENTRY(AFMySQLPlugin)
ARK_DLL_PLUGIN_EXIT(AFMySQLPlugin)

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