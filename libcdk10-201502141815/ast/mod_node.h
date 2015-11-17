// $Id: mod_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_MOD_H__
#define __CDK9_AST_EXPRESSION_MOD_H__

#include <cdk/ast/binary_expression_node.h>

namespace cdk {

  /**
   * Class for describing the modulus operator
   */
  class mod_node: public binary_expression_node {
  public:
    /**
     * @param lineno source code line number for this node
     * @param left first operand
     * @param right second operand
     */
    inline mod_node(int lineno, expression_node *left, expression_node *right) :
        binary_expression_node(lineno, left, right) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_mod_node(this, level);
    }

  };

} // cdk

#endif
