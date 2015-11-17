#ifndef __PWN_AST_FUNCTION_DEF_NODE_H__
#define __PWN_AST_FUNCTION_DEF_NODE_H__

#include <cdk/ast/basic_node.h>
#include <cdk/ast/sequence_node.h>
#include <string>

namespace pwn {
	class function_def_node : public cdk::basic_node {
		std::string *_name;
		cdk::sequence_node *_arg;
		basic_type *_return;
		cdk::basic_node *_block;
		bool _local;
		cdk::expression_node *_returnDefault;

		bool _main;

	public:
		inline function_def_node(int lineno, pwn::function_dec_node *node, cdk::basic_node *block) :
		basic_node(lineno), _block(block) {}
		inline function_def_node(int lineno, std::string *name,
			cdk::sequence_node *arg, basic_type *ret, cdk::basic_node *block, bool local) :
			basic_node(lineno), _name(name), _arg(arg), _return(ret), _block(block), _local(local) {
				if(!name->compare("pwn")) _main = true; else _main = false;
			}

		inline function_def_node(int lineno, std::string *name,
			cdk::sequence_node *arg, basic_type *ret, cdk::basic_node *block, bool local, cdk::expression_node *returnDefault) :
			basic_node(lineno), _name(name), _arg(arg), _return(ret), _block(block), _local(local), _returnDefault(returnDefault) {
				if(!name->compare("pwn")) _main = true; else _main = false;
			}

		std::string *name(){ return _name; }
		cdk::sequence_node * arg(){ return _arg; }
		basic_type * ret(){ return _return; }
		bool main(){ return _main; }
		cdk::basic_node * block(){ return _block; }

		cdk::expression_node* returnDefault(){ return _returnDefault; }

		virtual void accept(basic_ast_visitor *sp, int level) {
			sp->do_function_def_node(this, level);
		}
	};
}

#endif
