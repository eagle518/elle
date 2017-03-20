#pragma once

#include <openssl/dsa.h>

#include <elle/cryptography/serialization.hh>

namespace elle
{
  namespace cryptography
  {
    namespace dsa
    {
      namespace serialization
      {
        /*-----------.
        | Structures |
        `-----------*/

        struct DSA
        {
          using Type = ::DSA;
          static constexpr const char* identifier = "dsa";
        };
      }
    }
  }
}
