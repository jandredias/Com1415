// $Id: simple_value_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_SIMPLE_H__
#define __CDK9_AST_EXPRESSION_SIMPLE_H__

#include <cdk/ast/expression_node.h>

namespace cdk {

  /**
   * Class for describing syntactic tree leaves for holding simple
   * (atomic) types. This is a template class that will be instantiated
   * by the various classes for holding specific leaves.
   *
   * @param VisitorType is the type for visitor classes
   * @param StoredType is the type held by the leaf
   * @see Double, Integer, String
   */
  template<typename StoredType>
  class simple_value_node: public expression_node {
    StoredType _value;

  protected:
    inline simple_value_node(int lineno, const StoredType &value) :
        expression_node(lineno), _value(value) {
    }

  public:
    inline const StoredType &value() const {
      return _value;
    }

  };

} // cdk

#endif
