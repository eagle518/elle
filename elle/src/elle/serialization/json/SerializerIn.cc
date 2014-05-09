#include <limits>

#include <elle/Backtrace.hh>
#include <elle/json/exceptions.hh>
#include <elle/memory.hh>
#include <elle/printf.hh>
#include <elle/serialization/Error.hh>
#include <elle/serialization/json/MissingKey.hh>
#include <elle/serialization/json/SerializerIn.hh>
#include <elle/serialization/json/TypeError.hh>

namespace elle
{
  namespace serialization
  {
    namespace json
    {
      /*-------------.
      | Construction |
      `-------------*/

      SerializerIn::SerializerIn(std::istream& input)
        : Super(input)
        , _json()
        , _current()
      {
        try
        {
          this->_json = elle::json::read(input);
          this->_current.push_back(&this->_json);
        }
        catch (elle::json::ParserError const& e)
        {
          Error exception("json parse error");
          exception.inner_exception(
            elle::make_unique<elle::json::ParserError>(e));
          throw exception;
        }
      }

      void
      SerializerIn::_serialize(std::string const& name, int64_t& v)
      {
        v = this->_check_type<int64_t>(name);
      }

      void
      SerializerIn::_serialize(std::string const& name, int32_t& v)
      {
        int64_t value;
        this->_serialize(name, value);
        if (value > std::numeric_limits<int32_t>::max())
          throw Error(elle::sprintf(
                        "32-bits overflow on key \"%s\"", name));
        if (value < std::numeric_limits<int32_t>::min())
          throw Error(elle::sprintf(
                        "32-bits underflow on key \"%s\"", name));
        v = value;
      }

      void
      SerializerIn::_serialize(std::string const& name, double& v)
      {
        v = this->_check_type<double>(name);
      }

      void
      SerializerIn::_serialize(std::string const& name, bool& v)
      {
        v = this->_check_type<bool>(name);
      }

      void
      SerializerIn::_serialize_option(std::string const& name,
                                      bool,
                                      std::function<void ()> const& f)
      {
        auto& object = this->_check_type<elle::json::Object>(name);
        if (object.find(name) != object.end())
          f();
      }

      void
      SerializerIn::_serialize(std::string const& name, std::string& v)
      {
        v = this->_check_type<std::string>(name);
      }

      void
      SerializerIn::_enter(std::string const& name)
      {
        auto& object = this->_check_type<elle::json::Object>(name);
        auto it = object.find(name);
        if (it == object.end())
          throw MissingKey(name);
        this->_current.push_back(&it->second);
      }

      void
      SerializerIn::_leave(std::string const&)
      {
        this->_current.pop_back();
      }

      void
      SerializerIn::_serialize_array(
        std::string const& name,
        std::function<void ()> const& serialize_element)
      {
        auto& array = this->_check_type<elle::json::Array>(name);
        for (auto& elt: array)
        {
          this->_current.push_back(&elt);
          serialize_element();
          this->_current.pop_back();
        }
      }

      template <typename T>
      T&
      SerializerIn::_check_type(std::string const& name)
      {
        auto& current = *this->_current.back();
        if (current.type() != typeid(T))
          throw TypeError(name, typeid(T), current.type());
        return boost::any_cast<T&>(current);
      }
    }
  }
}
