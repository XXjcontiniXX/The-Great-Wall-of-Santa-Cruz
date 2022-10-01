#!/bin/bash

#Reconstruct collatz executable

make clean && make collatz

for i in {1..10000}; do ./collatz -n $i | sort -g | tail -1 >> collatz_l.dat; done

gnuplot <<END
    set terminal pdf
    set output "length.pdf"
    set xlabel "\$n$"
    set ylabel "\$max_num$"
    set zeroaxis
    set title "\$Maximum_Number$"
    plot "./collatz_l.dat" with dots
END
# This is the heredoc that is sent to gnuplot

