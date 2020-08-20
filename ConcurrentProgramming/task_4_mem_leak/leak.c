#include <stdlib.h>
#include <dmalloc.h>

void leak(int n){
	malloc(n);
}

int main(){
	leak(1);
	leak(2);
	leak(3);
	return 0;
}
