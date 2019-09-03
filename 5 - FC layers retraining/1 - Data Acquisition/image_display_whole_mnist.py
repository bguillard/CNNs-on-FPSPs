import numpy as np
import mnist
from cv2 import resize, imshow, waitKey

"""
After showing a target, displays only the whole MNIST dataset. 
Iterates over the ordered labels (0 to 9), to display the training set, 
and then the testing set. Between each subset/label pair, a 30 s white 
screen is displayed.
Commands: press
s : to skip the current subset/label pair
q : to quit
"""

IMG_SIZE = 140    # in px
WAIT_TIME = 150   # in ms

# Load data
x_train, y_train, x_test, y_test = mnist.load()
# Reshape and invert colors
x_train = 255 - x_train.reshape((60000,28,28))
x_test = 255 - x_test.reshape((10000,28,28))

# Sort it
x_train_sorted = [()]*10
x_test_sorted = [()]*10
for label in range(10):
  x_train_sorted[label] = x_train[y_train==label]
  x_test_sorted[label] = x_test[y_test==label]

# Two dimensionnal list:
# imgs[set][label] is (batch, 28, 28) numpy array,
#   set is 0 (training set) or 1 (testing set)
#   label is between 0 and 9
imgs = [x_train_sorted, x_test_sorted]


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

# Wait 30 seconds
white_background = (255*np.ones((IMG_SIZE*3,IMG_SIZE*3))).astype(np.uint8)
imshow('img', white_background)
waitKey(30*1000)

# Start looping as follows:
# training 0s, testing 0s, training 1s, testing 1s...
# with a 30 seconds white screen between each phase
to_show = (255*np.ones((IMG_SIZE*3,IMG_SIZE*3))).astype(np.uint8)
for label in range(10):
  for set in range(2):
    for img in imgs[set][label]:
      # Reshape (4* upsampling and padding)
      img = resize(img, (IMG_SIZE,IMG_SIZE))
      to_show[IMG_SIZE:IMG_SIZE*2,IMG_SIZE:IMG_SIZE*2] = img
      # Display + wait
      imshow('img', to_show)
      k = waitKey(WAIT_TIME)
      if k == ord('s'):
        # skip actual [set][label] pair
        break
      if k == ord('q'):
        # quit
        exit()
    # Display black + wait 30 seconds
    imshow('img', white_background)
    waitKey(30*1000)

imshow('img', white_background)
waitKey(0)
