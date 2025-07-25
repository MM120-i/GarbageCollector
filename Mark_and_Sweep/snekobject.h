#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "stack.h"

typedef struct SnekObject snek_object_t;
typedef struct VirtualMachine vm_t;

typedef struct
{
  size_t size;
  snek_object_t **elements;
} snek_array_t;

typedef struct
{
  snek_object_t *x;
  snek_object_t *y;
  snek_object_t *z;
} snek_vector_t;

typedef enum SnekObjectKind
{
  INTEGER,
  FLOAT,
  STRING,
  VECTOR3,
  ARRAY,
} snek_object_kind_t;

typedef union SnekObjectData
{
  int v_int;
  float v_float;
  char *v_string;
  snek_vector_t v_vector3;
  snek_array_t v_array;
} snek_object_data_t;

struct SnekObject
{
  bool is_marked;

  snek_object_kind_t kind;
  snek_object_data_t data;
};

void snek_object_free(snek_object_t *);
bool snek_array_set(snek_object_t *, size_t, snek_object_t *);
snek_object_t *snek_array_get(snek_object_t *, size_t);
snek_object_t *snek_add(vm_t *, snek_object_t *, snek_object_t *);
