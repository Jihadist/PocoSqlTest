#include <Poco/Data/ODBC/Connector.h>
#include <Poco/Data/ODBC/ODBCException.h>
#include <Poco/Data/Session.h>
#include <iostream>

using namespace Poco::Data;

int main() {
  try {
    std::cout << "Testing Poco C++ with MS SQL Server" << std::endl;
    Poco::Data::ODBC::Connector::registerConnector();
    // clang-format off
    const std::string CONNECTION_STRING(
        "Driver={ODBC Driver 17 for SQL Server};Server=ip,port;Database=db_name;UID=username;PWD=password;");
    // clang-format on
    Session session("ODBC", CONNECTION_STRING);
    if (session.isConnected()) {
      std::cout << "Connected" << std::endl;
      using namespace Poco::Data::Keywords;
      {
        int sessionId = 0;
        session << "IF(OBJECT_ID('tempdb..#test') is not null) drop table "
                   "#test; select @@SPID;",
            into(sessionId), now;
        std::cout << sessionId << std::endl; // 74
      }
      {
        int sessionId = 0;
        session << "create table #test (s1 int,s2 int ,s3 int); select @@SPID;",
            into(sessionId), now;
        std::cout << sessionId << std::endl; // 74
      }
      {
        int sessionId = 0;
        session << "insert into #test select 1,2,3; select @@SPID;",
            into(sessionId), now;
        std::cout << sessionId << std::endl;
      }
      typedef Poco::Tuple<int, int, int> testParam;
      std::vector<testParam> testParams;
      session << ("select * from #test;"), into(testParams), now;

    } else {
      std::cerr << "Session not able to connect" << std::endl;
    }
  } catch (Poco::Data::ODBC::StatementException &e) {
    std::cerr << "Exception: " << e.toString() << std::endl;
  } catch (Poco::Data::ODBC::ConnectionException &e) {
    std::cerr << "Exception: " << e.toString() << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  Poco::Data::ODBC::Connector::unregisterConnector();
  return 0;
}
