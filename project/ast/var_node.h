#ifndef __PWN_AST_VAR_NODE_H__
#define __PWN_AST_VAR_NODE_H__

#include <cdk/ast/expression_node.h>
namespace pwn {

	/**
	 * Class for describing variable nodes.
	 */
	class var_node: public lvalue_node {
		basic_type *_type;
		std::string *_name;

	public:
		inline var_node(int lineno, basic_type *type, std::string * str) :
			lvalue_node(lineno, str), _type(type), _name(str) {}

	public:
		inline basic_type *type(){ return _type; }
		inline std::string *name(){ return _name; }
		
		void accept(basic_ast_visitor *sp, int level) {
			sp->do_var_node(this, level);
		}
	};

} // pwn

#endif
