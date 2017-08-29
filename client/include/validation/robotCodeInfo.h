#ifndef ROBOT_CODE_INFO_H
#define ROBOT_CODE_INFO_H

#include "common_include.h"
#include "websocket/robotBase.h"

namespace ramlabrobot
{
class RobotCodeInfo
{
public:
  typedef std::shared_ptr<RobotCodeInfo> Ptr;
  string 	_robot_id;
  string 	_code_id;
  string 	_url_server;		// the url of validation server
  string 	_ip_client;
  
  
private:	//users cannnot visit these members directly
  RobotClient	_client;
  double 	_time_stamp;
  signed int	_used_times_client;	// the used times logged in client
  signed int	_total_times;		// total times of this code (accept from server)
  double	_due_time;		// due time of this code (accept from server)
  signed int 	_used_times_server;	// the used times logged in server
  
public:
  RobotCodeInfo();			
  RobotCodeInfo(string robot_id, string code_id, string url_server);	 // initialize RobotCodeInfo
  ~RobotCodeInfo();							 // recycle the RobotCodeInfo
  
  void init();				// connect the validation server
  bool validate();			// validate and return the result (true: authorized, false: unauthorized)
  void show();				// show all members of RobotCodeInfo
  void close();
  static RobotCodeInfo::Ptr createCodeInfo(string robot_id, string code_id, string url_server); 	// create an object
  
  double 	updateTimeStamp();			 // update _time_stamp and return the value
  signed int    updateUsedTimesClient( signed int incr );// update _used_times_client and return the value
  signed int 	updateUsedTimesServer( signed int used_times_server);
  void 		setRobotID(string robot_id);
  void 		setIpClient(string ip_client);
  void 		setTotalTimes(signed int total_times);   
  void        	setDueTime(double due_time);
  signed int 	getUsedTimesClient();
  signed int 	getUsedTimesServer();
  signed int 	getTotalTimes();
  double 	getDueTime();
     
private:
//   string getIpClient();					
  double  getTimeStamp();
  
};
}
#endif // ROBOT_CODE_INFO_H