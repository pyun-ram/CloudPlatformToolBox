#include "validation/robotCodeInfo.h"
#include <unistd.h>
void ssleep(int s)
{
  usleep(s * 1000* 1000);
  return;
}

double getTimeStamp()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (double)tv.tv_sec + (double)tv.tv_usec / 1000000.0;
}
int main(int argc, char** argv)
{
  cout<<GREEN "TEST BEGIN" RESET<<endl;
//   int s = 20;
//   cout<<"main: Will sleep "<<s<<" seconds."<<endl;
//   ssleep(s);
//   cout<<"main: seep Done!"<<endl;
  cout<<YELLOW "Hello World! -- before validation" RESET<<endl;

  for(char i = 0; i < 100; i++)
  {
    double t1 = getTimeStamp();
    ramlabrobot::RobotCodeInfo::Ptr p_code_info = ramlabrobot::RobotCodeInfo::createCodeInfo(
    string("R0002"), string("LOC0001"), string("ws://54.255.247.61:80/validate"));
    p_code_info->init();
    if(p_code_info->validate())
    {
      cout<<YELLOW "Hello World! -- validate OK!" RESET<<endl;
//       p_code_info->show();
    }
    else
    {
      cout<<YELLOW "VALIDATE NON-AUTHORIZED!" RESET<<endl;
//       p_code_info->show();
    }
    cout<<BLUE "main: "<<(getTimeStamp() - t1)*1000<<" ms" RESET<<endl;
    p_code_info->close();
//     ssleep(1);
  }
  
  return 0;
}

