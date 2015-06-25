#include <elle/serialization/binary/SerializerIn.hh>

#include <elle/serialization/json/Overflow.hh>
#include <elle/serialization/json/FieldError.hh>
ELLE_LOG_COMPONENT("elle.serialization.binary.SerializerIn")

namespace elle
{
  namespace serialization
  {
    namespace binary
    {
      SerializerIn::SerializerIn(std::istream& input,
                                 bool versioned)
        : Self(input, elle::Version(), versioned)
      {}

      SerializerIn::SerializerIn(std::istream& input,
                                 elle::Version version,
                                 bool versioned)
        : Super(input, version, versioned)
      {

      }


      void
      SerializerIn::_serialize(std::string const& name, int64_t& v)
      {
        ELLE_TRACE_SCOPE("%s: deserialize integer \"%s\"", *this, name);
        v = _serialize_number();
      }

      void
      SerializerIn::_serialize(std::string const& name, uint64_t& v)
      {
        int64_t value;
        this->_serialize(name, value);
        if (value < 0)
          throw Error(elle::sprintf(
                        "64-bits unsigned underflow on key \"%s\"", name));
        v = value;
      }

      void
      SerializerIn::_serialize(std::string const& name, int32_t& v)
      {
        this->_serialize_int<int32_t>(name, v);
      }

      void
      SerializerIn::_serialize(std::string const& name, uint32_t& v)
      {
        this->_serialize_int<uint32_t>(name, v);
      }

      void
      SerializerIn::_serialize(std::string const& name, int8_t& v)
      {
        this->_serialize_int<int8_t>(name, v);
      }

      void
      SerializerIn::_serialize(std::string const& name, uint8_t& v)
      {
        this->_serialize_int<uint8_t>(name, v);
      }

      template <typename T>
      void
      SerializerIn::_serialize_int(std::string const& name, T& v)
      {
        int64_t value;
        this->_serialize(name, value);
        if (value > std::numeric_limits<T>::max())
          throw json::Overflow(name, sizeof(T) * 8, true);
        if (value < std::numeric_limits<T>::min())
          throw json::Overflow(name, sizeof(T) * 8, false);
        v = value;
      }

      void
      SerializerIn::_serialize(std::string const& name, double& v)
      {
        ELLE_TRACE_SCOPE("%s: deserialize double \"%s\"", *this, name);
        input().read((char*)(void*)&v, sizeof(double));
      }

      void
      SerializerIn::_serialize(std::string const& name, bool& v)
      {
        int val;
        _serialize(name, val);
        if (val !=0 && val != 1)
          throw json::Overflow(name, 1, true);
        v = val ? true:false;
      }

      void
      SerializerIn::_serialize_option(std::string const& name,
                                      bool,
                                      std::function<void ()> const& f)
      {
        bool isOn;
        _serialize(name, isOn);
        if (isOn)
          f();
      }

      void
      SerializerIn::_serialize(std::string const& name, std::string& v)
      {
        ELLE_TRACE_SCOPE("%s: deserialize string \"%s\"", *this, name);
        int sz = _serialize_number();
        v.resize(sz);
        input().read((char*)v.data(), sz);
        ELLE_ASSERT(input().gcount() == sz);
      }

      void
      SerializerIn::_serialize(std::string const& name, elle::Buffer& buffer)
      {
        ELLE_TRACE_SCOPE("%s: deserialize buffer \"%s\"", *this, name);
        int sz = _serialize_number();
        buffer.size(sz);
        input().read((char*)buffer.mutable_contents(), sz);
        ELLE_ASSERT(input().gcount() == sz);
      }

      void
      SerializerIn::_serialize(std::string const& name,
                               boost::posix_time::ptime& time)
      {
        ELLE_TRACE_SCOPE("%s: deserialize date \"%s\"", *this, name);
        std::string str;
        _serialize(name, str);
        // Use the ISO extended input facet to interpret the string.
        std::stringstream ss(str);
        auto input_facet =
          elle::make_unique<boost::posix_time::time_input_facet>();
        // ISO 8601
        input_facet->format("%Y-%m-%dT%H:%M:%S%F");
        ss.imbue(std::locale(ss.getloc(), input_facet.release()));
        if (!(ss >> time))
          throw json::FieldError
            (name, elle::sprintf("invalid ISO8601 date: %s", str));
        // Check there isn't any leftover.
        std::string leftover;
        std::getline(ss, leftover);
        if (leftover.empty())
          return;
        // Boost can't parse timezones, handle it manually.
        if (leftover == "Z")
          ; // Accept UTC suffix.
        else if ((leftover[0] == '+' || leftover[0] == '-') && leftover.size() == 5)
        {
          // Handle timezone.
          std::stringstream tz(leftover);
          int direction = tz.get() == '+' ? -1 : 1;
          int amount;
          tz >> amount;
          if (tz.get() != -1)
            throw json::FieldError
              (name, elle::sprintf("garbage at end of date: %s", leftover));
          time += boost::posix_time::hours(direction * amount / 100);
        }
        else
          throw json::FieldError
            (name, elle::sprintf("garbage at end of date: %s", leftover));
        if (!ss.eof())
        {
          std::string leftover;
          std::getline(ss, leftover);
          throw json::FieldError
            (name, elle::sprintf("garbage at end of date: %s", leftover));
        }
      }

      bool
      SerializerIn::_enter(std::string const& name)
      {
        ELLE_TRACE_SCOPE("%s: enter \"%s\"", *this, name);
        return true;
      }

      void
      SerializerIn::_leave(std::string const& name)
      {
        ELLE_TRACE_SCOPE("%s: leave \"%s\"", *this, name);
      }

      void
      SerializerIn::_serialize_array(
        std::string const& name,
        int,
        std::function<void ()> const& serialize_element)
      {
        ELLE_TRACE_SCOPE("%s: serialize array \"%s\"", *this, name);
        int count = _serialize_number();
        for (int i=0; i<count; ++i)
          serialize_element();
      }

      int64_t
      SerializerIn::_serialize_number()
      {
        uint64_t res;
        unsigned char c = input().get();
        bool negative = c & 0x80;
        if (! (c&0x40))
        {
          res = c&0x3f;
        }
        else if (! (c&0x20))
        {
          unsigned char c2 = input().get();
          res = ((c&0x1F) << 8) + c2;
        }
        else if (! (c&0x10))
        {
          unsigned char c2 = input().get();
          unsigned char c3 = input().get();
          res = ((c&0x0F) << 16) + (c2 << 8) + c3; 
        }
        else
        {
          input().read((char*)(void*)&res, 8);
          /*
          unsigned char elems[8];
          input().read((char*)elems, 8);
          res = 0;
          for (int i=0; i<8; ++i)
            res += uint64_t(elems[i]) << (8*i);*/
        }
        if (negative)
          return - (int64_t)res;
        else
          return res;
      }
    }
  }
}