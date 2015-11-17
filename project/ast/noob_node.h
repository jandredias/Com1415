#ifndef __PWN_AST_NOOB_NODE_H__
#define __PWN_AST_NOOB_NODE_H__

#include <cdk/ast/basic_node.h>
namespace pwn {
	
	class noob_node : public cdk::expression_node{
		
	public:
		inline noob_node(int lineno) : cdk::expression_node(lineno) {}
		
		void accept(basic_ast_visitor *sp, int level){
			sp->do_noob_node(this, level);
		}
		
	};
}

#endif
