"""
Loads all the images in the "data/" directory.

This folder should contain 6400 images:
- for a number of times the identity operation is applied, k in (1-32):
  - for a number of iteration it in (0-99):
    - we have 2 images:
      - one input image: "Input f k_it.BMP"
      - one output image: "Output f k_it.BMP"

The expected number of images is therefore 32 * 100 * 2 = 6400.
The input image corresponds to an uniform array, filled with a single value.
The output image corresponds to the result we get when applying our supposedly
identity function k times.

For each k (number of times the identity operation is applied in (1-32)), we compute
the mean of the average of the difference between the input and the output. This
gives us 32 scalar values, each of them corresponding to the systematic bias introduced
by the application of k repetitions of out identity function.

For each input/output pair, we then compute the PSNR,
after having compensated the systematic bias by adding the above
term.

For each Output image, we then compute the STD of the output.
"""

import numpy as np
import math
import scipy.misc

def psnr(img1, img2):
  mse = np.mean( (img1 - img2) ** 2 )
  if mse == 0:
    return 100
  PIXEL_MAX = 255.0
  return 20 * math.log10(PIXEL_MAX / math.sqrt(mse))

## Load all images
# inputs[0-31][0-99] : all original inputs
inputs = []
# outputs[0-31][0-99] : result images
outputs = []

directory = 'data/'
for it in range(32):
  inputs.append([])
  outputs.append([])

  for k in range(100):
    i_img_filename = directory + 'Input f ' + str(it+1) + '_{:05d}.BMP'.format(k)
    o_img_filename = directory + 'Output f ' + str(it+1) + '_{:05d}.BMP'.format(k)

    i_img = scipy.misc.imread(i_img_filename)[:,:,0]
    o_img = scipy.misc.imread(o_img_filename)[:,:,0]

    inputs[it].append(i_img)
    outputs[it].append(o_img)

## Compute constant term to compensate for systematic noise
# (one value per # of iterations, applied to the whole batch of 100 images)
constant_terms = []
for it in range(32):
  mean_differences = []
  for k in range(100):
    mean_differences.append((inputs[it][k] - outputs[it][k]).mean())
  val = sum(mean_differences) / 100
  constant_terms.append(val)

# Add constant terms to compensate for systematic bias
for it in range(32):
  for k in range(100):
    outputs[it][k] = outputs[it][k] + constant_terms[it]

## Compute mean PSNR
mean_psnr_values = []
for it in range(32):
  mean_psnr = []
  for k in range(100):
    mean_psnr.append(psnr(inputs[it][k], outputs[it][k]))
  mean_psnr = sum(mean_psnr) / 100
  mean_psnr_values.append(mean_psnr)

## Compute mean STD
mean_std_values = []
for it in range(32):
  mean_std = []
  for k in range(100):
    mean_std.append(outputs[it][k].std())
  mean_std = sum(mean_std) / 100
  mean_std_values.append(mean_std)

baseline_std = []
for it in range(32):
  for k in range(100):
    baseline_std.append(inputs[it][k].std())
mean_std_values = [sum(baseline_std)/len(baseline_std)] + mean_std_values

## Print results
print('bias:')
print(constant_terms)

print('')
print('PSNR:')
print(mean_psnr_values)

print('')
print('STD:')
print(mean_std_values)
