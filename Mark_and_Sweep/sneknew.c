#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sneknew.h"
#include "vm.h"
#include "snekobject.h"

/**
 * Allocates and initializes a new Snek integer object.
 *
 * @param vm Pointer to the virtual machine instance responsible for tracking objects.
 * @return Pointer to the newly allocated snek_object_t, or NULL on failure.
 */
snek_object_t *_new_snek_integer(vm_t *vm)
{
    snek_object_t *object = (snek_object_t *)calloc(1, sizeof(snek_object_t));

    if (!object)
    {
        perror("memalloc failed, object is null\n");
        return NULL;
    }

    vm_track_object(vm, object);
    return object;
}

/**
 * Allocates and initializes a new Snek array object.
 *
 * @param vm Pointer to the virtual machine instance.
 * @param size Number of elements in the array.
 * @return Pointer to the newly created snek_object_t representing the array,
 *         or NULL if memory allocation fails.
 */
snek_object_t *new_snek_array(vm_t *vm, size_t size)
{
    snek_object_t *obj = _new_snek_object(vm);

    if (!obj)
    {
        perror("Memalloc failed on obj\n");
        return NULL;
    }

    snek_object_t **elements = (snek_object_t **)calloc(size, sizeof(snek_object_t *));

    if (!elements)
    {
        perror("Memalloc failed on elements\n");
        free(obj);
        return NULL;
    }

    obj->kind = ARRAY;
    obj->data.v_array = (snek_array_t){.size = size, .elements = elements};

    return obj;
}

/**
 * Creates a new snek_object_t of kind VECTOR3 with specified x, y, and z components.
 *
 * @param vm Pointer to the virtual machine context.
 * @param x Pointer to the snek_object_t representing the x component.
 * @param y Pointer to the snek_object_t representing the y component.
 * @param z Pointer to the snek_object_t representing the z component.
 * @return Pointer to the newly created snek_object_t of kind VECTOR3, or NULL on failure.
 */
snek_object_t *new_snek_vector3(vm_t *vm, snek_object_t *x, snek_object_t *y, snek_object_t *z)
{
    if (!x || !y || !z)
    {
        perror("Memalloc failed on x or y or z");
        return NULL;
    }

    snek_object_t *obj = _new_snek_object(vm);

    if (!obj)
    {
        perror("Memalloc failed on obj\n");
        return NULL;
    }

    obj->kind = VECTOR3;
    obj->data.v_vector3 = (snek_vector_t){.x = x, .y = y, .z = z};

    return obj;
}

/**
 * Creates a new Snek integer object and initializes it with the given value.
 *
 * @param vm Pointer to the virtual machine instance managing object allocation.
 * @param value The integer value to store in the new Snek object.
 * @return Pointer to the newly created snek_object_t representing an integer, or NULL on failure.
 */
snek_object_t *new_snek_integer(vm_t *vm, int value)
{
    snek_object_t *obj = _new_snek_object(vm);

    if (!obj)
    {
        perror("Memalloc failed on obj\n");
        return NULL;
    }

    obj->kind = INTEGER;
    obj->data.v_int = value;

    return obj;
}

/**
 * Creates a new Snek float object and initializes it with the given value.
 *
 *
 * @param vm Pointer to the virtual machine context.
 * @param value The float value to store in the new object.
 * @return Pointer to the newly created snek_object_t, or NULL on allocation failure.
 */
snek_object_t *new_snek_float(vm_t *vm, float value)
{
    snek_object_t *obj = _new_snek_object(vm);

    if (!obj)
    {
        perror("Memalloc failed on obj\n");
        return NULL;
    }

    obj->kind = FLOAT;
    obj->data.v_float = value;

    return obj;
}

/**
 * Creates a new Snek string object and allocates memory for its value.
 *
 * @param vm Pointer to the virtual machine context.
 * @param value The null-terminated string to be stored in the new object.
 * @return Pointer to the newly created snek_object_t, or NULL on failure.
 */
snek_object_t *new_snek_string(vm_t *vm, char *value)
{
    snek_object_t *obj = _new_snek_object(vm);

    if (!obj)
    {
        perror("Memalloc failed on obj\n");
        return NULL;
    }

    int len = strlen(value);
    char *dst = (char *)malloc(len + 1);

    if (!dst)
    {
        perror("Memalloc failed on dst\n");
        free(obj);
        return NULL;
    }

    strcpy(dst, value);

    obj->kind = STRING;
    obj->data.v_string = dst;

    return obj;
}

/**
 * Allocates and initializes a new snek_object_t instance, tracking it in the VM.
 *
 * @param vm Pointer to the virtual machine instance used for tracking the object.
 * @return Pointer to the newly allocated snek_object_t object, or NULL on failure.
 *
 * @note If vm is NULL, an error is printed and NULL is returned.
 *       If memory allocation for the object fails, an error is printed, the vm is freed,
 *       and NULL is returned.
 */
snek_object_t *_new_snek_object(vm_t *vm)
{
    if (!vm)
    {
        perror("vm is null\n");
        return NULL;
    }

    snek_object_t *object = (snek_object_t *)calloc(1, sizeof(snek_object_t));

    if (!object)
    {
        perror("object is null\n");
        free(vm);
        return NULL;
    }

    vm_track_object(vm, object);
    object->is_marked = false;

    return object;
}