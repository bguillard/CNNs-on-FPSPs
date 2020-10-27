# Summer project: Implementing CNNs on an FPSP
Author: Benoît GUILLARD, bag1418

Date: Summer 2019

This repository contains the code written during my Summer Project.

## Content
The directories #1, 2, 3 and 4 contain the code for whole CNNs, and share the same structure:
 * One notebook for training the network.
 * One script to extract the convolutional weights and convert them into C++ SCAMP5 code.
 * One full implementation in C++.

The two fully connected layers must be retrained using in-situ, noisy data, that is captured by the SCAMP5 vision system once the convolutional layers have been loaded into it. The scripts to display the whole MNIST dataset, retrain FC layers, and convert them to C++ code is in directory #5.

The directory #6 contains small C++ demonstrations that run on the SCAMP5 device. They consist in isolating some functionalities that were developped as parts of whole CNN architectures, and helped supporting our discussions in the thesis.

## Software versions and environment
### Python scripts
All the Python scripts (.py files) contained in this repository have been developed and tested locally, using Python 3.6.6, and the following environment: [python_script_requirements.txt](/python_script_requirements.txt)

### SCAMP5 code
The SCAMP5 code presented here (.cpp and .hpp files) has been developed and tested using the library version 0.9.8 and the host program 1.3.9 (see Jianing Chen's website: https://personalpages.manchester.ac.uk/staff/jianing.chen/scamp5d_lib_doc_html/index.html  )

### Colab Notebooks
All the Python notebooks contained in this repository have been developed and tested on Google Colab GPU instances. As of August 2019, this means Python 3.6.8, and the following environment: [python_notebook_requirements.txt](/python_notebook_requirements.txt)


