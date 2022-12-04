# A Huffman encoding and decoding algorithm  
  
## How to encode  

  Compresses a file using the Huffman coding algorithm.
  
USAGE  
  `./encode [-h] [-i infile] [-o outfile]`  
  
OPTIONS  
  `-h`             Program usage and help.  
  `-v`             Print compression statistics.  
  `-i` infile      Input file to compress.  
  `-o` outfile     Output of compressed data.  

## How to decode  
  
  Decompresses a file using the Huffman coding algorithm.  

USAGE  
  `./decode [-h] [-i infile] [-o outfile]`  

OPTIONS  
  `-h`             Program usage and help.  
  `-v`             Print compression statistics.  
  `-i` infile      Input file to decompress.  
  `-o` outfile     Output of decompressed data.  

### Final notes  
The decode function will not attempt to decode a file unless the correct identification number 0xBEEFBBAD is explicitly delclared within the first four bytes of file to be decoded.  
#### Happy compressing!
