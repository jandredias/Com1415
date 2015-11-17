#ifndef __PWN_AST_INDEX_NODE_H__
#define __PWN_AST_INDEX_NODE_H__

#include <cdk/ast/binary_expression_node.h>
#include <string>
namespace pwn {

  class index_node : public lvalue_node {
    cdk::expression_node *_index;
    std::string *_pointer;

  public:
    inline index_node(int lineno, std::string *str, cdk::expression_node *exp) :
		lvalue_node(lineno, str), _index(exp), _pointer(str) {}

     inline cdk::expression_node *index(){ return _index; }
     inline std::string *pointer(){ return _pointer; }
    /**
     * @param sp semantic processor visitor
     * @param level syntactic tree level
     */
    void accept(basic_ast_visitor *sp, int level) {
      sp->do_index_node(this, level);
    }

  };
}

#endif
