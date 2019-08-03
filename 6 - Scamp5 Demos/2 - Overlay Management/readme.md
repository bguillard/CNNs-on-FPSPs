The scamp5_main.cpp present in this directory contains a demonstration of the principle of recompiling vision chip program kernels mid-loop. This trick can be used to write longer programs. This is especially useful for implementing large CNNs.


## Background: SCAMP5 architecture

The SCAMP5 vision system is enabled with:
 * two traditional digital processors, cores M0 and M4. We write and compile our vision programs for the M0 core, while the M4 core is in charge of other miscellaneous tasks (USB interfacing...).
 * one analog processor, that is light-sensitive. This is our vision-chip, composed of independent Processing Elements (PEs) that enable pixel-parallel computations.


Arithmetic and logic operations executed in parallel on SCAMP5 PEs are called SCAMP5 Kernels. Kernels are written in special sections among the M0 source code, and compiled into SCAMP5 machine code on first encounter.

A SCAMP5 Kernel is defined using either a lambda function, or scamp5_kernel_begin() / scamp5_kernel_end() delimiters. See the [official documentation](https://personalpages.manchester.ac.uk/staff/jianing.chen/scamp5d_lib_doc_html/_p_a_g_e__d_e_v_i_c_e__a_p_i__c_a_t_e_g_o_r_y.html). Upon the first execution of a scamp5_kernel, it is first compiled, and then sent to the vision chip. The compiled version is stored, so that further executions are faster: the code can directly be sent to the vision chip.

The compiled SCAMP5 Kernels are stored in a specific part of the memory, that can only store 1024 instruction words. Once that limit is reached, the whole program stops functionning. For relatively long programs, this limit is often quickly reached.

## Dynamic allocation = Overlay management

The main idea here to circumvent that issue is to dynamically allocate SCAMP5 Kernels. A kernel is in turn:
 * Created, in the heap (using the new() instruction)
 * Compiled
 * Sent to the vision chip for execution (as many times as required)
 * Deleted

/!\ When using such dynamically allocated SCAMP5 Kernels, one should not use 'traditionally' defined Kernels. Mixing the two leads to highly erratic behaviours.

This frees us from the above limitation: one can cut his program in subparts, virtually as many as he wants.


The only drawback of that method is speed. Indeed, vision algorithm are often infinite loops, in which the same kernels are run at each iteration. With our solution, the kernels are recompiled at each iteration, which slows down the main loop of the program. This howevers allows for framerates that are still in hundreds of frames per seconds, and has the great advantage of allowing the creation of prototypes and proof of concept programs.


