#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sneknew.h"
#include "vm.h"
#include "snekobject.h"

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