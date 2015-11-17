// $Id: expression_node.h,v 1.3 2014/02/27 20:32:41 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_EXPRESSION_H__
#define __CDK9_AST_EXPRESSION_EXPRESSION_H__

#include <cdk/ast/basic_node.h>
#include <cdk/basic_type.h>

namespace cdk {

  /**
   * Expressions are typed nodes, i.e., able to store
   * a type description.
   */
  class expression_node: public basic_node {
  protected:
    // This must be a pointer, so that we can anchor a dynamic
    // object and be able to change/delete it afterwards.
    basic_type *_type;

  public:
    /**
     * Simple constructor.
     *
     * @param lineno the source code line number corresponding to
     * the node
     */
    inline expression_node(int lineno) :
        basic_node(lineno), _type(nullptr) {
    }

    virtual basic_type *type() {
      return _type;
    }
    virtual void type(basic_type *type) {
      _type = type;
    }

  };

} // cdk

#endif
