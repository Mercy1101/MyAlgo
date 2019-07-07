#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>
#include <vector>
#include <string>
/* uncomment for applications that use vectors */
/*#include <vector>*/
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define EXAMPLE_HOST "tcp://127.0.0.1:3306"
#define EXAMPLE_USER "root"
#define EXAMPLE_PASS "password"
#define EXAMPLE_DB "smd"

struct DIAL_HISTORY_T
{
    std::string    sDialNumber;
    int            iAgentID;
    int            iDialTime;
    int            iDialResult;
    int            iDialResultType;
    std::string    sRecFileName;
    int            iScheduleID;
    int            iSRState;
    int            iSRResult;
    int            iSRResultType;
    std::string    sSRResultInfo;
    int            iRecheckResult;
    int            iResponseTime;
    int            iLoopTime;
    int            iDialChan;
};

class MySQLQuery
{
public:
    MySQLQuery(const std::string& url, const std::string& user, const std::string& pass)
    {
        m_pCon = MysqlConnect(url, user, pass);
    }

    ~MySQLQuery()
    {
        if (nullptr != m_pCon)
        {
            delete m_pCon;
        }
    }

    void MysqlQuery (const sql::SQLString& sCondition,      // [in]
                    const sql::SQLString& sLimit,           // [in]
                    const sql::SQLString& sTableName,       // [in]
                    const sql::SQLString& database,         // [in]
                    std::vector<DIAL_HISTORY_T>& vstDialHistory  // [out]
    )
    {
        if (nullptr == m_pCon)
        {
            std::cout << "数据库连接错误...";
            return;
        }

        // sql::Statement* stmt;
        sql::ResultSet* res;
        sql::PreparedStatement* pstmt;
        /* Connect to the MySQL test database */
        m_pCon->setSchema (database);

        // 创建一个SQL语句
        sql::SQLString sSQL;
        CreateSQLString (sCondition, sLimit, sTableName, sSQL);

        std::cout << "当前执行的SQL语句为：" << sSQL << "\t\n" << std::flush;
        /* Select in ascending order */
        pstmt = m_pCon->prepareStatement (sSQL);
        res = pstmt->executeQuery ();

        DIAL_HISTORY_T stTemp;
        /* Fetch in reverse = descending order! */
        while (res->next ())
        {
            stTemp.sDialNumber = res->getString("DialNumber");
            stTemp.iAgentID = res->getInt("AgentID");
            stTemp.iDialTime = res->getInt("DialTime");
            stTemp.iDialResult = res->getInt("DialResult");
            stTemp.iDialResultType = res->getInt("DialResultType");
            stTemp.sRecFileName = res->getString("RecFileName");
            stTemp.iScheduleID = res->getInt("ScheduleID");
            stTemp.iSRState = res->getInt("SRState");
            stTemp.iSRResult = res->getInt("SRResult");
            stTemp.iSRResultType = res->getInt("SRResultType");
            stTemp.sSRResultInfo = res->getString("SRResultInfo");
            stTemp.iRecheckResult = res->getInt("RecheckResult");
            stTemp.iResponseTime = res->getInt("ResponseTime");
            stTemp.iLoopTime = res->getInt("LoopTime");
            stTemp.iDialChan = res->getInt("DialChan");
            vstDialHistory.push_back(stTemp);
        }

        delete res;
        delete pstmt;
    }

private:
    sql::Connection* m_pCon;
    sql::Connection* MysqlConnect (const std::string& url, const std::string& user, const std::string& pass)
    {
        try {
            /* INSERT TUTORIAL CODE HERE! */
            /* Create a connection */
            sql::Driver* driver = get_driver_instance ();
            std::cout << url << '\t' << user << '\t' << pass;
            system("pause");
            return driver->connect (url, user, pass);
        }
        catch (sql::SQLException& e) {
            /*
            MySQL Connector/C++ throws three different exceptions:
            - sql::MethodNotImplementedException (derived from sql::SQLException)
            - sql::InvalidArgumentException (derived from sql::SQLException)
            - sql::SQLException (derived from std::runtime_error)
            */
            std::cout << "# ERR: SQLException in " << __FILE__;
            std::cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
            /* what() (derived from std::runtime_error) fetches error message */
            std::cout << "# ERR: " << e.what ();
            std::cout << " (MySQL error code: " << e.getErrorCode ();
            std::cout << ", SQLState: " << e.getSQLState () << " )" << std::endl;
            return nullptr;
        }
        std::cout << "Done." << std::endl;
    }

    bool IsValidSQLCondition (const sql::SQLString& sCondition)
    {
        /// 应该不为空，为空就是无效sql条件
        if (0 == sCondition.length ())
        {
            return false;
        }

        /// 判断符有哪些？ =,
        std::string sParser (sCondition);
        //sParser.find ("");
    }

    /// 最大获取的行数
    const int MAX_ROW_LIMIT = 1000;
    bool IsValidSQLLimit (const sql::SQLString& sLimit)
    {
        std::string sParser (sLimit);
        if (std::all_of (sParser.cbegin (), sParser.cend (), ::isdigit))
        {
            return true;
        }

        int iLimit = atoi (sParser.c_str ());

        if (iLimit <= 0 || iLimit >= MAX_ROW_LIMIT)
        {
            return false;
        }
    }

    bool IsValidTableName (const sql::SQLString& sTableName)
    {
        if (0 == sTableName.length())
        {
            return false;
        }

        /// 下面这段应该写一个正则表达式来匹配一个类型名字的表名
        //if (regex.not match (sTableName))
        //{
        //    return false;
        //}
        return true;
    }


    int CreateSQLString (const sql::SQLString& sCondition,
                         const sql::SQLString& sLimit,
                         const sql::SQLString& sTableName,
                         sql::SQLString& sRetSQL
    )
    {
        sRetSQL = "SELECT DialNumber, AgentID, DialTime, DialResult, DialResultType, RecFileName, ScheduleID, SRState, \
                SRResult, SRResultType, SRResultInfo, RecheckResult, ResponseTime, LoopTime, DialChan ";
        sql::SQLString sQueryCondition = "WHERE 1=1";
        sql::SQLString sLimitCondition = "LIMIT ";
        sql::SQLString sTableCondition = "FROM ";

        if (IsValidSQLCondition (sCondition))
        {
            sQueryCondition = "AND " + sCondition;
        }

        if (IsValidSQLLimit (sLimit))
        {
            sLimitCondition += sLimit;
        }
        else
        {
            /// 如果为无效Limit 则默认为10行
            sLimitCondition += "10";
        }

        if (IsValidTableName (sTableName))
        {
            sTableCondition += sTableName;
        }
        else
        {
            sTableCondition += "dialhistory_all";
        }

        sRetSQL = sRetSQL + " " + sTableCondition + " " + sQueryCondition + " " + sLimitCondition + " ";

        return 1;
    }
};



int main (int argc, const char** argv)
{
    const sql::SQLString url (argc >= 2 ? argv[1] : EXAMPLE_HOST);
    const sql::SQLString user (argc >= 3 ? argv[2] : EXAMPLE_USER);
    const sql::SQLString pass (argc >= 4 ? argv[3] : EXAMPLE_PASS);
    const sql::SQLString database (argc >= 5 ? argv[4] : EXAMPLE_DB);

    const sql::SQLString sCondition("ScheduleID>300");
    const sql::SQLString sLimit("50");
    const sql::SQLString sTableName("dialhistory_all");

    std::vector<DIAL_HISTORY_T> vstDialHistory;

    {
        MySQLQuery  FirstQuery(url, user, pass);
        FirstQuery.MysqlQuery (sCondition, sLimit, sTableName, database, vstDialHistory);
    }
    
    for (auto it : vstDialHistory)
    {
        std::cout << it.sDialNumber << "\t";
        std::cout << it.iAgentID << "\t";
        std::cout << it.iDialTime << "\t";
        std::cout << it.iDialResult << "\t";
        std::cout << it.iDialResultType << "\t";
        std::cout << it.sRecFileName << "\t";
        std::cout << it.iScheduleID << "\t";
        std::cout << it.iSRState << "\t";
        std::cout << it.iSRResult << "\t";
        std::cout << it.iSRResultType << "\t";
        std::cout << it.sSRResultInfo << "\t";
        std::cout << it.iRecheckResult << "\t";
        std::cout << it.iResponseTime << "\t";
        std::cout << it.iLoopTime << "\t";
        std::cout << it.iDialChan << "\t";
        std::cout << "\n";
    }
#if 0
    while (true)
    {
        std::string sTemp;
        std::cout << "请输入WHRER条件...\n" << std::flush;
        std::cin.clear ();
        getline (std::cin, sTemp);
        std::cin.clear ();
        const sql::SQLString sCondition(sTemp);  // ("ScheduleID > 300");

        //int iTemp = 10;
        sTemp.clear ();
        std::cout << "请输入LIMIT的限制个数...\n" << std::flush;
        std::cin.clear ();
        getline (std::cin, sTemp);
        std::cin.clear ();
        const sql::SQLString sLimit (sTemp); // ("10");

        std::cout << "请输入表名...\n" << std::flush;
        std::cin.clear ();
        getline (std::cin, sTemp);
        std::cin.clear ();
        const sql::SQLString sTableName (sTemp);
        std::vector<DIAL_HISTORY_T> vstDialHistory;

        {
            MySQLQuery  FirstQuery(url, user, pass);
            FirstQuery.MysqlQuery (sCondition, sLimit, sTableName, database, vstDialHistory);
        }

        for (auto it : vstDialHistory)
        {
            std::cout << it.sDialNumber << "\t";
            std::cout << it.iAgentID << "\t";
            std::cout << it.iDialTime << "\t";
            std::cout << it.iDialResult << "\t";
            std::cout << it.iDialResultType << "\t";
            std::cout << it.sRecFileName << "\t";
            std::cout << it.iScheduleID << "\t";
            std::cout << it.iSRState << "\t";
            std::cout << it.iSRResult << "\t";
            std::cout << it.iSRResultType << "\t";
            std::cout << it.sSRResultInfo << "\t";
            std::cout << it.iRecheckResult << "\t";
            std::cout << it.iResponseTime << "\t";
            std::cout << it.iLoopTime << "\t";
            std::cout << it.iDialChan << "\t";
            std::cout << "\n";
        }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for (500ms);
    }
#endif
    system ("pause");
    return 0;
}