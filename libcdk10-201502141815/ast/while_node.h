// $Id: while_node.h,v 1.1 2014/05/02 22:33:16 david Exp $ -*- c++ -*-
#ifndef __CDK_WHILENODE_H__
#define __CDK_WHILENODE_H__

#include <cdk/ast/expression_node.h>

namespace cdk {

  /**
   * Class for describing while-cycle nodes.
   */
  class while_node: public basic_node {
    expression_node *_condition;
    basic_node *_block;

  public:
    inline while_node(int lineno, expression_node *condition, basic_node *block) :
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
      sp->do_while_node(this, level);
    }

  };

} // cdk

#endif
