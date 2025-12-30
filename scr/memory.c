#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// These symbols are provided by the linker
extern char etext;  // End of text segment
extern char edata;  // End of initialized data segment
extern char end;    // End of BSS segment (start of heap)

// Global variables
int g_uninitialized;          // Goes to BSS [cite: 671]
int g_initialized = 10;       // Goes to .data [cite: 672]

// Function in text segment
void sample_function() {
    printf("hey");
}

int main() {
    // Local variable → stack [cite: 678]
    int local_var = 5;

    // Allocate memory on heap
    int *heap_var = malloc(sizeof(int));
    *heap_var = 123;

    printf("========== MEMORY SEGMENTS ==========\n\n");

    printf("Text segment (code):\n");
    printf("  Address of main():             %p\n", main);
    printf("  Address of sample_function():  %p\n", sample_function);
    printf("  &etext (end of text):          %p\n\n", &etext);

    printf("Initialized Data Segment (.data):\n");
    printf("  g_initialized variable:        %p\n", &g_initialized);
    printf("  &edata (end of .data):         %p\n\n", &edata);

    printf("Uninitialized Data Segment (BSS):\n");
    printf("  g_uninitialized variable:      %p\n", &g_uninitialized);
    printf("  &end (end of BSS → start of heap):   %p\n\n", &end);

    printf("Heap:\n");
    printf("  heap_var via malloc():         %p\n", heap_var);
    printf("  sbrk(0) current program break: %p\n\n", sbrk(0));

    printf("Stack:\n");
    printf("  Address of local_var:          %p\n\n", &local_var);

    printf("========== CONSTANTS FOR COMPARISON ==========\n\n");
    printf("  0x1000  = %p\n", (void*)0x1000);
    printf("  0xFFFF  = %p\n\n", (void*)0xFFFF);
    printf("==============================================\n");

    free(heap_var);
    return 0;
}
