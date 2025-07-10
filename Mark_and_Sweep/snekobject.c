#include "snekobject.h"

void snek_object_free(snek_object_t *object)
{
    switch (object->kind)
    {
    case INTEGER:
    case FLOAT:
        break;

    case STRING:
        free(object->data.v_string);
        break;

    case VECTOR3:
        break;

    case ARRAY:
        free(object->data.v_array.elements);
        break;

    default:
        break;
    }

    free(object);
}
