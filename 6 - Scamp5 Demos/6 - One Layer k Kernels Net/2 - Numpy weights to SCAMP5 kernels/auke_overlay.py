"""
An overlay to Thomas’s code generator, that takes CSIM code as input, and produces 
valid SCAMP5 code as output. '_transform' placeholders are transformed into their 
corresponding SCAMP5 instructions.
It also enforces some additional constraints on SCAMP5 operations, such as:
- sub: target and source2 different
- add: both sources different, and target and source2 different
- div: distinguish between inplace/to a different target reg.
- neg: distinguish between inplace/to a different target reg.
Moreover, it allows to replace shifting instructions by 'super-shifting ones', to 
run kernels on interleaved feature maps.

On its own, this script is not sufficient to produce SCAMP5 code. It also relies on 
C++ macros that we provide at the end of this file, or in conv_instructions.hpp.
"""

# In case of ADD: do target and source2 need to be different?

def csim_to_scamp5(program, doubleShifts=False):
  """
  Takes a CSIM program as a list of instructions (strings),
  and maps it to a SCAMP5 program.
  program: list of instructions, ['instr1', 'instr2', '// comment', ...]. Should
    be the exact output of AUKE (we here rely on its specific syntax, such as
    whitespace location etc.)
  doubleShifts: boolean, if True shifting instructions are replaced
    by double_shifting instructions (see macros in .hpp file)
  """
  outputProgram = []

  # Create prefix for shifting instructions
  if doubleShifts:
    shiftPrefix = 'double_'
  else:
    shiftPrefix = ''

  # Iterate on instructions
  for instr in program:

    # Remove _transform instructions
    if instr.startswith('_transform'):
        instr = '// ' + instr

    # Uncomment shifting instructions, and
    # add prefix if necessary
    elif (instr.startswith('// north')
          or instr.startswith('// east')
          or instr.startswith('// south')
          or instr.startswith('// west')):
      instr = shiftPrefix + instr[3:]

    # Uncomment division instruction
    elif instr.startswith('// div2'):
      instr = instr[3:]
      # Differentiate between inplace division,
      # and to a different target register
      if instr[5] == instr[8]:
        instr = 'div2_inplace' + instr[4:]

    # neg: differentiate between inplace or to a different target
    elif instr.startswith('neg'):
      if instr[4] == instr[7]:
        instr = 'neg_inplace' + instr[3:]

    # add: both sources different, and target and source2 different
    elif instr.startswith('add'):
      # If both sources are the same
      if instr[7] == instr[10]:
        instr = 'add_twice' + instr[3:]
      # If target = source2, swap them
      elif instr[4] == instr[10]:
        instr = instr[:7] + instr[10] + instr[8:10] + instr[7] + instr[11:]

    # sub: target and source2 different
    elif instr.startswith('sub'):
      if instr[4] == instr[10]:
        instr = 'sub_inplace' + instr[3:]

    outputProgram.append(instr)
  return outputProgram

"""
Corresponding C++ macros:
#define west(X,Y)   {mov(X,Y,west);}
#define east(X,Y)   {mov(X,Y,east);}
#define south(X,Y)  {mov(X,Y,south);}
#define north(X,Y)  {mov(X,Y,north);}
#define div2(X,Y) {diva(X,F,Y);}
#define div2_inplace(X,Y)   {diva(X,E,F);}
#define neg_inplace(X,Y)({\
  mov(F,Y);\
  neg(X,F);\
})
#define add_twice(X, Y, Z)({\
  mov(F, Z);\
  add(X, Y, F);\
})
#define sub_inplace(X, Y, Z)({\
  sub(F, Y, Z);\
  mov(X, F);\
})
"""

