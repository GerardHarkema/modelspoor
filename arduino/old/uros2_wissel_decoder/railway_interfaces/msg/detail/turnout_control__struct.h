// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from railway_interfaces:msg/TurnoutControl.idl
// generated code does not contain a copyright notice

#ifndef RAILWAY_INTERFACES__MSG__DETAIL__TURNOUT_CONTROL__STRUCT_H_
#define RAILWAY_INTERFACES__MSG__DETAIL__TURNOUT_CONTROL__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/TurnoutControl in the package railway_interfaces.
typedef struct railway_interfaces__msg__TurnoutControl
{
  int32_t number;
  bool state;
} railway_interfaces__msg__TurnoutControl;

// Struct for a sequence of railway_interfaces__msg__TurnoutControl.
typedef struct railway_interfaces__msg__TurnoutControl__Sequence
{
  railway_interfaces__msg__TurnoutControl * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} railway_interfaces__msg__TurnoutControl__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RAILWAY_INTERFACES__MSG__DETAIL__TURNOUT_CONTROL__STRUCT_H_
