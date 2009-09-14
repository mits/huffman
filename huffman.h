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
	uint32_t index;
	uint32_t freq;
	uint8_t blocksize;
	struct listEntry *prev;
	struct listEntry *next;
	struct listEntry *childs[2];
};

int parseFile(FILE *f, int blocksize);
int findIndex(char *s,int blocksize);
int findString(char *s,int blocksize,uint32_t index);
void addBlock(char *block, int blocksize);
void removeEntry(struct listEntry *block);
void insertAfter(struct listEntry *block, struct listEntry *previous);
struct listEntry* findFreqPos(struct listEntry *block);
struct listEntry* createEntry(int index, uint8_t blocksize);
void deleteEntry(struct listEntry *entry);
void initList();
void printEntry(struct listEntry *block);
void mergeAll();
struct listEntry* findNextUnspecified(struct listEntry *block);
int canMerge(char *str1,  char *str2, int blocksize);
void mergeX(struct listEntry *one, struct listEntry *two);
void merge(struct listEntry *one, struct listEntry *two);
void mergeStrings(char *str1, char *str2, int blocksize);
void mergeRest();
void makeTree();
void chopList(int blocks2encode);
void searchTree(struct listEntry *node,int position, char *encoding, uint32_t *blocks, char **codes);
void makeBlocksTable(uint32_t *table);
void makeOutput(FILE *in, FILE *out, uint32_t *blocks, char **codes,int blocksize, int encodedblocks);
#endif /* HUFFMAN_H_ */