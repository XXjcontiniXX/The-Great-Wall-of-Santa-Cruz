# Keygen  
Usage: ./keygen [options]  
  ./keygen generates a public / private key pair, placing the keys into the public and private  
  key files as specified below. The keys have a modulus (n) whose length is specified in  
  the program options.  
    -s <seed>   : Use <seed> as the random number seed. Default: time()  
    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024  
    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50  
    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub  
    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv  
    -v          : Enable verbose output.  
    -h          : Display program synopsis and usage.  

# Encrypt  
Usage: ./encrypt [options]  
  ./encrypt encrypts an input file using the specified public key file,  
writing the result to the specified output file.  
    -i <infile> : Read input from <infile>. Default: standard input.  
    -o <outfile>: Write output to <outfile>. Default: standard output.  
    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.  
    -v          : Enable verbose output.  
    -h          : Display program synopsis and usage.  
### Decrypt's bugs:  
If ./encrypt is not given an -i argument and stdin is also empty, the program runs forever and should be halted with ctrl + c.  

# Decrypt  
Usage: ./decrypt [options]  
./decrypt decrypts an input file using the specified private key file,  
writing the result to the specified output file.  
    -i <infile> : Read input from <infile>. Default: standard input.  
    -o <outfile>: Write output to <outfile>. Default: standard output.  
    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.  
    -v          : Enable verbose output.  
    -h          : Display program synopsis and usage.  

### Encrypt's bugs:  
  If ./decrypt is not given an -i argument and stdin is also empty, the program runs forever and should be halted with ctrl + c.  
  
  Additionally, decrypt does not automatically print with a newline so when decrypt reads to stdout text can print behind the user's and path instead of in its own line.
