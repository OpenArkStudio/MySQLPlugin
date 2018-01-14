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

#include "AFCMysqlDriver.h"

const std::string AFCMySQLDriver::strDefaultKey = "ID";
const std::string AFCMySQLDriver::strDefaultTable = "RoleInfo";

AFCMySQLDriver::AFCMySQLDriver(const int nReconnectTime/* = 60*/, const int nReconnectCount /*= -1*/)
{
    mfCheckReconnect = 0.0f;
    mnDBPort        = 0;
    m_pMysqlConnect = NULL;
    mnReconnectTime = nReconnectTime;
    mnReconnectCount = nReconnectCount;
}

AFCMySQLDriver::AFCMySQLDriver(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
    mfCheckReconnect = 0.0f;
    mnDBPort        = 0;
    m_pMysqlConnect = NULL;

    mnReconnectTime = 60;
    mnReconnectCount = -1;

    Connect(strDBName, strDBHost, nDBPort, strDBUser, strDBPwd);
}

AFCMySQLDriver::~AFCMySQLDriver()
{
    CloseConnection();
}

bool AFCMySQLDriver::Connect(const std::string& strDBName, const std::string& strDBHost, const int nDBPort, const std::string& strDBUser, const std::string& strDBPwd)
{
    mstrDBName  = strDBName   ;
    mstrDBHost  = strDBHost   ;
    mnDBPort    = nDBPort     ;
    mstrDBUser  = strDBUser   ;
    mstrDBPwd   = strDBPwd    ;

    return Connect();
}

bool AFCMySQLDriver::Execute()
{
    if (IsNeedReconnect() && CanReconnect())
    {
        Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);
    }

    return true;
}

bool AFCMySQLDriver::Query(const std::string& qstr, mysqlpp::StoreQueryResult& queryResult)
{
    mysqlpp::Connection* pConection = GetConnection();
    if (pConection)
    {
		ARK_MYSQL_TRY_BEGIN
        mysqlpp::Query query = pConection->query(qstr);
        //query.execute();

        queryResult = query.store();
        query.reset();

		ARK_MYSQL_TRY_END(qstr)
        return true;
    }

    return false;
}

bool AFCMySQLDriver::Query(const std::string& qstr)
{
    mysqlpp::Connection* pConection = GetConnection();
    if (pConection)
    {
		ARK_MYSQL_TRY_BEGIN
        mysqlpp::Query query = pConection->query(qstr);
        query.execute();
        query.reset();
		ARK_MYSQL_TRY_END(qstr)
        return true;
    }

    return false;
}

mysqlpp::Connection* AFCMySQLDriver::GetConnection()
{
    return m_pMysqlConnect;
}

void AFCMySQLDriver::CloseConnection()
{
    delete m_pMysqlConnect;
    m_pMysqlConnect = NULL;
}

bool AFCMySQLDriver::Enable()
{
    return !IsNeedReconnect();
}

bool AFCMySQLDriver::CanReconnect()
{

    mfCheckReconnect += 0.1f;

    //30分钟检查断线重连
    if (mfCheckReconnect < mnReconnectTime)
    {
        return false;
    }

    if (mnReconnectCount == 0)
    {
        return false;
    }

    mfCheckReconnect = 0.0f;

    return true;
}

bool AFCMySQLDriver::Reconnect()
{
    CloseConnection();
    Connect(mstrDBName, mstrDBHost, mnDBPort, mstrDBUser, mstrDBPwd);

    if (mnReconnectCount > 0)
    {
        mnReconnectCount--;
    }

    return true;
}

bool AFCMySQLDriver::IsNeedReconnect()
{
    //没有配置表
    if (mstrDBHost.length() < 1 || mstrDBUser.length() < 1)
    {
        return false;
    }

    if (NULL == m_pMysqlConnect)
    {
        return true;
    }

    if (!m_pMysqlConnect->connected())
    {
        CloseConnection();
        return true;
    }

    if (!m_pMysqlConnect->ping())
    {
        CloseConnection();
        return true;
    }

    return false;
}

bool AFCMySQLDriver::Connect()
{
    m_pMysqlConnect = new mysqlpp::Connection();
    if (NULL == m_pMysqlConnect)
    {
        return false;
    }
	ARK_MYSQL_TRY_BEGIN
    m_pMysqlConnect->set_option(new mysqlpp::MultiStatementsOption(true));
    m_pMysqlConnect->set_option(new mysqlpp::SetCharsetNameOption("utf8mb4"));
    m_pMysqlConnect->set_option(new mysqlpp::ReconnectOption(true));
    m_pMysqlConnect->set_option(new mysqlpp::ConnectTimeoutOption(60));
    if (!m_pMysqlConnect->connect(mstrDBName.c_str(), mstrDBHost.c_str(), mstrDBUser.c_str(), mstrDBPwd.c_str(), mnDBPort))
    {
        CloseConnection();
        // 连接失败
        return false;
    }

    // 设置超时时间为24小时
    mysqlpp::Query query = m_pMysqlConnect->query("set interactive_timeout = 24*3600");
    query.execute();
    query.reset();
	ARK_MYSQL_TRY_END("Connect failed")
    return true;
}

bool AFCMySQLDriver::Updata(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec)
{
    mysqlpp::Connection* pConnection = GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    bool bExist = false;
    if (!Exists(strRecordName, strKey, bExist))
    {
        return false;
    }

    if (fieldVec.size() != valueVec.size())
    {
        return false;
    }

	ARK_MYSQL_TRY_BEGIN
    mysqlpp::Query query = pConnection->query();
    if (bExist)
    {
        // update
        query << "UPDATE " << strRecordName << " SET ";
        for (int i = 0; i < fieldVec.size(); ++i)
        {
            if (i == 0)
            {
                query << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i];
            }
            else
            {
                query << "," << fieldVec[i] << " = " << mysqlpp::quote << valueVec[i];
            }
        }

        query << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";
    }
    else
    {
        // insert
        query << "INSERT INTO " << strRecordName << "(" << strDefaultKey << ",";
        for (int i = 0; i < fieldVec.size(); ++i)
        {
            if (i == 0)
            {
                query << fieldVec[i];
            }
            else
            {
                query << ", " << fieldVec[i];
            }
        }

        query << ") VALUES(" << mysqlpp::quote << strKey << ",";
        for (int i = 0; i < valueVec.size(); ++i)
        {
            if (i == 0)
            {
                query << mysqlpp::quote << valueVec[i];
            }
            else
            {
                query << ", " << mysqlpp::quote << valueVec[i];
            }
        }

        query << ");";
    }

    query.execute();
    query.reset();
	ARK_MYSQL_TRY_END("update or insert error")

    return true;
}


bool AFCMySQLDriver::Query(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
    mysqlpp::Connection* pConnection = GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

	ARK_MYSQL_TRY_BEGIN
    mysqlpp::Query query = pConnection->query();
    query << "SELECT ";
    for (std::vector<std::string>::const_iterator iter = fieldVec.begin(); iter != fieldVec.end(); ++iter)
    {
        if (iter == fieldVec.begin())
        {
            query << *iter;
        }
        else
        {
            query << "," << *iter;
        }
    }
    query << " FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";
    //query.execute(); // 官网例子不需要execute
    mysqlpp::StoreQueryResult xResult = query.store();
    query.reset();

    if (xResult.empty() || !xResult)
    {
        return false;
    }

    // xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
    for (int i = 0; i < xResult.size(); ++i)
    {
        for (int j = 0; j < fieldVec.size(); ++j)
        {
            const std::string& strFieldName = fieldVec[j];
            std::string strValue(xResult[i][strFieldName.data()].data(), xResult[i][strFieldName.data()].length());
            valueVec.push_back(strValue);
        }
    }
	ARK_MYSQL_TRY_END("query error")

    return true;
}


bool AFCMySQLDriver::Select(const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, std::vector<std::string>& valueVec)
{
    return false;
}

bool AFCMySQLDriver::Delete(const std::string& strRecordName, const std::string& strKey)
{
    mysqlpp::Connection* pConnection = GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

	ARK_MYSQL_TRY_BEGIN
    mysqlpp::Query query = pConnection->query();
    query << "DELETE FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << ";";

    query.execute();
    query.reset();
	ARK_MYSQL_TRY_END("delete error")

    return true;
}

bool AFCMySQLDriver::Exists(const std::string& strRecordName, const std::string& strKey, bool& bExit)
{
    mysqlpp::Connection* pConnection = GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

	ARK_MYSQL_TRY_BEGIN
    mysqlpp::Query query = pConnection->query();
    query << "SELECT 1 FROM " << strRecordName << " WHERE " << strDefaultKey << " = " << mysqlpp::quote << strKey << " LIMIT 1;";

    //query.execute();
    mysqlpp::StoreQueryResult result = query.store();
    query.reset();

    if (!result || result.empty())
    {
        bExit = false;
        return true;
    }

	ARK_MYSQL_TRY_END("exist error")

    bExit = true;
    return true;
}

bool AFCMySQLDriver::Keys(const std::string& strRecordName, const std::string& strKeyName, std::vector<std::string>& valueVec)
{
    mysqlpp::Connection* pConnection = GetConnection();
    if (NULL == pConnection)
    {
        return false;
    }

    const std::string strLikeKey = "%" + strKeyName + "%";

	ARK_MYSQL_TRY_BEGIN
    mysqlpp::Query query = pConnection->query();
    query << "SELECT " << strDefaultKey << " FROM " << strRecordName << " WHERE " << strDefaultKey << " LIKE " << mysqlpp::quote << strLikeKey << " LIMIT 100;";

    mysqlpp::StoreQueryResult xResult = query.store();
    query.reset();

    if (xResult.empty() || !xResult)
    {
        return false;
    }

    // xResult应该只有一行的，为了以后可能出现的多条，所以用了循环
    for (int i = 0; i < xResult.size(); ++i)
    {
        std::string strValue(xResult[i][strDefaultKey.data()].data(), xResult[i][strDefaultKey.data()].length());
        valueVec.push_back(strValue);
    }

	ARK_MYSQL_TRY_END("exist error")

    return true;
}
