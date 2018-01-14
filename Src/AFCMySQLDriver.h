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

#include "Interface/AFIMySQLDriver.h"

#define  ARK_MYSQL_TRY_BEGIN try {

#define  ARK_MYSQL_TRY_END(msg) }\
    catch (mysqlpp::BadQuery er) \
    { \
        std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
        return false; \
    } \
    catch (const mysqlpp::BadConversion& er)  \
    { \
        std::cout << "BadConversion [" << msg << "] Error:" << er.what() << " retrieved data size:" << er.retrieved << ", actual size:" << er.actual_size << std::endl; \
        return false; \
    } \
    catch (const mysqlpp::Exception& er) \
    { \
        std::cout << "mysqlpp::Exception [" << msg << "] Error:" << er.what() << std::endl; \
        return false; \
    }\
    catch ( ... ) \
    { \
        std::cout << "std::exception [" <<msg << "] Error:Unknown " << std::endl; \
        return false; \
    }

#define  ARK_MYSQL_TRY_END_NULL_RETURN(msg) }\
    catch (mysqlpp::BadQuery er) \
    { \
        std::cout << "BadQuery [" << msg << "] Error: " << er.what() << std::endl; \
        return ; \
    } \
    catch (const mysqlpp::BadConversion& er)  \
    { \
        std::cout << "BadConversion [" << msg << "] Error:" << er.what() << " retrieved data size:" << er.retrieved << ", actual size:" << er.actual_size << std::endl; \
        return ; \
    } \
    catch (const mysqlpp::Exception& er) \
    { \
        std::cout << "mysqlpp::Exception ["<<msg<<"] Error:" << er.what() << std::endl; \
        return ; \
    }\
    catch ( ... ) \
    { \
        std::cout << "std::exception [" << msg << "] Error:Unknown " << std::endl; \
        return ; \
    }

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class AFCMySQLDriver : public AFIMySQLDriver
{
public:
    AFCMySQLDriver(const int nReconnectTime = 60, const int nReconnectCount = -1);
    AFCMySQLDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd);
    virtual ~AFCMySQLDriver();


    virtual bool Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd);
    virtual  bool Execute();

    virtual bool Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult);
    virtual bool Query(const std::string& qstr);

    virtual mysqlpp::Connection* GetConnection();
    virtual void CloseConnection();
    virtual bool Enable();

    virtual bool CanReconnect();
    virtual bool Reconnect();

    virtual bool Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec);
    virtual bool Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
    virtual bool Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec);
    virtual bool Delete(const std::string& strRecordName, const std::string& strKey);
    virtual bool Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit);
    virtual bool Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec);

protected:
    bool IsNeedReconnect();
    bool Connect();

private:
    std::string mstrDBName  ;
    std::string mstrDBHost  ;
    int         mnDBPort    ;
    std::string mstrDBUser  ;
    std::string mstrDBPwd   ;

    mysqlpp::Connection* m_pMysqlConnect; // mysql连接对象
    float mfCheckReconnect;             //检查重连的时间

    int mnReconnectTime;
    int mnReconnectCount;

    const static std::string strDefaultKey;
    const static std::string strDefaultTable;
};