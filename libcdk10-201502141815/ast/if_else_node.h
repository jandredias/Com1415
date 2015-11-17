// $Id: if_else_node.h,v 1.1 2014/05/02 22:33:16 david Exp $ -*- c++ -*-
#ifndef __CDK_IFELSENODE_H__
#define __CDK_IFELSENODE_H__

#include <cdk/ast/expression_node.h>

namespace cdk {

  /**
   * Class for describing if-then-else nodes.
   */
  class if_else_node: public basic_node {
    expression_node *_condition;
    basic_node *_thenblock, *_elseblock;

  public:
    inline if_else_node(int lineno, expression_node *condition, basic_node *thenblock, basic_node *elseblock) :
        basic_node(lineno), _condition(condition), _thenblock(thenblock), _elseblock(elseblock) {
    }

  public:
    inline expression_node *condition() {
      return _condition;
    }
    inline basic_node *thenblock() {
      return _thenblock;
    }
    inline basic_node *elseblock() {
      return _elseblock;
    }

    void accept(basic_ast_visitor *sp, int level) {
      sp->do_if_else_node(this, level);
    }

  };

} // cdk

#endif
