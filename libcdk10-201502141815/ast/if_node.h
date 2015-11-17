// $Id: if_node.h,v 1.1 2014/05/02 22:33:16 david Exp $ -*- c++ -*-
#ifndef __CDK_IFNODE_H__
#define __CDK_IFNODE_H__

#include <cdk/ast/expression_node.h>

namespace cdk {

  /**
   * Class for describing if-then nodes.
   */
  class if_node: public basic_node {
    expression_node *_condition;
    basic_node *_block;

  public:
    inline if_node(int lineno, expression_node *condition, basic_node *block) :
        basic_node(lineno), _condition(condition), _block(block) {
    }

  public:
    inline expression_node *condition() {
      return _condition;
    }
    inline basic_node *block() {
      return _block;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_if_node(this, level);
    }

  };

} // cdk

#endif
