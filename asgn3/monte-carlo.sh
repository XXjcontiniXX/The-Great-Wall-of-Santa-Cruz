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


a=0
#b=0
#c=0
#d=0
#e=0
#f=0
#g=0
#h=0

while read letter;
do
	while read line;
	do
	
		for name in $line;
		do
			counter=$((counter+1)) # increments
			
			if [ $((counter%5)) == 1 ];then # if a name

				if [[ ${name:0:1} == "A" ]];then # if first letter of name is A incr A
					a=$((a+1))
				fi

				if [[ ${name:0:1} == "B" ]];then # if B incr B
					b=$((b+1))
				fi
				
				<<com
				if [[ ${name:0:1} == "C" ]];then # if C incr C
                                        c=$((c+1))
                                fi
				
				if [[ ${name:0:1} == "D" ]];then # if C incr C
                                        d=$((d+1))
                                fi
				
				if [[ ${name:0:1} == "E" ]];then # if first letter of name is A incr A
                                        e=$((e+1))
                                fi

				if [[ ${name:0:1} == "F" ]];then # if first letter of name is A incr A
                                        f=$((f+1))
                                fi
				
				if [[ ${name:0:1} == "G" ]];then # if first letter of name is A incr A
                                        g=$((g+1))
                                fi
				
				if [[ ${name:0:1} == "H" ]];then # if first letter of name is A incr A
                                        h=$((h+1))
                                fi
com
			fi
		done
	done < average.dat		
done < first_letter.dat
echo $a
echo $b

echo -n -e "$a\n$b" > player_vs_player_wins.dat









gnuplot <<END
    set termoption enhanced
    set terminal pdf
    set output "ab.pdf"
    set xrange [-1:2]
    set yrange [0:200]
    set xlabel "{/:Italic player}"
    set ylabel "wins" 
    set zeroaxis
    set style fill solid 1.0
    set boxwidth 0.5 relative
    set title "a vs b"
    plot "./player_vs_player_wins.dat" with boxes notitle
END

