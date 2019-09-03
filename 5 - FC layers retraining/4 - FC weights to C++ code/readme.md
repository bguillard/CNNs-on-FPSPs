weights_pck_to_cpp_unrolled_loop.py reads numpy weights stored in a pickled file, corresponding to two layers of a fully connected network.
It outputs 3 different C++ versions:
- one with legacy arrays
- one with unrolled loops
- one with unrolled loops and hardcoded weights.