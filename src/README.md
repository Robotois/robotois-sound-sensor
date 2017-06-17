# Test the C++ Library

In the file `test.cpp` it is provided a simple test for the Sound Sensor module. The test consists in taking measurements every `10ms`, the result is printed and the values goes from `[-2.5, 2.5]` and the basic values are in the range `[-100, 100]`.

To run the test follow these steps:

* Compile the sources with the following command: `make all`

* The previous command will generate an executable file named `runner`, which you can run as follows: `sudo ./runner`.

* To remove the compiled `*.o` files and the dist file `runner`, execute the following line: `make clean`.
