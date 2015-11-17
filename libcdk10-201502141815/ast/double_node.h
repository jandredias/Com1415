// $Id: double_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_EXPRESSION_DOUBLE_H__
#define __CDK9_AST_EXPRESSION_DOUBLE_H__

#include <cdk/ast/simple_value_node.h>

namespace cdk {

  /**
   * Class for describing syntactic tree leaves for holding double
   * precision values.
   */
  class double_node: public virtual simple_value_node<double> {
  public:
    inline double_node(int lineno, double d) :
        simple_value_node<double>(lineno, d) {
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_double_node(this, level);
    }

  };

} // cdk

#endif
