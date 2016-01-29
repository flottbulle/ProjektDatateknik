#include <iostream>
#include <stdio.h>
//using namespace std;



int keyreg (){
    char c1;
    scanf("%c", &c1);
    
    switch(c1)
    {
        case 'w':
            return 1;

        case 's':
            return 2;

        case 'a':
            return 3;

	case 'd':
	    return 4;
    }
    return 5;
}

int main() {
    while(1){
	int out = keyreg();
	if(out != 5){
	        printf("Output: %i \n", out);
	}
    }
}
