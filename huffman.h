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

int makeBlocksList(FILE *fin, int blocksize, struct listEntry **head, struct listEntry **tail);
int doHuffman(struct listEntry **head, struct listEntry **tail, int num2encode, uint32_t **encodedBlocks, char ***codes);
int parseFile(FILE *f, int blocksize);
size_t readLine(char *line, size_t maxBytes,FILE *f);
int findIndex(char *s,int blocksize);
int findString(char *s,int blocksize,uint32_t index);
void addBlock(char *block, int blocksize);
void removeEntry(struct listEntry *block);
void insertAfter(struct listEntry *block, struct listEntry *previous);
struct listEntry* findFreqPos(struct listEntry *block, struct listEntry *start);
struct listEntry* createEntry(int index, uint8_t blocksize);
void deleteEntry(struct listEntry *entry);
void initList(struct listEntry **h, struct listEntry **t);
void printEntry(struct listEntry *block);
void printList(struct listEntry *head);
void mergeAll();
struct listEntry* findUnspecified(struct listEntry *block);
int canMerge(char *str1,  char *str2, int blocksize);
void mergeX(struct listEntry *one, struct listEntry *two);
void merge(struct listEntry *one, struct listEntry *two);
void mergeStrings(char *str1, char *str2, int blocksize);
void mergeRest();
void makeTree();
int chopList(int blocks2encode);
void searchTree(struct listEntry *node,int position, char *encoding, uint32_t *blocks, char **codes);
void makeBlocksTable(uint32_t *table);
void makeOutput(FILE *in, FILE *out, uint32_t *blocks, char **codes,int blocksize, int encodedblocks);
void encodeBlock(char *unenc, char *enc, uint32_t *blocks, char **codes, int blocksize, int encodedblocks, char *unencodedCode);
#endif /* HUFFMAN_H_ */
