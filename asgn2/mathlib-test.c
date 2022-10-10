#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mathlib.h"
#define OPTIONS "ascSCTl"


void one (int *rtc) {
	int something = 1; // This was inspired by the swap function from lecture 
	int *uno;	// pointer type var that harbors *something*'s memory address
	uno = &something;
	*rtc = *uno; // the value at memory address rtc is now the value at memory adress uno because pointers point to the value at the adress
}


int main(int argc, char **argv) {
        int opt = 0;
	int call[7] = {0,0,0,0,0,0,0};
	while ((opt = getopt(argc, argv, OPTIONS)) != -1) {   //loops through options
		
		switch(opt) {
		case 's':
			one(&call[0]); //set array index to 1 to make it globally available that an option is present
			break;
		case 'c':
			one(&call[1]);
			break;
                case 'S':
			one(&call[2]);
                        break;
                case 'C':
			one(&call[3]);
                        break;
                case 'T':
                        one(&call[4]);
			break;
                case 'l':
			one(&call[5]);
                        break;	
                case 'a':
			one(&call[6]);
                        break;	
		}

	}
	



		if (call[0] == 1 || call[6] == 1){ //if sin is found will trigger or if -a is found
			printf("  x             sin              Library       Difference\n  -             ------           -------       ----------\n");
			for (double i = 0; i <= 2 * M_PI; i = i + (.05 * M_PI)) {  //using float because M_PI is a constant so we wont run into any equality issues
				printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, sin(i), my_sin(i), sin(i) - my_sin(i));	
					} 
		}	
		if (call[1] == 1 || call[6] == 1) {
			printf("  x             cos              Library       Difference\n  -             ------           -------       ----------\n");
                       	for (double i = 0; i <= 2 * M_PI; i = i + (.05 * M_PI)) {  //using float because M_PI is a constant so we wont run into any equality issues
                        	printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, cos(i), my_cos(i), cos(i) - my_cos(i));
                                        }
		}
	       	if (call[2] == 1 || call[6] == 1) {
                        printf("  x             arcsin              Library       Difference\n  -             ------           -------       ----------\n");
                        for (double i = -1; i <= 1.04; i = i + (.05)) {  //using float because M_PI is a constant so we wont run into any equality issues
                                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, asin(i), my_arcsin(i), asin(i) - my_arcsin(i));
                                        }
		}
	       	if (call[3] == 1 || call[6] == 1) {
                	printf("  x             arccos              Library       Difference\n  -             ------           -------       ----------\n");
                        for (double i = -1; i <= 1.04; i = i + (.05)) {  //using float because M_PI is a constant so we wont run into any equality issues
                		printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, acos(i), my_arccos(i), acos(i) - my_arccos(i));        
			}
		}
	       	if (call[4] == 1 || call[6] == 1) {
			printf("  x             arctan              Library       Difference\n  -             ------           -------       ----------\n");
                        for (double i = 1; i <= 10; i = i + (.05)) {  //using float because M_PI is a constant so we wont run into any equality issues
                                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, atan(i), my_arctan(i), atan(i) - my_arctan(i));
			}
		}
	       	if (call[5] == 1 || call[6] == 1) { 
			printf("  x             log              Library       Difference\n  -             ------           -------       ----------\n");
                        for (double i = 1; i <= 10; i = i + (.05)) {  //using float because M_PI is a constant so we wont run into any equality issues
                                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", i, log(i), my_log(i), log(i) - my_log(i));
		}
	}

return 0;
}
