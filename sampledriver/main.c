/*
 * SampleDriver [mruby]
 * This file is a sample driver of MVM.
 *
 * This sample driver [mruby] do:
 *   - creats (argc-1) VMs.
 *   - lets each VM run ruby script specified by argv[i]
 *     in parallel.
 *   - exit sample driver when all VMs are terminated.
 *
 * Terminology:
 *   - Driver: A generator/controller/manager of VMs with Ruby MVM C API.
 *   - Virtual Machine (VM): VM executs ruby program which is passed by Driver.
 *   - Ruby MVM C API: APIs for managing MVMs, which includes generation,
 *     sending events, sending messages and termination.
 */

#include <ruby/mvm.h>

/* mruby main */

int
main(int argc, char *argv[])
{
    int i, n = argc - 1;
    rb_vm_t **vms = alloca(sizeof(rb_vm_t *) * n);

    ruby_sysinit(&argc, &argv); /* process level initialize */

    /* mruby a.rb b.rb c.rb */
    /* => run parallel as:
     *    ruby a.rb & ruby b.rb & ruby c.rb
     */
    
    for (i=0; i<n; i++) {
	rb_vm_t *vm;
	rb_vm_attr_t *attr;
	static void IntiVM_mruby(rb_vm_t *vm);

	/* set option */
	attr = ruby_vm_attr_create(1, &argv[i+1]); /* initaialize with arguments */

	ruby_vm_attr_add_initializ_function(attr, InitVM_mruby);
	ruby_vm_attr_own_timer(attr, 1);
	ruby_vm_attr_create_thread(attr, 1); /* default */
	ruby_vm_attr_add_expression(attr, "p true");
	ruby_vm_attr_add_option(attr, "baz");
	ruby_vm_attr_set_verbose(attr, 1);
	ruby_vm_attr_set_debug(attr, 1);

	/* set stdin, out, err */
	ruby_vm_attr_set_stdin(attr, 0);  /* default */
	ruby_vm_attr_set_stdout(attr, 1); /* default */
	ruby_vm_attr_set_stderr(attr, 2); /* default */

	/* create and invoke virtual machine instance */
	vm = vms[i] = ruby_vm_new(attr);

	ruby_vm_attr_destruct(attr);
    }

    for (i=0; i<n; i++) {
	int err = rb_vm_join(vms[i]);
	printf("VM %d was terminated with error code %d.\n", i, err);
    }

    return 0;
}

#include <ruby/ruby.h>

static void
IntiVM_mruby(rb_vm_t *vm)
{
    rb_define_method(...);
}

