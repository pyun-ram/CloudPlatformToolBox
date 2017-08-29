#include "log/robotLog.h"

using namespace std;
double getTimeStamp()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}

int main(int argc, char** argv)
{

  ramlabrobot::RobotLogger::Ptr p_logger = ramlabrobot::RobotLogger::createLogger("R0002", "LOC0001", "ws://localhost/log");
  string format = " {\"timestamp\" : 153.0, \"i_string\" : \"XXX\", \"i_bool\" : true, \"i_int\" : 0, \"i_double\" : 1.5}";
  p_logger->setMode(ramlabrobot::RobotLogger::SendandReceive);
  p_logger->init(format);
  
  p_logger->show();
  for(char i = 0; i < 100; i++)
  {
    p_logger->_message.setDocDouble("timestamp", getTimeStamp());
    p_logger->_message.setDocString("i_string", "RAM-LAB");
    p_logger->_message.setDocBool("i_bool", true);
    p_logger->_message.setDocInt("i_int", i);
    p_logger->_message.setDocDouble("i_double", 100.0);
    p_logger->show();
  
    cout<<"main: "<<(p_logger->log() == true? "TRUE" : "FALSE")<<endl;    
  }
  
  p_logger->close();
  return 0;
  
  
}
