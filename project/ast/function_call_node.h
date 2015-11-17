#ifndef __PWN_AST_FUNCTION_CALL_NODE_H__
#define __PWN_AST_FUNCTION_CALL_NODE_H__

#include <string>
#include <cdk/ast/expression_node.h>

namespace pwn {
	class function_call_node : public cdk::expression_node {
		std::string *_name;
		cdk::sequence_node *_arg;
		
	public:
		inline function_call_node(int lineno, std::string *name, cdk::sequence_node *arg) : expression_node(lineno), _name(name), _arg(arg){}
			
		std::string *name(){ return _name; }
		cdk::sequence_node * argument(){ return _arg; }
		
		virtual void accept(basic_ast_visitor *sp, int level) {
			sp->do_function_call_node(this, level);
		}
	};
}

#endif
