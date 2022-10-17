#!/bin/bash

make

c=0


echo -n > average.dat
echo -n > rounds.dat

for i in {1..100}; do
		./play-dreidel -s $i -p 2 -c 4 >> average.dat
done

<<comment
while read line;
do
	for sec in $line;
	do
		c=$((c+1)) # increments
		if [ $((c%5)) == 4 ] then  # if the substr % 5 is 4 its def round number #if 1 its winner name
			echo $sec >> rounds.dat
		fi
	done

done < average.dat

comment


echo -n > p_v_w.dat

counter=0


echo -e -n "A\nB\n" > first_letter.dat  


a=0
b=0

while read letter;
do
	while read line;
	do
	
		for name in $line;
		do
			counter=$((counter+1)) # increments
			
			if [ $((c%5)) == 1 ];then # if a name
				echo $name
				if [[ ${name:0:1}=$letter ]];then # check if first letter of name is letter
				      
					if [[ $letter="a" ]];then # increment the letter that is $letter
						a=$((a+1))
						echo "a"

					elif [[ $letter="b" ]];then
						b=$((b+1))
						echo "b"
					fi
				fi
			fi
		done
	done < average.dat		
done < first_letter.dat

echo "$a\n$b" > player_vs_player_wins.dat









gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "ab.pdf"
    set xrange [1:2]
    set yrange [0:100]
    set xlabel "{/:Italic players}"
    set ylabel "wins" 
    set zeroaxis
    set title "a vs b"
    plot "./player_vs_player_wins.dat" with boxes notitle
END

