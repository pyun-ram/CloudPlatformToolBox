#include "validation/robotCodeInfo.h"

namespace ramlabrobot
{
RobotCodeInfo::RobotCodeInfo()
: _robot_id("R0001"),_code_id(string("XXX0000")), _url_server("ws://XXXX"),
_ip_client("XXX.XXX.XXX.XXX"), _time_stamp(0.0), _used_times_client(0),
_used_times_server(0),_total_times(0), _due_time(0.0)
{
}

RobotCodeInfo::RobotCodeInfo(string robot_id, string code_id, string url_server)
: _robot_id(robot_id), _code_id(code_id), _url_server(url_server)
{
  _ip_client = "XXX.XXX.XXX.XXX";
  _time_stamp = getTimeStamp();
  _used_times_client = 0;
  _used_times_server = 0;
  _total_times = 0;
  _due_time = 0.0;
}

RobotCodeInfo::~RobotCodeInfo()
{
}

RobotCodeInfo::Ptr RobotCodeInfo::createCodeInfo(string robot_id, string code_id, string url_server)
{
  return RobotCodeInfo::Ptr(new RobotCodeInfo(robot_id, code_id, url_server) );
}

void RobotCodeInfo::init()
{
    _client.setURL(_url_server.c_str());
    _client.connect();
    return;
}

bool RobotCodeInfo::validate()
{
  RobotJson json;
  json.setString(" {\"robot_id\" : \" XXX \" , \"code_id\" : \" XXX \" , \"timestamp\" : 100000.000, \"used_times_client\" : 0}");
  json.setDocString("robot_id", _robot_id);
  json.setDocString("code_id", _code_id);
  json.setDocDouble("timestamp", updateTimeStamp());
  json.setDocInt("used_times_client", getUsedTimesClient());
  string data_send = json.getString();
  _client.setDataSend(data_send);
  // cout<<"RobotCodeInfo::validate : data_send"<<endl<<data_send<<endl;
  _client.sendTextData();
  
  _client.receiveTextData();
  string data_receive = _client.getDataReceive();
  json.setString(data_receive);
  // cout<<"RobotCodeInfo::validate : data_receive"<<endl<<data_receive<<endl;
  setIpClient(json.getDocString("ip_client"));
  bool status = json.getDocBool("status");
  if (status)
    updateUsedTimesClient(1);
  setTotalTimes(json.getDocInt("total_times"));
  setDueTime(json.getDocDouble("due_time"));
  updateUsedTimesServer(json.getDocInt("used_times_server"));
  cout<<BLUE "RobotCodeInfo::validate: "<< (status == true ? "TRUE" : "FLASE" )<<" " RESET <<endl;
  return status;
}

void RobotCodeInfo::show()
{
  cout<<BLUE "-----------RobotCodeInfo::show()------------"<<endl;
  cout<<"robot_id: "<<_robot_id<<endl;
  cout<<"code_id: "<<_code_id<<endl;
  cout<<"url_server: "<<_url_server<<endl;
  cout<<"ip_client: "<<_ip_client<<endl;
  cout<<"used_times_client: "<<getUsedTimesClient()<<endl;
  cout<<"used_times_server: "<<getUsedTimesServer()<<endl;
  cout<<"total_times: "<<getTotalTimes()<<endl;
  cout<<"time_stamp: "<<setprecision(14)<<_time_stamp<<setprecision(6)<<endl;
  cout<<"due_time: "<<setprecision(14)<<getDueTime()<<setprecision(6)<<endl;
  cout<<"--------------------------------------------" RESET<<endl;
  return;
}

void RobotCodeInfo::close()
{
  _client.close();
  cout<<GREEN "RobotCodeInfo::close : The connection to validation server closed! :)" RESET<<endl;
  return;
}

signed int RobotCodeInfo::getUsedTimesServer()
{
  return _used_times_server;
}

signed int RobotCodeInfo::updateUsedTimesServer(signed int used_times_server)
{
  _used_times_server = used_times_server;
  return _used_times_server;
}


double RobotCodeInfo::getTimeStamp()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

double RobotCodeInfo::updateTimeStamp()
{
  _time_stamp = getTimeStamp();
  return _time_stamp;
}

signed int RobotCodeInfo::updateUsedTimesClient( signed int incr )
{
    _used_times_client += incr;
    return _used_times_client;
}

void RobotCodeInfo::setRobotID(string robot_id)
{
  _robot_id = robot_id;
  return;
}

void RobotCodeInfo::setTotalTimes(signed int total_times)
{
  _total_times = total_times;
  return;
}

void RobotCodeInfo::setDueTime(double due_time)
{
  _due_time=due_time;
  return;
}

void RobotCodeInfo::setIpClient(string ip_client)
{
  _ip_client = ip_client;
  return;
}

signed int RobotCodeInfo::getUsedTimesClient()
{
  return _used_times_client;
}

signed int RobotCodeInfo::getTotalTimes()
{
  return _total_times;
}

double RobotCodeInfo::getDueTime()
{
  return _due_time;
}

}