#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "str.h"


size_t strlen_(const char *s) {
	size_t i = 0;
	while (s[i] != '\0') {
		i++;
	}
	return i;
}

int strcmp_(const char *s1, const char *s2) {
	uint32_t i = 0;
	for (; s1[i] != '\0'; i++) {
		uint8_t sum =  (unsigned char)(s1[i]) - (unsigned char)(s2[i]);
		if (sum != 0) {
			return 0;
		}

	}
	return !((unsigned char)(s1[i]) - (unsigned char)(s2[i])); // only returns 1 when same ending									   // else some random number
}

void strcpy_(char *s1, const char *s2) {
	size_t len = strlen_(s2);	
	for (size_t i = 0; i < len; i++) {
		s1[i] = s2[i];
	}
	return;

}
