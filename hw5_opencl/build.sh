#!/bin/bash
if [ $# -eq 0 ]
then
   echo "input Student ID"
else
   unzip -o "HW5_$1.zip"
   g++ -o histogram histogram.cpp -lOpenCL
fi
