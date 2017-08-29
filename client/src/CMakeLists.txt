# rapidjson
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/rapidjson-master/include)

add_library( robotJson SHARED robotJson.cpp )
target_link_libraries( robotJson )

add_library( robotWS SHARED robotWS.cpp )
target_link_libraries( robotWS
  boost_system
  crypto
  ssl
  cpprest)
	
add_library( robotCodeInfo SHARED robotCodeInfo.cpp)
target_link_libraries(robotCodeInfo
  robotWS
  robotJson)
  
add_library( robotLog SHARED robotLog.cpp)
target_link_libraries(robotLog
  robotWS
  robotJson)