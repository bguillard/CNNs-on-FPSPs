import pickle
import numpy as np
from scamp_filter.scamp_filter import generate, PairGenProps
from auke_overlay import csim_to_scamp5

# AUKE parameters
pair_props = PairGenProps(
            sort_distinct_pos=True,
            short_distance_first=True,
            low_scale_first=True,
            exhaustive=False,
            line=True,
            generate_all=True,
            max_sets=True,
            randomize=False
)
search_time = 30
approx_depth = 3

# Load numpy weights
with open('NP_WEIGHTS.pck', 'rb') as f:
  (w1_val, b1_val, w2_val, thresh_values) = pickle.load(f)

#################################################
# Generate the first 4 convolutions (first layer)
#################################################

# Generate CSIM code, with all available registers
start_register = ['A', 'B', 'B', 'B']
available_registers = [['B', 'C', 'D', 'E'], ['C', 'D', 'E'],
                       ['C', 'D', 'E'], ['C', 'D', 'E']]
filename_prefix = '_out_conv1_'

for k in range(4):
  csimProgram, _, _ = generate(w1_val[:,:,0,k], search_time,
                          start_reg=start_register[k],
                          target_reg=start_register[k],
                          available_regs=[start_register[k]]+available_registers[k],
                          verbose=0,
                          out_format='CSIM',
                          approx_depth=approx_depth,
                          pair_props=pair_props)
  scamp5Program = csim_to_scamp5(csimProgram)

  # Output to files
  with open(filename_prefix+str(k+1)+'.txt', 'w') as f:
    for l in scamp5Program:
      f.write(l+'\n')

#################################################
# Generate the remaining 4*8 convolutions (second layer)
# conv_2_1 to conv_2_8 are computed in AREG C.
#################################################
def sparsify_kernel(original_kernel):
  out = np.zeros((5,5))
  out[::2,::2] = original_kernel
  return out

start_register = ['C'] * 8
available_registers = [['D', 'E']] * 8
filename_prefix = '_out_conv_2_'
for k in range(8):
  for l in range(4):
    # Generate CSIM code
    csimProgram, _, _ = generate(w2_val[:,:,l,k], search_time,
                          start_reg=start_register[k],
                          target_reg=start_register[k],
                          available_regs=[start_register[k]]+available_registers[k],
                          verbose=0,
                          out_format='CSIM',
                          approx_depth=approx_depth,
                          pair_props=pair_props)
    # No double shift with sparse kernels
    scamp5Program = csim_to_scamp5(csimProgram, doubleShifts=True)
    
    # Output the program in a file
    filename = filename_prefix + str(k+1) + '_' + str(l+1)
    with open(filename+'.txt', 'w') as f:
      for l in scamp5Program:
        f.write(l+'\n')
