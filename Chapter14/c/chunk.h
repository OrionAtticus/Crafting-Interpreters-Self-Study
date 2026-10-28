#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"
typedef enum
{
    OP_CONSTANT,
    OP_CONSTANT_LONG, // New 3 bytes + operand opcode
    OP_RETURN,
} OpCode;

// New Struct for LineStart
typedef struct{
    int offset;
    int line;
} LineStart;

typedef struct
{
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
    int lineCount; // Count of LineStart Records
    int lineCapacity; // How many we have room for
    LineStart* lines; // Changed from int
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, int line);
int addConstant(Chunk* chunk, Value value);
int getLine(Chunk* chunk, int instruction); // New Prototype problem 1
void writeConstant(Chunk* chunk, Value value, int line); // Problem 2 Prototype

#endif
