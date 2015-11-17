#ifndef __PWN_AST_VAR_DEC_NODE_H__
#define __PWN_AST_VAR_DEC_NODE_H__

#include <string>
#include <cdk/basic_type.h>
namespace pwn {

	/**
	 * Class for describing declaration variable nodes.
	 */
	class var_dec_node: public cdk::basic_node {
		basic_type *_type;
		std::string *_name;
		bool _local;
		cdk::expression_node *_init;
  public:
		inline var_dec_node(int lineno, basic_type *type, std::string* name) :
			basic_node(lineno), _type(type), _name(name), _local(false){}
		inline var_dec_node(int lineno, basic_type *type, std::string* name, cdk::expression_node *init) :
			basic_node(lineno), _type(type), _name(name), _local(false), _init(init){}
		inline var_dec_node(int lineno, basic_type *type, std::string* name, bool local) :
			basic_node(lineno), _type(type), _name(name), _local(local){}
		inline var_dec_node(int lineno, basic_type *type, std::string* name, bool local, cdk::expression_node *init) :
			basic_node(lineno), _type(type), _name(name), _local(local), _init(init){}
	public:
		inline bool local(){ return _local; }
		inline basic_type *type(){ return _type; }
		inline void type(basic_type*t){ _type = t; }
                inline std::string *name(){ return _name; }
		inline cdk::expression_node *init(){ return _init;}
		void accept(basic_ast_visitor *sp, int level) {
		  sp->do_var_dec_node(this, level);
		}
	};

}

#endif
