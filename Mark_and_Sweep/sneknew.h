#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "stack.h"
#include "snekobject.h"

snek_object_t *_new_snek_integer(vm_t *, int);
snek_object_t *new_snek_float(vm_t *, float);
snek_object_t *new_snek_string(vm_t *, char *);
snek_object_t *new_snek_vector3(vm_t *, snek_object_t *, snek_object_t *, snek_object_t *);
snek_object_t *new_snek_array(vm_t *, size_t);
snek_object_t *_new_snek_object(vm_t *);
snek_object_t *new_snek_integer(vm_t *, int);