#ifndef __PWN_STOPNODE_H__
#define __PWN_STOPNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

	/**
	 * Class for describing stop node.
	 */
	class stop_node: public cdk::basic_node {
		int _argument;

	public:
		inline stop_node(int lineno, int argument) :
			cdk::basic_node(lineno), _argument(argument) {}
			
		inline stop_node(int lineno) :
			cdk::basic_node(lineno), _argument(1) {}
			
		

	public:
		inline int argument() { return _argument; }

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_stop_node(this, level);
		}
	};

} // pwn

#endif
