// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from railway_interfaces:msg/LocomotiveControl.idl
// generated code does not contain a copyright notice
#include "railway_interfaces/msg/detail/locomotive_control__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `name`
// Member `protocol`
#include "rosidl_runtime_c/string_functions.h"

bool
railway_interfaces__msg__LocomotiveControl__init(railway_interfaces__msg__LocomotiveControl * msg)
{
  if (!msg) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__init(&msg->name)) {
    railway_interfaces__msg__LocomotiveControl__fini(msg);
    return false;
  }
  // protocol
  if (!rosidl_runtime_c__String__init(&msg->protocol)) {
    railway_interfaces__msg__LocomotiveControl__fini(msg);
    return false;
  }
  // address
  // speed
  // direction
  return true;
}

void
railway_interfaces__msg__LocomotiveControl__fini(railway_interfaces__msg__LocomotiveControl * msg)
{
  if (!msg) {
    return;
  }
  // name
  rosidl_runtime_c__String__fini(&msg->name);
  // protocol
  rosidl_runtime_c__String__fini(&msg->protocol);
  // address
  // speed
  // direction
}

bool
railway_interfaces__msg__LocomotiveControl__are_equal(const railway_interfaces__msg__LocomotiveControl * lhs, const railway_interfaces__msg__LocomotiveControl * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->name), &(rhs->name)))
  {
    return false;
  }
  // protocol
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->protocol), &(rhs->protocol)))
  {
    return false;
  }
  // address
  if (lhs->address != rhs->address) {
    return false;
  }
  // speed
  if (lhs->speed != rhs->speed) {
    return false;
  }
  // direction
  if (lhs->direction != rhs->direction) {
    return false;
  }
  return true;
}

bool
railway_interfaces__msg__LocomotiveControl__copy(
  const railway_interfaces__msg__LocomotiveControl * input,
  railway_interfaces__msg__LocomotiveControl * output)
{
  if (!input || !output) {
    return false;
  }
  // name
  if (!rosidl_runtime_c__String__copy(
      &(input->name), &(output->name)))
  {
    return false;
  }
  // protocol
  if (!rosidl_runtime_c__String__copy(
      &(input->protocol), &(output->protocol)))
  {
    return false;
  }
  // address
  output->address = input->address;
  // speed
  output->speed = input->speed;
  // direction
  output->direction = input->direction;
  return true;
}

railway_interfaces__msg__LocomotiveControl *
railway_interfaces__msg__LocomotiveControl__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  railway_interfaces__msg__LocomotiveControl * msg = (railway_interfaces__msg__LocomotiveControl *)allocator.allocate(sizeof(railway_interfaces__msg__LocomotiveControl), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(railway_interfaces__msg__LocomotiveControl));
  bool success = railway_interfaces__msg__LocomotiveControl__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
railway_interfaces__msg__LocomotiveControl__destroy(railway_interfaces__msg__LocomotiveControl * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    railway_interfaces__msg__LocomotiveControl__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
railway_interfaces__msg__LocomotiveControl__Sequence__init(railway_interfaces__msg__LocomotiveControl__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  railway_interfaces__msg__LocomotiveControl * data = NULL;

  if (size) {
    data = (railway_interfaces__msg__LocomotiveControl *)allocator.zero_allocate(size, sizeof(railway_interfaces__msg__LocomotiveControl), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = railway_interfaces__msg__LocomotiveControl__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        railway_interfaces__msg__LocomotiveControl__fini(&data[i - 1]);
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
railway_interfaces__msg__LocomotiveControl__Sequence__fini(railway_interfaces__msg__LocomotiveControl__Sequence * array)
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
      railway_interfaces__msg__LocomotiveControl__fini(&array->data[i]);
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

railway_interfaces__msg__LocomotiveControl__Sequence *
railway_interfaces__msg__LocomotiveControl__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  railway_interfaces__msg__LocomotiveControl__Sequence * array = (railway_interfaces__msg__LocomotiveControl__Sequence *)allocator.allocate(sizeof(railway_interfaces__msg__LocomotiveControl__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = railway_interfaces__msg__LocomotiveControl__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
railway_interfaces__msg__LocomotiveControl__Sequence__destroy(railway_interfaces__msg__LocomotiveControl__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    railway_interfaces__msg__LocomotiveControl__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
railway_interfaces__msg__LocomotiveControl__Sequence__are_equal(const railway_interfaces__msg__LocomotiveControl__Sequence * lhs, const railway_interfaces__msg__LocomotiveControl__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!railway_interfaces__msg__LocomotiveControl__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
railway_interfaces__msg__LocomotiveControl__Sequence__copy(
  const railway_interfaces__msg__LocomotiveControl__Sequence * input,
  railway_interfaces__msg__LocomotiveControl__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(railway_interfaces__msg__LocomotiveControl);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    railway_interfaces__msg__LocomotiveControl * data =
      (railway_interfaces__msg__LocomotiveControl *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!railway_interfaces__msg__LocomotiveControl__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          railway_interfaces__msg__LocomotiveControl__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!railway_interfaces__msg__LocomotiveControl__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
