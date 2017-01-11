#pragma once

using namespace pixl;

namespace pixl {

template <typename Type>
Type PushQueue(Type* array, int length, Type value) {
  Type rtrn = array[length - 1];
  for (int i = (length - 1); i > 0; i--) {
    array[i] = array[i - 1];
  }
  array[0] = value;
  return rtrn;
}

} // end namespace pixl
