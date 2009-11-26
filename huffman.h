/*
 * huffman.h
 *
 *  Created on: Sep 12, 2009
 *      Author: mits
 */

#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include "stdio.h"
#include <stdint.h>

#define MAXLINESIZE 255

struct listEntry
{
	uint32_t freq;
	uint8_t blocksize;
	char *string;
	struct listEntry *prev;
	struct listEntry *next;
	struct listEntry *childs[2];
};

int makeBlocksList(FILE *fin, int blocksize, struct listEntry **head, struct listEntry **tail);
int doHuffman(struct listEntry **head, struct listEntry **tail, int num2encode, char ***encodedBlocks, char ***codes, int **blockFreqsPtr);
int parseFile(FILE *f, int blocksize);
void makeOutput(FILE *in, FILE *out, char **blocks, char **codes,int blocksize, int encodedblocks);


#endif /* HUFFMAN_H_ */
