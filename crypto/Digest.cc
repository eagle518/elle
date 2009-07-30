//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit (c)
//
// file          /home/mycure/infinit/elle/crypto/Digest.cc
//
// created       julien quintard   [mon feb  2 22:22:12 2009]
// updated       julien quintard   [thu jul 30 13:03:36 2009]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/crypto/Digest.hh>

namespace elle
{
  using namespace core;
  using namespace misc;
  using namespace archive;

  namespace crypto
  {

//
// ---------- constructors & destructors --------------------------------------
//

    ///
    /// this method initializes the attributes.
    ///
    Digest::Digest()
    {
    }

    ///
    /// this method releases the resources.
    ///
    Digest::~Digest()
    {
    }

//
// ---------- entity ----------------------------------------------------------
//

    ///
    /// assign the given digest by duplicating the attributes.
    ///
    Digest&		Digest::operator=(const Digest&		element)
    {
      // self-check
      if (this == &element)
	return (*this);

      // recycle the digest.
      if (this->Recycle<Digest>() == StatusError)
	yield("unable to recycle the digest", *this);

      // set the region.
      this->region = element.region;

      return (*this);
    }

    ///
    /// this method check if two digests match.
    ///
    Boolean		Digest::operator==(const Digest&	element)
    {
      // compare the regions.
      return (this->region == element.region);
    }

    ///
    /// this method checks if two digests dis-match.
    ///
    Boolean		Digest::operator!=(const Digest&	element)
    {
      return (!(*this == element));
    }


//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the digest.
    ///
    Status		Digest::Dump(const Natural32		margin)
    {
      String		alignment(margin, ' ');

      std::cout << alignment << "[Digest]" << std::endl;

      if (this->region.Dump(margin + 2) == StatusError)
	escape("unable to dump the region");

      leave();
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method serializes a digest object.
    ///
    Status		Digest::Serialize(Archive&		archive) const
    {
      // serialize the region.
      if (archive.Serialize(this->region) == StatusError)
	escape("unable to serialize the region");

      leave();
    }

    ///
    /// this method extracts a digest object.
    ///
    Status		Digest::Extract(Archive&		archive)
    {
      // extract the content.
      if (archive.Extract(this->region) == StatusError)
	escape("unable to extract the region");

      leave();
    }

  }
}
