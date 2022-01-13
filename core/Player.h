/*
 * @Author: Xinyu Tu
 * @Date: 2021-12-07 09:52:08
 * @LastEditors: Xinyu Tu
 * @LastEditTime: 2021-12-16 17:44:14
 */
//
// Created by tangdingyi on 2019/12/26.
//

#ifndef MPC_ML_PLAYER_H
#define MPC_ML_PLAYER_H

#include "../Constant.h"
#include "Mat.h"

class Player {
public:
    int id;
    ll128 key, lagrange;
    Player();
    Player(int id, ll128 key, ll128 lagrange);
    static void init();
    static Mat getMetadata();
    static vector<Player> player;
    static Mat metadata;
};
#endif //MPC_ML_PLAYER_H
