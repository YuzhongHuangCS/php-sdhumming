/*
* STester.cpp  SDHumming Tester routines
*
* Copyright (c) 2010 Shanda Interactive Entertainment Limited.
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or   
* any later version. See the COPYRIGHT.txt for more details.       
*/

#include "SModel.h"
#include "SDFuzzySearch.h"
#include "SUtil.h"
#include "SMelody.h"
#include <stdio.h>
#include <time.h>

/* search one query */
int STester(char* szModel, char* szModelinfo, char* szWav, char* szOut){
	int i=0;

	//0, Load Model
	SModel *SQBHModels=NULL;
	char **szModelInfoStrs=NULL; 
	int nModels=0;
	int nTotalModel=SLoadModel(szModel, SQBHModels, nModels);
	if(nTotalModel<=0){
		printf("Error on loading model!\n");
		return -1;
	}
	int nTotalSongs=SReadMelodyDBInfoFile(szModelinfo,szModelInfoStrs);
	if(nTotalModel<=0){
		printf("Error on loading modelinfo!\n");
		return -1;
	}
	//1, Feature Extraction
	float *pFeaBuf = NULL;
	int nFeaLen = 0;
	SNote *QueryNotes= NULL;
	int nNoteLen=0;
	SMelodyFeatureExtraction(szWav,pFeaBuf,nFeaLen,QueryNotes,nNoteLen);	
	
	//2, Melody Search
	NoteBasedResStru *myEMDResStru=new NoteBasedResStru[nTotalModel];
	SNoteBasedMatch(SQBHModels, nModels, QueryNotes, nNoteLen,myEMDResStru,nFeaLen);

	FrameBasedResStru *myDTWResStru=new FrameBasedResStru[20];
	SFrameBasedMatch(SQBHModels, nModels, pFeaBuf, nFeaLen, myEMDResStru, 20, myDTWResStru);

	//3, Finalize and print the result
	FILE *OutFile=fopen(szOut,"a+t");
	fprintf(OutFile,"%s ",szWav);
	for(i=0;i<20;i++){
		fprintf(OutFile,"%d: %s, %f; ",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
		printf("%d: %s, %f\n",myDTWResStru[i].nModelID+1,szModelInfoStrs[myDTWResStru[i].nModelID], myDTWResStru[i].fScore);
	}
	fprintf(OutFile,"\n");
	fclose(OutFile);
	
	for(i=0;i<nTotalSongs;i++){
		if(NULL!=SQBHModels[i].PhrasePos){
			delete[] SQBHModels[i].PhrasePos; 
			SQBHModels[i].PhrasePos=NULL;
		}
		if(NULL!=SQBHModels[i].sNotes){
			delete[] SQBHModels[i].sNotes;
			SQBHModels[i].sNotes=NULL;
		}
	}

	for(i=0;i<nTotalSongs;i++){
		if(NULL != szModelInfoStrs[i]){
			delete[] szModelInfoStrs[i];
			szModelInfoStrs[i]=NULL;
		}
	}
	if(NULL!=myEMDResStru){
		delete[] myEMDResStru;
		myEMDResStru=NULL;
	}
	if(NULL!=myDTWResStru){
		delete[] myDTWResStru;
		myDTWResStru=NULL;
	}
	if(NULL!=SQBHModels){
		delete[] SQBHModels;
		SQBHModels=NULL;
	}
	if(NULL!=szModelInfoStrs){
		delete[] szModelInfoStrs;
		szModelInfoStrs=NULL;
	}
	if(NULL!=QueryNotes){
		delete[] QueryNotes;
		QueryNotes=NULL;
	}
	if(NULL!=pFeaBuf){
		delete[] pFeaBuf;
		pFeaBuf=NULL;
	}
	return 0;
}


int main(int argc, char* argv[]){
	if (argc != 5){ 
		printf("usage: ./SDHumming QBHModel.dat QBHModel.info test.wav result.txt \n");
		return 0; 
	}

	STester(argv[1],argv[2],argv[3],argv[4]);
	printf("Elapsed time: %f \n",clock()/(CLOCKS_PER_SEC+0.0f));
	return 0;
}

 