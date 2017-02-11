/*
 *  Copyright (c) 2017, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
#pragma once

namespace carbon {

/**
 * List for holding arbitrary types
 */
template <class... Xs>
struct List {};

/**
 * ListContains<L, T>::value == true if and only if T appears in L
 */
template <class L, class T>
struct ListContains;

/**
 * (T, List<Ts...>) -> List<T, Ts...>
 */
template <class T, class L>
struct Prepend;

/**
 * ListDedup<L>::type, contains a List of unique items from L
 */
template <class L, class Enable = void>
struct ListDedup;

} // carbon

#include "TypeList-inl.h"
