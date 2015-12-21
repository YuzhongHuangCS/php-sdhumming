#include <iostream>
#include <stdio.h>
#include "SBuildModel.h"

int main(int argc, char* argv[]){
	if (argc != 3){ 
		printf("usage: ./SDHBuildModel mid_list.txt model_dir\n");
		return 0; 
	}

	BuildSDHummingModel myMidi(argv[1],argv[2]);
	
	if(!myMidi.GenFilelist()){
		std::cout<<"ERROR: Could Not Loading Label File!"<<endl;
		return 0;
	}
	
	if(myMidi.Write2Model()!=0){
		std::cout<<"ERROR: Could Not Write New Model!"<<endl;
		return 0;
	}
	return 0;
}
