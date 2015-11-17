// $Id: lvalue_node.h,v 1.6 2015/05/17 17:48:45 ist175741 Exp $
#ifndef __PWN_NODE_EXPRESSION_LEFTVALUE_H__
#define __PWN_NODE_EXPRESSION_LEFTVALUE_H__

#include <string>

namespace pwn {

  /**
   * Class for describing syntactic tree leaves for holding lvalues.
   */
  class lvalue_node: public cdk::unary_expression_node {
    std::string *_string;
  public:
    inline lvalue_node(int lineno, const char *s, cdk::expression_node *left = NULL) :
        cdk::unary_expression_node(lineno, left), _string(new std::string(s)){
    }
    inline lvalue_node(int lineno, const std::string &s, cdk::expression_node *left = NULL) :
        cdk::unary_expression_node(lineno, left), _string(new std::string(s)) {
    }
    inline lvalue_node(int lineno, const std::string *s, cdk::expression_node *left = NULL) :
        cdk::unary_expression_node(lineno, left), _string(new std::string(*s)) {
    }
    std::string* name(){ return _string; }

	/**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    virtual void accept(basic_ast_visitor *sp, int level) {
      sp->do_lvalue_node(this, level);
    }


  };

} // pwn

#endif
