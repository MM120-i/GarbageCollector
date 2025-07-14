#include <stdio.h>

#include "vm.h"
#include "sneknew.h"
#include "snekobject.h"

// Test cases
int main(void)
{

    vm_t *virtualMachine = vm_new();

    if (!virtualMachine)
    {
        printf("Failed to create a VM\n");
        exit(EXIT_FAILURE);
    }

    // Test case 1: Create and Collect a single integer
    snek_object_t *integerObject = new_snek_integer(virtualMachine, 42);
    printf("Created integer object at %p\n", (void *)integerObject);
    vm_collect_garbage(virtualMachine);

    // Test case 2: Create an array and garbage collect it

    return 0;
}