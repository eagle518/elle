//
// ---------- header ----------------------------------------------------------
//
// project       lune
//
// license       infinit
//
// author        julien quintard   [tue mar  6 15:01:36 2012]
//

#ifndef LUNE_SET_HH
#define LUNE_SET_HH

//
// ---------- includes --------------------------------------------------------
//

#include <elle/Elle.hh>

namespace lune
{

//
// ---------- classes ---------------------------------------------------------
//

  ///
  /// this class holds the set or subset of initial nodes required for
  /// a hole implementation to initiate connections.
  ///
  class Set:
    public elle::Object,
    public elle::Fileable<Set>
  {
  public:
    //
    // constants
    //
    static const elle::String           Extension;

    //
    // types
    //
    // XXX use std::set<elle::Locus> !
    typedef std::vector<elle::Locus>    Container;

    //
    // methods
    //
    elle::Status        Add(const elle::Locus&);
    elle::Status        Remove(const elle::Locus&);

    //
    // interfaces
    //

    // object
    declare(Set);

    // dumpable
    elle::Status        Dump(const elle::Natural32 = 0) const;

    // archivable
    elle::Status        Serialize(elle::Archive&) const;
    elle::Status        Extract(elle::Archive&);

    // fileable
    elle::Status        Load(const elle::String&);
    elle::Status        Store(const elle::String&) const;
    elle::Status        Erase(const elle::String&) const;
    elle::Status        Exist(const elle::String&) const;

    //
    // attributes
    //
    Container           loci;
  };

}

#endif
