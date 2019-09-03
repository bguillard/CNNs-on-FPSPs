import pickle
import numpy as np

INPUT_FILENAME = 'NP_WEIGHTS.pck'
PRECISION = 100

# Open weights
fc1_k, fc1_b, fc2_k, fc2_b = pickle.load(
    open(INPUT_FILENAME, 'rb'))
# Round them
fc1_k, fc1_b, fc2_k, fc2_b = fc1_k*PRECISION//1, fc1_b*PRECISION//1, fc2_k*PRECISION//1, fc2_b*PRECISION*PRECISION//1
fc1_k, fc1_b, fc2_k, fc2_b = fc1_k.astype(np.int), fc1_b.astype(np.int), fc2_k.astype(np.int), fc2_b.astype(np.int)

"""
0: GENERATE C++ ARRAYS, TO BE USED IN A STANDARD LOOP
"""
OUTPUT_FILENAME = 'fc_weights_arrays.cpp'

def to_cpp_1_dim(array):
  txt = '{\t'
  for coeff in array[:-1]:
    txt += str(coeff) + ',\t'
  txt += str(array[-1]) + '}'
  return txt

def to_cpp_2_dims(array):
  txt = '{'
  for line in array[:-1]:
    txt += to_cpp_1_dim(line) + ',\n'
  txt += to_cpp_1_dim(array[-1]) + '}'
  return txt

# Generate .cpp text
out = 'int fc1_k[' + str(fc1_k.shape[0]) + '][' + str(fc1_k.shape[1]) + '] = '
out += to_cpp_2_dims(fc1_k) + ';\n\n'

out += 'int fc1_b[' + str(fc1_b.shape[0]) + '] = '
out += to_cpp_1_dim(fc1_b) + ';\n\n'

out += 'int fc2_k[' + str(fc2_k.shape[0]) + '][' + str(fc2_k.shape[1]) + '] = '
out += to_cpp_2_dims(fc2_k) + ';\n\n'

out += 'int fc2_b[' + str(fc2_b.shape[0]) + '] = '
out += to_cpp_1_dim(fc2_b) + ';\n\n'

# Output it
with open(OUTPUT_FILENAME, 'w+', encoding='utf-8') as f:
  f.write(out)

"""
1: GENERATE C++ LOOP, USING THE ABOVE ARRAY
"""
OUTPUT_FILENAME = 'fc_loop_unrolled.cpp'

def to_cpp_function(k, b, function_name, in_dim, out_dim):
  """
  Generates C++ code for computing a fully connected layer of int values,
  applying weights k and bias b, with hardcoded values in the source code.
  The function is names after function_name.
  """
  out = ""
  out += "inline void "+function_name+"(int in["+str(in_dim)+"], int out["+str(out_dim)+"]){\n"
  for j in range(out_dim):
    out += "\tout["+str(j)+"] = \n"
    for i in range(in_dim):
      out += "\t\tin["+str(i)+"]*("+k+"["+str(i)+"]["+str(j)+"]) +\n"
    out += "\t\t("+b+"["+str(j)+"]);\n"
  out += "}\n\n"
  return out


## Generate .cpp text
out = ""
# First layer
out += to_cpp_function('fc1_k', 'fc1_b', 'fc_1', 27, 50)

# Second layer
out += to_cpp_function('fc2_k', 'fc2_b', 'fc_2', 50, 10)

# Output it
with open(OUTPUT_FILENAME, 'w+', encoding='utf-8') as f:
  f.write(out)

"""
3: GENERATE C++ LOOP, WITH HARDCODED WEIGHTS
"""
OUTPUT_FILENAME = 'fc_loop_unrolled_hardcoded_weights.cpp'

def to_cpp_function(k, b, function_name):
  """
  Generates C++ code for computing a fully connected layer of int values,
  applying weights k and bias b, with hardcoded values in the source code.
  The function is names after function_name.
  """
  out = ""
  (in_dim, out_dim) = k.shape
  out += "inline void "+function_name+"(int in["+str(in_dim)+"], int out["+str(out_dim)+"]){\n"
  for j in range(out_dim):
    out += "\tout["+str(j)+"] = \n"
    for i in range(in_dim):
      out += "\t\tin["+str(i)+"]*("+str(k[i][j])+") +\n"
    out += "\t\t("+str(b[j])+");\n"
  out += "}\n\n"
  return out


## Generate .cpp text
out = ""
# First layer
out += to_cpp_function(fc1_k, fc1_b, 'fc_1')

# Second layer
out += to_cpp_function(fc2_k, fc2_b, 'fc_2')

# Output it
with open(OUTPUT_FILENAME, 'w+', encoding='utf-8') as f:
  f.write(out)
