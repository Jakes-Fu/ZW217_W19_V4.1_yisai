#!/bin/bash

proj=$1;
echo "starting building nv......";
for pid in AA AE1 EA AE2 AH AE3 ED EI ER OG DTAG IC IM IJ IB IF IE MF MS IO MA MC MG MD MR MY MP ZZZZ QB MH QA; 
do  
echo start building project=$proj  PID=$pid; 
make p=$proj job=32 -j16 PID=$pid m=nv_parameter > err_1 ;
done
echo "Finish building nv......";
