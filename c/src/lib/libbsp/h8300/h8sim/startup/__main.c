/*
 *  Copied from newlib 1.8.2
 *
 *  $Id: __main.c,v 1.2 2008/08/19 13:47:03 ralf Exp $
 */

void __main (void)
{
  static int initialized;
  if (! initialized)
    {
      typedef void (*pfunc) (void);
      extern pfunc __ctors[];
      extern pfunc __ctors_end[];
      pfunc *p;

      initialized = 1;
      for (p = __ctors_end; p > __ctors; )
	(*--p) ();

    }
}
