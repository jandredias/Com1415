// $Id: symbol.h,v 1.8 2015/05/19 18:01:16 ist175741 Exp $ -*- c++ -*-
#ifndef __PWN_SEMANTICS_SYMBOL_H__
#define __PWN_SEMANTICS_SYMBOL_H__

#include <string>
#include <cdk/basic_type.h>
#include <vector>
namespace pwn {

  class symbol {
    basic_type *_type; //Symbol's type: int, double, string or pointer
    std::string _name; //Symbol's identifier
    long _value;       // no idea
    int _offset;       //Symbol's offset to FP address
	  bool _function;    //true if symbol is a function
    bool _defined;     //if symbol is a function, true if it is defined
    std::vector<basic_type*> args;
    public:
  		inline symbol(basic_type *type, const std::string &name, int offset,
        bool function = false, bool defined = true)
        : _type(type), _name(name), _offset(offset),
          _function(function), _defined(defined) {}

		virtual ~symbol()                      { delete _type; }
		inline const bool function()           { return _function; }
		inline const bool defined()            { return _defined; }
		inline const void defined(bool d)      { _defined = d; }

		inline basic_type *type() const        { return _type; }
		inline const std::string &name() const { return _name; }
		inline long value() const              { return _value; }
    inline int offset() const              { return _offset; }
    inline void offset(int i)              { _offset = i; }
		inline long value(long v)              { return _value = v; }
  };
} // pwn

#endif
