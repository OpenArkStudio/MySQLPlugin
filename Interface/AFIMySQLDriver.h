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

#ifdef _MSC_VER
#include <windows.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <stdint.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/time.h>
//#include <sys/timeb.h>
#include <sys/types.h>

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#endif

#include <string>
#include <vector>
#include <set>
#include "MySQL++/include/mysql++.h"

class AFIMySQLDriver
{
public:
	virtual ~AFIMySQLDriver() {}
	
    virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd) = 0;
    virtual void Update() = 0;
    virtual mysqlpp::Connection* GetConnection() = 0;
    virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult) = 0;
    virtual bool Query(const std::string& qstr) = 0;

    virtual bool Enable() = 0;
    virtual bool Reconnect() = 0;
    virtual bool CanReconnect() = 0;

    virtual bool Update(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec) = 0;
    virtual bool Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
    virtual bool Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec) = 0;
    virtual bool Delete(const std::string& strRecordName, const std::string& strKey) = 0;
    virtual bool Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit) = 0;
    virtual bool Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec) = 0;
};