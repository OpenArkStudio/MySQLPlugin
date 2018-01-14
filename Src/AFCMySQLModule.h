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

#pragma once

#include "AFCMysqlDriver.h"
#include "SDK/Base/AFPlatform.hpp"
#include "SDK/Interface/AFIPluginManager.h"
#include "Interface/AFIMySQLModule.h"
#include "Interface/AFIMysqlDriverManager.h"

class AFCMySQLModule : public AFIMySQLModule
{
public:
	AFCMySQLModule(AFIPluginManager* p);
    virtual ~AFCMySQLModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    //////////////////////////////////////////////////////////////////////////
    virtual bool Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
    virtual bool Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
    virtual bool Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);

    virtual bool Delete(const std::string& strRecordName, const std::string& strKey);
    virtual bool Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit);
    virtual bool Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec);
    virtual bool AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime = 10, const int nRconneCount = -1);
private:
    ARK_SHARE_PTR<AFIMySQLDriverManager> m_pMysqlDriverManager;

    int64_t mnLastCheckTime;
};