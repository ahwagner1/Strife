#!/bin/bash

# using this to quickly compile the various test scripts instead of constantly updating a makefile

# check if the required arguements are provided
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <test_file> <impl_directory> [output_name]"
    echo "Example: $0 client_test networking client_test.out"
    exit 1
fi

# get the test file name without extension
TEST_FILE=$(basename "$1" .cpp)

# get the implementaiton directory
IMPL_DIR="$2"

# set the oputput name (use provided name or defualt to test file name + .out)
if [ "$#" -ge 3 ]; then
    OUTPUT="$3"
else
    OUTPUT="${TEST_FILE}.out"
fi

# determine the implementaiton file based on the test file name
# this assumes test files are names like xxx_test.cpp and implementation files are xxx.cpp
IMPL_FILE=$(echo "$TEST_FILE" | sed 's/_test$//')

echo "Compiling $TEST_FILE.cpp with implementation ${IMPL_DIR}/$IMPL_FILE.cpp"

g++ tests/${TEST_FILE}.cpp ${IMPL_DIR}/${IMPL_FILE}.cpp -I. -o ${OUTPUT}

# check if compilation was a success
if [ $? -eq 0 ]; then
    echo "Compilation successful, Output: $OUTPUT"
    echo "Run with ./$OUTPUT"
else
    echo "Compilation failed"
fi