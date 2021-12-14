/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-11 14:41:29
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-13 17:01:53
 */
#include "Config.hpp"


Config * Config::config_instance = nullptr;


//jsonfile path
Config* Config::init(std::string file_name){
    if(Config::config_instance == nullptr){
        Json::Value root;
        Json::Reader reader;
        //open json file
        std::ifstream ifs(file_name);
        if(!reader.parse(ifs, root)){
            std::cout<<"fail to parse"<<std::endl;
        }

        //Initialize vectors of ip and port
        std::vector<std::string> ip;
        std::vector<int> port;
        int sz = root["IP"].size();
        for(int i = 0; i < sz; i++){
            ip.push_back(root["IP"][i].asString().c_str());
            port.push_back(root["PORT"][i].asInt());
        }
        
        Config::config_instance = new Config(root, ip, port);
    }
    return Config::config_instance;
};