// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from railway_interfaces:msg/TurnoutState.idl
// generated code does not contain a copyright notice

#ifndef RAILWAY_INTERFACES__MSG__DETAIL__TURNOUT_STATE__STRUCT_H_
#define RAILWAY_INTERFACES__MSG__DETAIL__TURNOUT_STATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

/// Struct defined in msg/TurnoutState in the package railway_interfaces.
typedef struct railway_interfaces__msg__TurnoutState
{
  int32_t number;
  bool state;
} railway_interfaces__msg__TurnoutState;

// Struct for a sequence of railway_interfaces__msg__TurnoutState.
typedef struct railway_interfaces__msg__TurnoutState__Sequence
{
  railway_interfaces__msg__TurnoutState * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} railway_interfaces__msg__TurnoutState__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // RAILWAY_INTERFACES__MSG__DETAIL__TURNOUT_STATE__STRUCT_H_
