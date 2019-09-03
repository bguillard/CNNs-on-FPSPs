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
with open('NP_KERNELS_WEIGHTS.pck', 'rb') as f:
  (w1_val, b1_val, w2_1_val, w2_2_val,
        w2_3_val, thresh_val) = pickle.load(f)

#################################################
# Generate the first 3 convolutions (first layer)
#################################################
# Generate CSIM code, with all available registers
start_register = ['A', 'B', 'C']
available_registers = [['B', 'C', 'D', 'E'], ['C', 'D', 'E'],
                       ['D', 'E']]
filename_prefix = '_out_conv1_'

for k in range(3):
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
  with open(filename_prefix+str(k+1)+'_bias.txt', 'w') as f:
    f.write(str(b1_val[k]))

#################################################
# Generate the remaining 3*3 convolutions (second layer)s
#################################################
w2_val = [w2_1_val, w2_2_val, w2_3_val]
# Register attribution according to what is presented in 
# 'Fitting a 2 layer CNN on SCAMP-5d - V2.pdf'
start_register = [['A', 'B', 'B'],
                  ['B', 'C', 'C'],
                  ['B', 'B', 'A']]

available_registers = [ [['D', 'E']] * 3,
                        [['D', 'E']] * 3,
                        [['C', 'D', 'E'], ['C', 'D', 'E'], ['B', 'C', 'D', 'E']]]

filename_prefix = '_out_conv_2_'
for k in range(3):      # Output feature map
  for l in range(3):    # Input feature map
    # Generate CSIM code
    csimProgram, _, _ = generate(w2_val[k][:,:,l,0], search_time,
                          start_reg=start_register[k][l],
                          target_reg=start_register[k][l],
                          available_regs=[start_register[k][l]]+available_registers[k][l],
                          verbose=0,
                          out_format='CSIM',
                          approx_depth=approx_depth,
                          pair_props=pair_props)
    scamp5Program = csim_to_scamp5(csimProgram)
    
    # Output the program in a file
    filename = filename_prefix + str(k+1) + '_' + str(l+1)
    with open(filename+'.txt', 'w') as f:
      for l in scamp5Program:
        f.write(l+'\n')
  with open(filename_prefix+str(k+1)+'_bias.txt', 'w') as f:
    f.write(str(thresh_val[k]))

#################################################
# Export the fc weights
#################################################

