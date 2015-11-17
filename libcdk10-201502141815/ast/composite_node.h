// $Id: composite_node.h,v 1.3 2014/02/26 18:53:32 david Exp $ -*- c++ -*-
#ifndef __CDK9_AST_COMPOSITE_H__
#define __CDK9_AST_COMPOSITE_H__

#include <cdk/ast/basic_node.h>
#include <vector>
#include <cstdarg>

namespace cdk {

  class composite_node: public basic_node {
    std::vector<basic_node*> _subnodes;

  public:
    std::vector<basic_node*> &subnodes() {
      return _subnodes;
    }
    size_t size() {
      return _subnodes.size();
    }
    basic_node *at(size_t i) {
      return _subnodes.at(i);
    }

  public:
    /**
     * The constructor of a composite node takes the same first
     * argument as any other node.
     * The second argument is the number of child nodes: this
     * argument is followed by the child nodes themselves. Note
     * that no effort is made to ensure that the given number of
     * children matches the actual children passed to the
     * function. <b>You have been warned...</b>
     *
     * @param lineno the source code line number that originated the node
     * @param nops the number of child nodes
     * @param ... the child nodes
     */
    composite_node(int lineno, int nops, ...) :
        basic_node(lineno) {
      va_list ap;
      va_start(ap, nops);
      for (int i = 0; i < nops; i++)
        _subnodes.push_back(va_arg(ap, basic_node*));
      va_end(ap);
    }

  public:
    /**
     * This is the destructor for composite nodes. Note that this
     * destructor also causes the destruction of the node's
     * children.
     */
    inline ~composite_node() {
      for (auto node : _subnodes)
        delete node;
      _subnodes.clear();
    }

    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_composite_node(this, level);
    }

  };

} // cdk

#endif
