//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/standalone/Region.hxx
//
// created       julien quintard   [sun may  2 12:28:06 2010]
// updated       julien quintard   [sat jun 18 12:13:51 2011]
//

#ifndef ELLE_STANDALONE_REGION_HXX
#define ELLE_STANDALONE_REGION_HXX

namespace elle
{
  namespace standalone
  {

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method recycles a region.
    ///
    template <typename T>
    Status		Region::Recycle(const T*		object)
    {
      // release the resources.
      this->~Region();

      if (object == NULL)
	{
	  // initialize the object with default values.
	  new (this) T;
	}
      else
	{
	  // initialize the object with defined values.
	  new (this) T(*object);
	}

      // return StatusOk in order to avoid including Report, Status and Maid.
      return (StatusOk);
    }

  }
}

#endif
