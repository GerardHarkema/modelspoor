// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from railway_interfaces:msg/TurnoutState.idl
// generated code does not contain a copyright notice
#include "railway_interfaces/msg/detail/turnout_state__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


bool
railway_interfaces__msg__TurnoutState__init(railway_interfaces__msg__TurnoutState * msg)
{
  if (!msg) {
    return false;
  }
  // number
  // state
  return true;
}

void
railway_interfaces__msg__TurnoutState__fini(railway_interfaces__msg__TurnoutState * msg)
{
  if (!msg) {
    return;
  }
  // number
  // state
}

bool
railway_interfaces__msg__TurnoutState__are_equal(const railway_interfaces__msg__TurnoutState * lhs, const railway_interfaces__msg__TurnoutState * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // number
  if (lhs->number != rhs->number) {
    return false;
  }
  // state
  if (lhs->state != rhs->state) {
    return false;
  }
  return true;
}

bool
railway_interfaces__msg__TurnoutState__copy(
  const railway_interfaces__msg__TurnoutState * input,
  railway_interfaces__msg__TurnoutState * output)
{
  if (!input || !output) {
    return false;
  }
  // number
  output->number = input->number;
  // state
  output->state = input->state;
  return true;
}

railway_interfaces__msg__TurnoutState *
railway_interfaces__msg__TurnoutState__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  railway_interfaces__msg__TurnoutState * msg = (railway_interfaces__msg__TurnoutState *)allocator.allocate(sizeof(railway_interfaces__msg__TurnoutState), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(railway_interfaces__msg__TurnoutState));
  bool success = railway_interfaces__msg__TurnoutState__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
railway_interfaces__msg__TurnoutState__destroy(railway_interfaces__msg__TurnoutState * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    railway_interfaces__msg__TurnoutState__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
railway_interfaces__msg__TurnoutState__Sequence__init(railway_interfaces__msg__TurnoutState__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  railway_interfaces__msg__TurnoutState * data = NULL;

  if (size) {
    data = (railway_interfaces__msg__TurnoutState *)allocator.zero_allocate(size, sizeof(railway_interfaces__msg__TurnoutState), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = railway_interfaces__msg__TurnoutState__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        railway_interfaces__msg__TurnoutState__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
railway_interfaces__msg__TurnoutState__Sequence__fini(railway_interfaces__msg__TurnoutState__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      railway_interfaces__msg__TurnoutState__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

railway_interfaces__msg__TurnoutState__Sequence *
railway_interfaces__msg__TurnoutState__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  railway_interfaces__msg__TurnoutState__Sequence * array = (railway_interfaces__msg__TurnoutState__Sequence *)allocator.allocate(sizeof(railway_interfaces__msg__TurnoutState__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = railway_interfaces__msg__TurnoutState__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
railway_interfaces__msg__TurnoutState__Sequence__destroy(railway_interfaces__msg__TurnoutState__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    railway_interfaces__msg__TurnoutState__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
railway_interfaces__msg__TurnoutState__Sequence__are_equal(const railway_interfaces__msg__TurnoutState__Sequence * lhs, const railway_interfaces__msg__TurnoutState__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!railway_interfaces__msg__TurnoutState__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
railway_interfaces__msg__TurnoutState__Sequence__copy(
  const railway_interfaces__msg__TurnoutState__Sequence * input,
  railway_interfaces__msg__TurnoutState__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(railway_interfaces__msg__TurnoutState);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    railway_interfaces__msg__TurnoutState * data =
      (railway_interfaces__msg__TurnoutState *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!railway_interfaces__msg__TurnoutState__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          railway_interfaces__msg__TurnoutState__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!railway_interfaces__msg__TurnoutState__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
