#ifndef __CDK_REPEATNODE_H__
#define __CDK_REPEATNODE_H__

#include <cdk/ast/expression_node.h>

namespace pwn {

	/**
	 * Class for describing repeat-cycle nodes.
	 */
	class repeat_node: public cdk::basic_node {
		cdk::basic_node *_init;
		cdk::expression_node *_condition;
		cdk::basic_node *_step;
		cdk::basic_node *_block;

	public:
		inline repeat_node(int lineno, 	cdk::basic_node *init,
			cdk::expression_node *condition, cdk::basic_node *step,
			cdk::basic_node *block) : basic_node(lineno), _init(init),
			_condition(condition), _step(step), _block(block) {}

	public:
		inline cdk::basic_node *init() { return _init; }
		inline cdk::expression_node *condition() { return _condition; }
		inline cdk::basic_node *step() { return _step; }
		inline cdk::basic_node *block() { return _block; }

		void accept(basic_ast_visitor *sp, int level) {
			sp->do_repeat_node(this, level);
		}

	};

} // pwn

#endif
