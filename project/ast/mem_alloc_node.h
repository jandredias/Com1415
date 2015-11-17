#ifndef __PWN_AST_MEMALLOC_NODE_H__
#define __PWN_AST_MEMALLOC_NODE_H__

#include <cdk/ast/expression_node.h>
#include <cdk/ast/basic_node.h>

namespace pwn {
	class mem_alloc_node : public cdk::basic_node {
		int  _size;
		std::string *_name;
		bool _local;

		public:
		inline mem_alloc_node(int lineno, std::string* name, int size, bool local = false) :
		  basic_node(lineno),	_size(size), _name(name), _local(local) {};

    bool local(){ return _local; }
		int size(){ return _size; }
		std::string *name(){ return _name; }
		void accept(basic_ast_visitor *sp, int level) {
			sp->do_mem_alloc_node(this, level);
		}

	};
}

#endif
