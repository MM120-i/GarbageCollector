#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vm.h"
#include "snekobject.h"
#include "stack.h"

// this the function that runs the gc! (omg wow)
void vm_collect_garbage(vm_t *vm)
{
    if (!vm)
    {
        perror("vm is null\n");
        return;
    }

    printf("=== [GC] Starting garbage collection ===\n");
    printf("[GC] Mark phase...\n");
    mark(vm);

    printf("[GC] Trace phase...\n");
    trace(vm);

    printf("[GC] Sweep phase...\n");
    sweep(vm);

    printf("=== [GC] Garbage collection complete ===\n");
}

vm_t *vm_new(void)
{
    vm_t *vm = (vm_t *)malloc(sizeof(vm_t));

    if (!vm)
    {
        perror("Memory allocation failed on vm\n");
        return NULL;
    }

    vm->frames = stack_new(INITIAL_CAPACITY);

    if (!vm->frames)
    {
        perror("malloc failed for frames\n");
        free(vm);
        return NULL;
    }

    vm->objects = stack_new(INITIAL_CAPACITY);

    if (!vm->objects)
    {
        perror("malloc failed for objects\n");
        free(vm);
        free(vm->frames);
        return NULL;
    }

    return vm;
}

void vm_free(vm_t *vm)
{
    if (!vm)
    {
        perror("vm is null\n");
        return;
    }

    for (size_t i = 0; i < vm->frames->count; i++)
    {
        frame_free(vm->frames->data[i]);
    }

    stack_free(vm->frames);

    for (size_t i = 0; i < vm->objects->count; i++)
    {
        snek_object_free(vm->objects->data[i]);
    }

    stack_free(vm->objects);
    free(vm);
}

void frame_free(frame_t *frame)
{
    if (!frame)
    {
        perror("frame is null\n");
        return;
    }

    stack_free(frame->references);
    free(frame);
}

frame_t *vm_new_frame(vm_t *vm)
{
    frame_t *frame = (frame_t *)malloc(sizeof(frame_t));

    if (!frame)
    {
        perror("frame is null\n");
        return NULL;
    }

    frame->references = stack_new(INITIAL_CAPACITY);

    if (!(frame->references))
    {
        perror("frame reference is null\n");
        free(frame);
        return NULL;
    }

    vm_frame_push(vm, frame);

    return frame;
}

void vm_frame_push(vm_t *vm, frame_t *frame)
{
    if (!vm || !frame)
    {
        perror("vm or frame is null\n");
        return;
    }

    stack_push(vm->frames, (void *)frame);
}

void vm_track_object(vm_t *vm, snek_object_t *obj)
{
    if (!vm || !obj)
    {
        perror("vm or obj is null\n");
        return;
    }

    stack_push(vm->objects, (void *)obj);
}

void frame_reference_object(frame_t *frame, snek_object_t *obj)
{
    if (!frame || !obj)
    {
        perror("frame or obj is null\n");
        return;
    }

    stack_push(frame->references, (void *)obj);
}

void mark(vm_t *vm)
{
    if (!vm)
    {
        perror("vm is null\n");
        return;
    }

    printf("[GC][Mark] Marking root references in all frames...\n");

    for (size_t i = 0; i < vm->frames->count; i++)
    {
        frame_t *frame = vm->frames->data[i];
        printf("[GC][Mark] Frame %zu: %zu references\n", i, frame->references->count);

        for (size_t j = 0; j < frame->references->count; j++)
        {
            snek_object_t *object = frame->references->data[j];
            object->is_marked = true;
            printf("[GC][Mark] Marked object at %p\n", (void *)object);
        }
    }
}

void trace(vm_t *vm)
{
    if (!vm)
    {
        perror("vm is null");
        return;
    }

    printf("[GC][Trace] Tracing reachable objects...\n");
    stack_t *gray_objects = stack_new(INITIAL_CAPACITY);

    if (!gray_objects)
    {
        perror("stack is null\n");
        return;
    }

    for (size_t i = 0; i < vm->objects->count; i++)
    {
        snek_object_t *object = vm->objects->data[i];

        if (object && object->is_marked)
        {
            stack_push(gray_objects, (void *)object);
            printf("[GC][Trace] Added object at %p to gray stack\n", (void *)object);
        }
    }

    while (gray_objects->count > 0)
    {
        snek_object_t *poppedObject = stack_pop(gray_objects);
        printf("[GC][Trace] Blackening object at %p\n", (void *)poppedObject);
        trace_blacken_object(gray_objects, poppedObject);
    }

    stack_free(gray_objects);
}

void trace_blacken_object(stack_t *gray_objects, snek_object_t *obj)
{

    if (!obj)
    {
        perror("obj is null\n");
        return;
    }

    switch (obj->kind)
    {
    case INTEGER:
        printf("[GC][Trace] Object at %p is INTEGER, nothing to trace.\n", (void *)obj);
        return;
    case FLOAT:
        printf("[GC][Trace] Object at %p is FLOAT, nothing to trace.\n", (void *)obj);
        return;
    case STRING:
        printf("[GC][Trace] Object at %p is STRING, nothing to trace.\n", (void *)obj);
        return;

    case VECTOR3:
        printf("[GC][Trace] Object at %p is VECTOR3, tracing fields...\n", (void *)obj);
        trace_mark_object(gray_objects, obj->data.v_vector3.x);
        trace_mark_object(gray_objects, obj->data.v_vector3.y);
        trace_mark_object(gray_objects, obj->data.v_vector3.z);
        break;

    case ARRAY:

        printf("[GC][Trace] Object at %p is ARRAY, tracing elements...\n", (void *)obj);

        for (size_t i = 0; i < obj->data.v_array.size; i++)
        {
            trace_mark_object(gray_objects, snek_array_get(obj, i));
        }

        break;
    }
}

void trace_mark_object(stack_t *gray_objects, snek_object_t *obj)
{
    if (!obj)
    {
        perror("Obj is null\n");
        return;
    }

    if (obj->is_marked)
    {
        printf("[GC][Trace] Object at %p already marked.\n", (void *)obj);
        return;
    }

    obj->is_marked = true;
    printf("[GC][Trace] Marking and pushing object at %p to gray stack\n", (void *)obj);
    stack_push(gray_objects, (void *)obj);
}

void sweep(vm_t *vm)
{
    if (!vm)
    {
        perror("vm is null\n");
        return;
    }

    printf("[GC][Sweep] Sweeping unreachable objects...\n");

    for (size_t i = 0; i < vm->objects->count; i++)
    {
        snek_object_t *object = vm->objects->data[i];

        if (object && object->is_marked)
        {
            printf("[GC][Sweep] Object at %p is still alive. Unmarking.\n", (void *)object);
            object->is_marked = false;
        }
        else
        {
            printf("[GC][Sweep] Object at %p is unreachable. Freeing.\n", (void *)object);
            snek_object_free(object);
            vm->objects->data[i] = NULL;
        }
    }

    stack_remove_nulls(vm->objects);
    printf("[GC][Sweep] Sweep complete.\n\n\n");
}