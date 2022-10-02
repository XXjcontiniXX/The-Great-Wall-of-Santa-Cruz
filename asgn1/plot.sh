#!/bin/bash

#Reconstruct collatz executable

make clean && make collatz

#n x Length ###################
echo > collatz_2.dat

for i in {2..10000}; do
	./collatz -n $i | wc -l >> collatz_2.dat
	
done

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "length.pdf"
    set xrange [0:10000]
    set yrange [0:300]
    set xlabel "{/:Italic n}"
    set ylabel "length"
    set xtics 1000
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
    set output "max_val.pdf"
    set xrange [0:10000]
    set yrange [0:100000]
    set xlabel "{/:Italic n}"
    set ylabel "value"
    set xtics 1000
    set zeroaxis
    set title "Maximum Collatz Sequence Value"
    plot "./collatz_3.dat" with dots notitle
END
#Legnths x Frequency #################
echo > collatz_4.dat

echo -n "2" >> collatz_4.dat

sort -n collatz_2.dat | tail -c +2 > tmp_sort.dat

c=0

prev_line=2

while read line;
do
	if [[ $line -eq $prev_line ]];
       	then
		c=$((c+1));
	else
		
		echo -n ' ' >> collatz_4.dat
		echo $c >> collatz_4.dat
		echo -n $line >> collatz_4.dat
		prev_line=$line
		c=1;
	fi
	
done < tmp_sort.dat

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "freq.pdf"
    set xrange [0:225]
    set yrange [0:200]
    set xlabel "length"
    set ylabel "frequency"
    set ytics 20
    set xtics 25
    set zeroaxis
    set title "Collatz Sequence Length Histogram"
    plot "./collatz_4.dat" with boxes notitle
END
