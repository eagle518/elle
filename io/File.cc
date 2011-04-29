//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/io/File.cc
//
// created       julien quintard   [thu may 27 16:18:11 2010]
// updated       julien quintard   [thu apr 28 18:30:24 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/io/File.hh>
#include <elle/io/Directory.hh>

#include <elle/standalone/Maid.hh>
#include <elle/standalone/Report.hh>

#include <elle/system/System.hh>

#include <vector>
#include <sstream>

#include <elle/idiom/Close.hh>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <libgen.h>
#include <elle/idiom/Open.hh>

namespace elle
{
  using namespace system;

  namespace io
  {

//
// ---------- static methods --------------------------------------------------
//

    ///
    /// this method reads the given file's content.
    ///
    Status		File::Read(const Path&			path,
				   Region&			data)
    {
      struct ::stat	status;
      int		fd;

      enter();

      // does the file exist.
      if (File::Exist(path) == StatusFalse)
	escape("the file does not seem to exist");

      // retrieve information on the associat.
      if (::stat(path.string.c_str(), &status) == -1)
	escape(::strerror(errno));

      // prepare the data.
      if (data.Prepare(status.st_size) == StatusError)
	escape("unable to prepare the region");

      // set the correct size.
      data.size = status.st_size;

      // open the file.
      if ((fd = ::open(path.string.c_str(), O_RDONLY)) == -1)
	escape(::strerror(errno));

      // read the file's content.
      if (::read(fd, data.contents, data.size) == -1)
	{
	  ::close(fd);

	  escape(::strerror(errno));
	}

      // close the file.
      ::close(fd);

      leave();
    }

    ///
    /// this method writes the given data into the given file.
    ///
    Status		File::Write(const Path&			path,
				    const Region&		data)
    {
      int		fd;

      enter();

      // dig the directory which will hold the target file.
      if (File::Dig(path) == StatusError)
	escape("unable to dig the chain of directories");

      // open the file.
      if ((fd = ::open(path.string.c_str(),
		       O_CREAT | O_TRUNC | O_WRONLY,
		       0600)) == -1)
	escape(::strerror(errno));

      // write the text to the file.
      if (::write(fd,
		  data.contents,
		  data.size) != data.size)
	{
	  ::close(fd);

	  escape(::strerror(errno));
	}

      // close the file.
      ::close(fd);

      leave();
    }

    ///
    /// this method erases the given file path.
    ///
    Status		File::Erase(const Path&			path)
    {
      enter();

      // does the file exist.
      if (File::Exist(path) == StatusFalse)
	escape("the file does not seem to exist");

      // unlink the file.
      ::unlink(path.string.c_str());

      leave();
    }

    ///
    /// this method returns true if the pointed to file exists.
    ///
    Status		File::Exist(const Path&			path)
    {
      struct ::stat		stat;

      enter();

      // does the path points to something.
      if (::stat(path.string.c_str(), &stat) != 0)
	false();

      // does the path points to a regular file.
      if (!S_ISREG(stat.st_mode))
	false();

      true();
    }

    ///
    /// this method takes a path to a file and creates, if necessary,
    /// the intermediate directory leading to the file.
    ///
    Status		File::Dig(const Path&			path)
    {
      String		target(::strdup(path.string.c_str()));
      String		directory(::dirname((char*)target.c_str()));
      std::stringstream	stream(directory);
      String		item;
      Path		chemin;

      enter();

      // go through the components of the path.
      while (std::getline(stream, item, System::Path::Separator))
	{
	  // update the intermediate chemin.
	  if (chemin.string.empty() && item.empty())
	    chemin.string = System::Path::Separator;
	  else
	    {
	      chemin.string.append(item);
	      chemin.string.append(1, System::Path::Separator);
	    }

	  // retrieve information on the path. should this operation fail
	  // would mean that the target directory does not exist.
	  if (Directory::Exist(chemin) == StatusFalse)
	    {
	      // create the intermediate directory.
	      if (Directory::Create(chemin) == StatusError)
		escape("unable to create the intermediate directory");
	    }
	}

      leave();
    }

  }
}
