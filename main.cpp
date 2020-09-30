#include "initres.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){

	init();

	userMain(argc, argv);

	reset();

}



