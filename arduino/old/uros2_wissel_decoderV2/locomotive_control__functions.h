// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from railway_interfaces:msg/LocomotiveControl.idl
// generated code does not contain a copyright notice

#ifndef RAILWAY_INTERFACES__MSG__DETAIL__LOCOMOTIVE_CONTROL__FUNCTIONS_H_
#define RAILWAY_INTERFACES__MSG__DETAIL__LOCOMOTIVE_CONTROL__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "railway_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "railway_interfaces/msg/detail/locomotive_control__struct.h"

/// Initialize msg/LocomotiveControl message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * railway_interfaces__msg__LocomotiveControl
 * )) before or use
 * railway_interfaces__msg__LocomotiveControl__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
bool
railway_interfaces__msg__LocomotiveControl__init(railway_interfaces__msg__LocomotiveControl * msg);

/// Finalize msg/LocomotiveControl message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
void
railway_interfaces__msg__LocomotiveControl__fini(railway_interfaces__msg__LocomotiveControl * msg);

/// Create msg/LocomotiveControl message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * railway_interfaces__msg__LocomotiveControl__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
railway_interfaces__msg__LocomotiveControl *
railway_interfaces__msg__LocomotiveControl__create();

/// Destroy msg/LocomotiveControl message.
/**
 * It calls
 * railway_interfaces__msg__LocomotiveControl__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
void
railway_interfaces__msg__LocomotiveControl__destroy(railway_interfaces__msg__LocomotiveControl * msg);

/// Check for msg/LocomotiveControl message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
bool
railway_interfaces__msg__LocomotiveControl__are_equal(const railway_interfaces__msg__LocomotiveControl * lhs, const railway_interfaces__msg__LocomotiveControl * rhs);

/// Copy a msg/LocomotiveControl message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
bool
railway_interfaces__msg__LocomotiveControl__copy(
  const railway_interfaces__msg__LocomotiveControl * input,
  railway_interfaces__msg__LocomotiveControl * output);

/// Initialize array of msg/LocomotiveControl messages.
/**
 * It allocates the memory for the number of elements and calls
 * railway_interfaces__msg__LocomotiveControl__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
bool
railway_interfaces__msg__LocomotiveControl__Sequence__init(railway_interfaces__msg__LocomotiveControl__Sequence * array, size_t size);

/// Finalize array of msg/LocomotiveControl messages.
/**
 * It calls
 * railway_interfaces__msg__LocomotiveControl__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
void
railway_interfaces__msg__LocomotiveControl__Sequence__fini(railway_interfaces__msg__LocomotiveControl__Sequence * array);

/// Create array of msg/LocomotiveControl messages.
/**
 * It allocates the memory for the array and calls
 * railway_interfaces__msg__LocomotiveControl__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
railway_interfaces__msg__LocomotiveControl__Sequence *
railway_interfaces__msg__LocomotiveControl__Sequence__create(size_t size);

/// Destroy array of msg/LocomotiveControl messages.
/**
 * It calls
 * railway_interfaces__msg__LocomotiveControl__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
void
railway_interfaces__msg__LocomotiveControl__Sequence__destroy(railway_interfaces__msg__LocomotiveControl__Sequence * array);

/// Check for msg/LocomotiveControl message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
bool
railway_interfaces__msg__LocomotiveControl__Sequence__are_equal(const railway_interfaces__msg__LocomotiveControl__Sequence * lhs, const railway_interfaces__msg__LocomotiveControl__Sequence * rhs);

/// Copy an array of msg/LocomotiveControl messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_railway_interfaces
bool
railway_interfaces__msg__LocomotiveControl__Sequence__copy(
  const railway_interfaces__msg__LocomotiveControl__Sequence * input,
  railway_interfaces__msg__LocomotiveControl__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // RAILWAY_INTERFACES__MSG__DETAIL__LOCOMOTIVE_CONTROL__FUNCTIONS_H_
