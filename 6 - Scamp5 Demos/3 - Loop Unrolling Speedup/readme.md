The scamp5_main.cpp present in this directory contains a demonstration of the principle of unrolling matrix-vector multiplication loop, for speeding up fully connected layers computations. A fully connected layer indeed boils down to multiplying a matrix with a vector, and adding a vectorized bias.

The trick we present here can be seen as a pre-compilation optimisation, and saves a considerable fraction of the total time needed for one forward pass of our CNNs.

Notes:
 * in all the discussed examples below, we use a 2 layer fully connected network, that takes vectors of length 27 as inputs, has 50 hidden units, and 10 output units
 * when reporting an execution time, we only report one figure, and do not bother doing an average of multiple ones. This is because the execution time of a given portion of code is extremely stable on the SCAMP5 vision system, and can be considered as constant.

## Background: profiling AnalogNet, with traditional matrix-vector multiplication

Here is the breakdown of the elapsed time during one forward pass of AnalogNet on the SCAMP5 device, after an image is captured:

| Step                                   | Time taken (micro-seconds) | Total elapsed time (micro-seconds) |
|----------------------------------------|----------------------------|------------------------------------|
| Input binarisation                     | 13                         | 13                                 |
| 3 Convolutions and output binarisation | 68                         | 81                                 |
| Events reading and binning             | 273                        | 354                                |
| Fully connected (2 layers)             | 389                        | 743                                |

As we can see, a considerable amount of time is spent gathering events (ie. transferring data from the vision chip to the micro-controller), and computing the fully connected layers result. The former cannot really be improved, as we use the already optimised official SCAMP5 library to interface with the analog vision chip.
However, there is room for improvement on the latter point, since the legacy AnalogNet only implements traditional double loops for matrix-vector multiplication. The current and standard implementation is in the form of:

```cpp
int weights[IN_SIZE][OUT_SIZE] = {{...},{...},...};
int biases[OUT_SIZE] = {...};

void fc_computation(const int weights[IN_SIZE][OUT_SIZE], const int biases[OUT_SIZE],
              int in[IN_SIZE], int out[OUT_SIZE]){
  long int partialSum;
  for (int j=0; j<OUT_SIZE; j++){
    partialSum = 0;
    for (int i=0; i<IN_SIZE; i++){
      partialSum += in[i]*weights[i][j];
    }
    out[j] = partialSum + biases[j];
  }
}

int main(){
  int input[IN_SIZE], output[OUT_SIZE];
  ...
  fc_computation(weights, biases, input, output);
  ...
}
```

We scripted the generation of C++ code for defining int arrays, based on numpy objects (see [Python code generation/weights_pck_to_cpp_unrolled_loop.py](./Python code generation/weights_pck_to_cpp_unrolled_loop.py)). This script takes a numpy 2 dimensional array as input that corresponds to the weights matrix of one of the fully connected layers, and simply parse it into a valid C++ code. This can then be directly copied and pasted into our SCAMP5 source code.


For our demonstration purpose, we have here isolated this fully connected network part. The program in [scamp5_main.cpp](./scamp5_main.cpp) implements our 2 layer network, with a vector of length 27 as input, 50 hidden units, and 10 output units. The weights, biases and input vector are constant, and their values correspond to typical weights of an AnalogNet architecture.

This explicit loop, corresponding to what is presented above, can be executed setting
```cpp
#define FC_COMPUTATION_TYPE 0
```

The output we then get from the SCAMP5 device is the following:
```
It took 389 usec to run the FC network.
FC result: -95993,157554,145256,94030,216396,-100540,20355,-29113,66564,-212817,
```

## First step: unrolling the loop

In our case, we can take advantage of two things:
 * the fact we know in advance (at compilation time), the number of iterations needed in each loop. This is because our vectors are all of fixed size.
 * the fact that we do not use all the 512KB available for storing the program and constant data.

Based on these two points, we propose to unroll the loop before compilation. Using a python script, we generate C++ code that corresponds to the linear program equivalent to the loop presented above.

Again, the code generation is scripted (see Python [code generation/weights_pck_to_cpp_unrolled_loop.py](code generation/weights_pck_to_cpp_unrolled_loop.py)), and the user only has to copy and paste the corresponding output code. The length of our program is considerably increased, but still largely within our limit of 512 KB.


This is what the C++ code now looks like:
```cpp
int weights[IN_SIZE][OUT_SIZE] = {{...},{...},...};
int biases[OUT_SIZE] = {...};

void fc_computation(int in[IN_SIZE], int out[OUT_SIZE]){
  out[0] =
    in[0]*(weights[0][0]) +
    ... +
    in[IN_SIZE-1]*(weights[IN_SIZE-1][0]) +
    (biases[0]);
  ...
  out[OUT_SIZE-1] =
    in[0]*(weights[0][OUT_SIZE-1]) +
    ... +
    in[IN_SIZE-1]*(weights[IN_SIZE-1][OUT_SIZE-1]) +
    (biases[OUT_SIZE-1]);
}

int main(){
  int input[IN_SIZE], output[OUT_SIZE];
  ...
  fc_computation(input, output);
  ...
}
```


For a demonstration of this, set
```cpp
#define FC_COMPUTATION_TYPE 1
```
in [scamp5_main.cpp](./scamp5_main.cpp).

The output we get with this unrolled loop is the following:
```
It took 374 usec to run the FC network.
FC result: -95993,157554,145256,94030,216396,-100540,20355,-29113,66564,-212817,
```

The output is effectively the same, and we are able to save 15 microseconds with this technique. This is not a substantial amount of time, but will be of great help for the next part.


## Second step: hardcoding the weights
Pushing the above idea even further, we can directly replace the weights by their value in the unrolled loop. Instead of declaring a global constant array for the weights and accessing them, we replace them by their actual value. This removes the overhead of accessing memory, and even frees some RAM.


This is what the C++ code now looks like (mock values for weights and biases):
```cpp
void fc_computation(int in[IN_SIZE], int out[OUT_SIZE]){
  out[0] =
    in[0]*(322) +
    ... +
    in[IN_SIZE-1]*(77) +
    (-47);
  ...
  out[OUT_SIZE-1] =
    in[0]*(18) +
    ... +
    in[IN_SIZE-1]*(27) +
    (20);
}

int main(){
  int input[IN_SIZE], output[OUT_SIZE];
  ...
  fc_computation(input, output);
  ...
}
```


For a demonstration of this, set
```cpp
#define FC_COMPUTATION_TYPE 2
```
in [scamp5_main.cpp](./scamp5_main.cpp).

Output with unrolled loop and hardcoded weights:
```
It took 109 usec to run the FC network.
FC result: -95993,157554,145256,94030,216396,-100540,20355,-29113,66564,-212817,
```

We get a very substantial speed improvement.

## Conclusion

In terms of speeding-up the fully connected layers computation, our last method allows for a drastic improvement:

| Method                            | Time for fully-connected computation (micro-seconds) | % of baseline |
|-----------------------------------|------------------------------------------------------|---------------|
| Standard loop (baseline)          | 389                                                  | 100           |
| Unrolled loop                     | 374                                                  | 96.1          |
| Unrolled loop + hardcoded weights | 109                                                  | 28.0          |


What really interests us is how it speeds up the total inference time of AnalogNet on the SCAMP5 vision system, of which the fully-connected computations is only a part. In that regard, we also notice a considerable improvement:


| Method                            | Time for AnalogNet forward pass (micro-seconds) | % of baseline |
|-----------------------------------|-------------------------------------------------|---------------|
| Standard loop (baseline)          | 743                                             | 100           |
| Unrolled loop                     | 728                                             | 98.0          |
| Unrolled loop + hardcoded weights | 463                                             | 62.3          |

