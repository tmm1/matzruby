/**********************************************************************

  main.c -

  $Author$
  created at: Fri Aug 19 13:19:58 JST 1994

  Copyright (C) 1993-2007 Yukihiro Matsumoto

**********************************************************************/

#undef RUBY_EXPORT

#include "ruby/vm.h"

#include "debug.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

RUBY_GLOBAL_SETUP

int
main(int argc, char **argv)
{
#ifdef RUBY_DEBUG_ENV
    ruby_set_debug_option(getenv("RUBY_DEBUG"));
#endif
#ifdef HAVE_LOCALE_H
    setlocale(LC_CTYPE, "");
#endif

    ruby_sysinit(&argc, &argv);
    {
	ruby_vm_t *vm;
	int ret;

	RUBY_INIT_STACK;
	vm = ruby_vm_new(argc, argv);
	ret = ruby_vm_run(vm);
	ruby_vm_destruct(vm);
	return ret;
    }
}
