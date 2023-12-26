// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from railway_interfaces:msg/LocomotiveControl.idl
// generated code does not contain a copyright notice

#ifndef RAILWAY_INTERFACES__MSG__DETAIL__LOCOMOTIVE_CONTROL__STRUCT_H_
#define RAILWAY_INTERFACES__MSG__DETAIL__LOCOMOTIVE_CONTROL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'name'
// Member 'protocol'
#include "rosidl_runtime_c/string.h"

/// Struct defined in msg/LocomotiveControl in the package railway_interfaces.
typedef struct railway_interfaces__msg__LocomotiveControl
{
  rosidl_runtime_c__String name;
  rosidl_runtime_c__String protocol;
  int32_t address;
  int32_t speed;
  bool direction;
} railway_interfaces__msg__LocomotiveControl;

// Struct for a sequence of railway_interfaces__msg__LocomotiveControl.
typedef struct railway_interfaces__msg__LocomotiveControl__Sequence
{
  railway_interfaces__msg__LocomotiveControl * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} railway_interfaces__msg__LocomotiveControl__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RAILWAY_INTERFACES__MSG__DETAIL__LOCOMOTIVE_CONTROL__STRUCT_H_
