#!/bin/bash

# Just run this file and you can test your circ files!
# Make sure your files are in the directory above this one though!

cp ex1.circ ex2.circ ex3.circ ex4.circ ex5.circ testing/circ_files

cd testing
rm -rf student_output
mkdir student_output
./test.py
cd ..
