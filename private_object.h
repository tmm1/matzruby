#ifndef RUBY_PRIVATE_OBJECT_H
#define RUBY_PRIVATE_OBJECT_H 1

#include "ruby/public_object.h"

enum ruby_private_object_vmkey {
    ruby_private_object_vmkey_begin = ruby_public_object_count - 1,

    rb_errReenterError_vmkey,
#define rb_errReenterError (*rb_vm_specific_ptr(rb_errReenterError_vmkey))
    rb_errNoMemError_vmkey,
#define rb_errNoMemError (*rb_vm_specific_ptr(rb_errNoMemError_vmkey))
    rb_errSysStack_vmkey,
#define rb_errSysStack (*rb_vm_specific_ptr(rb_errSysStack_vmkey))

    /* ThreadGroup */
    /* Marshal */
    /* ObjectSpace */
    /* Signal */

    rb_vmkey_cISeq,
#define rb_cISeq (*rb_vm_specific_ptr(rb_vmkey_cISeq))
    rb_vmkey_cRubyVM,
#define rb_cRubyVM (*rb_vm_specific_ptr(rb_vmkey_cRubyVM))
    rb_vmkey_cEnv,
#define rb_cEnv (*rb_vm_specific_ptr(rb_vmkey_cEnv))
    rb_vmkey_mRubyVMFrozenCore,
#define rb_mRubyVMFrozenCore (*rb_vm_specific_ptr(rb_vmkey_mRubyVMFrozenCore))

    rb_vmkey_eNOERROR,
#define rb_eNOERROR (*rb_vm_specific_ptr(rb_vmkey_eNOERROR))
    rb_vmkey_mFConst,
#define rb_mFConst (*rb_vm_specific_ptr(rb_vmkey_mFConst))
    rb_vmkey_cProcessStatus,
#define rb_cProcessStatus (*rb_vm_specific_ptr(rb_vmkey_cProcessStatus))
    rb_vmkey_cProcessTms,
#define rb_cProcessTms (*rb_vm_specific_ptr(rb_vmkey_cProcessTms))
    rb_vmkey_cContinuation,
#define rb_cContinuation (*rb_vm_specific_ptr(rb_vmkey_cContinuation))
    rb_vmkey_cFiber,
#define rb_cFiber (*rb_vm_specific_ptr(rb_vmkey_cFiber))
    rb_vmkey_eFiberError,
#define rb_eFiberError (*rb_vm_specific_ptr(rb_vmkey_eFiberError))
    rb_vmkey_eMutex_OrphanLock,
#define rb_eMutex_OrphanLock (*rb_vm_specific_ptr(rb_vmkey_eMutex_OrphanLock))

    rb_vmkey_encoding_list,
#define rb_encoding_list (*rb_vm_specific_ptr(rb_vmkey_encoding_list))

    rb_vmkey_syserr_tbl,
#define rb_syserr_tbl (*rb_vm_specific_ptr(rb_vmkey_syserr_tbl))
    rb_vmkey_immediate_frozen_tbl,
#define rb_immediate_frozen_tbl (*rb_vm_specific_ptr(rb_vmkey_immediate_frozen_tbl))
    rb_vmkey_generic_iv_tbl,
#define rb_generic_iv_tbl (*rb_vm_specific_ptr(rb_vmkey_generic_iv_tbl))
    rb_vmkey_compat_allocator_tbl,
#define rb_compat_allocator_tbl (*rb_vm_specific_ptr(rb_vmkey_compat_allocator_tbl))

    rb_vmkey_coverages,
#define rb_vm_coverages (*rb_vm_specific_ptr(rb_vmkey_coverages))

    ruby_builtin_object_count,
    ruby_private_object_count = ruby_builtin_object_count - ruby_private_object_vmkey_begin + 1
};

#define rb_wrap_st_table(tbl) Data_Wrap_Struct(0, rb_mark_tbl, st_free_table, tbl)

#endif	/* RUBY_PRIVATE_OBJECT_H */
