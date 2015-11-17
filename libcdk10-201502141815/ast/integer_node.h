// $Id: integer_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_INTEGER_H__
#define __CDK9_AST_EXPRESSION_INTEGER_H__

#include <cdk/ast/simple_value_node.h>

namespace cdk {

  /**
   * Class for describing syntactic tree leaves for holding integer values.
   */
  class integer_node: public virtual simple_value_node<int> {
  public:
    inline integer_node(int lineno, int i) :
        simple_value_node<int>(lineno, i) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_integer_node(this, level);
    }

  };

} // cdk

#endif
