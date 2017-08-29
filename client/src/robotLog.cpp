# include "log/robotLog.h"

namespace ramlabrobot 
{
RobotLogger::RobotLogger()
{
  _robot_id = "XXX";
  _code_id = "XXX";
  _url_server = "ws://XXX";
  _ip_client = "XXX.XXX.XXX.XXX";
  setMode(SendOnly);
}

RobotLogger::RobotLogger(string robot_id, string code_id, string url_server, LoggerMode mode)
: _robot_id(robot_id), _code_id(code_id), _url_server(url_server)
{
  _ip_client = "XXX.XXX.XXX.XXX";
  setMode(mode);
}

RobotLogger::~RobotLogger()
{
}

RobotLogger::Ptr RobotLogger::createLogger(string robot_id, string code_id, string url_server, RobotLogger::LoggerMode mode)
{
  return RobotLogger::Ptr(new RobotLogger(robot_id, code_id, url_server, mode));
}

void RobotLogger::setMode(RobotLogger::LoggerMode mode)
{
  _mode = mode;
  return;
}

RobotLogger::LoggerMode RobotLogger::getMode()
{
  return _mode;
}

void RobotLogger::show()
{
  cout<<BLUE "-----------RobotLogger::show()------------"<<endl;
  cout<<"robot_id: "<<_robot_id<<endl;
  cout<<"code_id: "<<_code_id<<endl;
  cout<<"url_server: "<<_url_server<<endl;
  cout<<"ip_client: "<<_ip_client<<endl;
  cout<<"mode: "<<(getMode() == SendOnly ? "SendOnly" : "Send and Receive")<<endl;
  cout<<"message: "<<_message.getString()<<endl;
  cout<<"--------------------------------------------" RESET<<endl;
  return;
}

void RobotLogger::init(string format)
{
  _message.setString(format);
  _client.setURL(_url_server);
  _client.connect();
  return;
}

bool RobotLogger::log()
{
  RobotJson temp_json;
  temp_json.setString(" {\"robot_id\" : \" XXX \" , \"code_id\" : \" XXX \" }");
  temp_json.setDocString("robot_id", _robot_id);
  temp_json.setDocString("code_id", _code_id);
  temp_json.addDocString("mode", (_mode == SendOnly ? "SendOnly" : "SendandReceive"));
  temp_json.addDocString("message", _message.getString());
  _client.setDataSend(temp_json.getString());
  _client.sendTextData();
  switch(_mode)
  {
    case SendOnly :
    {
      return true;
      break;
    } 
    case SendandReceive :
    {
      _client.receiveTextData();
      string data_receive = _client.getDataReceive();
      temp_json.setString(data_receive);
      _ip_client = temp_json.getDocString("ip_client");
      cout<<"RobotLogger::log() : "<<"RecievedData: "<<data_receive<<endl;
      cout<<"RobotLogger::log() : "<<"comment: "<<temp_json.getDocString("comment")<<endl;
      return temp_json.getDocBool("res");
      break;
    }
    default :
      return false;
  }
  return false;
}

void RobotLogger::close()
{
  _client.close();
  return;
}


}