#include <stdlib.h>

#include "chunk.h"
#include "memory.h"


void initChunk(Chunk* chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lineCount = 0; // Adding new fields
    chunk->lineCapacity = 0;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk)
{
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineStart, chunk->lines, chunk->lineCapacity); // Swap int for LineStart, capacity for lineCapacity
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line)
{
    // Growing array if needed so not touching lines, two arrays grow separately
    if (chunk->capacity < chunk->count + 1)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
                                 oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    // If same line as last record, then nothing to add
    if (chunk->lineCount > 0 && chunk->lines[chunk->lineCount - 1].line == line)
    {
        return;
    }
    // Grow line array if needed
    if (chunk->lineCapacity < chunk->lineCount + 1)
    {
        int oldCapacity = chunk->lineCapacity;
        chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
        chunk->lines = GROW_ARRAY(LineStart, chunk->lines, oldCapacity, chunk->lineCapacity);
    }

    // Record where line starts
    LineStart* lineStart = &chunk->lines[chunk->lineCount];
    lineStart->offset = chunk->count - 1; // -1 since count++ earlier
    lineStart->line = line;
    chunk->lineCount++;
}

// New getLine function
int getLine(Chunk* chunk, int instruction)
{
    int line = -1; // Not found

    for (int i = 0; i < chunk->lineCount; i++)
    {

        // Break if record is after instruction
        if (chunk->lines[i].offset > instruction) break;

        line = chunk->lines[i].line;
    }
    return line;
}

int addConstant(Chunk* chunk, Value value)
{
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

// New Method for Problem 2
void writeConstant(Chunk* chunk, Value value, int line)
{
    // Add value to constants array, get position
    int constantIndex = addConstant(chunk, value);

    if (constantIndex < 256)
    {
        // If fits in one byte, use old short instruction
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (uint8_t)constantIndex, line);
    }
    else
    {
        // If larger than one byte, use new long instruction
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, (uint8_t)(constantIndex & 0xff), line); // Lowest 8 bits
        writeChunk(chunk, (uint8_t)((constantIndex >> 8) & 0xff), line); // Middle 8 bits
        writeChunk(chunk, (uint8_t)((constantIndex >> 16) & 0xff), line); // Highest 8 bits
    }
}
