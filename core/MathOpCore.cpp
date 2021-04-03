//
// Created by haoqi wu on 2021/4/3.
//

#include "MathOp.h"

MathOp::Div_Const_Trunc_Optimized::Div_Const_Trunc_Optimized() {}

MathOp::Div_Const_Trunc_Optimized::Div_Const_Trunc_Optimized(Mat *res, Mat *a, ll b) {
    
}

void MathOp::Div_Const_Trunc_Optimized::forward() {
    
}

void MathOp::Div_Const_Trunc_Optimized::back() {}

MathOp::KOrCL::KOrCL() {}

MathOp::KOrCL::KOrCL(Mat* res, Mat *d_B, int k) {
    
}

void MathOp::KOrCL::forward() {
    
}

void MathOp::KOrCL::back() {}

MathOp::Hard_Tanh::Hard_Tanh() {}

MathOp::Hard_Tanh::Hard_Tanh(NeuronMat *res, NeuronMat *a){
    
}

void MathOp::Hard_Tanh::forward(){
    
}
void MathOp::Hard_Tanh::back(){
    
}

MathOp::Tanh_ex::Tanh_ex() {}

MathOp::Tanh_ex::Tanh_ex(NeuronMat *res, NeuronMat *a){
    
}

void MathOp::Tanh_ex::forward(){
    
}
void MathOp::Tanh_ex::back(){

}
MathOp::Tanh::Tanh() {}

MathOp::Tanh::Tanh(NeuronMat *res, NeuronMat *a) {
    
}

void MathOp::Tanh::forward() {
    
}

void MathOp::Tanh::back() {
    
}

MathOp::Tanh_Mat::Tanh_Mat() {}

MathOp::Tanh_Mat::Tanh_Mat(NeuronMat *res, NeuronMat *a){
    
}

void MathOp::Tanh_Mat::forward(){
    
}
void MathOp::Tanh_Mat::back(){

}

MathOp::Raw_Tanh::Raw_Tanh() {}

MathOp::Raw_Tanh::Raw_Tanh(NeuronMat *res, NeuronMat *a){

}

void MathOp::Raw_Tanh::forward(){

}

void MathOp::Raw_Tanh::back(){

}

MathOp::Tanh_change::Tanh_change() {}

MathOp::Tanh_change::Tanh_change(NeuronMat *res, NeuronMat *a) {
    
}

void MathOp::Tanh_change::forward() {
    
}

void MathOp::Tanh_change::back() {

}