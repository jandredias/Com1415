#ifndef __PWN_AST_BLOCK_NODE_H__
#define __PWN_AST_BLOCK_NODE_H__

#include <cdk/ast/basic_node.h>
namespace pwn {
	class block_node : public cdk::basic_node {
		cdk::sequence_node *_dec;
		cdk::sequence_node *_inst;
	public:
		inline block_node(int lineno, cdk::sequence_node *dec, cdk::sequence_node *inst) :
			cdk::basic_node(lineno), _dec(dec), _inst(inst){}
			
		cdk::sequence_node *dec(){ return _dec; }
		cdk::sequence_node *inst(){ return _inst; }
	/**
	 * @param sp semantic processor visitor
	 * @param level syntactic tree level
	 */
		virtual void accept(basic_ast_visitor *sp, int level) {
			sp->do_block_node(this, level);
		}
	};
}

#endif
