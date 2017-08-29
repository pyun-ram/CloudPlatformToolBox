#include "websocket/robotBase.h"

int main(int argc, char** argv)
{
  string url = "ws://localhost/websocket_server";
  RobotClient client;
  client.setURL(url.c_str());
  client.connect();
  
  string data_send = "Hello Server! -- from Casablanca";
  client.setDataSend(data_send);
  client.sendTextData();
  
  client.receiveTextData();
  string data_receive = client.getDataReceive();
  cout<<data_receive<<endl;
  return 0;  
}