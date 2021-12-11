/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-11 14:41:29
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-11 14:45:23
 */
#include "Config.hpp"


Config * Config::config_instance = nullptr;
std::string Config::file_name = "/home/txy/SecMML/config/parameter/constant.json";

Config* Config::init(){
    if(Config::config_instance == nullptr){
        Json::Value root;
        Json::Reader reader;
        std::ifstream ifs(Config::file_name);//open file example.json
        if(!reader.parse(ifs, root)){
            std::cout<<"fail to parse"<<std::endl;
        }
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