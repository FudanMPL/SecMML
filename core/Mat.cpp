//
// Created by tangdingyi on 2019/12/25.
//

#include "Mat.h"

double generateGaussianNoise(double mu, double sigma) {
    static const double epsilon = std::numeric_limits<double>::min();
    static const double two_pi = 2.0 * 3.14159265358979323846;

    thread_local double z1;
    thread_local bool generate;
    generate = !generate;

    if (!generate)
        return z1 * sigma + mu;

    double u1, u2;
    do {
        u1 = rand() * (1.0 / RAND_MAX);
        u2 = rand() * (1.0 / RAND_MAX);
    } while (u1 <= epsilon);

    double z0;
    z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
    z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
    return z0 * sigma + mu;
}

// Construct Function

Mat::Mat(int r, int c) {
   
    this->r = r;
    this->c = c;
    order = 0;
    val.resize(r * c);
}

Mat::Mat(int r, int c, ll128 b) {
  
    this->r = r;
    this->c = c;
    order = 0;
    val.resize(r * c, b);
}

Mat::Mat() {
    order = 0;
}

//Copy Constuct Fucntion

Mat::Mat(const Mat &a) {

    r = a.rows();
    c = a.cols();
    order = a.order;
    val.resize(r * c);
    val.assign(a.val.begin(), a.val.end());
}

//Deconstruct Function

Mat::~Mat() {
    order = 0;
}

//Initialization Function

void Mat::init(int r, int c) {
    this->r = r;
    this->c = c;
    order = 0;
    val.resize(r * c);
}

//Overload Opeartor '()'

ll128 &Mat::operator()(int row, int col) {
    if(row >= 0 && row < r && col >= 0 && col < c)
    {
        if(order == Config::config->MatColMajor)
        {
            return val[col * r + row];
        }
        else
        {
            return val[row * c + col];
        }
    }
    else{
        DBGprint("Function operator() The row value or column value entered is out of range\n");
    }
}

//Get the value of row and column 

ll128 Mat::get(int row, int col) const {
     if(row >= 0 && row < r && col >= 0 && col < c){
        if(order == Config::config->MatColMajor)
        {
            return val[col * r + row];
        }
        else
        {
            return val[row * c + col];
        }
     }
    else{
        DBGprint("Function get The row value or column value entered is out of range\n");
    }
}

//Get the value of the a-th element in the matrix

ll128 &Mat::getVal(int a) {
    if(a >= 0 && a < r * c)
        return val[a];
    else{
        DBGprint("Function getVal The input is out of range\n");
    }
}

//Set the value of the index-th element to v

void Mat::setVal(int index, ll128 v) {
    if(index >= 0 && index < r * c)
        val[index] = v;
    else{
        DBGprint("Function setVal The input is out of range\n");
    }
}

//Get the number of rows of the matrix

int Mat::rows() const {
    return r;
}

//Get the number of columns of the matrix

int Mat::cols() const {
    return c;
}

//Get the size of the matrix

int Mat::size() const {
    return r * c;
}

//Overload the Operator '=' used for assignment between matrices

Mat &Mat::operator=(const Mat &a) {     
    r = a.rows();
    c = a.cols();
    order = a.order;
    val.resize(r * c);
    int l = r * c;
    val.assign(a.val.begin(), a.val.end());
    return *this;
}

//Overload the Operator '=' Assign values to matrix with vector a

Mat &Mat::operator=(vector<ll128> &a) {
    int l = 0;
    if(a.size() >= r * c)
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                this->operator()(i, j) = a[l++];
        return *this;
    }
    else
    {
        DBGprint("Function operator= The input vector is too small\n");
    }
}
    

//Overload the Operator '=' Used for assignment after socket receives data

Mat &Mat::operator=(char *&p) {         //用于socket收到数据后的赋值
    int pr = Constant::Util::char_to_int(p);
    int pc = Constant::Util::char_to_int(p);
    this->r = pr;
    this->c = pc;
    order = 0;
    val.resize(r * c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        val[i] = Constant::Util::char_to_ll(p);
    }
    return *this;
}

//Overload the operator '==' Judge whether the matrices are equal

bool Mat::operator==(Mat p) {       
    int l = r * c;
//    bool match = true;
    if(p.rows() == r && p.cols() == c)
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                if (this->operator()(i,j) != p.operator()(i,j)) {
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        DBGprint("Function operator== The row or column between the two matrices is not equal\n");
        return false;
    }
    
}

//Matrix transpose

Mat Mat::transpose() const {     
    Mat ret(c, r);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            ret.val[i * c + j] = val[j * r + i];
        }
    }
    return ret;
}

//Overload the operator '+' Used for matrix addition

Mat Mat::operator+(const Mat &a) {     //矩阵加法
    if(a.cols() == c && a.rows() == r)
    {
        Mat ret(r, c);
        int l = r * c;
        for (int i = 0; i < r; i++){
            for (int j = 0; j < c; j++) {
                ret.operator()(i,j) = this->operator()(i,j) + a.get(i,j);
                ret.operator()(i,j) = ret.operator()(i,j) >= Config::config->MOD ? ret.operator()(i,j) - Config::config->MOD : ret.operator()(i,j);
                ret.operator()(i,j) = ret.operator()(i,j) <= -Config::config->MOD ? ret.operator()(i,j) + Config::config->MOD : ret.operator()(i,j);
            }
        }
        return ret;
    }
    else
    {
        DBGprint("Function operator+ The input matrix format is illegal\n");
    }
   
}

//Overload the operator '+='

void Mat::operator+=(const Mat &a) {   //矩阵+=操作
    if(a.cols() == c && a.rows() == r)
    {
        int l = r * c;
         for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                this->operator()(i,j) += a.get(i,j);
                this->operator()(i,j) = this->operator()(i,j) >= Config::config->MOD ? this->operator()(i,j) - Config::config->MOD : this->operator()(i,j);
                this->operator()(i,j) = this->operator()(i,j) <= -Config::config->MOD ? this->operator()(i,j) + Config::config->MOD : this->operator()(i,j);
            }
    }
    else
    {
        DBGprint("Function operator += The input matrix format is illegal\n");
    }
}

//Add b to each element in the matrix and map it to the field

Mat Mat::operator+(const ll128 &b) {     //矩阵每个数都加上b
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] + b;
        ret.val[i] = ret.val[i] >= Config::config->MOD ? ret.val[i] - Config::config->MOD : ret.val[i];
        ret.val[i] = ret.val[i] <= -Config::config->MOD ? ret.val[i] + Config::config->MOD : ret.val[i];
    }
    return ret;
}

//Overload the operator '-' Used for matrix subtraction

Mat Mat::operator-(const Mat &a) {      //矩阵减法操作
    if(a.cols() == c && a.rows() == r)
    {
        Mat ret(r, c);
        int l = r * c;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                ret.operator()(i,j) = this->operator()(i,j) - a.get(i,j);
                ret.operator()(i,j) = ret.operator()(i,j) > Config::config->MOD ? ret.operator()(i,j) - Config::config->MOD : ret.operator()(i,j);
                ret.operator()(i,j) = ret.operator()(i,j) < -Config::config->MOD ? ret.operator()(i,j) + Config::config->MOD : ret.operator()(i,j);
            }
        return ret;
    }
    else
    {
        DBGprint("Function operator- The input matrix format is illegal\n");
    }
}

//Subtract b to each element in the matrix and map it to the field

Mat Mat::operator-(ll128 b) {    //矩阵每个数减去b
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] - b;
        ret.val[i] = ret.val[i] > Config::config->MOD ? ret.val[i] - Config::config->MOD : ret.val[i];
        ret.val[i] = ret.val[i] < -Config::config->MOD ? ret.val[i] + Config::config->MOD : ret.val[i];
    }
    ret.residual();
    return ret;
}

// Base implementation, with complexity of O(N^3)
//Multiplication operation of different storage methods of matrix


Mat Mat::operator*(const Mat &a) {     //不同矩阵存储方式的乘法操作
    if(c == a.rows())
    {
        int tmp_c;
        tmp_c = a.cols();
        Mat ret(r, a.cols());
        int i, j;
        ll128 tmp = 0;
        if (pair_order_type(this, &a) == Config::config->MM_NT){     // Column storage multiply row storage
            for (int k = 0; k < c; k++){
                for (int j = 0; j < tmp_c; j++){
                    tmp = a.val[k * tmp_c + j];
                    for (int i = 0; i < r; i++){
                        ret.val[j * r + i] += val[k * r + i] * tmp;
                    }
                }
    
            }
        }
        if (pair_order_type(this, &a) == Config::config->MM_NN){     //Column storage multiply column storage
            for (int j = 0; j < tmp_c; j++){
                for (int k = 0; k < c; k++){
                    tmp = a.val[j * c + k];
                    for (int i = 0; i < r; i++){
                        ret.val[j * r + i] += val[k * r + i] * tmp;
                    }
                }
    
            }
        }
        if (pair_order_type(this, &a) == Config::config->MM_TN){     //Row storage multiply column storage
            for (int j = 0; j < tmp_c; j++){
                for (int i = 0; i < r; i++){
                    tmp = ret.val[j * r + i];
                    for (int k = 0; k < c; k++){
                        tmp += val[i * c + k] * a.val[j * c + k];
                    }
                    ret.val[j * r + i] += tmp;
                }
    
            }
        }
        if (pair_order_type(this, &a) == Config::config->MM_TT){     //Row storage multiply row storage
            for (int j = 0; j < tmp_c; j++){
                for (int i = 0; i < r; i++){
                    tmp = ret.val[j * r + i];
                    for (int k = 0; k < c; k++){
                        tmp += val[i * c + k] * a.val[k * tmp_c + j];
                    }
                    ret.val[j * r + i] += tmp;
                }
            }
        }
        // Standard implementation 
        // for (int j = 0; j < tmp_c; j++) {
        //     for (int i = 0; i < r; i++) {
        //         switch (pair_order_type(this, &a)) {
        //             case MM_NN:
        //                 for (int k = 0; k < c; k++) {
        //                     ret.val[j * r + i] += val[k * r + i] * a.val[j * c + k];
        //                 }
        //                 break;
        //             case MM_NT:
        //                 for (int k = 0; k < c; k++) {
        //                     ret.val[j * r + i] += val[k * r + i] * a.val[k * tmp_c + j];
        //                 }
        //                 break;
        //             case MM_TN:
        //                 for (int k = 0; k < c; k++) {
        //                     ret.val[j * r + i] += val[i * c + k] * a.val[j * c + k];
        //                 }
        //                 break;
        //             case MM_TT:
        //                 for (int k = 0; k < c; k++) {
        //                     ret.val[j * r + i] += val[i * c + k] * a.val[k * tmp_c + j];
        //                 }
        //                 break;
        //         }
        //     }
        // }
        ret.residual();
        return ret;
    }
    else
    {
        DBGprint("Function operator* The input matrix format is illegal\n");
    }
 
}

//Multiply each element in the matrix by b

Mat Mat::operator*(const ll128 &b) {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] * b;
    }
    ret.residual();
    return ret;
}

//Divide each element in the matrix by b

Mat Mat::operator/(const ll128 &b) {
    Mat ret;
    ret = *this;
    ret.sign();
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            ret(i, j) = ret(i, j) / b;
        }
    ret.residual();
    return ret;
}

//Divide each element in the matrix by each element in the corresponding position in matrix a

Mat Mat::operator/(Mat a) {
    if(a.cols() == c && a.rows() == r)
    {
        Mat ret;
        ret = *this;
        ret.sign();
        int l = r * c;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                ret.operator()(i,j) = this->operator()(i,j) / a.get(i,j);
            }
        ret.residual();
        return ret;
    }
    else
    {
        DBGprint("Function operator/ The input matrix format is illegal\n");
    }
}

//Each element in the matrix is shifted to the left by b bits

Mat Mat::operator<<(int b) const {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] << b;
    }
    ret.residual();
    return ret;
}

//Map each element in the matrix to a signed number and shift to the right by b bits

Mat Mat::operator>>(int b) const {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] > Config::config->MOD / 2 ? val[i] - Config::config->MOD >> b : val[i] >> b;
    }
    return ret;
}

//Each element in the matrix AND b

Mat Mat::operator&(int b) const {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] & b;
    }
    return ret;
}

//Each element in the matrix AND each element in the corresponding position in matrix a

Mat Mat::operator&(const Mat &a) const {
    if(a.cols() == c && a.rows() == r)
    {
        Mat ret(r, c);
        int l = r * c;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                ret.operator()(i,j) = this->get(i,j) & a.get(i,j);
            }
        return ret;
    }
    else
    {
        DBGprint("Function operator& The input matrix format is illegal\n");
    }
}

// 将矩阵中的每个数用1减去 得到的新矩阵所有数映射至域上 并返回新矩阵
//Use 1 to substract each element in the matrix and map all the elements in the new matrix to the field return the new matrix

Mat Mat:: oneMinus() {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = 1 - val[i];
    }
    ret.residual();
    return ret;
}

// 将矩阵中的每个数用2的20次方减去 得到的新矩阵所有数映射至域上 并返回新矩阵
//Use 2^20 to substract each element in the matrix and map all the elements in the new matrix to the filed return the new matrix

Mat Mat::oneMinus_IE() {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = Config::config->IE - val[i];
    }
    ret.residual();
    return ret;
}

// 将该矩阵与矩阵a进行对应元素乘法 得到的新矩阵所有数映射至域上 并返回新矩阵
//Each element in the matrix Multiply each element in the corresponding position in matrix 
//and map all the elements in the new matrix to the field return the new matrix

Mat Mat::dot(const Mat &a) {
    if(a.cols() == c && a.rows() == r && a.order == order)
    {
        Mat ret(r, c);
        int l = r * c;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                ret.operator()(i,j) = this->operator()(i,j) * a.get(i,j);
            }
        ret.residual();
        return ret;
    }
    else
    {
        DBGprint("Function dot The input matrix format is illegal\n");
    }

    // Multithread version
    // std::vector<std::thread> thrds;
    // int thread_num = 2;
    // int seg_len = ceil(l*1.0 / thread_num);
    // for (int idx = 0; idx < thread_num; idx++) {
    //     thrds.emplace_back(std::thread([this, idx, l, seg_len, a, &ret]() {
    //         for (int j = idx*seg_len; j < (idx+1)*seg_len && j < l; j++) {
    //             ret.val[j] = val[j] * a.val[j];
    //         }
    //     }));
    // }
    // for (auto& t: thrds) t.join();
}

// 获取对应行的元素存入vector 返回该vector
//Get the element of the ath row and store it in the vector return the vector

vector<ll128> Mat::row(int a) {
    if(a < r && a >= 0)
    {
        vector<ll128> ret(c);
        for (int i = 0; i < c; i++) {
            ret[i] = get(a, i);
        }
        return ret;
    }
    else
    {
        DBGprint("Function row(int a) The input is out of range\n");
    }
}

//将vector a中的元素存入矩阵的b行
//Store the elements in vector a into row b of the matrix

void Mat::init_row(vector<ll128> a, int b) {
    if(a.size() >= c && b >= 0 && b < r)
    {
        for (int i = 0; i < c; i++) {
            operator()(b, i) = a[i];
        }
    }
    else
    {
        DBGprint("Function init_row The row input is out of range or the vector is too small\n");
    }
}

// 将矩阵前 a*b个元素取出 组成 a行b列的矩阵返回(按行取)
//Take out a*b elements in the original matrix and compose a a*b matrix return this matrix(take by row) 

Mat Mat::resize(int a, int b) {
    if(a >= 1 && b >= 1 && a * b <= r * c)
    {
        Mat ret(a, b);
        vector<ll128> temp;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++)
                temp.push_back(get(i, j));
        int l = 0;
        for (int i = 0; i < a; i++)
            for (int j = 0; j < b; j++) {
                ret(i, j) = temp[l++];
            }
        return ret;
    }
    else
    {
         DBGprint("Function resize The input is out of range\n");
    }   
    
}

//取出矩阵每一列元素的最大值 存入矩阵并返回
// Take the maximum element of each column of the matrix, store it in the matrix and return the matrix 

Mat Mat::argmax() {
    Mat ret(1, c);
    for (int i = 0; i < c; i++) {
        int k = 0;
        for (int j = 1; j < r; j++)
            if (Constant::Util::get_sign(get(j, i)) > Constant::Util::get_sign(get(k, i)))
                k = j;
        ret(0, i) = k;
    }
    return ret;
}

// 判断预测值与实际值是否相同, 由于连续值,误差小于1/2认为相同 对应处元素置1 返回结果矩阵
//Determine whether the predicted value is the same as the actual value. Because of the continuous value, 
//if the error is less than 1/2 the corresponding element is set to 1(be thought to same) and the result matrix is returned.

Mat Mat::equal(const Mat &a) {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            ll128 tmp = this->operator()(i,j) - a.get(i,j);
            tmp = (tmp % Config::config->MOD + Config::config->MOD) % Config::config->MOD;
            tmp = tmp > Config::config->MOD / 2 ? tmp - Config::config->MOD : tmp;
            tmp = tmp < 0 ? -tmp : tmp;
            ret.val[i] = tmp < Config::config->IE / 2 ? 1 : 0;
        }
//    for (int j = 0; j < 10; ++j) {
//        DBGprint("%d: %lld, %lld, %lld\n", j, (ll)val[j], (ll)a.val[j], (ll)ret.val[j]);
//    }
    return ret;
}

// 判断该矩阵与矩阵a是否相等,返回值为相同大小的矩阵，对于处元素相等置为1，否则置为0
//Judge whether the matrix and matrix a are equal, the return value is a matrix of the same size
//set to 1 for equal elements, otherwise set to 0

Mat Mat::eq(const Mat &a) {
    if(a.rows() == r && a.cols() == c)
    {
        Mat ret(r, c);
        int l = r * c;
        for (int i = 0; i < r; i++)
            for (int j = 0; j < c; j++) {
                ret.operator()(i,j) = (this->operator()(i,j) == a.get(i,j)) ? 1 : 0;
            }
        return ret;
    }
    else
    {
        DBGprint("Function eq The matrix input is illegal\n");
    }
}
    

// 将矩阵中的数符号化
//Convert the elements in the matrix to signed numbers

Mat Mat::getSign() {
    Mat ret;
    ret = *this;
    ret.sign();
    return ret;
}

//The element in the matrix do radical operations on the filed 

Mat Mat::sqrt() {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = Constant::Util::sqrt(val[i]);
    }
    return ret;
}

// 将矩阵中的每个数在域上进行求逆运算 返回对应矩阵(注：不是求逆矩阵)
//Invert each element in the matrix on the field and return the matrix(not inverse matrix)

Mat Mat::inverse() {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = Constant::Util::inverse(val[i], Config::config->MOD);
    }
    return ret;
}

//将矩阵中的每个数在域上做平方根然后倒数
//Take the square root of each number in the matrix on the field and then compute their reciprocal return the matrix

Mat Mat::sqrt_inv() {
    Mat ret(r, c, 1);
    Mat a(*this);
    int l = r * c;
    ll b = Config::config->SQRTINV;
    while (b != 0) {
        if (b & 1) {
            for (int i = 0; i < l; i++) {
                ret.val[i] = ret.val[i] * a.val[i];
                ret.val[i] %= Config::config->MOD;
            }
        }
        for (int i = 0; i < l; i++) {
            a.val[i] *= a.val[i];
            a.val[i] %= Config::config->MOD;
        }
        b >>= 1;
    }
    ret.residual();
    return ret;
}

// 将矩阵在域上做除2运算 返回结果矩阵
//Divide the matrix by 2 on the field return the matrix 

Mat Mat::divideBy2() {
    Mat ret(r, c);
    ll128 inv2 = Config::config->INV2;
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] * inv2;
    }
    ret.residual();
    return ret;
}

Mat Mat::relu() {
    Mat ret(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            ret(i, j) = Constant::Util::get_sign(get(i, j)) > 0 ? this->operator()(i, j) : 0;
    return ret;
}

Mat Mat::d_relu() {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = Constant::Util::get_sign(val[i]) > 0 ? 1 : 0;
    }
    return ret;
}

Mat Mat::sigmoid() const {
    Mat ret(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            ll128 u = this->get(i, j);
            u = Constant::Util::get_residual(u);
            ll128 u_add = u + Config::config->IE / 2;
            ll128 u_sub = u - Config::config->IE / 2;
            bool p, q;
            p = u < Config::config->MOD / 2;
            q = u > Config::config->MOD - Config::config->IE / 2;
            ret(i, j) = u_add * (!(!p & !q) ? 1 : 0);
            q = u > Config::config->IE / 2;
            ret(i, j) -= u_sub * ((p & q) ? 1 : 0);
            ret(i, j) = Constant::Util::get_residual(ret(i, j));
        }
    return ret;
}

Mat Mat::hard_tanh() {
    Mat ret(r, c);
    double temp, ans;
    ll128 temp_l;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            ll128 u = this->get(i, j);
            u = Constant::Util::get_residual(u);
            if (u > Config::config->MOD / 2)
                u = u - Config::config->MOD;
            temp = (u * 1.0) / Config::config->IE;
            if (temp > 1)
                ans = 1;
            else if (temp < -1)
                ans = 0;
            else ans = (temp + 1) / 2;
            temp_l = ans * Config::config->IE;
            temp_l = Constant::Util::get_residual(temp_l);
            ret(i, j) = temp_l;
            /*ll128 u_add = u/2 + IE / 2;
            ll128 u_sub = u/2 - IE / 2;
            bool p, q;
            p = u < Config::config->MOD / 2;
            q = u > Config::config->MOD - IE ;
            ret(i, j) = u_add * (!(!p & !q) ? 1 : 0);
            q = u > IE ;
            ret(i, j) -= u_sub * ((p & q) ? 1 : 0);
            ret(i, j) = Constant::Util::get_residual(ret(i, j));*/

        }
    return ret;
}

Mat Mat::tanh() {
    Mat ret(r,c);
    //const ll128 coefficients[10]={10,0,99999999999999351,0,15392,0,99999999999835840,0,971505,0};
    const ll128 coefficients[10]={10,0,-652,0,15392,0,-164163,0,971505,0};
    const double coeffi[10]={9.55107240406973E-6, 0.0, -6.214997173868465E-4, 2.168404344971009E-18, 0.014678550658336058, 2.7755575615628914E-17, -0.15655792147527794, 1.6653345369377348E-16, 0.9264993039978429, 2.8449465006019636E-16 };
    ll128 temp;
    for (int i=0;i<r;i++)
        for (int j=0;j<c;j++){
            ll128 u=this->get(i,j);
            temp=0;
            u = Constant::Util::get_residual(u);
            for (int k=0;k<9;k++){
                temp=(temp+coefficients[k])*u/Config::config->IE;
                temp=Constant::Util::get_residual(temp);
            }
            ret(i,j)=(temp+Config::config->IE)/2;
        }
    return ret;
}

//模拟SecureML中的激活函数（大于1/2置为1，（-1/2,1/2）置为x+1/2，小于-1/2置为0）
//Simulate the activation function in SecureML(more than 1/2 set 1) ((-1/2,1/2) set x+1/2) (less than -1/2 set 0)

Mat Mat::cross_entropy() {
    Mat ret(r, c);
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            ll128 u = this->get(i, j);
            u = Constant::Util::get_residual(u);
            ll128 u_add = u + Config::config->IE / 2;
            ll128 u_sub = u - Config::config->IE / 2;
            bool p, q;
            p = u < Config::config->MOD / 2;
            q = u > Config::config->MOD - Config::config->IE / 2;
            ret(i, j) = u_add * (!(!p & !q) ? 1 : 0);
            q = u > Config::config->IE / 2;
            ret(i, j) -= u_sub * ((p & q) ? 1 : 0);
            ret(i, j) = Constant::Util::get_residual(ret(i, j));
        }
    return ret;
}

Mat Mat::LTZ() {
    Mat ret(r,c);

    for (int i=0;i<r;i++)
        for (int j=0;j<c;j++){
            ll128 u=this->get(i,j);
            u = Constant::Util::get_residual(u);
            if (u>Config::config->MOD/2)
                ret(i,j)=Config::config->IE;
            else
                ret(i,j)=0;
        }
    return ret;
}
Mat Mat::chebyshev_tanh() {
    Mat ret(r,c);
    //const ll128 coefficients[10]={10,0,99999999999999351,0,15392,0,99999999999835840,0,971505,0};
    //const ll128 coefficients[10]={10,0,-652,0,15392,0,-164163,0,971505,0};
    const double coeffi2[10]={9.55107240406973E-6, 0.0, -6.214997173868465E-4, 2.168404344971009E-18, 0.014678550658336058, 2.7755575615628914E-17, -0.15655792147527794, 1.6653345369377348E-16, 0.9264993039978429, 2.8449465006019636E-16 };
    const double coeffi3[5]={0.00901040601686517,-0.04476498296490821,0.1303347756923826,-0.33297383540025915,0.9999928678521409};
    const double coeffi[15]={-5.323116509545539E-23,3.2676296605006156E-8,5.050611498014173E-21,-3.144710363396315E-6,-1.5219403675173483E-19,1.227163507410755E-4,3.957640767317014E-18,-0.0025008607932465025,-6.516689962623843E-17,0.02875331197983296,1.8401105450910262E-16,-0.19296543477430353,-6.161469801253599E-16,0.9260667562220247,3.1636718852538824E-16};
    const double coeffi4[10]= {0.00901040601686517,-3.0184188481996443E-16,-0.04476498296490821,5.672545766444159E-16,0.1303347756923826,-2.2724877535296173E-16,-0.33297383540025915,-1.0289078616887437E-16,0.9999928678521409,1.6903389495406818E-017};
    const double coeffi5[3]={0.07838827741822856,-0.31509828453372835,0.9990092444928546};
    const double coeffi6[10]={4.2365028926709517E-5,2.710505431213761E-20,-0.0018009317623920522,8.673617379884035E-18,0.028283792450319457,-5.551115123125783E-17,-0.20942779769908448,8.326672684688674E-17,0.9649012890871488,-7.632783294297951E-17};
    const double coeffi7[2]={-0.2350385836885359,0.988304607980704};
    double temp,ans;
    ll128 temp_l;
    for (int i=0;i<r;i++)
        for (int j=0;j<c;j++){
            ll128 u=this->get(i,j);
            u = Constant::Util::get_residual(u);
            if (u>Config::config->MOD/2)
                u=u-Config::config->MOD;
            temp=(u*1.0)/Config::config->IE;
            if (temp>1)
                ans=1;
            else if (temp<-1)
                ans=0;
            else {
                ans = coeffi3[0];
                for (int k = 1; k < 5; k++) {
                    ans = coeffi3[k] + ans * temp*temp;
                }
                ans*=temp;
                ans = (ans + 1) / 2;
            }
            temp_l=ans*Config::config->IE;
            temp_l=Constant::Util::get_residual(temp_l);
            ret(i,j)=temp_l;
        }
    return ret;
}

Mat Mat::raw_tanh(){
    Mat ret(r,c);
    double temp;
    ll128 temp_l;
    for (int i=0;i<r;i++)
        for (int j=0;j<c;j++){
            ll128 u=this->get(i,j);
            u = Constant::Util::get_residual(u);
            if (u>Config::config->MOD/2)
                u=u-Config::config->MOD;
            temp=(u*1.0)/Config::config->IE;
            temp=(exp(temp)-exp(-temp))/(exp(temp)+exp(-temp));
            //temp=(temp+1)/2;
            temp_l=temp*Config::config->IE;
            temp_l= Constant::Util::get_residual(temp_l);

            ret(i,j)=temp_l;
        }
    return ret;
}

Mat Mat::not_eqz() const {
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] != 0 ? 1 : 0;
    }
    return ret;
}

// 从矩阵中取出[st,ed)行 赋值给新矩阵并返回
// Take [st,ed) row from matrix, assign to new matrix and return

Mat Mat::row(int st, int ed) const {
    if(st >= 0 && st <= r && ed >= 0 && ed <= r && (ed - st) % r != 0)
    {
        Mat ret((ed - st + r) % r, c);
        if (order == Config::config->MatColMajor || 1) {
            if (st < ed) {
                int tmp_r = ed - st;
                for (int i = 0; i < c; i++) {
                    copy(val.begin() + i * r + st, val.begin() + i * r + st + tmp_r, ret.val.begin() + i * tmp_r);
                }
            } else {
                int ret_r = (ed - st + r) % r;
                int tmp_r = r - st;
                for (int i = 0; i < c; i++) {
                    copy(val.begin() + i * r + st, val.begin() + i * r + st + tmp_r, ret.val.begin() + i * ret_r);
                    copy(val.begin() + i * r, val.begin() + i * r + (ret_r - tmp_r), ret.val.begin() + i * ret_r + tmp_r);
                }
            }
        } else {
            if (st < ed) {
                ret.val.assign(val.begin() + st * c, val.begin() + ed * c);
            } else {
                ret.val.assign(val.begin() + st * c, val.end());
                ret.val.insert(ret.val.end(), val.begin(), val.begin() + ed * c);
            }
            ret.order = order;
            ret.reorder();
        }
        return ret;
    }
    else
    {
        DBGprint("Function row(int st, int ed) The input is out of range\n");
    }
}


Mat Mat::col(int st, int ed) const {
    Mat ret(r, (ed - st + c) % c);
    ret.val.assign(val.begin() + st * r, val.begin() + ed * r);
    return ret;
}

// 对矩阵中每个元素进行操作,若该元素第b位(最低位为第1位)为1则该元素置1,为0则置0
// Operate on each element in the matrix If the b-th position of the element is 1, then the element is set to 1,otherwise set to 0
// (The lowest bit is the first bit)

Mat Mat::get_bit(int b) const {    
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] >> b & 1;
    }
    return ret;
}

// 将矩阵中每个元素在域上取相反数,返回结果矩阵
// Take the opposite number of each element in the matrix on the field   

Mat Mat::opposite() const {         
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = Config::config->MOD - val[i];
    }
    return ret;
}

//Mat Mat::LTZ() {
//    Mat ret(r,c);
//
//    for (int i=0;i<r;i++)
//        for (int j=0;j<c;j++){
//            ll128 u=this->get(i,j);
//            u = Constant::Util::get_residual(u);
//            if (u>Config::config->MOD/2)
//                ret(i,j)=IE;
//            else
//                ret(i,j)=0;
//        }
//    return ret;
//}

Mat Mat::SmoothLevel() {
    Mat ret(r,c);

    for (int i=0;i<r;i++)
        for (int j=0;j<c;j++){
            ll128 u=this->get(i,j);
            u = log(Config::config->IE/Constant::Util::get_residual(u));
            if (u>Config::config->MAX_SMOOTHING_LEVEL) {
                u = Config::config->MAX_SMOOTHING_LEVEL;
            }
            ret(i,j)=u;
        }
    return ret;
}

Mat Mat::toOneHot() const {
    Mat ret(10, c);
    for (int i = 0; i < c; i++) {
        int y = val[i] >> Config::config->DECIMAL_PLACES;
        ret(y, i) = Config::config->IE;
    }
    return ret;
}

// 从该矩阵中取[st,ed)列赋值给a
// Take [st,ed) column from the matrix and assign it to a

void Mat::col(int st, int ed, Mat &a) const {
    if(st >= 0 && ed <= c && st <= ed && a.size() >= ((ed - st) * r) && a.rows() == r)
        a.val.assign(val.begin() + st * r, val.begin() + ed * r);
    else
        DBGprint("Function col The input is out of range\n");
}

void Mat::append(int st, int ed, Mat *a) {
//    a->val.insert(val.end(), st, ed);
//    for (int i = 0; i < c; ++i) {
//        a->setVal(i+st, getVal(i));
//    }z
    copy(val.begin(), val.end(), a->val.begin() + st);
}

// 将矩阵中每个元素对b取余数,返回结果矩阵
// Take each element in the matrix %b return the matrix

Mat Mat::mod(ll b) {     
    Mat ret(r, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret.val[i] = val[i] % b;
    }
    return ret;
}


ll Mat::count_sum() {
    ll ret = 0;
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret += val[i];
    }
    return ret;
}

// 计算矩阵中等于1的元素的个数 返回结果值
// Count the number of elements equal to 1 in the matrix return the result

int Mat::count() {
    int ret = 0;
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret += val[i] == 1;
    }
    return ret;
}

int Mat::count(const ll128 &b) {
    int ret = 0;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (get(i, j) == b)
                ret++;
    return ret;
}

int Mat::countNe(const ll128 &b) const {
    int ret = 0;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            if (get(i, j) != b)
                ret++;
    return ret;
}

int Mat::count_not_eqz() const {
    int ret = 0;
    int l = r * c;
    for (int i = 0; i < l; i++) {
        ret += val[i] != 0 ? 1 : 0;
    }
    return ret;
}

bool Mat::is_zero() {
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            if (this->operator()(i, j) == 0)
                return 1;
        }
    return 0;
}

bool Mat::fill(const Mat &a) {
    int k = 0;
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++) {
            while (a.get(0, k) == 0)
                k++;
            this->operator()(i, j) = a.get(0, k);
            k++;
        }
    return 1;
}

void Mat::init(const ll128 &b) {
    int l = r * c;
    for (int i = 0; i < l; i++) {
        val[i] = b;
    }
}

void Mat::cp(const Mat &a, int st, int len) {
    copy(a.val.begin() + st, a.val.begin() + st + len, val.begin());
}

void Mat::cp(const Mat &a, const Mat &mask) {
    int l = r * c;
    int j = 0;
    for (int i = 0; i < l; i++) {
        if (mask.val[i] != 0) {
            val[j++] = a.val[i];
        }
    }
}

// 将矩阵中的每个数映射到域上
// Map each number in the matrix to the field

void Mat::residual() {
    int l = r * c;
    for (int i = 0; i < l; i++) {
        val[i] = (val[i] % Config::config->MOD + Config::config->MOD) % Config::config->MOD;
    }
}



void Mat::AddDot(int k, ll128 *x, int incx, ll128 *y, ll128 *gamma) {
    int p;
    for (p = 0; p < k; p++) {
        *gamma += x[p * incx] * y[p];
    }
}

// 将矩阵中的数转换成有符号数
// Convert the number in the vector to a signed number

void Mat::sign() {
    int l = r * c;
    for (int i = 0; i < l; i++) {
        val[i] = (val[i] > Config::config->MOD / 2) ? val[i] - Config::config->MOD : val[i];
    }
}

// Convert row storage to column storage or Convert column storage to row storage

void Mat::reorder() {         //将行存转为列存或者列存转为行存
    vector<ll128> v(r * c);
    int tmp_r = r;
    int tmp_c = c;
    if (order == Config::config->MatRowMajor) {
        swap(tmp_r, tmp_c);
    }
    for (int i = 0; i < tmp_r; i++) {
        for (int j = 0; j < tmp_c; j++) {
            v[i * tmp_c + j] = val[j * tmp_r + i];
        }
    }
    val.assign(v.begin(), v.end());
    order ^= 1;
}

// Tranpose the matrix and change the storage method

void Mat::transorder() {    //将矩阵转置并改变排序方式
    swap(r, c);
    order ^= 1;
}

void Mat::truncated_normal(double mean, double stddev) {
    for (int i = 0; i < r; i++)
        for (int j = 0; j < c; j++)
            this->operator()(i, j) = floor(generateGaussianNoise(mean, stddev) * Config::config->IE);
}

void Mat::constant(double b) {
    int l = r * c;
    for (int i = 0; i < l; i++) {
        val[i] = b;
    }
}

void Mat::col(int u, vector<ll128> &a) {
    for (int i = 0; i < r; i++)
        this->operator()(i, u) = a[i];
}

// Clear the matrix and set all to 0

void Mat::clear() {    //矩阵清空全部置0
    val = vector<ll128>(r * c);
}

// Output the matrix in matrix form if a column vector output as a line

void Mat::print() const {    //将矩阵按矩阵形式输出，如为列向量则输出成一行
    DBGprint("r: %d c: %d\n", r, c);
    if (c == 1) {
        for (int i = 0; i < r; i++) {
            DBGprint("%lld ", (ll) get(i, 0));
        }
        DBGprint("\n");
        return;
    }
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            DBGprint("%lld ", (ll) get(i, j));
        }
        DBGprint("\n");
    }
}

void Mat::printSign() {
    if (c == 1) {
        for (int i = 0; i < r; i++) {
            DBGprint("%lld ", (ll) Constant::Util::get_sign(get(i, 0)));
        }
        DBGprint("\n");
        return;
    }
    DBGprint("r: %d c: %d\n", r, c);
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            DBGprint("%lld ", (ll) Constant::Util::get_sign(get(i, j)));
        }
        DBGprint("\n");
    }
}

void Mat::toString(char *p) {
    Constant::Util::ll_to_char(p, r);
    *p++ = ' ';
    Constant::Util::ll_to_char(p, c);
    *p++ = ' ';
    int l = r * c;
    for (int i = 0; i < l; i++) {
        Constant::Util::ll_to_char(p, val[i]);
        *p++ = ' ';
    }
    *p = 0;
}

// Read the matrix into char *  return the size of matrix(include row and column of matrix)

int Mat::toString_pos(char *p) const {      // 将矩阵读入char *中,返回矩阵长度（包括矩阵的行和列）
    Constant::Util::int_to_char(p, r);
    Constant::Util::int_to_char(p, c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        Constant::Util::ll_to_char(p, val[i]);
    }
    *p = 0;
    return 2 * 4 + r * c * 8;
}

// Return the size of matrix(include row and column of matrix)

int Mat::getStringLen() {     // 返回矩阵的长度（包括矩阵的行和列）
    return 2 * 4 + r * c * 8;
}

// Read the matrix from char *

void Mat::getFrom_pos(char *&p) {    // 从char *中读出矩阵
    r = Constant::Util::char_to_int(p);
    c = Constant::Util::char_to_int(p);
    val.resize(r * c);
    int l = r * c;
    for (int i = 0; i < l; i++) {
        val[i] = Constant::Util::char_to_ll(p);
    }
}

// Add matrix(taken from p) to the matrix

void Mat::addFrom_pos(char *&p) {     // 将该矩阵加上从p中取出的矩阵
    int tmp_r, tmp_c;
    tmp_r = Constant::Util::char_to_int(p);
    tmp_c = Constant::Util::char_to_int(p);
    int l = r * c;
//    cout << "l: " << l << endl;
    for (int i = 0; i < l; i++) {
        val[i] += Constant::Util::char_to_ll(p);
        val[i] = val[i] >= Config::config->MOD ? val[i] - Config::config->MOD : val[i];
        val[i] = val[i] <= -Config::config->MOD ? val[i] + Config::config->MOD : val[i];
    }
}

// 使用a_r矩阵中的非0元素填充a（按元素顺序逐一填充 遇0则选择下一个），如果未能完全填充则返回0，完全填充返回1，同时使用b_r填充b，填充元素顺序和a完全对应
// Fill a with the non-zero elements in the a_r matrix (fill one by one in the order of elements, and choose the next one if it meets 0)
// if it fails to fill completely, return 0, completely fill it returns 1
// and use b_r to fill b at the same time, fill the element order and a completely correspond

bool Mat::fill(Mat *a, Mat *a_r, Mat *b, Mat *b_r) {
    if(b_r->size() >= a_r->size())
    {
        int l = a->rows() * a->cols();
        int l_r = a_r->rows() * a_r->cols();
        int k = 0;
        for (int i = 0; i < l; i++) {
            while (k < l_r && a_r->val[k] == 0) {
                k++;
            }
            if (k >= l_r) {
                return 0;
            }
            a->val[i] = a_r->val[k];
            b->val[i] = b_r->val[k];
            k++;
        }
        return 1;
    }
    else
    {
        DBGprint("Function fill The input may lead to out-of-bounds\n");
        return 0;
    }    
    
}

// 将a矩阵插入res后再将b矩阵插入res 即res为a矩阵和b矩阵合并后的结果
// Insert a matrix into res and then insert b matrix into res(res is the result of combining a matrix and b matrix)

void Mat::concat(Mat *res, Mat *a, Mat *b) {
    if(res->rows() == (a->rows() + b->rows()) && res->cols() == a->cols() && res->cols() == b->cols())
    {
        for (int i = 0; i < a->rows(); i++)
            for (int j = 0; j < a->cols(); j++)
                res->operator()(i, j) = a->operator()(i, j);
        for (int i = 0; i < b->rows(); i++)
            for (int j = 0; j < b->cols(); j++)
                res->operator()(i + a->rows(), j) = b->operator()(i, j);
    }
    else
    {
        DBGprint("Function concat The input matrix format is illegal\n");
    }    
    
}

// 拆分res矩阵 分别加上给a和b if(fa)将res矩阵前矩阵a大小的矩阵加上给a if(fb)将res中矩阵a大小之后b矩阵大小的矩阵加上给b(紧接着a矩阵之后)
// Split the res matrix and assign it to a and b if (fa) assign the matrix (size of matrix a) of res matrix add to a
// if (fb) assign the matrix (size of matrix b after matrix a) of res add to b (following matrix a) 

void Mat::reconcat(Mat *res, Mat *a, bool fa, Mat *b, bool fb) {
    if(res->rows() == (a->rows() + b->rows()) && res->cols() == a->cols() && res->cols() == b->cols())
    {
         if (fa) {
            for (int i = 0; i < a->rows(); i++)
                for (int j = 0; j < a->cols(); j++)
                    a->operator()(i, j) += res->operator()(i, j);
        }
        if (fb) {
            for (int i = 0; i < b->rows(); i++)
                for (int j = 0; j < b->cols(); j++)
                    b->operator()(i, j) += res->operator()(i + a->rows(), j);
        }
    }
    else
    {
        DBGprint("Function reconcat The input matrix format is illegal\n");
    }
    
}

void Mat::vstack(Mat *res, Mat *a, Mat *b) {}

void Mat::re_vstack(Mat *res, Mat *a, bool fa, Mat *b, bool fb) {}

//将矩阵a赋值给矩阵res 然后将矩阵b插入矩阵res
//Assign matrix a to matrix res and insert matrix b into matrix res

void Mat::hstack(Mat *res, Mat *a, Mat *b) {
    if(res->size() == (a->size() + b->size()))
    {
        res->val.assign(a->val.begin(), a->val.end());
        res->val.insert(res->val.end(), b->val.begin(), b->val.end());
    }
    else        
   {
       DBGprint("Function hstack The input matrix format is illegal\n");
   }
}

// if(fa)将矩阵res的前矩阵a大小的矩阵赋值给a if(fb)将res中矩阵a大小之后的矩阵b大小的矩阵赋值给b
//if(fa) assign the matrix (size of matrix a) of res to a 
//if(fb) assign the matrix (size of matrix b after matrix a) in res to b

void Mat::re_hstack(Mat *res, Mat *a, bool fa, Mat *b, bool fb) {
    if(res->size() == (a->size() + b->size()))
    {
        int len_a = a->rows() * a->cols();
        int len_b = a->rows() * a->cols();
        if (fa) {
            a->val.assign(res->val.begin(), res->val.begin() + len_a);
        }
        if (fb) {
            b->val.assign(res->val.begin() + len_a, res->val.end());
        }
    }
    else
    {
        DBGprint("Function re_hstack The input matrix format is illegal\n");
    }
}

//判断a和b的存储方式,返回值为0表示列存和列存,为1表示列存和行存,为2表示行存和列存,为3表示行存和行存
//Determine the storage mode of a and b. 
//The return value is 0 for column storage and column storage, 
//1 for column storage and row storage, 
//2 for row storage and column storage, 
//3 for row storage and row storage.

int Mat::pair_order_type(Mat *a, const Mat *b) {
    return (a->order << 1) + b->order;
}

//产生一个随机数矩阵和a的大小相同，并赋值给a
//Generate a random matrix with the same size as a and assign it to a

void Mat::random_neg(Mat *a) {      
    int r = a->rows();
    int c = a->cols();
    Mat ret(r, c);
    for (int i = 0; i < r*c; i++) {
        ret.val[i] = Constant::Util::randomlong() % Config::config->IE;
        if (rand() % 2)
        {
            ret.val[i] = Config::config->MOD - ret.val[i];
        }
    }
    *a = ret;
}