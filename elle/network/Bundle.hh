//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/network/Bundle.hh
//
// created       julien quintard   [fri jun  3 22:23:13 2011]
// updated       julien quintard   [fri jul  8 12:06:03 2011]
//

#ifndef ELLE_NETWORK_BUNDLE_HH
#define ELLE_NETWORK_BUNDLE_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/radix/Status.hh>
#include <elle/radix/Entity.hh>
#include <elle/radix/Arguments.hh>

#include <elle/network/Tag.hh>
#include <elle/network/Message.hh>

namespace elle
{
  using namespace radix;

  namespace network
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class represents a set of arguments associated
    /// with a network tag.
    ///
    /// note that this class is specialized for inputs (const) and ouptuts
    /// (non-const).
    ///
    struct Bundle
    {
      ///
      /// XXX
      ///
      template <const Tag G,
		typename... T>
      class Inputs;

      ///
      /// XXX
      ///
      template <const Tag G,
		typename... T>
      class Inputs< G, Parameters<const T...> >:
	public Entity,
	public virtual Archivable
      {
      public:
	//
	// types
	//
	typedef Parameters<const T&...>				P;

	//
	// constructors & destructors
	//
	Inputs(const T&...);

	//
	// interfaces
	//

	// archivable
	Status		Serialize(Archive&) const;
	Status		Extract(Archive&);

	// dumpable
	Status		Dump(const Natural32 = 0) const;

	//
	// attributes
	//
	Tag		tag;
	Arguments<P>	arguments;
      };

      ///
      /// XXX
      ///
      template <const Tag G,
		typename... T>
      class Outputs;

      ///
      /// XXX
      ///
      template <const Tag G,
		typename... T>
      class Outputs< G, Parameters<T...> >:
	public Entity,
	public virtual Archivable
      {
      public:
	//
	// types
	//
	typedef Parameters<T&...>				P;

	//
	// constructors & destructors
	//
	Outputs(T&...);

	//
	// interfaces
	//

	// archivable
	Status		Serialize(Archive&) const;
	Status		Extract(Archive&);

	// dumpable
	Status		Dump(const Natural32 = 0) const;

	//
	// attributes
	//
	Tag		tag;
	Arguments<P>	arguments;
      };

    };

  }
}

//
// ---------- templates -------------------------------------------------------
//

#include <elle/network/Bundle.hxx>

#endif
