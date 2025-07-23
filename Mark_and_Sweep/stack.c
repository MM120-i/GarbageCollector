#include <stdio.h>

#include "stack.h"

/**
 * @brief Pushes an object onto the stack.
 *
 * If the stack is full, its capacity is doubled and memory is reallocated.
 * Exits the program if memory allocation fails.
 *
 * @param stack Pointer to the stack structure.
 * @param object Pointer to the object to be pushed onto the stack.
 */
void stack_push(stack_t *stack, void *object)
{
    if (stack->count == stack->capacity)
    {
        // So basically double the stack's capacity to void reallocating often
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->capacity * sizeof(void *));

        // Checks if we were able to reallocate
        if (stack->data == NULL)
        {
            exit(EXIT_FAILURE);
        }
    }

    stack->data[stack->count] = object;
    stack->count++;

    return;
}

/**
 * Pops an element from the top of the stack.
 *
 * @param stack Pointer to the stack structure.
 * @return Pointer to the popped element, or NULL if the stack is empty.
 */
void *stack_pop(stack_t *stack)
{
    if (stack->count == 0)
    {
        return NULL;
    }

    stack->count--;

    return stack->data[stack->count];
}

/**
 * Frees the memory allocated for a stack.
 *
 * @param stack Pointer to the stack to be freed.
 */
void stack_free(stack_t *stack)
{
    if (stack == NULL)
    {
        return;
    }

    if (stack->data != NULL)
    {
        free(stack->data);
    }

    free(stack);
}

void stack_remove_nulls(stack_t *stack)
{
    size_t newCount = 0;

    // Iterate through the stack and compact non-null pointers
    for (size_t i = 0; i < stack->count; i++)
    {
        if (stack->data[i] != NULL)
        {
            stack->data[newCount++] = stack->data[i];
        }
    }

    // then we update the count to reflect the new number of elements
    stack->count = newCount;

    // we r zeroing out the remaining slots:
    for (size_t i = newCount; i < stack->capacity; i++)
    {
        stack->data[i] = NULL;
    }
}

stack_t *stack_new(size_t capacity)
{
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));

    if (!stack)
    {
        perror("Memory allocation failed on the stack\n");
        return NULL;
    }

    stack->count = 0;
    stack->capacity = capacity;
    stack->data = malloc(stack->capacity * sizeof(void *));

    if (!stack->data)
    {
        perror("Memory allocation on the stack->data failed\n");
        free(stack);
        return NULL;
    }

    return stack;
}