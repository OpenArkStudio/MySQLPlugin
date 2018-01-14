#ifdef _MSC_VER
#include <WinSock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#endif

#include "AFCMysqlDriver.h"
#include "SDK/Base/AFIDataList.h"
#include "AFCMysqlDriverManager.h"

AFIMySQLDriver* AFCMySQLDriverManager::GetMySQLDriver()
{
    return mxMySQLs.First(); // ÔÝÊ±ÏÈ¸øfirst
}

void AFCMySQLDriverManager::CheckMySQL()
{
    //////////////////////////////////////////////////////////////////////////
    int nServerID = 0;
    std::vector<int> xIntVec;
    for (AFIMySQLDriver* pMysqlDriver = mxMySQLs.First(nServerID); pMysqlDriver != nullptr; pMysqlDriver = mxMySQLs.Next(nServerID))
    {
        if (!pMysqlDriver->Enable())
        {
            xIntVec.push_back(nServerID);
            mxInvalidMySQLs.AddElement(nServerID, pMysqlDriver);
        }
    }

    for (int i = 0; i < xIntVec.size(); ++i)
    {
        mxMySQLs.RemoveElement(xIntVec[i]);
    }
    //////////////////////////////////////////////////////////////////////////
    xIntVec.clear();
    nServerID = 0;

    for (AFIMySQLDriver* pMysqlDriver = mxInvalidMySQLs.First(nServerID); pMysqlDriver != nullptr; pMysqlDriver = mxInvalidMySQLs.Next(nServerID))
    {
        if (!pMysqlDriver->Enable() && pMysqlDriver->CanReconnect())
        {
            pMysqlDriver->Reconnect();
            if (pMysqlDriver->Enable())
            {
                xIntVec.push_back(nServerID);
                mxMySQLs.AddElement(nServerID, pMysqlDriver);
            }
        }
    }

    for (int i = 0; i < xIntVec.size(); ++i)
    {
        mxInvalidMySQLs.RemoveElement(xIntVec[i]);
    }

}
bool AFCMySQLDriverManager::AddMySQLServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
    std::string  strDnsIp;
    if (!strDns.empty())
    {
        strDnsIp = GetIPByHostName(strDns);
    }

    if (strDnsIp.empty())
    {
        strDnsIp = strIP;
    }

    if (strDnsIp.empty())
    {
        return false;
    }


    if (strDnsIp.empty())
    {
        return false;
    }

    AFIMySQLDriver* pMysqlDriver = mxMySQLs.GetElement(nServerID);
    if (pMysqlDriver)
    {
        return false;
    }

    AFIMySQLDriver* pInvalidRedisDriver = mxInvalidMySQLs.GetElement(nServerID);
    if (pInvalidRedisDriver)
    {
        return false;
    }


    pMysqlDriver = ARK_NEW AFCMySQLDriver(nRconnectTime, nRconneCount);
    if (pMysqlDriver->Connect(strDBName, strDnsIp, nPort, strDBUser, strDBPwd))
    {
        mxMySQLs.AddElement(nServerID, pMysqlDriver);
    }
    else
    {
        mxInvalidMySQLs.AddElement(nServerID, pMysqlDriver);
    }

    return true;
}

std::string AFCMySQLDriverManager::GetIPByHostName(const std::string& strHostName)
{
    if (strHostName.empty())
    {
        return NULL_STR;
    }

    hostent* pHost = gethostbyname(strHostName.c_str());
    if (pHost == NULL)
    {
        return NULL_STR;
    }

    if (pHost->h_addr_list[0] != NULL)
    {
        char strIp[32] = {0};
        inet_ntop(pHost->h_addrtype, pHost->h_addr_list[0], strIp, sizeof(strIp));
        return std::string(strIp);
    }

    return NULL_STR;
}
