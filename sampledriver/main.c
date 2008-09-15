/*
 * SampleDriver [mruby]
 * This file is a sample driver of MVM.
 *
 * $Author$
 * 
 *
 * This sample includes two example of VM treatment.
 *   - Simple Driver: same as ruby interpreter.
 *   - MultiVM Driver: Making Multi-VM example.
 *
 * Simple Driver do:
 *   - Simple Ruby interpreter.
 *   - Make and Run a VM in synchronous.
 *
 * MultiVM Driver do:
 *   - Creats (argc-1) VMs.
 *   - Set extra initialize options.
 *   - Invoke VMs in parallel.
 *     They run scripts specified by argv[i].
 *   - Exit the driver when all VMs are terminated.
 *
 * Terminology:
 *   - Driver: A generator/controller/manager of VMs with Ruby MVM C API.
 *   - Virtual Machine (VM): VM executs ruby program which is passed by Driver.
 *   - Ruby MVM C API: APIs for managing MVMs, which includes generation,
 *     sending events, sending messages and termination.
 */

#include <stdio.h>
#include <ruby/mvm.h>

/* simple interpreter */
static int
simple_driver_example(int argc, const char *argv[])
{
    rb_vm_t *vm;
    int err;

    /* create VM with command line options */
    vm = ruby_vm_new(argc-1, argv+1);

    /* invoke this VM in current thread (synchronous) */
    err = ruby_vm_run(vm);
    /* NOTE: VM execution was terminated */

    /* destruct this VM */
    err = ruby_vm_destruct(vm);

    return err;
}

/* multivm driver */
static int
multivm_driver_example(int argc, const char *argv[])
{
    int i, n = argc - 1;
    rb_vm_t **vms = alloca(sizeof(rb_vm_t *) * n);

    /* mruby a.rb b.rb c.rb */
    /* => run parallel as:
     *    ruby a.rb & ruby b.rb & ruby c.rb
     */

    for (i=0; i<n; i++) {
	int err;
	rb_vm_t *vm;
	static void InitVM_mruby(rb_vm_t *vm);

	/* create VM with command line arguments */
	vm = vms[i] = ruby_vm_new(0, 0);

	/* set options */
	ruby_vm_init_add_argv(vm, "baz");           /* push an arg to ARGV */
	ruby_vm_init_add_library(vm, "fileutils");  /* -r fileutils */
	ruby_vm_init_add_library_path(vm, "./lib"); /* -I ./lib */

	if (1) {
	    /* NOTE: you can't specify -e and script simultaneously */
	    ruby_vm_init_add_expression(vm, "p 42"); /* -e 'p 42' */
	    ruby_vm_init_add_expression(vm, "exit"); /* -e 'exit' */
	}
	else {
	    ruby_vm_init_script(vm, argv[i+1]);      /* script file ($0) */
	}

	ruby_vm_init_verbose(vm, 1);             /* -v */
	ruby_vm_init_debug(vm, 1);               /* -d */

	/* set additional options */

	/* ruby_vm_init_compile_option(...); // compile option */

	/* additonal initializer provided by the driver */
	ruby_vm_init_add_initializer(vm, "mruby", InitVM_mruby);

	/* set stdin, out, err */
	ruby_vm_init_stdin(vm, 0);  /* default */
	ruby_vm_init_stdout(vm, 1); /* default */
	ruby_vm_init_stderr(vm, 2); /* default */

	/* invoke VM in another thread (asynchronous) */
	err = ruby_vm_start(vm);

	if (err != 0) {
	    fprintf(stderr, "VM invoke error (%d).\n", err);
	}
    }

    for (i=0; i<n; i++) {
	int err;

	/* wait VM termination and get error code */
	err = rb_vm_join(vms[i]);
	fprintf(stderr, "VM %d was terminated with error code %d.\n", i, err);

	/* destruct VM vm */
	ruby_vm_destruct(vm);
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    RUBY_INITSTACK();
    ruby_sysinit(&argc, &argv); /* process level initialize */

    if (1) {
	return simple_driver_example(argc, argv);
    }
    else {
	return multivm_driver_example(argc, argv);
    }
}

#include <ruby/ruby.h>

/* executed in each VM context */
static void
InitVM_mruby(rb_vm_t *vm)
{
    /* rb_define_method(...); */
}
