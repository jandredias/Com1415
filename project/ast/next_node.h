#ifndef __PWN_NEXTNODE_H__
#define __PWN_NEXTNODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/basic_type.h>

namespace pwn {

	class next_node: public cdk::basic_node {
		int _arg;

	public:
		inline next_node(int lineno, int arg) :
			cdk::basic_node(lineno), _arg(arg) {}
			
		inline next_node(int lineno) : cdk::basic_node(lineno), _arg(1) {}

		public:
		inline int argument() { return _arg; }

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_next_node(this, level);
		}

	};

} // pwn

#endif
