//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/io/Console.hh
//
// created       julien quintard   [sat jun 18 11:09:29 2011]
// updated       julien quintard   [sat jun 18 11:26:33 2011]
//

#ifndef ELLE_IO_CONSOLE_HH
#define ELLE_IO_CONSOLE_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/core/String.hh>

#include <elle/radix/Status.hh>

namespace elle
{
  using namespace core;
  using namespace radix;

  namespace io
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this static class provides functionalities related to the console.
    ///
    class Console
    {
    public:
      //
      // enumerations
      //
      enum Option
	{
	  OptionNone,

	  OptionPassword
	};

      //
      // static methods
      //
      static Status	Input(String&,
			      const String& = "",
			      const Option = OptionNone);
    };

  }
}

#endif
