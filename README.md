# Turing Machine Simulator

This is a C++ program written to simulate the running of a Turing Machine with a single tape.

To use this program, the transition function must be specified in the state_machine.txt file and the initial configuration must be specified in the config.txt file.

After this, the program can be run and used using command line arguments. These are detailed further in main.cpp.

There are two modes, continuous and step-through, which can be selected using the command line arguments.
Continuous mode runs the Turing Machine on the input, and step-through mode pauses after every iteration and waits for key input before proceeding.

These are both visualised in the terminal, with the tape being displayed up to a certain point and the tape head and current state being specified.

The tape is implemented using a simple linked list to allow for a dynamically sized tape.

Please note that when giving file paths, absolute paths need to be provided.
