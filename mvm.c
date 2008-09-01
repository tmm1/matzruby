/**********************************************************************

  mvm.c - Ruby/MVM C API

  $Author$
  created at: Sun Aug 31 22:27:03 2008

  Copyright (C) 2008 Koichi Sasada

**********************************************************************/

#include "ruby/mvm.h"
#include "ruby/ruby.h"
#include "vm_core.h"
#include "eval_intern.h"

static void vmmgr_add(rb_vm_t *vm);
static void vmmgr_del(rb_vm_t *vm);
static rb_vm_t *vm_new(int argc, char *argv[]);
static void vm_cleanup();

/* core API */

rb_vm_t *
ruby_vm_new(int argc, char *argv[])
{
    rb_vm_t *vm = vm_new(argc, argv);
    vmmgr_add(vm);

    return vm;
}

int
ruby_vm_run(rb_vm_t *vm)
{
    return 0;
}

int
ruby_vm_start(rb_vm_t *vm)
{
    rb_bug("unsupporeted");
    return 0;
}

int
ruby_vm_join(rb_vm_t *vm)
{
    rb_bug("unsupporeted");
    return 0;
}

int
ruby_vm_destruct(rb_vm_t *vm)
{
    vm_cleanup(vm);
    vmmgr_del(vm);
    return 0;
}


/* initialize API */

int
ruby_vm_init_add_argv(rb_vm_t *vm, const char *arg)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_add_library(rb_vm_t *vm, const char *lib)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_add_library_path(rb_vm_t *vm, const char *path)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_add_expression(rb_vm_t *vm, const char *expr)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_script(rb_vm_t *vm, const char *script)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_verbose(rb_vm_t *vm, int verbose_p)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_debug(rb_vm_t *vm, int debug)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_add_initializer(rb_vm_t *vm, void (*initializer)(rb_vm_t *))
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_stdin(rb_vm_t *vm, int fd)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_stdout(rb_vm_t *vm, int fd)
{
    rb_bug("not supported");
    return 1;
}

int
ruby_vm_init_stderr(rb_vm_t *vm, int fd)
{
    rb_bug("not supported");
    return 1;
}


/* specific key management API */

#define MVM_CRITICAL(lock, expr) do { \
    ruby_native_thread_lock(&(lock)); \
    {expr;} \
    ruby_native_thread_unlock(&(lock)); \
} while (0)

static struct {
    rb_thread_lock_t lock;
    int last;
} specific_key;

int
rb_vm_key_count(void)
{
    return specific_key.last;
}

int
rb_vm_key_create(void)
{
    int key;
    MVM_CRITICAL(specific_key.lock, {
	key = specific_key.last++;
    });
    return key;
}

void *
ruby_vm_specific_ptr(rb_vm_t *vm, int key)
{
    VALUE *ptr;
    long len;

    ptr = vm->specific_storage.ptr;
    len = vm->specific_storage.len;
    if (!ptr || len <= key) {
	long newlen = (key + 8) & ~7;
	ptr = realloc(ptr, sizeof(VALUE) * newlen);
	vm->specific_storage.ptr = ptr;
	vm->specific_storage.len = newlen;
	MEMZERO(ptr + len, VALUE, newlen - len);
    }
    return &ptr[key];
}

VALUE *
rb_vm_specific_ptr(int key)
{
    return ruby_vm_specific_ptr(GET_VM(), key);
}

/* VM management */

static struct {
    rb_thread_lock_t lock;
    struct vm_list_struct {
	struct vm_list_struct *next;
	rb_vm_t *vm;
    } *list;
    rb_vm_t *main;
} vm_manager;

static void
vmmgr_initialize(rb_vm_t *vm)
{
    /* vm_manager */
    ruby_native_thread_lock_initialize(&vm_manager.lock);
    vm_manager.main = vm;

    /* specific_key */
    ruby_native_thread_lock_initialize(&specific_key.lock);
    specific_key.last = (ruby_builtin_object_count + 8) & ~7;
}

static void
vmmgr_add(rb_vm_t *vm)
{
    struct vm_list_struct *entry =
      (struct vm_list_struct *)malloc(sizeof(struct vm_list_struct));

    entry->vm = vm;

    if (vm_manager.main == 0) {
	vmmgr_initialize(vm);
    }

    MVM_CRITICAL(vm_manager.lock, {
	entry->next = vm_manager.list;
	vm_manager.list = entry;
    });
}

static void
vmmgr_del(rb_vm_t *vm)
{
    struct vm_list_struct *entry, *prev = 0;

    for (entry = vm_manager.list;
	 entry && entry->vm != vm;
	 prev = entry, entry = entry->next) {
	/* nop */
    }

    if (entry) {
	MVM_CRITICAL(vm_manager.lock, {
	    if (entry == vm_manager.list) {
		vm_manager.list = entry->next;
	    }
	    else {
		prev->next = entry->next;
	    }
	});
    }
    free(entry);
}

void
ruby_vm_foreach(int (*func)(rb_vm_t *, void *), void *arg)
{
    if (vm_manager.list) {
	MVM_CRITICAL(vm_manager.lock, {
	    struct vm_list_struct *entry = vm_manager.list;

	    while (entry) {
		if (func(entry->vm, arg) == 0) {
		    break;
		}
	    }
	});
    }
}

static rb_vm_t *
vm_new(int argc, char *argv[])
{
    /* place holder */
    return 0;
}

static void
vm_cleanup()
{
    /* place holder */
}
