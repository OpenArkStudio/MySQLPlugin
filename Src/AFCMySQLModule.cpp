/*
* This source file is part of ArkGameFrame
* For the latest info, see https://github.com/ArkGame
*
* Copyright (c) 2013-2017 ArkGame authors.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#include "AFCMySQLDriver.h"
#include "AFCMySQLModule.h"
#include "AFCMySQLDriverManager.h"

AFCMySQLModule::AFCMySQLModule(AFIPluginManager* p)
{
    pPluginManager = p;
    mnLastCheckTime = 0;
}

AFCMySQLModule::~AFCMySQLModule()
{

}

bool AFCMySQLModule::Init()
{
    m_pMysqlDriverManager = std::make_shared<AFCMySQLDriverManager>();
    return true;
}

bool AFCMySQLModule::Shut()
{
    return true;
}

bool AFCMySQLModule::PostInit()
{
    return true;
}

void AFCMySQLModule::Update()
{
    if (mnLastCheckTime + 10 > GetPluginManager()->GetNowTime())
    {
        return;
    }

    mnLastCheckTime = GetPluginManager()->GetNowTime();

    if (m_pMysqlDriverManager != nullptr)
    {
        m_pMysqlDriverManager->CheckMySQL();
    }
}

bool AFCMySQLModule::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/)
{
    if (m_pMysqlDriverManager == nullptr)
    {
        return false;
    }

    return m_pMysqlDriverManager->AddMySQLServer(nServerID, strDns, strIP, nPort, strDBName, strDBUser, strDBPwd, nRconnectTime, nRconneCount);
}

bool AFCMySQLModule::Update(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
    AFIMySQLDriver* pDriver = m_pMysqlDriverManager->GetMySQLDriver();
    if (pDriver)
    {
        return pDriver->Update(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool AFCMySQLModule::Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	AFIMySQLDriver* pDriver = m_pMysqlDriverManager->GetMySQLDriver();
    if (pDriver)
    {
        return pDriver->Query(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool AFCMySQLModule::Delete(const std::string& strRecordName, const std::string& strKey)
{
	AFIMySQLDriver* pDriver = m_pMysqlDriverManager->GetMySQLDriver();
    if (pDriver)
    {
        return pDriver->Delete(strRecordName, strKey);
    }

    return false;
}

bool AFCMySQLModule::Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit)
{
	AFIMySQLDriver* pDriver = m_pMysqlDriverManager->GetMySQLDriver();
    if (pDriver)
    {
        return pDriver->Exists(strRecordName, strKey, bExit);
    }

    return false;
}

bool AFCMySQLModule::Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
	AFIMySQLDriver* pDriver = m_pMysqlDriverManager->GetMySQLDriver();
    if (pDriver)
    {
        return pDriver->Select(strRecordName, strKey, fieldVec, valueVec);
    }

    return false;
}

bool AFCMySQLModule::Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec)
{
	AFIMySQLDriver* pDriver = m_pMysqlDriverManager->GetMySQLDriver();
    if (pDriver)
    {
        return pDriver->Keys(strRecordName, strKeyName, valueVec);
    }

    return false;
}