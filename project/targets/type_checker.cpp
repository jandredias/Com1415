// $Id: type_checker.cpp,v 1.21 2015/05/19 18:12:15 ist175741 Exp $ -*- c++ -*-
#include <string>
#include "targets/type_checker.h"
#include "ast/all.h"  // automatically generated

#define ASSERT_UNSPEC \
    { if (node->type() != nullptr && \
          node->type()->name() != basic_type::TYPE_UNSPEC) return; }

//---------------------------------------------------------------------------

void pwn::type_checker::do_double_node(cdk::double_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
}
void pwn::type_checker::do_integer_node(cdk::integer_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->type(new basic_type(4, basic_type::TYPE_INT));
}
void pwn::type_checker::do_string_node(cdk::string_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->type(new basic_type(4, basic_type::TYPE_STRING));
}

//---------------------------------------------------------------------------

inline void pwn::type_checker::processUnaryExpression(cdk::unary_expression_node * const node, int lvl) {
  node->argument()->accept(this, lvl + 2);
  //Expressions may be int, double or pointer
  if(node->argument()->type()->name() == basic_type::TYPE_INT)
    node->type(new basic_type(4, basic_type::TYPE_INT));
  else if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE)
    node->type(new basic_type(4, basic_type::TYPE_DOUBLE));
  else if(node->argument()->type()->name() == basic_type::TYPE_POINTER)
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  else
    throw std::string("wrong type in argument of unary expression");
}

inline void pwn::type_checker::processBinaryExpression(cdk::binary_expression_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl + 2);
    node->right()->accept(this, lvl + 2);
    //If left or right is double, then node is double
    //if both are int, then node is int
    //else error
    if ( node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT){
        node->type(new basic_type(4,basic_type::TYPE_INT));
    }
    else if ((node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT) ||
             (node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE) ||
             (node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
        node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
    }
    else
	    throw std::string("processBinaryExpression: Wrong type in arguments of binary expression");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_not_node(pwn::not_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->argument()->accept(this, lvl);
    if(node->argument()->type()->name() == basic_type::TYPE_INT){
      node->type(new basic_type(4,basic_type::TYPE_INT));
    }else throw std::string("expecting integer value on not node");
}
void pwn::type_checker::do_or_node(pwn::or_node * const node, int lvl)  {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() != basic_type::TYPE_INT ||
     node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("expecting integer value on or node");
  node->type(new basic_type(4, basic_type::TYPE_INT));
}
void pwn::type_checker::do_and_node(pwn::and_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() != basic_type::TYPE_INT ||
     node->right()->type()->name() != basic_type::TYPE_INT)
    throw std::string("expecting integer value on and node");
  node->type(new basic_type(4, basic_type::TYPE_INT));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_assignment_node(pwn::assignment_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->rvalue()->accept(this, lvl);
  node->lvalue()->accept(this, lvl);

  if((node->lvalue()->type()->name() == node->lvalue()->type()->name()) ||
     (node->lvalue()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->rvalue()->type()->name() == basic_type::TYPE_INT))
    node->type(new basic_type(node->lvalue()->type()->size(), node->lvalue()->type()->name()));
  else
    throw std::string("wrong type of arguments of assignment expression");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_ne_node(cdk::ne_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if((node->left()->type()->name() == node->right()->type()->name() &&
      (node->left()->type()->name() == basic_type::TYPE_INT ||
       node->left()->type()->name() == basic_type::TYPE_DOUBLE ||
       node->left()->type()->name() == basic_type::TYPE_POINTER)) ||
       //If types are equal and they are int double or pointer
     (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) ||
      //if int and double
     (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) ||
      //if double and int
     (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_POINTER) ||
      //if pointer and int
     (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_INT)
      //if int and pointer
    ){
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }else throw std::string("wrong type in ne");
}
void pwn::type_checker::do_eq_node(cdk::eq_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if((node->left()->type()->name() == node->right()->type()->name() &&
      (node->left()->type()->name() == basic_type::TYPE_INT ||
       node->left()->type()->name() == basic_type::TYPE_DOUBLE ||
       node->left()->type()->name() == basic_type::TYPE_POINTER)) ||
       //If types are equal and they are int double or pointer
     (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) ||
      //if int and double
     (node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
      node->left()->type()->name() == basic_type::TYPE_INT) ||
      //if double and int
     (node->left()->type()->name() == basic_type::TYPE_INT &&
      node->left()->type()->name() == basic_type::TYPE_POINTER) ||
      //if pointer and int
     (node->left()->type()->name() == basic_type::TYPE_POINTER &&
      node->left()->type()->name() == basic_type::TYPE_INT)
      //if int and pointer
    ){
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }else throw std::string("wrong type in eq");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_lt_node(cdk::lt_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if((node->left()->type()->name() == basic_type::TYPE_INT ||
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) &&
     (node->right()->type()->name() == basic_type::TYPE_INT ||
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else throw std::string("wrong arguments type on lt node");
}
void pwn::type_checker::do_le_node(cdk::le_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if((node->left()->type()->name() == basic_type::TYPE_INT ||
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) &&
     (node->right()->type()->name() == basic_type::TYPE_INT ||
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else throw std::string("wrong arguments type on lt node");
}
void pwn::type_checker::do_ge_node(cdk::ge_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if((node->left()->type()->name() == basic_type::TYPE_INT ||
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) &&
     (node->right()->type()->name() == basic_type::TYPE_INT ||
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else throw std::string("wrong arguments type on lt node");
}
void pwn::type_checker::do_gt_node(cdk::gt_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if((node->left()->type()->name() == basic_type::TYPE_INT ||
      node->left()->type()->name() == basic_type::TYPE_DOUBLE) &&
     (node->right()->type()->name() == basic_type::TYPE_INT ||
      node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else throw std::string("wrong arguments type on lt node");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_add_node(cdk::add_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT &&
     node->right()->type()->name() == basic_type::TYPE_INT){
     node->type(new basic_type(4,basic_type::TYPE_INT));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT &&
          node->right()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
         (node->right()->type()->name() == basic_type::TYPE_INT ||
          node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
    node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
          node->right()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_POINTER));
  }
  else if(node->left()->type()->name() == basic_type::TYPE_INT &&
          node->right()->type()->name() == basic_type::TYPE_POINTER){
    node->type(new basic_type(4,basic_type::TYPE_POINTER));
  }else throw std::string("wrong type in add");
}
void pwn::type_checker::do_sub_node(cdk::sub_node * const node, int lvl) {
    ASSERT_UNSPEC;
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT &&
       node->right()->type()->name() == basic_type::TYPE_INT){
      node->type(new basic_type(4,basic_type::TYPE_INT));
    }
    else if(node->left()->type()->name() == basic_type::TYPE_INT &&
            node->right()->type()->name() == basic_type::TYPE_DOUBLE){
      node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
    }
    else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE &&
           (node->right()->type()->name() == basic_type::TYPE_INT ||
            node->right()->type()->name() == basic_type::TYPE_DOUBLE)){
      node->type(new basic_type(8,basic_type::TYPE_DOUBLE));
    }
    else if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
            node->right()->type()->name() == basic_type::TYPE_INT){
      node->type(new basic_type(4,basic_type::TYPE_POINTER));
    }else if(node->left()->type()->name() == basic_type::TYPE_INT &&
            node->right()->type()->name() == basic_type::TYPE_POINTER){
      node->type(new basic_type(4,basic_type::TYPE_POINTER));
    }else if(node->left()->type()->name() == basic_type::TYPE_POINTER &&
             node->right()->type()->name() == basic_type::TYPE_POINTER){
      node->type(new basic_type(4, basic_type::TYPE_INT));
    }
    else
      throw std::string("wrong type in sub node");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_mul_node(cdk::mul_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else throw std::string("wrong arguments type on mul node");
}
void pwn::type_checker::do_div_node(cdk::div_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->left()->accept(this, lvl);
  node->right()->accept(this, lvl);
  if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else if(node->left()->type()->name() == basic_type::TYPE_INT && node->right()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else if(node->left()->type()->name() == basic_type::TYPE_DOUBLE && node->right()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else throw std::string("wrong arguments type on div node");
}
void pwn::type_checker::do_mod_node(cdk::mod_node * const node, int lvl) {

    ASSERT_UNSPEC;
    node->left()->accept(this, lvl);
    node->right()->accept(this, lvl);
    if(node->left()->type()->name() == basic_type::TYPE_INT &&
       node->right()->type()->name() == basic_type::TYPE_INT){
      node->type(new basic_type(4,basic_type::TYPE_INT));
    }else throw std::string("wrong arguments type on mod node");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_neg_node(cdk::neg_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl);
  if(node->argument()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else throw std::string("expecting integer or double value on neg node");
}
void pwn::type_checker::do_identity_node(pwn::identity_node * const node, int lvl) {
  ASSERT_UNSPEC;
  node->argument()->accept(this, lvl);
  if(node->argument()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4,basic_type::TYPE_INT));
  }else if(node->argument()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(4,basic_type::TYPE_DOUBLE));
  }else throw std::string("expecting integer or double value on identity node");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_repeat_node(pwn::repeat_node * const node, int lvl)  {
  node->condition()->accept(this, lvl);
  if(node->condition()->type()->name() != basic_type::TYPE_INT)
    throw std::string("repeat node expecting integer value on condition statement");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_rvalue_node(pwn::rvalue_node * const node, int lvl) {
  node->lvalue()->accept(this, lvl);
  node->type(node->lvalue()->type());
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_lvalue_node(pwn::lvalue_node * const node, int lvl) {
  std::shared_ptr<pwn::symbol> symbol = _symtab.find(*node->name());
  if(symbol.get() == NULL) throw *node->name() + " undeclared";

  if(symbol.get()->type()->name() == basic_type::TYPE_INT){
    node->type(new basic_type(4, basic_type::TYPE_INT));
  }else if(symbol.get()->type()->name() == basic_type::TYPE_DOUBLE){
    node->type(new basic_type(8, basic_type::TYPE_DOUBLE));
  }else if(symbol.get()->type()->name() == basic_type::TYPE_STRING){
    node->type(new basic_type(4, basic_type::TYPE_STRING));
  }else if(symbol.get()->type()->name() == basic_type::TYPE_POINTER){
    node->type(new basic_type(4, basic_type::TYPE_POINTER));
  }else
    throw std::string("wrong type of lvalue");
}


void pwn::type_checker::do_println_node(pwn::println_node * const node, int lvl){
  node->argument()->accept(this, lvl);
}
void pwn::type_checker::do_print_node(pwn::print_node * const node, int lvl){
  node->argument()->accept(this, lvl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_evaluation_node(pwn::evaluation_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_read_node(pwn::read_node * const node, int lvl) {
  node->argument()->accept(this, lvl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_index_node(pwn::index_node * const node, int lvl) {
  ASSERT_UNSPEC;
  if (node->index()->type()->name() != basic_type::TYPE_INT)
    throw std::string("wrong type of index on index node. expecting an integer value");
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}

void pwn::type_checker::do_address_node(pwn::address_node * const node, int lvl) {
  node->type(new basic_type(4,basic_type::TYPE_POINTER));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_function_call_node(pwn::function_call_node * const node, int lvl) {
  ASSERT_UNSPEC;
  std::shared_ptr<pwn::symbol> f = _symtab.find(*node->name());
  if(f.get() == NULL) throw std::string("Function" + *node->name() + "undeclared");
  node->type(new basic_type(f.get()->type()->size(), f.get()->type()->name()));
  if(node->argument() != NULL)
    node->argument()->accept(this, lvl);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void pwn::type_checker::do_noob_node(pwn::noob_node * const node, int lvl) {
  node->type(new basic_type(4, basic_type::TYPE_POINTER));
}
