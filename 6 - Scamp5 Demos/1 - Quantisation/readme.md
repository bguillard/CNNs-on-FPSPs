The scamp5_main.cpp present in this directory contains macros for quantising the content of an analog register (AREG) to multiple digital registers (DREGs), and restoring it back. As a reminder:
 * An AREG stores analog value in the range [-127, 127] for each pixel.
 * A DREG stores a binary value (0 or 1) for each pixel.

The example we show here only use the first three most significant bits, and hence  perform an approximation of the analog input values. This was shown to be a sufficient enough precision for our needs.

Furthermore, we give two versions of our quantisation/unquantisation macros: one that uses a fourth DREG to store the sign, another that assumes all value are positive (unsigned). The latter uses one DREG less, and is sufficient in the case all values are positive - which is typically the case after applying a ReLU on the result of a convolution...


Here are visual demonstrations of how it works:

Signed quantisation, on the device: ![Signed Quantisation](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/blob/assets/assets/Capture_signed_quantisation.PNG?raw=true)
Unsigned quantisation, on the device: ![Unsigned Quantisation](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/blob/assets/assets/Capture_unsigned_quantisation.PNG?raw=true)

Signed quantisation, in a simulator: 
 * input image: ![Quantisation Input](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/blob/assets/assets/quantisation_-_simulator_input.png?raw=true)
 * after being restored: ![Quantisation Output](https://gitlab.doc.ic.ac.uk/bag1418/cnns-on-fpsps/blob/assets/assets/quantisation_-_simulator_output.png?raw=true)