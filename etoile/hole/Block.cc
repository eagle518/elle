//
// ---------- header ----------------------------------------------------------
//
// project       etoile
//
// license       infinit
//
// file          /home/mycure/infinit/etoile/hole/Block.cc
//
// created       julien quintard   [fri sep 11 22:44:58 2009]
// updated       julien quintard   [thu apr 28 18:06:47 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <etoile/hole/Block.hh>

namespace etoile
{
  namespace hole
  {

//
// ---------- definitions -----------------------------------------------------
//

  ///
  /// this string defines the block files extension.
  ///
  const elle::String		Block::Extension = ".blk";

//
// ---------- constructs & destructors ----------------------------------------
//

    Block::Block():
      family(FamilyUnknown),
      component(ComponentUnknown)
    {
    }

    Block::Block(const Family&					family,
		 const Component&				component):
      family(family),
      component(component)
    {
    }

//
// ---------- methods ---------------------------------------------------------
//

    ///
    /// this method computes the address of the block.
    ///
    elle::Status	Block::Bind(Address&) const
    {
      enter();

      fail("this method should never have been called");
    }

    ///
    /// this method returns StatusTrue if the block is valid, StatusFalse
    /// otherwise.
    ///
    elle::Status	Block::Validate(const Address&) const
    {
      enter();

      fail("this method should never have been called");
    }

//
// ---------- object ----------------------------------------------------------
//

    ///
    /// this macro-function call generates the object.
    ///
    embed(Block, _());

//
// ---------- dumpable --------------------------------------------------------
//

    ///
    /// this method dumps the block's internals.
    ///
    elle::Status	Block::Dump(const elle::Natural32	margin) const
    {
      elle::String	alignment(margin, ' ');

      enter();

      std::cout << alignment << "[Block]" << std::endl;

      // dump the family.
      std::cout << alignment << elle::Dumpable::Shift << "[Family] "
		<< (elle::Natural32)this->family << std::endl;

      // dump the component.
      std::cout << alignment << elle::Dumpable::Shift << "[Component] "
		<< (elle::Natural32)this->component << std::endl;

      leave();
    }

//
// ---------- archivable ------------------------------------------------------
//

    ///
    /// this method archives the block attributes.
    ///
    elle::Status	Block::Serialize(elle::Archive&	archive) const
    {
      enter();

      // serialize the attributes.
      if (archive.Serialize((elle::Natural8&)this->family,
			    (elle::Natural8&)this->component) ==
	  elle::StatusError)
	escape("unable to serialize the block's attributes");

      leave();
    }

    ///
    /// this method extracts the attributes.
    ///
    elle::Status	Block::Extract(elle::Archive&		archive)
    {
      enter();

      // extracts the attributes.
      if (archive.Extract((elle::Natural8&)this->family,
			  (elle::Natural8&)this->component) ==
	  elle::StatusError)
	escape("unable to extract the block's attributes");

      leave();
    }

//
// ---------- fileable --------------------------------------------------------
//

    ///
    /// this method loads the block.
    ///
    elle::Status	Block::Load(const Address&		address)
    {
      elle::Path	path;
      elle::Unique	unique;
      elle::Region	region;

      enter();

      // first, turn the block's address into a string.
      if (address.Save(unique) == elle::StatusError)
	escape("unable to save the address' unique");

      // create a path.
      if (path.Create(elle::String("/home/mycure/.infinit/HOLE/") +
		      unique + Block::Extension) == elle::StatusError)
	escape("unable to create a path");

      // read the file's content.
      if (elle::File::Read(path, region) == elle::StatusError)
	escape("unable to read the file's content");

      // decode and extract the object.
      if (elle::Hexadecimal::Decode(elle::String((char*)region.contents,
						 region.size),
				    *this) == elle::StatusError)
	escape("unable to decode the object");

      leave();
    }

    ///
    /// this method stores the block in its file format.
    ///
    elle::Status	Block::Store(const Address&		address) const
    {
      elle::Path	path;
      elle::Unique	unique;
      elle::Region	region;
      elle::String	string;

      enter();

      // first, turn the block's address into a string.
      if (address.Save(unique) == elle::StatusError)
	escape("unable to save the address' unique");

      // create a path.
      if (path.Create(elle::String("/home/mycure/.infinit/HOLE/") +
		      unique + Block::Extension) == elle::StatusError)
	escape("unable to create a path");

      // encode in hexadecimal.
      if (elle::Hexadecimal::Encode(*this, string) == elle::StatusError)
	escape("unable to encode the object in hexadecimal");

      // wrap the string.
      if (region.Wrap((elle::Byte*)string.c_str(),
		      string.length()) == elle::StatusError)
	escape("unable to wrap the string in a region");

      // write the file's content.
      if (elle::File::Write(path, region) == elle::StatusError)
	escape("unable to write the file's content");

      leave();
    }

    ///
    /// this method erases a block.
    ///
    elle::Status	Block::Erase(const Address&		address) const
    {
      elle::Path	path;
      elle::Unique	unique;

      enter();

      // first, turn the block's address into a string.
      if (address.Save(unique) == elle::StatusError)
	escape("unable to save the address' unique");

      // create a path.
      if (path.Create(elle::String("/home/mycure/.infinit/HOLE/") +
		      unique + Block::Extension) == elle::StatusError)
	escape("unable to create a path");

      // erase the file.
      if (elle::File::Erase(path) == elle::StatusError)
	escape("unable to erase the file");

      leave();
    }

    ///
    /// this method returns true if the block exists.
    ///
    elle::Status	Block::Exist(const Address&		address) const
    {
      elle::Path	path;
      elle::Unique	unique;

      enter();

      // first, turn the block's address into a string.
      if (address.Save(unique) == elle::StatusError)
	escape("unable to save the address' unique");

      // create a path.
      if (path.Create(elle::String("/home/mycure/.infinit/HOLE/") +
		      unique + Block::Extension) == elle::StatusError)
	escape("unable to create a path");

      // test the file.
      if (elle::File::Exist(path) == elle::StatusError)
	escape("unable to test the file");

      leave();
    }

  }
}
