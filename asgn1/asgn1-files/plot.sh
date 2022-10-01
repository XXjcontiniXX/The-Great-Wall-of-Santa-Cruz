#!/bin/bash

#Reconstruct collatz executable

make clean && make collatz

#n x Length ###################
echo > collatz_2.dat

declare -i c

for i in {2..10000}; do ./collatz -n $i | awk -b

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "start_v_length.pdf"
    set xrange [0:10000]
    set yrange [0:100000]
    set xlabel "{/:Italic n}"
    set ylabel "length"
    set zeroaxis
    set title "Collatz Sequence Lengths"
    plot "./collatz_2.dat" with dots notitle
END

#n x MaxValue ###################
echo > collatz_3.dat

for i in {2..10000}; do ./collatz -n $i | sort -n | tail -1 >> collatz_3.dat; done

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "start_v_MaxVal.pdf"
    set xrange [0:10000]
    set yrange [0:100000]
    set xlabel "{/:Italic n}"
    set ylabel "value"
    set zeroaxis
    set title "Maximum Collatz Sequence Value"
    plot "./collatz_3.dat" with dots notitle
END

