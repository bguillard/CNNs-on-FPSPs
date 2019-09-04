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
with open('NP_WEIGHTS_two_layers_bin_reg_lrelu.pck', 'rb') as f:
  (w1_values, b1_values, w2_1_values, w2_2_values, w2_3_values) = pickle.load(f)

#####################################
# Negate the third kernel
first_layer = [w1_values[:,:,0,0], w1_values[:,:,0,1], w1_values[:,:,0,2]]
second_layer = [w2_1_values[:,:,0,0], w2_2_values[:,:,0,0], w2_3_values[:,:,0,0]]
kernels = [first_layer, second_layer]

#################################################
# Generate the 3 convolutions (first and only layer)
# with all registers as available (outputs during computations)
#################################################
# Generate CSIM code, with all registers available
start_register = ['A']*3
available_registers = [['B', 'C', 'D', 'E']]*3
filename_prefix = '_out_conv_'

for j in range(2):
  for k in range(3):
    csimProgram, _, _ = generate(kernels[j][k], search_time,
                            start_reg=start_register[k],
                            target_reg=start_register[k],
                            available_regs=[start_register[k]]+available_registers[k],
                            verbose=0,
                            out_format='CSIM',
                            approx_depth=approx_depth,
                            pair_props=pair_props)
    scamp5Program = csim_to_scamp5(csimProgram)

    # Output to files
    with open(filename_prefix+str(j+1)+'_'+str(k+1)+'.txt', 'w') as f:
      for l in scamp5Program:
        f.write(l+'\n')
