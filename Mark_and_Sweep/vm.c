#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "vm.h"
#include "snekobject.h"

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