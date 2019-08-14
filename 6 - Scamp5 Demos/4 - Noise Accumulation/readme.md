The code present in this directory aims at collecting quantitative evidence of the accumulation of noise when running analog computations on the SCAMP5 device.

## Data collection
The code present in [scamp5_main.cpp](./scamp5_main.cpp) runs computations on the SCAMP5 device to produce results that are subject to real noise, in a controlled manner.

We repeatedly apply the following kernel on uniform inputs:
```cpp
scamp5_kernel id_kernel([]{
    using namespace scamp5_kernel_api;
  diva(A,E,F);
  mov(F,A);
  add(A,A,F);
});
```
This SCAMP5 kernel should normally be equivalent to the identity function: the content of the AREG A is divided by two, copied to AREG F and added to itself. Division by 2, copy and addition are 3 operations that are commonly used in our convolution kernels produced by AUKE.

Our program creates uniform inputs, and applies this id_kernel increasingly many times (or iterations). For statistical relevance, this operation is repeated 100 times.

More precisely, for a given INPUT_VALUE (in the (-127,+127) range), and a given PRE_ITERATIONS value, this program will:
```
for iteration ranging from 1 to 8:
  for k ranging from 1 to 100:
    create input, with uniform value INPUT_VALUE
    apply id_kernel (PRE_ITERATIONS+iteration) times on input, to create result
```

The resulting images can be saved within the host application. We use this to collect 100 input/result pairs for each input value in [0, 5, 10, 60, 100, 120], for iterations ranging from 1 to 32 (2\*100\*6\*32=38400 images). We believed it important to not only save results, but inputs too, as they also are subject to random noise.

## Metrics computations

## Results

