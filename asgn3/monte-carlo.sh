#!/bin/bash

#Reconstruct collatz executable

make

c=0


echo -n > average.dat
echo -n > rounds.dat

for i in {1..1000}; do
	./play-dreidel -s $i -p 6 -c 4 >> average.dat
done

for i in {2..1000}; do
	while read line;
	do
		for sec in $line;
		do
			c=$((c+1)) # increments
			if [ $((c%5)) == 4 ]; then  # if the substr % 5 is 4 its def a round number
				echo $sec >> rounds.dat
			fi
		done	

	done < average.dat
		
done





