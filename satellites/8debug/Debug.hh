//
// ---------- header ----------------------------------------------------------
//
// project       8debug
//
// license       infinit
//
// file          /home/mycure/infinit/applications/8debug/Debug.hh
//
// created       julien quintard   [sat mar 27 08:37:14 2010]
// updated       julien quintard   [thu apr 29 13:42:10 2010]
//

#ifndef DEBUG_DEBUG_HH
#define DEBUG_DEBUG_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>
#include <etoile/Etoile.hh>

#include <elle/idiom/Close.hh>
# include <sys/stat.h>
#include <elle/idiom/Open.hh>

namespace application
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// this class implements the 8debug application.
  ///
  class Debug
  {
  public:
    //
    // enumerations
    //
    enum Operation
      {
	OperationUnknown = 0,

	OperationResolve,
	OperationDump
      };

    //
    // static methods
    //
    static Status	Dump(const etoile::hole::Address&,
			     const KeyPair&,
			     const etoile::kernel::Token&);
  };

}

#endif
