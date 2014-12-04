/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2012 Scientific Computing and Imaging Institute,
   University of Utah.

   License for the specific language governing rights and limitations under
   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#ifndef ALGORITHMS_BASE_ALGORITHMVARIABLE_H
#define ALGORITHMS_BASE_ALGORITHMVARIABLE_H

#include <string>
#include <vector>
#include <iosfwd>
#include <boost/variant.hpp>
#include <boost/filesystem/path.hpp>
#include <Core/Datatypes/DatatypeFwd.h>
#include <Core/Algorithms/Base/Name.h>
#include <Core/Algorithms/Base/Option.h>
#include <Core/Algorithms/Base/share.h>

namespace SCIRun {
namespace Core {
namespace Algorithms {

  class SCISHARE Variable
  {
  public:
    typedef std::vector<Variable> List;

    typedef boost::variant<
      int,
      double,
      std::string,
      bool,
      AlgoOption,
      List
    > Value;

    Variable() {}
    Variable(const Name& name, const Value& value);
    Variable(const Name& name, const Datatypes::DatatypeHandle& data) : name_(name), data_(data) {}
    virtual ~Variable() {}

    const Name& name() const { return name_; }
    const Value& value() const { return value_; }
    //TODO: remove virtual on this class
    virtual void setValue(const Value& val);

    int toInt() const;
    double toDouble() const;
    std::string toString() const;
    boost::filesystem::path toFilename() const;
    bool toBool() const;
    List toVector() const;
    AlgoOption toOption() const;

    Datatypes::DatatypeHandle getDatatype() const;

    Name& nameForXml() { return name_; }
    Value& valueForXml() { return value_; }

  private:
    /*const*/ Name name_;
    Value value_;
    Datatypes::DatatypeHandleOption data_;
  };

  SCISHARE bool operator==(const Variable& lhs, const Variable& rhs);
  SCISHARE bool operator!=(const Variable& lhs, const Variable& rhs);
  SCISHARE std::ostream& operator<<(std::ostream& out, const Variable& var);
  SCISHARE Variable makeVariable(const std::string& name, const Variable::Value& value);

  typedef Variable AlgorithmParameter;

  typedef boost::shared_ptr<Variable> VariableHandle;

}

//type-converting bolt-on class template
template <typename T>
class TypedVariable : public Algorithms::Variable
{
public:
  typedef T value_type;
  TypedVariable(const std::string& name, const value_type& value) : Algorithms::Variable(Algorithms::Name(name), value) {}

  operator value_type() const { return val(); }
  value_type val() const { throw "unknown type"; }
};

#define TYPED_VARIABLE_CLASS(type, func) template <> \
class TypedVariable<type> : public Algorithms::Variable \
{\
public:\
  typedef type value_type;\
  TypedVariable(const std::string& name, const value_type& value) : Algorithms::Variable(Algorithms::Name(name), value) {}\
  operator value_type() const { return val(); }\
  value_type val() const { return func(); }\
};\

TYPED_VARIABLE_CLASS(bool, toBool)
TYPED_VARIABLE_CLASS(std::string, toString)

typedef TypedVariable<bool> BooleanVariable;
typedef TypedVariable<std::string> StringVariable;

}}

#endif
