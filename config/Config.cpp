/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-11 14:41:29
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2022-01-06 14:47:24
 */
#include "Config.hpp"



Config* Config::config = nullptr;
//jsonfile path
Config* Config::init(std::string file_name){
    if(Config::config == nullptr){
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
        for(int i = 0; i < root["M"].asInt(); i++){
            ip.push_back(root["IP"][i].asString());
            port.push_back(root["PORT"][i].asInt());
        }
        std::cout<<"ip[0]:"<<ip[0]<<std::endl;
      Config::config = new Config(root, ip, port);
    }
    return Config::config;
}

__int128_t Config::inverse(__int128_t a, __int128_t b, long MOD) {
    return power(a, b-2, MOD);
}

__int128_t Config::power(__int128_t a, __int128_t b, long MOD) {
   __int128_t ret = 1;
    a = (a%MOD+MOD)%MOD;
    b = (b%MOD+MOD)%MOD;
    if (b == 0)
        return 1;
    while (b > 0) {
        if (b&1) ret = ret * a;
        ret = get_residual(ret, MOD);
        a = a * a;
        a = get_residual(a, MOD);
        b >>= 1;
    }
    return ret;
}

__int128_t Config::get_residual(__int128_t a, long MOD) {
    return (a % MOD + MOD) % MOD;
}
