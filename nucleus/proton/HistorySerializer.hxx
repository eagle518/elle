#ifndef  NUCLEUS_PROTON_HISTORYSERIALIZER_HXX
# define NUCLEUS_PROTON_HISTORYSERIALIZER_HXX

# include <cassert>

# include <elle/serialize/VectorSerializer.hxx>
# include <nucleus/proton/VersionSerializer.hxx>

# include <nucleus/proton/History.hh>

ELLE_SERIALIZE_SIMPLE(nucleus::proton::History,
                      archive,
                      value,
                      version)
{
  assert(version == 0);

  archive & value.container;
}

#endif
