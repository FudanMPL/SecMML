/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 10:04:24
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-09 15:57:20
 */

#include <cstdio>
#include <fstream>
#include "json/json.h"
#include <string.h>
#include <iostream>
using namespace std;

class IOconfig{
    public:
        static std::string constant_file_path;
        static std::string party_file_path;
        static Json::Value init();
        //解析，new constant
        class Constant{
            public:
                Constant(Json::Value constant_root) : B(constant_root["B"].asInt()){};
                Constant(int _B) : B(_B){};
                const int B;
        };
};

//init
std::string IOconfig::constant_file_path = "../parameter/constant.json";
std::string IOconfig::party_file_path = "../parameter/0.json";

Json::Value IOconfig::init(){
    Json::Value root;
    Json::Reader reader;
    std::ifstream ifs(constant_file_path);//open file example.json
    if(!reader.parse(ifs, root)){
        std::cout<<"fail to parse"<<std::endl;
    }
    return root;
}



int main()
{
    Json::Value root;
    cout<<IOconfig::constant_file_path<<endl;
    root = IOconfig::init();
    cout << root["B"] << endl;
    IOconfig::Constant c(root);
    cout<<c.B<<endl;
}   