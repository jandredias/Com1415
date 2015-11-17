#ifndef __PWN_AST_ADDRESS_NODE_H__
#define __PWN_AST_ADDRESS_NODE_H__

#include <cdk/ast/unary_expression_node.h>
namespace pwn {
	
	class address_node : public pwn::lvalue_node {
	
	public:
		inline address_node(int lineno, pwn::lvalue_node *s) :
			pwn::lvalue_node(lineno,s->name()) {}

	/**
	 * @param sp semantic processor visitor
	 * @param level syntactic tree level
	 */
	void accept(basic_ast_visitor *sp, int level) {
		sp->do_address_node(this, level);
	}

	};

} // pwn

#endif
