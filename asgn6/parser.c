#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>
#include "str.h"
#include "parser.h"

// #define MAX_PARSER_LINE_LENGTH 1000
struct Parser {
	FILE *f;
	char current_line [ MAX_PARSER_LINE_LENGTH + 1];
	uint32_t line_offset ;
};


Parser *parser_create(FILE *f) {
	Parser *p = (Parser *)malloc(sizeof(Parser));
	if (p) {
		p->f = f;
		fgets(p->current_line, MAX_PARSER_LINE_LENGTH, f);
		p->line_offset = 0;
	}
	return p;
}

bool next_word(Parser *p, char *word) {
	int c;
	uint32_t line_length = (uint32_t)(strlen_(p->current_line));
	//printf("\n");
	//printf("length: %u\np->line_offset: %u\n", line_length, p->line_offset);
	//printf("full text: %s\n", p->current_line);
	//printf("\n");
	bool present = false;
	bool only_good = true;
	while (true) {
		uint32_t i = 0;
		for (; p->line_offset < line_length; p->line_offset += 1) { // start at line_offset and ++
			c = tolower((p->current_line)[p->line_offset]);
			if (isalnum(c) || (c == '-' || c == 39)) { // if c is alphnum or - or '
				word[i] = c;
				present = true;
			}else{
				if (present == true) {
					word[i] = '\0';
					only_good = false;
					break;
				}else{
					continue;
				}
			}
			i++;
		} // loop ends if there was only bad letters (line done), if there were only good letters (line done), if good and end with bad, 
		
		if (present == false) { // never a good letter, invalid word, buffer was exhausted call fgets, continue if file not empty
			if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) { // if file empty return false
				return false;
			}else{ // if file not empty continue loop
				p->line_offset = 0;
				line_length = (uint32_t)(strlen_(p->current_line));
				continue;
			}
		}
		
		if (present) { // if only good letters, valid word, so buffer exhausted, doesnt have null termination, if file not empty return true otherwise false
			if (only_good) {
				word[i] = '\0';
				if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) { // if file empty return false
					return false;
                        	}else{ // if file not empty continue loop
					p->line_offset = 0;
					return true;
                        	}	
			}
		
		}

		 // if there was a good char but ended with bad, valid word, buffer may not be exhausted, 
		 // upon next call
		if (p->line_offset == line_length) {
			if (fgets(p->current_line, MAX_PARSER_LINE_LENGTH, p->f) == NULL) { 
				return false;		
			}else{
				p->line_offset = 0;
				return true;
			}
		}else{
			p->line_offset += 1;
                	return true;
		}
	}
	p->line_offset += 1;
	return true;
}

void parser_delete(Parser **p);
	
