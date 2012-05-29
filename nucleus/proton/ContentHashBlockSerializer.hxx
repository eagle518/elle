#ifndef  NUCLEUS_PROTON_CONTENTHASHBLOCKSERIALIZER_HXX
# define NUCLEUS_PROTON_CONTENTHASHBLOCKSERIALIZER_HXX

# include <nucleus/proton/ContentHashBlock.hh>

# include <nucleus/proton/ImmutableBlockSerializer.hxx>

ELLE_SERIALIZE_SIMPLE(nucleus::proton::ContentHashBlock,
                      archive,
                      value,
                      version)
{
  assert(version == 0);
  archive & static_cast<nucleus::proton::ImmutableBlock&>(value);
  if (value.family != nucleus::proton::FamilyContentHashBlock)
    throw std::runtime_error("Invalid family");
}

#endif

