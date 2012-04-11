//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// author        julien quintard   [sun may 22 13:08:22 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include <elle/io/Link.hh>
#include <elle/io/File.hh>
#include <elle/io/Directory.hh>

#include <elle/standalone/Maid.hh>
#include <elle/standalone/Report.hh>

#include <elle/system/System.hh>
#include <elle/system/Platform.hh>

#include <vector>
#include <sstream>

#include <elle/idiom/Close.hh>
# include <sys/stat.h>
# include <unistd.h>
# include <fcntl.h>
# include <libgen.h>
# include <string.h>
# if defined(INFINIT_WINDOWS)
#  include <windows.h>
#  include <QFile>
# endif
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
    /// this method creates a link.
    ///
    Status              Link::Create(const Path&                link,
                                     const Path&                target)
    {
      // does the link exist.
      if (Link::Exist(link) == StatusTrue)
        escape("the link seems to already exist");

      // does the target exist.
      if ((File::Exist(target) == StatusFalse) &&
          (Directory::Exist(target) == StatusFalse))
        escape("the target does not seem to exist");

      // create the link.
#if defined(INFINIT_LINUX) || defined(INFINIT_MACOSX)
      if (::symlink(target.str().c_str(), link.str().c_str()))
        escape("symlink failed: %s -> %s: %s", link.str().c_str(),
               target.str().c_str(), ::strerror(errno));
#elif defined(INFINIT_WINDOWS)
      if (!QFile::link(QString::fromStdString(link.str()),
                       QString::fromStdString(target.str())))
        escape("symlink failed: %s -> %s", link.str().c_str(),
               target.str().c_str());
#else
# error "unsupported platform"
#endif

      return StatusOk;
    }

    ///
    /// this method erases the given link path.
    ///
    Status              Link::Erase(const Path&                 path)
    {
      // does the link exist.
      if (Link::Exist(path) == StatusFalse)
        escape("the link does not seem to exist");

      // unlink the link.
      ::unlink(path.str().c_str());

      return StatusOk;
    }

    ///
    /// this method returns true if the pointed to link exists.
    ///
    Status              Link::Exist(const Path&                 path)
    {
      struct ::stat             stat;

#if defined(INFINIT_LINUX) || defined(INFINIT_MACOSX)
      // does the path points to something.
      if (::lstat(path.str().c_str(), &stat) != 0)
        return StatusFalse;

      // does the path points to a regular file.
      if (!S_ISLNK(stat.st_mode))
        return StatusFalse;
#elif defined(INFINIT_WINDOWS)
      // does the path points to something.
      if (::stat(path.str().c_str(), &stat) != 0)
        return StatusFalse;
#else
# error "unsupported platform"
#endif

      return StatusTrue;
    }

    ///
    /// this method takes a path to a link and creates, if necessary,
    /// the intermediate directory leading to the file.
    ///
    Status              Link::Dig(const Path&                   path)
    {
      String            target(path.str());
      char *            tmp_str = ::strdup(path.str().c_str());
      String            directory(::dirname(tmp_str));
      std::stringstream stream(directory);
      String            item;
      Path              chemin;

      // free the temporary string used for directory
      free(tmp_str);

      // go through the components of the path.
      while (std::getline(stream, item, System::Path::Separator))
        {
          // update the intermediate chemin.
          if (chemin.str().empty() && item.empty())
            chemin.str() = System::Path::Separator;
          else
            {
              chemin.str().append(item);
              chemin.str().append(1, System::Path::Separator);
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

      return StatusOk;
    }

  }
}
