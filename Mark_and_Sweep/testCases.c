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

    // ====================================================================================================
    // Test case 1: Create and Collect a single integer
    snek_object_t *integerObject = new_snek_integer(virtualMachine, 42);
    printf("\nCreated integer object at %p\n", (void *)integerObject);
    vm_collect_garbage(virtualMachine);

    // ====================================================================================================
    // Test case 2: Create an array and garbage collect it
    snek_object_t *array = new_snek_array(virtualMachine, 3);
    snek_array_set(array, 0, new_snek_integer(virtualMachine, 1));
    snek_array_set(array, 1, new_snek_integer(virtualMachine, 2));
    snek_array_set(array, 2, new_snek_integer(virtualMachine, 3));
    printf("Created an array object at %p\n", (void *)array);
    vm_collect_garbage(virtualMachine);

    // ====================================================================================================
    // Test case 3: Create a vector object and garbage collect it
    snek_object_t *vector = new_snek_vector3(
        virtualMachine,
        new_snek_float(virtualMachine, 1.1f),
        new_snek_float(virtualMachine, 2.2f),
        new_snek_float(virtualMachine, 3.3f));
    printf("Created a vector3 object at %p\n", (void *)vector);

    // ====================================================================================================
    // Test case 4: Create a string and garbage collect it
    snek_object_t *string = new_snek_string(virtualMachine, "hello world");
    printf("Created string object at %p\n", (void *)string);
    vm_collect_garbage(virtualMachine);

    // ====================================================================================================
    // Test case 5: Remove references and garbage collect it
    array = NULL;
    vector = NULL;
    string = NULL;
    integerObject = NULL;
    vm_collect_garbage(virtualMachine);

    // Clean up the virtual machine itself
    vm_free(virtualMachine);
    printf("All tests completed.\n");

    return 0;
}