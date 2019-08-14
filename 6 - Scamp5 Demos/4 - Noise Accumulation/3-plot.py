import matplotlib as mpl
mpl.rcParams['figure.dpi'] = 200
import matplotlib.pyplot as plt

from values_psnr_std_bias import data

# Plot PSNR
x_psnr = [e+1 for e in list(range(len(data[100]['psnr'])))]

fig, ax1 = plt.subplots()
ax1.set_ylabel('PSNR')
ax1.set_xlabel('Number of iterations')

for input_value in sorted(list(data.keys())):
  ax1.plot(x_psnr, data[input_value]['psnr'], label='Input value: {}'.format(input_value), linestyle='--', linewidth=0.8)


plt.grid(True, linestyle=':')
ax1.legend(loc='upper right')
plt.show()

# Plot STD
x_std = list(range(len(data[100]['std'])))

fig, ax1 = plt.subplots()
ax1.set_ylabel('STD')
ax1.set_xlabel('Number of iterations')

for input_value in sorted(list(data.keys())):
  ax1.plot(x_std, data[input_value]['std'], label='Input value: {}'.format(input_value), linestyle='--', linewidth=0.8)


plt.grid(True, linestyle=':')
ax1.legend(loc='lower right')
plt.show()

# Plot Bias
x_bias = list(range(len(data[100]['bias'])))

fig, ax1 = plt.subplots()
ax1.set_ylabel('Systematic bias')
ax1.set_xlabel('Number of iterations')

for input_value in sorted(list(data.keys())):
  ax1.plot(x_bias, data[input_value]['bias'], label='Input value: {}'.format(input_value), linestyle='--', linewidth=0.8)


plt.grid(True, linestyle=':')
ax1.legend(loc='upper left')
plt.show()

