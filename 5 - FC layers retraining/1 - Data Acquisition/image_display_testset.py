import numpy as np
import mnist
import random
from cv2 import resize, imshow, waitKey

"""
Displays only the test set of MNIST. 
Commands: press
s : to trigger/stop autoplay. When autoplay is disabled, press any 
    key to display the next frame
a digit : to display examples of this digit
q : to quit
"""

IMG_SIZE = 140    # in px
WAIT_TIME = 150   # in ms
wait_time = WAIT_TIME

digitSwitch = [('0', 'à'), ('1', '&'), ('2', 'é'), ('3', '"'), ('4', "'"), 
                ('5', '('), ('6', '§'), ('7', 'è'), ('8', '!'), ('9', 'ç')]
           
def displayOneLabel(label):
  global wait_time
  #x_test_sorted[label] = random.sample(x_test_sorted[label], len(x_test_sorted[label]))
  #random.shuffle(x_test_sorted[label])
  np.random.shuffle(x_test_sorted[label])
  for img in x_test_sorted[label]:
    # Reshape (4* upsampling and padding)
    img = resize(img, (IMG_SIZE,IMG_SIZE))
    to_show[IMG_SIZE:IMG_SIZE*2,IMG_SIZE:IMG_SIZE*2] = img
    # Display + wait
    imshow('img', to_show)
    k = waitKey(wait_time)
    if k == ord('s'):
      # stop/restart autoplay
      wait_time = WAIT_TIME - wait_time
    if k == ord('q'):
      # quit
      exit()
    # Change digit
    for (l, (lowerCase, upperCase)) in enumerate(digitSwitch):
      if k == ord(lowerCase) or k == ord(upperCase):
        return displayOneLabel(l)
  return displayOneLabel((label+1)%10)

# Load data
_, _, x_test, y_test = mnist.load()
# Reshape and invert colors
x_test = 255 - x_test.reshape((10000,28,28))

# Sort it
x_test_sorted = [()]*10
for label in range(10):
  x_test_sorted[label] = x_test[y_test==label]


# Display loop
# Start with a centered cross, to align the camera
background = (255*np.ones((IMG_SIZE*3,IMG_SIZE*3))).astype(np.uint8)
foreground = np.zeros((IMG_SIZE,IMG_SIZE)).astype(np.uint8)
# Draw cross
foreground[IMG_SIZE//2-5:IMG_SIZE//2+5,:] = 255
foreground[:,IMG_SIZE//2-5:IMG_SIZE//2+5] = 255
background[IMG_SIZE:2*IMG_SIZE,IMG_SIZE:2*IMG_SIZE] = foreground
imshow('img', background)
waitKey(0)

# Start looping as follows:
# training 0s, testing 0s, training 1s, testing 1s...
# with a 30 seconds white screen between each phase
to_show = (255*np.ones((IMG_SIZE*3,IMG_SIZE*3))).astype(np.uint8)
while True:
  for label in range(10):
    displayOneLabel(label)
      