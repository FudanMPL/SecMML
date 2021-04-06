# SecMML

## Introduction
  SecMML, a branch of FudanMPL (<u>M</u>ulti-<u>P</u>arty Computation + Machine <u>L</u>earning) , is a scalable and efficient MPC framework for training machine learning models based on BGW protocol.
It has the generality to be extended in the application scenarios of three+ parties in both semi-honest and malicious (todo) settings .
Currently, SecMML is able to support several popular machine learning models, including linear regression, logistic regression, BP neural networks and LSTM neural networks.

## Application scenario

There are two practical situations as follow:

1. As the following figure shows, several companies hold their own data sets respectively and want to train a better model on their union data sets wihtout sharing the plaintext of their datasets. At first, they share their data to other parties in a secret sharing manner. In this way, each party has a share of the entire data set. Then, as a party, each company trains the model collaboratively.  Our framework is extensible to support arbitrary number of participants (three+) to train models on the entire data set composed of the data they hold. 

    ![scenario](scenario_sample.png)

2. There are a large number of individual data owners and they do not want their private data to be known by others. Internet companies want to make use of these distributed data to acquire better models. These companies may ﬁrst specify several servers to perform the computation and these servers must be independent of each other. All data owners then send their data to these servers in secret sharing manner. The servers collaboratively train the model with these data and the trained model is ﬁnally revealed to the data owners. The scalability of the framework is that it can support any number of data owners, and any number of servers can be selected as computing parties. 

## Repository Structure

* `core/`: Core libraries in MPL. The fundamental matrix lib, math operator lib and Player lib.  Some math computations are compiled as libraries (libcore_lib.so).

* `machine_learning/`: Machine learning algorithms: neural networks, linear regression and logistic regression.

* `datesets/mnist/`: Training dataset.

* `util/`: Data IO and network IO package. The network is implemented using socket, compatible on both Windows and Ubuntu.

* `Constant.h`: Some constants and general functions in SecMML. Note that, for windows users, the macro `UNIX_PLATFORM` should be defined to use the `winsocket` library.

* `CMakeLists.txt`: Define the compile rule for the project. Note that, for windows users, the `target_link_libraries(SMMLF ws2_32)` shall be uncommented.

## Running

Here take training a linear regression model among three parties as an example

* Clone the SecMML git repository by running:

  `git clone https://github.com/SMMLF/MPL-Public.git`

* Set the number of parties to 3 (in `Constant.h`. Note that, M can be any arbitrary number >= 3):
  
  `#define M 3`

* Specify the platform:
    - if Ubuntu  (in `Constant.h`)
    
            `#ifndef UNIX_PLATFORM`
    
            `#define UNIX_PLATFORM`
            
            `#endif`
    - if Windows (in `CMakeLists.txt`):

            Add `target_link_libraries(SMMLF ws2_32)` to the file.

* Choose the machine learning model (`main.cpp`):
    - Linear Regression Model: bp->linear_graph();
    - Logistic Regression Model: bp->logistic_graph();
    - Three-layer Model: bp->graph();
- LSTM
    
* Compile the executable file:
    - `cd MPL-Public`
    - `cmake .`
    - `make`

* Start three processes and input the party index, respectively:
    - `./MPL`
    - `Please enter party index:`
    - Enter 0,1,...,M for each process in order.

## Help

Any question, please contact 19212010008@fudan.edu.cn.

## Contributor

**Faculty**: Prof. Weili Han

**Students**: Haoqi Wu (Graduate Student), Zifeng Jiang (Graduate Student), Wenqiang Ruan (Ph.D Candidate), Lushan Song (Ph.D Candidate), Dingyi Tang (Post Graduate Student)



