//
// ---------- header ----------------------------------------------------------
//
// project       elle
//
// license       infinit
//
// file          /home/mycure/infinit/elle/test/fiber/Test.cc
//
// created       julien quintard   [wed jan 28 11:22:24 2009]
// updated       julien quintard   [sun jun 19 17:28:17 2011]
//

//
// ---------- includes --------------------------------------------------------
//

#include "Test.hh"

namespace elle
{
  namespace test
  {

//
// ---------- definitions -----------------------------------------------------
//

    Resource		Test::ResourceA;
    Resource		Test::ResourceB;
    Resource		Test::ResourceC;

    Timer		Test::Timer1;
    Timer		Test::Timer2;
    Timer		Test::Timer3;

//
// ---------- functions -------------------------------------------------------
//

    ///
    /// this function, invoked through in a fiber by Fiber3, simply awakens
    /// fibers waiting for ResourceA.
    ///
    Status		Fiber4()
    {
      enter();

      printf("[Fiber4] Start\n");

      printf("[Fiber4] Sleep()\n");

      // sleep.
      if (Fiber::Sleep(3000) == StatusError)
	escape("unable to sleep");

      printf("[Fiber4] /Sleep()\n");

      printf("[Fiber4] Awaken(ResourceA)\n");

      // awaken ResourceA.
      if (Fiber::Awaken(&Test::ResourceA) == StatusError)
	escape("unable to awaken the fiber");

      printf("[Fiber4] /Awaken(ResourceA)\n");

      printf("[Fiber4] End\n");

      leave();
    }

    ///
    /// finally, this function, also launched in a fiber, performs the
    /// following steps: spawn a new fiber, launching Fiber4, and
    /// and wait for the ResourceC.
    ///
    Status		Fiber3()
    {
      Callback< Parameters<> >	fiber4(&Fiber4);
      Closure< Parameters<> >	closure(fiber4);

      enter();

      printf("[Fiber3] Start\n");

      printf("[Fiber3] Spawn(Fiber4)\n");

      // spawn a new fiber.
      if (Fiber::Spawn(closure) == StatusError)
	escape("unable to spawn the fiber");

      printf("[Fiber3] /Spawn(Fiber4)\n");

      printf("[Fiber3] Wait(ResourceC)\n");

      // wait ResourceC.
      if (Fiber::Wait(&Test::ResourceC) == StatusError)
	escape("unable to wait for the resource");

      printf("[Fiber3] /Wait(ResourceC)\n");

      printf("[Fiber3] End\n");

      // exit.
      if (Program::Exit() == StatusError)
	escape("unable to exit from the program");

      leave();
    }

    ///
    /// this function is launched in a fiber and performs the following steps:
    /// wait for the ResourceB, then awaken fibers waiting for the
    /// ResourceC.
    ///
    Status		Fiber2()
    {
      enter();

      printf("[Fiber2] Start\n");

      printf("[Fiber2] Wait(ResourceB)\n");

      // wait for ResourceB.
      if (Fiber::Wait(&Test::ResourceB) == StatusError)
	escape("unable to wait for the resource");

      printf("[Fiber2] /Wait(ResourceB)\n");

      printf("[Fiber2] Awaken(ResourceC)\n");

      // awaken ResourceC.
      if (Fiber::Awaken(&Test::ResourceC) == StatusError)
	escape("unable to awaken the resource");

      printf("[Fiber2] /Awaken(ResourceC)\n");

      printf("[Fiber2] End\n");

      leave();
    }

    ///
    /// this function is launched in a fiber and performs the following steps:
    /// wait for the ResourceA, then awaken fibers waiting for the
    /// ResourceB.
    ///
    Status		Fiber1()
    {
      enter();

      printf("[Fiber1] Start\n");

      printf("[Fiber1] Wait(ResourceA)\n");

      // wait for ResourceA.
      if (Fiber::Wait(&Test::ResourceA) == StatusError)
	escape("unable to wait for the resource");

      printf("[Fiber1] /Wait(ResourceA)\n");

      printf("[Fiber1] Awaken(ResourceB)\n");

      // awaken ResourceB.
      if (Fiber::Awaken(&Test::ResourceB) == StatusError)
	escape("unable to awaken the resource");

      printf("[Fiber1] /Awaken(ResourceB)\n");

      printf("[Fiber1] End\n");

      leave();
    }

    ///
    /// the main function.
    ///
    Status		Main(const Natural32			argc,
			     const Character*			argv[])
    {
      Callback< Parameters<> >	fiber1(&Fiber1);
      Callback< Parameters<> >	fiber2(&Fiber2);
      Callback< Parameters<> >	fiber3(&Fiber3);

      enter();

      // initialize the library.
      if (Elle::Initialize() == StatusError)
	escape("unable to initialize the library");

      // setup the program.
      if (Program::Setup() == StatusError)
	escape("unable to set up the program");

      // create the timer1.
      if (Test::Timer1.Create(Timer::ModeSingle, fiber1) == StatusError)
	escape("unable to create the timer");

      // start the timer1, launching the fiber1.
      if (Test::Timer1.Start(100) == StatusError)
	escape("unable to start the timer");

      // create the timer2.
      if (Test::Timer2.Create(Timer::ModeSingle, fiber2) == StatusError)
	escape("unable to create the timer");

      // start the timer2, launching the fiber2.
      if (Test::Timer2.Start(1000) == StatusError)
	escape("unable to start the timer");

      // create the timer3.
      if (Test::Timer3.Create(Timer::ModeSingle, fiber3) == StatusError)
	escape("unable to create the timer");

      // start the timer3, launching the fiber3.
      if (Test::Timer3.Start(10000) == StatusError)
	escape("unable to start the timer");

      // launch the program.
      if (Program::Launch() == StatusError)
	escape("an error occured in the program");

      // clean the library.
      if (Elle::Clean() == StatusError)
	escape("unable to clean the library");

      leave();
    }

  }
}

//
// ---------- main ------------------------------------------------------------
//

int			main(const int				argc,
			     const char*			argv[])
{
  if (elle::test::Main(argc, argv) == elle::radix::StatusError)
    {
      show();

      return (1);
    }

  std::cout << "[success]" << std::endl;

  return (0);
}
