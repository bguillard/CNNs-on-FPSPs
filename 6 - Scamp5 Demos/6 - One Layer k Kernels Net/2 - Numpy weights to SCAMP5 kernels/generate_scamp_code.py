import pickle
import numpy as np
from scamp_filter.scamp_filter import generate, PairGenProps
from auke_overlay import csim_to_scamp5

WIDTH = 8


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
with open('NP_WEIGHTS_1_layer_'+str(WIDTH)+'_kernels.pck', 'rb') as f:
  w1_values = pickle.load(f)


# Special cases: clip very high values
if WIDTH == 7:
    w1_values[1,1,0,2] = -2
    w1_values[2,2,0,2] = -2

    w1_values[2,2,0,3] = -2.25
    w1_values[2,1,0,3] = -2.25
    w1_values[1,2,0,3] = -2.25
    w1_values[0,2,0,3] = -2

    w1_values[1,0,0,4] = -2

if WIDTH == 8:
    w1_values[0,0,0,0] = -2
    w1_values[2,2,0,2] = -1

for i in range(WIDTH):
    print(w1_values[:,:,0,i])

#################################################
# Generate the 3 convolutions (first and only layer)
# with all registers as available (outputs during computations)
#################################################
# Generate CSIM code, with all registers available
start_register = ['A']*WIDTH
available_registers = [['B', 'C', 'D', 'E']]*WIDTH
filename_prefix = '_out_conv_'


for k in range(WIDTH):
    csimProgram, _, _ = generate(w1_values[:,:,0,k], search_time,
                            start_reg=start_register[k],
                            target_reg=start_register[k],
                            available_regs=[start_register[k]]+available_registers[k],
                            verbose=0,
                            out_format='CSIM',
                            approx_depth=approx_depth,
                            pair_props=pair_props)
    scamp5Program = csim_to_scamp5(csimProgram)

    # Output to files
    with open(filename_prefix+str(WIDTH)+'_'+str(k+1)+'.txt', 'w') as f:
      for l in scamp5Program:
        f.write(l+'\n')
