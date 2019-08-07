The scamp5_main.cpp present in this directory contains a demonstration of the principle of unrolling matrix-vector multiplication loop, for speeding up fully connected layers computations. A fully connected layer indeed boils down to multiplying a matrix with a vector, and adding a vectorized bias.

The trick we present here can be seen as a pre-compilation optimisation, and saves a considerable fraction of the total time needed for one forward pass of our CNNs.

*Note: in all the discussed examples below, we use a 2 layer fully connected network, that takes vectors of length 27 as inputs, has 50 hidden units, and 10 output units*

## Background: profiling AnalogNet, with traditional matrix-vector multiplication

Here is the breakdown of the elapsed time during one forward pass of AnalogNet on the SCAMP5 device:
| Step                                   | Time taken (micro-seconds) | Total elapsed time (micro-seconds) |
|----------------------------------------|----------------------------|------------------------------------|
| Input binarisation                     | 13                         | 13                                 |
| 3 Convolutions and output binarisation | 68                         | 81                                 |
| Events reading and binning             | 273                        | 354                                |
| Fully connected (2 layers)             | 389                        | 743                                |

As we can see, a considerable amount of time is spent gathering events (ie. transferring data from the vision chip to the micro-controller), and computing the fully connected layers result. The former cannot really be improved, as we use the already optimised official SCAMP5 library to interface with the analog vision chip. However, there is room for improvement on the latter point, since the legacy AnalogNet only implements traditional double loops for matrix-vector multiplication.

```cpp
code
array[][]
void function(in, out, k, b){
	for(){
		for(){
			...
		}
	}
}
int main(){
	...
	function(...);
}
```

Note: we scripted the generation of C++ code for defining int arrays, based on numpy objects (see Python [code generation/weights_pck_to_cpp_unrolled_loop.py](code generation/weights_pck_to_cpp_unrolled_loop.py)).

The output we get from the SCAMP5 device with this explicit loop is the following:
```
It took 389 usec to run the FC network.
FC result: -95993,157554,145256,94030,216396,-100540,20355,-29113,66564,-212817,
```

## First step: unrolling the loop

Here we can take advantage of two things:
 * the fact we know in advance (at compilation time), the number of iterations needed in each loop. This is because our vectors are of fixed size.
 * the fact that we do not use all the 512KB available for storing the program and constant data.

Based on these two points, we propose to unroll the loop before compilation. Using a python script, we generate C++ code that corresponds to the linear program equivalent to the loop presented above.

The generation of the code is scripted (see Python [code generation/weights_pck_to_cpp_unrolled_loop.py](code generation/weights_pck_to_cpp_unrolled_loop.py)), and the user only has to copy / paste the corresponding output code. The length of our program is considerably increased, but still within our limit of 512 KB.


This is what the C++ code now looks like:
```c
code
array[][]
void function(in, out, k, b){
	for(){
		for(){
			...
		}
	}
}
int main(){
	...
	function(...);
}
```

Output with unrolled loop and matrix array:
```
It took 374 usec to run the FC network.
FC result: -95993,157554,145256,94030,216396,-100540,20355,-29113,66564,-212817,
```

## Second step: hardcoding the weights
Output with unrolled loop and hardcoded weights:
```
It took 109 usec to run the FC network.
FC result: -95993,157554,145256,94030,216396,-100540,20355,-29113,66564,-212817,
```

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





