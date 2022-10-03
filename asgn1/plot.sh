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
#Lengths x Frequency #################
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

echo > collatz_cre.dat

c=0
t=0

for i in {0..9}
do
	while read num_of_comp;
	do
		if [[ $i=${num_of_comp:-2:1} ]];
		then
			c=$((c+1))
			t=$((t+num_of_comp))																	               
		fi
	done < collatz_2.dat
	
	a=$((t/c))
	echo "$i $a" >> collatz_cre.dat
done;

gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "cre.pdf"
    set yrange [0:150]
    set xrange [-1:10]
    set style fill solid 1.0
    set boxwidth 0.9 relative
    set xlabel "{/:Italic n} digit"
    set ylabel "average number of computations"
    set xtics 1
    set zeroaxis
    set title "numbers 2-10,000: each bar is the average number of collatz steps when first digit = {/:Italic n}" offset -2,0
    plot "./collatz_cre.dat" with boxes notitle
END

rm collatz_4.dat
rm collatz_3.dat
rm collatz_2.dat
rm collatz_cre.dat
rm tmp_sort.dat
