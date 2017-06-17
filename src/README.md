# Test the C++ Library

In the file `tests.cpp` it is provided a simple test for the Rotary Sensor module. The test consist in taking measurements every `500ms`, the functions used to retrieve the values from the Analog Sensors Module are: `rotary.getValue()` and `rotary.getScaledValue()`, and then the measurements are printed out to the console as `float` and `integer` values, respectively.

To run the test follow these steps:

* Compile the sources with the following command: `make all`

* The previous command will generate an executable file named `runner`, which you can run as follows: `sudo ./runner`.

* To remove the compiled `*.o` files and the dist file `runner`, execute the following line: `make clean`.
