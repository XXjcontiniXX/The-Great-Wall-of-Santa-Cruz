#Instructions on how to use play-dreidel  
To use play-dreidel type `make` in your terminal. Once make has been run, all the necesary files will have been compiled.  

Next type `./play-dreidel` into the terminal to run the program with default settings.  

play-dreidel has 3 different starting enviroment parameters.

`-p` for the number of starting players  
`-c` for the number of starting coins per player  
`-s` for the seed  
and `-v` for showing elimination order  

p, c, and s all take arguments after they're typed.

an example call might be `./play-dreidel -s 613 -c 3 -p 4 -v`  (this is also the default settings)  

which would result in 4 player game, with 3 coins to start, a seed of 613, and elimination messages.  

**Happy Simulating!**  
