// $Id: unary_expression_node.h,v 1.2 2014/02/25 21:43:13 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_UNARYEXPRESSION_H__
#define __CDK9_AST_EXPRESSION_UNARYEXPRESSION_H__

#include <cdk/ast/expression_node.h>

namespace cdk {

  /** Class for describing unary operators. */
  class unary_expression_node: public expression_node {
    expression_node *_argument;

  public:
    inline unary_expression_node(int lineno, expression_node *arg) :
        expression_node(lineno), _argument(arg) {
    }

    inline expression_node *argument() {
      return _argument;
    }

  };

} // cdk

#endif
