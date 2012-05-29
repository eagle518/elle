#ifndef ELLE_CONCURRENCY_EVENT_HH
# define ELLE_CONCURRENCY_EVENT_HH

# include <elle/core/Natural.hh>
# include <elle/core/Boolean.hh>
# include <elle/radix/Object.hh>

# include <elle/types.hh>
# include <elle/serialize/fwd.hh>
# include <elle/radix/Object.hh>

# include <elle/idiom/Close.hh>
#  include <openssl/err.h>
#  include <reactor/signal.hh>
# include <elle/idiom/Open.hh>

namespace elle
{

  using namespace radix;

  // XXX
  namespace network
  {
    class Parcel;
  }

  namespace concurrency
  {

//
// ---------- classes ---------------------------------------------------------
//

    ///
    /// this class is used to uniquely identify events, network packets and
    /// so on.
    ///
    class Event
      : public Object
    {
    public:
      //
      // constants
      //
      static const Event        Null;

      //
      // constructors & destructors
      //
      Event();

      //
      // methods
      //
      Status            Generate();
      void              Cleanup();

      //
      // interfaces
      //

      // object
      declare(Event);
      Boolean           operator==(const Event&) const;
      Boolean           operator<(const Event&) const;

      void            XXX_OLD_Extract();

      // dumpable
      Status            Dump(const Natural32 = 0) const;

      //
      // attributes
      //
      Natural64 Identifier() const;

    public:
      typedef reactor::VSignal<elle::network::Parcel*> SignalType;
      SignalType& Signal();
    private:
      Natural64                       _identifier;
      SignalType*                     _signal;

      ELLE_SERIALIZE_FRIEND_FOR(Event);
    };

  }
}

#endif
