#pragma once

#include "stack.h"
#include "snekobject.h"

#define INITIAL_CAPACITY 8

typedef struct VirtualMachine
{
    stack_t *frames;
    stack_t *objects;
} vm_t;

typedef struct StackFrame
{
    stack_t *references;
} frame_t;

vm_t *vm_new(void);
void vm_free(vm_t *);
void frame_free(frame_t *);
frame_t *vm_new_frame(vm_t *);
void vm_frame_push(vm_t *, frame_t *);
void vm_track_object(vm_t *, snek_object_t *);
void frame_reference_object(frame_t *, snek_object_t *);
void mark(vm_t *);
void trace(vm_t *);
void trace_blacken_object(stack_t *, snek_object_t *);
void trace_mark_object(stack_t *, snek_object_t *);
void vm_collect_garbage(vm_t *);
void sweep(vm_t *);
