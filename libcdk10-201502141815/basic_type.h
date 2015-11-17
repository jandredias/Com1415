// $Id: basic_type.h,v 1.2 2014/02/25 21:43:13 david Exp $ -*- c++ -*-
#ifndef __CDK9_SEMANTICS_EXPRESSIONTYPE_H__
#define __CDK9_SEMANTICS_EXPRESSIONTYPE_H__

#include <cstdlib>

/**
 * This is a quick and very dirty approach to type information.
 * It is defined this way (even though it's not extensible at all)
 * for simplicity.
 *
 * Nevertheless, new types can be added simply by using other
 * integer values other than the ones listed.
 */
struct basic_type {
  typedef unsigned long int type;
  size_t _size = 0; // in bytes
  type _name = TYPE_UNSPEC;

  // For supporting simple complex types (nullptr by default)
  basic_type *_subtype = nullptr;

  inline basic_type() :
    _size(0), _name(TYPE_UNSPEC), _subtype(nullptr) {
  }
  inline basic_type(size_t size, type name) :
    _size(size), _name(name), _subtype(nullptr) {
  }
  inline size_t size() {
    return _size;
  }
  inline type name() {
    return _name;
  }
  inline basic_type *subtype() {
    return _subtype;
  }

public:

  static const type TYPE_UNSPEC  = 0;

  static const type TYPE_INT     = 1UL<<0;
  static const type TYPE_DOUBLE  = 1UL<<1;
  static const type TYPE_BOOLEAN = 1UL<<2;
  static const type TYPE_STRING  = 1UL<<3;
  static const type TYPE_POINTER = 1UL<<4;
  static const type TYPE_STRUCT  = 1UL<<5;
  static const type TYPE_VOID    = 1UL<<30;

  static const type TYPE_ERROR   = 1UL<<31;
};

#endif
