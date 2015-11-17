#ifndef __PWN_AST_FUNCTION_DEC_NODE_H__
#define __PWN_AST_FUNCTION_DEC_NODE_H__

#include <cdk/ast/basic_node.h>

namespace pwn {
	class function_dec_node : public cdk::basic_node {
		std::string *_name;
		cdk::sequence_node *_arg;
		basic_type *_return;
		bool _local;
		
	public:
		inline function_dec_node(int lineno, std::string* name,
			cdk::sequence_node *arg, basic_type *ret, bool local) :
			basic_node(lineno), _name(name), _arg(arg), _return(ret), _local(local) {}
			
		std::string *name(){ return _name; }
		cdk::sequence_node * arg(){ return _arg; }
		basic_type * ret(){ return _return; }
		
		virtual void accept(basic_ast_visitor *sp, int level) {
			sp->do_function_dec_node(this, level);
		}
	};
}

#endif

