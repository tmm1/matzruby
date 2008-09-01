/**********************************************************************

  ruby/mvm.h -

  $Author$
  created at: Sat May 31 15:17:36 2008

  Copyright (C) 2008 Yukihiro Matsumoto

**********************************************************************/

#ifndef RUBY_MVM_H
#define RUBY_MVM_H 1

#define HAVE_MVM 1

/* VM type declaration */
typedef struct rb_vm_struct rb_vm_t;

/* core API */
rb_vm_t *ruby_vm_new(int argc, char *argv[]);
int ruby_vm_run(rb_vm_t *vm);
int ruby_vm_start(rb_vm_t *vm);
int ruby_vm_join(rb_vm_t *vm);
int ruby_vm_destruct(rb_vm_t *vm);

/* initialize API */
int ruby_vm_init_add_argv(rb_vm_t *vm, const char *arg);
int ruby_vm_init_add_library(rb_vm_t *vm, const char *lib);
int ruby_vm_init_add_library_path(rb_vm_t *vm, const char *path);
int ruby_vm_init_add_expression(rb_vm_t *vm, const char *expr);
int ruby_vm_init_script(rb_vm_t *vm, const char *script);
int ruby_vm_init_verbose(rb_vm_t *vm, int verbose_p);
int ruby_vm_init_debug(rb_vm_t *vm, int debug);
int ruby_vm_init_add_initializer(rb_vm_t *vm, void (*initializer)(rb_vm_t *));
int ruby_vm_init_stdin(rb_vm_t *vm, int fd);
int ruby_vm_init_stdout(rb_vm_t *vm, int fd);
int ruby_vm_init_stderr(rb_vm_t *vm, int fd);

/* other API */
void ruby_vm_foreach(int (*)(rb_vm_t *, void *), void *); /* returning false stops iteration */
void *ruby_vm_specific_ptr(rb_vm_t *, int);

#endif /* RUBY_MVM_H */
