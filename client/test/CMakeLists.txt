# rapidjson
include_directories( ${PROJECT_SOURCE_DIR}/dependencies/rapidjson-master/include)

add_executable(test_echo_server test_echo_server.cpp)
target_link_libraries(test_echo_server 
  robotJson
  robotWS)
  
add_executable(test_robotCodeInfo test_robotCodeInfo.cpp)
target_link_libraries(test_robotCodeInfo
  robotCodeInfo)
  
add_executable(test_robotLog test_robotLog.cpp)
target_link_libraries(test_robotLog
  robotLog)