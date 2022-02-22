# SecMML TESTs

In this directory, we have designed several kinds of tests to figure out whether there exists bug in various classes.
To make sure that all users can use it correctly, guidances have been provided as below:

## Modify the path of datasets

As test files are restored in different path in comparison to main.cpp, we need to modify the constant.json file in root path:
Make sure that : 

...
"TRAIN_FILENAME": "../datasets/mnist/mnist_train.csv",
"TEST_FILENAME": "../datasets/mnist/mnist_test.csv",
...

And then, run these codes:
    - `cd SecMML/Test`
    - `cmake .`
    - `make`

In this way, you can test any classes as you like.