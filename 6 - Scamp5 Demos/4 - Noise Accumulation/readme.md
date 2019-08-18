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
Given one INPUT_VALUE, for each iteration value, we have at hand 100 input/result pair. Each pair is composed of 2 256\*256 arrays, saved as 8 bits greyscale .BMP files. The analysis of these images is done in [1-compute_psnr_std_bias.py](./1-compute_psnr_std_bias.py). We here describe how.

We assume a noise model that is composed of a constant bias and a centered random component, as follows:
```math
result = id\_kernel^{iteration}(input) = input + bias + random\_noise
```
with $`\mathbb{E}[random\_noise]=0`$.

### Systematic bias
Using the centered nature of the random noise, we simply compute the average of the mean difference between the inputs and the results to get an unbiased estimate of the bias:
```math
bias_{iteration}=\frac{1}{100}\left[\sum_{k=1}^{100}\frac{1}{256*256}\sum_{1\leq i,j\leq256}((input_{iteration,k})_{i,j}-(result_{iteration,k})_{i,j})\right]
```

### PSNR
A common measure of image degradation between a clean target image and its noisy version is the Peak Signal to Noise Ratio. It represents similarity between two images. Expressed in dB, the higher its value, the less noise is introduced by computations.

For a given input/result pair, the PSNR is calculated as:
```math
PSNR=20*log_{10}\left(\frac{max\_value}{mean\_square\_error}\right)=20*log_{10}\left(\frac{255}{\sqrt{\frac{1}{256*256}*\sum_{1\leq i,j\leq256}(input_{i,j}-result_{i,j}-bias)^{2}}}\right)
```

Notice that the mean square error is in fact calculated on images on which to systematic bias previously computed is compensated.

To get a more synthetic and meaningful result, we in fact only report the average of the 100 values computed for one INPUT_VALUE and iteration value.

### STD
To measure the random variability introduced by the stochastic noise, we computed its standard deviation accross one image.
```math
std=\sqrt{\frac{\sum_{1\leq i,j\leq256}\left(input_{i,j}-result_{i,j}-bias-mean\_diff\right)^{2}}{256*256-1}}
```
where
```math
mean\_diff=\frac{1}{256*256}*\sum_{1\leq i,j\leq256}(input_{i,j}-result_{i,j}-bias)
```
As is done with the PSNR metric, the 100 values of STD we compute for one iteration value are averaged.

## Results
Here are the figures we get:

![Systematic bias](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/raw/assets/assets/figure_bias.png?inline=false)

![Peak Signal to Noise Ratio](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/raw/assets/assets/figure_psnr.png?inline=false)

![Standard Deviation](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/raw/assets/assets/figure_std.png?inline=false)

We can notice that:
* the more we iterate our supposedly identity kernel, the more noise is introduced (PSNR decreases and STD increases).
* the systematic bias is dependent on the input value, in a seemingly increasing manner: the higher the original input value, the higher the systematic bias.


In addition to that, we can discern two phenomena that are slightly out of the general trend:
* with an input value of 120, there is a stronger than usual signal degradation when running the kernel between 6 and 9 times. This is shown by the drop in PSNR value and the surge in STD value for these iteration numbers.
* with an input of 60, the PSNR values seem considerably lower than with all other input values. However, as seen on the third figure, the input image we create contains a lot of internal variability (noise): the STD value for 0 iteration is anormally high. A further investigation shows this noise appears in the form of multiple spiking pixels. Those pixels are averaged out by applying the almost-identity kernel, which explains why the STD values for iteration counts strictly superior to zero rejoin the general trend. However, the corresponding PSNR values are heavily distorted, since we compare to a base image that is noisy.

The causes of these two observations remain unexplained, and multiple runs of the experiment all exhibit the same singularities. This seems to suggest that the camera's behavior cannot be approximated as a linear model. Specific input values, combined with specific instructions result in out of trend results.
 



