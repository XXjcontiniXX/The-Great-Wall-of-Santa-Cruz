#!/bin/bash

#Reconstruct collatz executable

make clean && make collatz

#n x Length ###################
echo > collatz_2.dat
c=0
for i in {2..1000}; do
	./collatz -n $i > tmp_length.dat
	while read line;
	do
		c=$((c+1))
	done < tmp_length.dat
	echo $c >> collatz_2.dat
	c=0; 
done

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "length.pdf"
    set xrange [0:1000]
    set yrange [0:300]
    set xlabel "{/:Italic n}"
    set ylabel "length"
    set zeroaxis
    set title "Collatz Sequence Lengths"
    plot "./collatz_2.dat" with dots notitle
END

#n x MaxValue ###################
echo > collatz_3.dat

for i in {2..1000}; do ./collatz -n $i | sort -n | tail -1 >> collatz_3.dat; done

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "max_val.pdf"
    set xrange [0:1000]
    set yrange [0:10000]
    set xlabel "{/:Italic n}"
    set ylabel "value"
    set zeroaxis
    set title "Maximum Collatz Sequence Value"
    plot "./collatz_3.dat" with dots notitle
END
#Legnths x Frequency #################
echo > collatz_4.dat

while read num;
do
	
done < tmp_length.dat
