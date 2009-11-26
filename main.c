/*
 * main.c
 *
 *  Created on: Sep 12, 2009
 *      Author: mits
 */

#include "huffman.h"
#include "math.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
	int i,m,blocksize,num2encode;
	struct listEntry *head,*tail;
	char **encodedBlocks;
	char **codes;
	int *blockFreqs;
	char inputFile[50],outputFile[50];
	int totalBlocks;
	FILE *fin,*fout;
	blocksize = 4;
	num2encode = 8;
	if (argc != 4) {
		printf("Xrisi: huffman <onoma arxeiou> <megethos block> <arithmos encoded blocks>\n");
		return -1;
	}
	strcpy(inputFile,argv[1]);
	blocksize = atoi(argv[2]);
	num2encode = atoi(argv[3]);
	fin = fopen(inputFile, "r");
	if (fin==NULL) {
		printf("Input file not found\n");
		return -1;
	}
	totalBlocks = makeBlocksList(fin, blocksize, &head, &tail);
	fclose(fin);
	printf("Total Blocks in file = %d\n", totalBlocks);
	m = doHuffman(&head, &tail, num2encode, &encodedBlocks, &codes, &blockFreqs);
	printf("Encoded blocks = %d\n", m);
	for (i=0;i<m;i++)
	{
		printf("block %d: %s,   code %s,   frequency %d\n", i, encodedBlocks[i], codes[i],  blockFreqs[i]);
	}
	int inputSize = totalBlocks * blocksize;
	int outputSize = 0;
	for (i = 0; i < m; i++)
	{
		outputSize += strlen(codes[i]) * blockFreqs[i];
	}
	if (encodedBlocks[m-1][0] == 'U')
	{
		outputSize += blocksize * blockFreqs[m-1];
	}
	printf("input size = %d\n", inputSize);
	printf("output size = %d\n", outputSize);
	fin = fopen(inputFile, "r");
	sprintf(outputFile, "out-%s-%d-%d.txt",inputFile, blocksize, num2encode);
	fout = fopen(outputFile, "w");
	makeOutput(fin, fout, encodedBlocks, codes, blocksize, m);
	fclose(fin);
	fclose(fout);

	return 0;
}
