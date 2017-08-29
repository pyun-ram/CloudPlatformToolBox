#ifndef ROBOTLOG_H
#define ROBOTLOG_H

#include "common_include.h"
#include "websocket/robotBase.h"
namespace ramlabrobot
{
class RobotLogger
{
public:
  typedef std::shared_ptr<RobotLogger> Ptr;
  enum LoggerMode {SendOnly, SendandReceive};
  
  string _robot_id;
  string _code_id;
  string _url_server;
  string _ip_client;
  RobotJson _message;
  
  RobotLogger();
  RobotLogger(string robot_id, string code_id, string url_server, LoggerMode mode = SendOnly);
  ~RobotLogger();
  
  static RobotLogger::Ptr createLogger(string robot_id, string code_id, string url_server, LoggerMode mode = SendOnly);
  void setMode(LoggerMode mode);
  LoggerMode getMode();
  void init(string format);
  bool log();
  void close();
  void show();
  
private:
  RobotClient _client;
  LoggerMode _mode = SendOnly;
      
};
}
#endif //ROBOTLOG_H

