#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gcRefcount.h"
#include "assert.h"

int main(void)
{
    return 0;
}

bool snek_array_set(snek_object_t *snek_obj, size_t index, snek_object_t *value)
{
    if (snek_obj == NULL || value == NULL)
    {
        perror("Memory allocation failed, object or value is null\n");
        return false;
    }

    if (snek_obj->kind != ARRAY)
    {
        perror("Type of object isnt an Array\n");
        return false;
    }

    refcount_inc(value);

    if (snek_obj->data.v_array.elements[index] != NULL)
    {
        refcount_dec(snek_obj->data.v_array.elements[index]);
    }

    snek_obj->data.v_array.elements[index] = value;

    return true;
}

void refcount_dec(snek_object_t *object)
{
    if (object == NULL)
    {
        perror("Memory allocation failed in refcount_dec\n");
        return;
    }

    object->refcount--;

    if (object->refcount == 0)
    {
        return refcount_free(object);
    }

    return;
}

void refcount_free(snek_object_t *object)
{
    if (object == NULL)
    {
        perror("Memory allocation failed in refcount_free\n");
        return;
    }

    switch (object->kind)
    {
    case INTEGER:
    case FLOAT:
        free(object);
        break;

    case STRING:
        free(object->data.v_string);
        free(object);
        break;

    case VECTOR3:
        refcount_dec(object->data.v_vector3.x);
        refcount_dec(object->data.v_vector3.y);
        refcount_dec(object->data.v_vector3.z);
        break;

    case ARRAY:

        for (size_t i = 0; i < object->data.v_array.size; i++)
        {
            refcount_dec(object->data.v_array.elements[i]);
        }

        free(object->data.v_array.elements);
        break;

    default:
        assert(false);
        break;
    }
}

void refcount_inc(snek_object_t *object)
{
    if (object == NULL)
    {
        perror("Memory allocation failed in refcount_inc\n");
        return;
    }

    object->refcount++;

    return;
}

snek_object_t *_new_snek_object(void)
{
    snek_object_t *object = (snek_object_t *)calloc(1, sizeof(snek_object_t));

    if (object == NULL)
    {
        perror("Memory allocation failed in new_object\n");
        return NULL;
    }

    object->refcount = 1;

    return object;
}
snek_object_t *new_snek_array(size_t size)
{
    snek_object_t *obj = _new_snek_object();

    if (obj == NULL)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    snek_object_t **elements = (snek_object_t **)calloc(size, sizeof(snek_object_t *));

    if (elements == NULL)
    {
        free(obj);
        perror("could not allocate memory for 'elements'\n");
        return NULL;
    }

    obj->kind = ARRAY;
    obj->data.v_array = (snek_array_t){.size = size, .elements = elements};

    return obj;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y, snek_object_t *z)
{
    if (x == NULL || y == NULL || z == NULL)
    {
        perror("Inputs r NULL\n");
        return NULL;
    }

    snek_object_t *obj = _new_snek_object();

    if (obj == NULL)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    obj->kind = VECTOR3;

    refcount_inc(x);
    refcount_inc(y);
    refcount_inc(z);

    obj->data.v_vector3 = (snek_vector_t){
        .x = x,
        .y = y,
        .z = z,
    };

    return obj;
}

snek_object_t *new_snek_integer(int value)
{
    snek_object_t *obj = _new_snek_object();

    if (obj == NULL)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    obj->kind = INTEGER;
    obj->data.v_int = value;

    return obj;
}

snek_object_t *new_snek_float(float value)
{
    snek_object_t *obj = _new_snek_object();

    if (obj == NULL)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    obj->kind = FLOAT;
    obj->data.v_float = value;

    return obj;
}

snek_object_t *new_snek_string(char *value)
{
    snek_object_t *obj = _new_snek_object();

    if (obj == NULL)
    {
        perror("Memory allocation failed for 'object'\n");
        return NULL;
    }

    int len = strlen(value);
    char *dst = (char *)malloc(len + 1);

    if (dst == NULL)
    {
        free(obj);
        perror("Memory allocation failed for 'v_string'\n");
        return NULL;
    }

    strcpy(dst, value);

    obj->kind = STRING;
    obj->data.v_string = dst;

    return obj;
}

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b) // can be extended for subtracting, dividing, multiplying
{
    if (!a || !b)
    {
        perror("input 'a' or 'b' is null\n");
        return NULL;
    }

    switch (a->kind)
    {
    case INTEGER:

        if (b->kind == INTEGER)
        {
            return new_snek_integer(a->data.v_int + b->data.v_int);
        }
        else if (b->kind == FLOAT)
        {
            return new_snek_float((float)a->data.v_int + b->data.v_float);
        }
        else
        {
            perror("Invalid INTEGER operation\n");
            return NULL;
        }

    case FLOAT:

        if (b->kind == INTEGER)
        {
            return new_snek_float(a->data.v_float + (float)b->data.v_int);
        }
        else if (b->kind == FLOAT)
        {
            return new_snek_float(a->data.v_float + b->data.v_float);
        }
        else
        {
            perror("Invalid FLOAT operation\n");
            return NULL;
        }

    case STRING:

        if (b->kind != STRING)
        {
            perror("Invalid STRING operation\n");
            return NULL;
        }

        size_t length = strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
        char *newString = (char *)calloc(sizeof(char *), length);
        strcat(newString, a->data.v_string);
        strcat(newString, b->data.v_string);

        snek_object_t *object = new_snek_string(newString);
        free(newString);

        return object;

    case VECTOR3:

        if (b->kind != VECTOR3)
        {
            perror("Invalid VECTOR operation\n");
            return NULL;
        }

        return new_snek_vector3(
            snek_add(a->data.v_vector3.x, b->data.v_vector3.x),
            snek_add(a->data.v_vector3.y, b->data.v_vector3.y),
            snek_add(a->data.v_vector3.z, b->data.v_vector3.z));

    case ARRAY:

        if (b->kind != ARRAY)
        {
            perror("Invalid ARRAY operation\n");
            return NULL;
        }

        size_t arrayLength = a->data.v_array.size + b->data.v_array.size;
        snek_object_t *arrayObject = new_snek_array(arrayLength);

        for (size_t i = 0; i < a->data.v_array.size; i++)
        {
            snek_array_set(arrayObject, i, snek_array_get(a, i));
        }

        for (size_t i = 0; i < b->data.v_array.size; i++)
        {
            snek_array_set(arrayObject, i + a->data.v_array.size, snek_array_get(b, i));
        }

        return arrayObject;

    default:
        perror("Invalid operation");
        return NULL;
    }
}
