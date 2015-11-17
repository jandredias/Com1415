// $Id: xml_writer.cpp,v 1.17 2015/05/19 15:10:49 ist175741 Exp $ -*- c++ -*-
#include <string>
#include "targets/xml_writer.h"
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

//---------------------------------------------------------------------------

void pwn::xml_writer::do_sequence_node(cdk::sequence_node * const node, int lvl) {
  os() << std::string(lvl, ' ') << "<sequence_node size='" << node->size() << "'>" << std::endl;
  for (size_t i = 0; i < node->size(); i++)
    node->node(i)->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_integer_node(cdk::integer_node * const node, int lvl) {
  processSimple(node, lvl);
}
void pwn::xml_writer::do_double_node(cdk::double_node * const node, int lvl) {
  //processSimple(node, lvl);
}

void pwn::xml_writer::do_string_node(cdk::string_node * const node, int lvl) {
  processSimple(node, lvl);
}

//---------------------------------------------------------------------------

inline void pwn::xml_writer::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_neg_node(cdk::neg_node * const node, int lvl) {
  processUnaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

inline void pwn::xml_writer::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
  ////CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->left()->accept(this, lvl + 2);
  node->right()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_mem_alloc_node(pwn::mem_alloc_node * const node, int lvl){
	openTag(node, lvl);
		openCloseTag("name", lvl + 2);
			os() << node->name();
		closeTag("name", lvl + 2);
    //openCloseInlineTag("size", node->size(),lvl + 2);
	closeTag(node,lvl);
}
void pwn::xml_writer::do_index_node(pwn::index_node * const node, int lvl){
	openTag(node, lvl);
		openCloseInlineTag(std::string("name"), *node->pointer(), lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_function_call_node(pwn::function_call_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
		node->argument()->accept(this, lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_block_node(pwn::block_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
		if(node->dec() != NULL)
			node->dec()->accept(this, lvl + 2);
		if(node->inst() != NULL)
			node->inst()->accept(this, lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_noob_node(pwn::noob_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_var_dec_node(pwn::var_dec_node * const node, int lvl) {
	openTag(node, lvl);
		openCloseInlineTag(std::string("local"), ((node->local()) ? "LOCAL" : "NOT LOCAL"), lvl + 2);
		openCloseInlineTag(std::string("name"), *node->name(), lvl + 2);


		if(node->type()->name() == basic_type::TYPE_INT)
			openCloseInlineTag(std::string("type"), "INT", lvl + 2);
		else if(node->type()->name() == basic_type::TYPE_STRING)
			openCloseInlineTag(std::string("type"), "STRING", lvl + 2);
		else if(node->type()->name() == basic_type::TYPE_DOUBLE)
			openCloseInlineTag(std::string("type"), "REAL", lvl + 2);
		else if(node->type()->name() == basic_type::TYPE_POINTER)
			openCloseInlineTag(std::string("type"), "POINTER", lvl + 2);
		else
			openCloseInlineTag(std::string("type"), "DONT KNOW", lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_var_node(pwn::var_node * const node, int lvl){
	openTag(node, lvl);
		openTag("name", lvl + 2);
			os() << node->name();
		closeTag("name", lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_not_node(pwn::not_node * const node, int lvl) {
	processUnaryExpression(node, lvl);
}
void pwn::xml_writer::do_println_node(pwn::println_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
		node->argument()->accept(this, lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_next_node(pwn::next_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
		openTag("levels ", lvl + 2);
			os() << node->argument();
		closeTag("levels", lvl + 2);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_stop_node(pwn::stop_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
		openTag("levels ", lvl + 2);
			os() << node->argument();
		closeTag("levels", lvl + 2);
	closeTag(node, lvl);
}

void pwn::xml_writer::do_return_node(pwn::return_node * const node, int lvl) {
	//CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
	closeTag(node, lvl);
}
void pwn::xml_writer::do_function_dec_node(pwn::function_dec_node * const node, int lvl) {
	openTag(node, lvl);
		openTag("name", lvl + 2);
			os() << node->name();
		closeTag("name", lvl + 2);
		openTag("arg", lvl + 2);
			node->arg()->accept(this, lvl + 4);
    closeTag("arg", lvl + 2);
		//~ openTag("ret", lvl + 2);
			//~ node->ret()->accept(this, lvl + 4);
		//~ closeTag("ret", lvl + 2);
	closeTag(node,lvl);
}
void pwn::xml_writer::do_function_def_node(pwn::function_def_node * const node, int lvl) {
	////CHECK_TYPES(_compiler, _symtab, node);
	openTag(node, lvl);
		openCloseInlineTag(std::string("name"),*node->name(), lvl + 2);
		if(node->arg() != NULL){
			openTag("arg", lvl + 2);
				node->arg()->accept(this, lvl + 4);
			closeTag("arg", lvl + 2);
		}
		//~ openTag("ret", lvl + 2);
			//~ node->ret()->accept(this, lvl + 4);
		//~ closeTag("ret", lvl + 2);
		openTag("block", lvl + 2);
			if(node->block() != NULL)
				node->block()->accept(this, lvl + 4);
		closeTag("block", lvl + 2);
	closeTag(node,lvl);
}

void pwn::xml_writer::do_identity_node(pwn::identity_node * const node, int lvl) {
	processUnaryExpression(node, lvl);
}
void pwn::xml_writer::do_and_node(pwn::and_node * const node, int lvl) {
	processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_or_node(pwn::or_node * const node, int lvl) {
	processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_add_node(cdk::add_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_sub_node(cdk::sub_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_mul_node(cdk::mul_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_div_node(cdk::div_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_mod_node(cdk::mod_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_lt_node(cdk::lt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_le_node(cdk::le_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_ge_node(cdk::ge_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_gt_node(cdk::gt_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_ne_node(cdk::ne_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}
void pwn::xml_writer::do_eq_node(cdk::eq_node * const node, int lvl) {
  processBinaryExpression(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 4);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  //processSimple(node, lvl);
  //No idea what it is supposed to do
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->lvalue()->accept(this, lvl + 2);
  openTag("rvalue", lvl + 2);
  node->rvalue()->accept(this, lvl + 4);
  closeTag("rvalue", lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_print_node(pwn::print_node * const node, int lvl) {
  //CHECK_TYPES(_compiler, _symtab, node);
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_read_node(pwn::read_node * const node, int lvl) {
  openTag(node, lvl);
  node->argument()->accept(this, lvl + 2);
  closeTag(node, lvl);
}

//---------------------------------------------------------------------------

void pwn::xml_writer::do_while_node(cdk::while_node * const node, int lvl) {
  openTag(node, lvl);
  openTag("condition", lvl + 2);
  node->condition()->accept(this, lvl + 4);
  closeTag("condition", lvl + 2);
  openTag("block", lvl + 2);
  node->block()->accept(this, lvl + 4);
  closeTag("block", lvl + 2);
  closeTag(node, lvl);
}
void pwn::xml_writer::do_repeat_node(pwn::repeat_node * const node, int lvl) {
	openTag(node, lvl);
		openTag("init", lvl + 2);
			node->init()->accept(this, lvl + 4);
		closeTag("init", lvl + 2);
		openTag("condition", lvl + 2);
			node->condition()->accept(this, lvl + 4);
		closeTag("condition", lvl + 2);
		openTag("step", lvl + 2);
			node->step()->accept(this, lvl + 4);
		closeTag("step", lvl + 2);
		openTag("block", lvl + 2);
			node->block()->accept(this, lvl + 4);
		closeTag("block", lvl + 2);
	closeTag(node, lvl);
}
//---------------------------------------------------------------------------

void pwn::xml_writer::do_if_node(cdk::if_node * const node, int lvl) {
  openTag(node, lvl);
	openTag("condition", lvl + 2);
		node->condition()->accept(this, lvl + 4);
	closeTag("condition", lvl + 2);
	openTag("then", lvl + 2);
		node->block()->accept(this, lvl + 4);
	closeTag("then", lvl + 2);
  closeTag(node, lvl);
}

void pwn::xml_writer::do_if_else_node(cdk::if_else_node * const node, int lvl) {
  openTag(node, lvl);
	  openTag("condition", lvl + 2);
	  node->condition()->accept(this, lvl + 4);
	  closeTag("condition", lvl + 2);
	  openTag("then", lvl + 2);
	  node->thenblock()->accept(this, lvl + 4);
	  closeTag("then", lvl + 2);
	  openTag("else", lvl + 2);
	  node->elseblock()->accept(this, lvl + 4);
	  closeTag("else", lvl + 2);
  closeTag(node, lvl);
}



void pwn::xml_writer::do_address_node(pwn::address_node * const node, int lvl) {
	openTag(node, lvl);
		openTag("argument", lvl + 2);
			node->argument()->accept(this, lvl + 4);
		closeTag("argument", lvl + 2);
	closeTag(node, lvl);
}
