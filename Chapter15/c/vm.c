#include "common.h"
#include "debug.h"
#include "vm.h"
#include <stdio.h>
#include "memory.h"

VM vm;

static void resetStack()
{
    vm.stackCount = 0; // Count 0 means empty
}

void initVM()
{
    vm.stack = NULL; // No Array yet
    vm.stackCapacity = 0;
    resetStack();
}

void freeVM()
{
    FREE_ARRAY(Value, vm.stack, vm.stackCapacity); // Problem 2 free memory, stack on heap
    initVM();
}

// New Helper for growing
static void growStack()
{
    int oldCapacity = vm.stackCapacity;
    vm.stackCapacity = GROW_CAPACITY(oldCapacity);
    vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
}

void push(Value value)
{
    if (vm.stackCapacity < vm.stackCount + 1) // Check if room for one more, otherwise grow
    {
        growStack();
    }

    vm.stack[vm.stackCount] = value;
    vm.stackCount++;
}

Value pop()
{
    vm.stackCount--;
    return vm.stack[vm.stackCount];
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

#define BINARY_OP(op) \
    do { \
      double b = pop(); \
      double a = pop(); \
      push(a op b); \
    } while (false)

    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (int i = i; i < vm.stackCount; i++)
        {
            printf("[ ");
            printValue(vm.stack[i]);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,
                               (int)(vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE())
        {
        case OP_CONSTANT:
        {
            Value constant = READ_CONSTANT();
            push(constant);
            break;
        }
        case OP_ADD:
            BINARY_OP(+);
            break;
        case OP_SUBTRACT:
            BINARY_OP(-);
            break;
        case OP_MULTIPLY:
            BINARY_OP(*);
            break;
        case OP_DIVIDE:
            BINARY_OP(/);
            break;
        case OP_NEGATE:
            push(-pop());
            break;
        case OP_RETURN:
        {
            printValue(pop());
            printf("\n");
            return INTERPRET_OK;
        }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk)
{
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}
