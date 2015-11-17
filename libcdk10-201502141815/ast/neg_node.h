// $Id: neg_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_NEG_H__
#define __CDK9_AST_EXPRESSION_NEG_H__

#include <cdk/ast/unary_expression_node.h>

namespace cdk {

  /**
   * Class for describing the negation operator
   */
  class neg_node: public unary_expression_node {
  public:
    inline neg_node(int lineno, expression_node *arg) :
        unary_expression_node(lineno, arg) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_neg_node(this, level);
    }

  };

} // cdk

#endif
