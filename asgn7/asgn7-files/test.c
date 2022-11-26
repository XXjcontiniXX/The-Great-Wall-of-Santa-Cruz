#include <stdio.h>
#include "node.h"


int main(void) {
	Node *n = node_create('\n', 5);
	node_print(n);
	node_print_sym(n);
	return 0;
}
