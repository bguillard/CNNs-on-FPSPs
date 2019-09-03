import glob

INPUT_PATH = 'files_to_merge_and_reencode/'
OUTPUT_FILENAME = 'out.txt'
BLOCKSIZE = 1048576

in_filenames = sorted([f for f in glob.glob(INPUT_PATH + '*.TXT')])

with open(OUTPUT_FILENAME, 'w+', encoding='utf-8') as out_f:
  for filename in in_filenames:
    with open(filename, 'r+', encoding='utf-16') as in_f:      
      while True:
        contents = in_f.read(BLOCKSIZE)
        if not contents:
          break
        out_f.write(contents)
