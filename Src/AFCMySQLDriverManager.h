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

#include "SDK/Base/AFMap.h"
#include "Interface/AFIMySQLDriver.h"
#include "Interface/AFIMySQLDriverManager.h"

class AFCMySQLDriverManager : public AFIMySQLDriverManager
{
public:
    virtual bool AddMySQLServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime/* = 10*/, const int nRconneCount/* = -1*/);
    virtual AFIMySQLDriver* GetMySQLDriver();
    virtual void CheckMySQL();

protected:
    std::string GetIPByHostName(const std::string& strHostName);

	AFMap<int, AFIMySQLDriver> mxMySQLs;
	AFMap<int, AFIMySQLDriver> mxInvalidMySQLs;
	int64_t mnLastCheckTime;
};