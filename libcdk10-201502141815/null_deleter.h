// $Id: null_deleter.h,v 1.2 2014/02/25 21:43:13 david Exp $ -*- c++ -*-
#ifndef __CDK9_NULL_DELETER_H__
#define __CDK9_NULL_DELETER_H__

namespace cdk {

  // this is a horrible way of not messing the
  // memory up when getting a shared ptr to a
  // static object
  struct null_deleter {
    void operator()(void * const) {
    }
  };

} // cdk

#endif
