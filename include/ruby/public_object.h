#ifndef RUBY_PUBLIC_OBJECT_H
#define RUBY_PUBLIC_OBJECT_H 1

#include "ruby/mvm.h"

enum ruby_public_object_vmkey {
    rb_vmkey_mKernel,
#define rb_mKernel (*rb_vm_specific_ptr(rb_vmkey_mKernel))
    rb_vmkey_mComparable,
#define rb_mComparable (*rb_vm_specific_ptr(rb_vmkey_mComparable))
    rb_vmkey_mEnumerable,
#define rb_mEnumerable (*rb_vm_specific_ptr(rb_vmkey_mEnumerable))
    rb_vmkey_mPrecision,
#define rb_mPrecision (*rb_vm_specific_ptr(rb_vmkey_mPrecision))
    rb_vmkey_mErrno,
#define rb_mErrno (*rb_vm_specific_ptr(rb_vmkey_mErrno))
    rb_vmkey_mFileTest,
#define rb_mFileTest (*rb_vm_specific_ptr(rb_vmkey_mFileTest))
    rb_vmkey_mGC,
#define rb_mGC (*rb_vm_specific_ptr(rb_vmkey_mGC))
    rb_vmkey_mMath,
#define rb_mMath (*rb_vm_specific_ptr(rb_vmkey_mMath))
    rb_vmkey_mProcess,
#define rb_mProcess (*rb_vm_specific_ptr(rb_vmkey_mProcess))

    rb_vmkey_cBasicObject,
#define rb_cBasicObject (*rb_vm_specific_ptr(rb_vmkey_cBasicObject))
    rb_vmkey_cObject,
#define rb_cObject (*rb_vm_specific_ptr(rb_vmkey_cObject))
    rb_vmkey_cArray,
#define rb_cArray (*rb_vm_specific_ptr(rb_vmkey_cArray))
    rb_vmkey_cBignum,
#define rb_cBignum (*rb_vm_specific_ptr(rb_vmkey_cBignum))
    rb_vmkey_cBinding,
#define rb_cBinding (*rb_vm_specific_ptr(rb_vmkey_cBinding))
    rb_vmkey_cClass,
#define rb_cClass (*rb_vm_specific_ptr(rb_vmkey_cClass))
    rb_vmkey_cCont,
#define rb_cCont (*rb_vm_specific_ptr(rb_vmkey_cCont))
    rb_vmkey_cDir,
#define rb_cDir (*rb_vm_specific_ptr(rb_vmkey_cDir))
    rb_vmkey_cData,
#define rb_cData (*rb_vm_specific_ptr(rb_vmkey_cData))
    rb_vmkey_cFalseClass,
#define rb_cFalseClass (*rb_vm_specific_ptr(rb_vmkey_cFalseClass))
    rb_vmkey_cEncoding,
#define rb_cEncoding (*rb_vm_specific_ptr(rb_vmkey_cEncoding))
    rb_vmkey_cEnumerator,
#define rb_cEnumerator (*rb_vm_specific_ptr(rb_vmkey_cEnumerator))
    rb_vmkey_cFile,
#define rb_cFile (*rb_vm_specific_ptr(rb_vmkey_cFile))
    rb_vmkey_cFixnum,
#define rb_cFixnum (*rb_vm_specific_ptr(rb_vmkey_cFixnum))
    rb_vmkey_cFloat,
#define rb_cFloat (*rb_vm_specific_ptr(rb_vmkey_cFloat))
    rb_vmkey_cHash,
#define rb_cHash (*rb_vm_specific_ptr(rb_vmkey_cHash))
    rb_vmkey_cInteger,
#define rb_cInteger (*rb_vm_specific_ptr(rb_vmkey_cInteger))
    rb_vmkey_cIO,
#define rb_cIO (*rb_vm_specific_ptr(rb_vmkey_cIO))
    rb_vmkey_cMatch,
#define rb_cMatch (*rb_vm_specific_ptr(rb_vmkey_cMatch))
    rb_vmkey_cMethod,
#define rb_cMethod (*rb_vm_specific_ptr(rb_vmkey_cMethod))
    rb_vmkey_cModule,
#define rb_cModule (*rb_vm_specific_ptr(rb_vmkey_cModule))
    rb_vmkey_cNameErrorMesg,
#define rb_cNameErrorMesg (*rb_vm_specific_ptr(rb_vmkey_cNameErrorMesg))
    rb_vmkey_cNilClass,
#define rb_cNilClass (*rb_vm_specific_ptr(rb_vmkey_cNilClass))
    rb_vmkey_cNumeric,
#define rb_cNumeric (*rb_vm_specific_ptr(rb_vmkey_cNumeric))
    rb_vmkey_cProc,
#define rb_cProc (*rb_vm_specific_ptr(rb_vmkey_cProc))
    rb_vmkey_cRange,
#define rb_cRange (*rb_vm_specific_ptr(rb_vmkey_cRange))
    rb_vmkey_cRational,
#define rb_cRational (*rb_vm_specific_ptr(rb_vmkey_cRational))
    rb_vmkey_cComplex,
#define rb_cComplex (*rb_vm_specific_ptr(rb_vmkey_cComplex))
    rb_vmkey_cRegexp,
#define rb_cRegexp (*rb_vm_specific_ptr(rb_vmkey_cRegexp))
    rb_vmkey_cStat,
#define rb_cStat (*rb_vm_specific_ptr(rb_vmkey_cStat))
    rb_vmkey_cString,
#define rb_cString (*rb_vm_specific_ptr(rb_vmkey_cString))
    rb_vmkey_cStruct,
#define rb_cStruct (*rb_vm_specific_ptr(rb_vmkey_cStruct))
    rb_vmkey_cSymbol,
#define rb_cSymbol (*rb_vm_specific_ptr(rb_vmkey_cSymbol))
    rb_vmkey_cThread,
#define rb_cThread (*rb_vm_specific_ptr(rb_vmkey_cThread))
    rb_vmkey_cTime,
#define rb_cTime (*rb_vm_specific_ptr(rb_vmkey_cTime))
    rb_vmkey_cTrueClass,
#define rb_cTrueClass (*rb_vm_specific_ptr(rb_vmkey_cTrueClass))
    rb_vmkey_cUnboundMethod,
#define rb_cUnboundMethod (*rb_vm_specific_ptr(rb_vmkey_cUnboundMethod))
    rb_vmkey_cMutex,
#define rb_cMutex (*rb_vm_specific_ptr(rb_vmkey_cMutex))
    rb_vmkey_cBarrier,
#define rb_cBarrier (*rb_vm_specific_ptr(rb_vmkey_cBarrier))

    rb_vmkey_eException,
#define rb_eException (*rb_vm_specific_ptr(rb_vmkey_eException))
    rb_vmkey_eStandardError,
#define rb_eStandardError (*rb_vm_specific_ptr(rb_vmkey_eStandardError))
    rb_vmkey_eSystemExit,
#define rb_eSystemExit (*rb_vm_specific_ptr(rb_vmkey_eSystemExit))
    rb_vmkey_eInterrupt,
#define rb_eInterrupt (*rb_vm_specific_ptr(rb_vmkey_eInterrupt))
    rb_vmkey_eSignal,
#define rb_eSignal (*rb_vm_specific_ptr(rb_vmkey_eSignal))
    rb_vmkey_eFatal,
#define rb_eFatal (*rb_vm_specific_ptr(rb_vmkey_eFatal))
    rb_vmkey_eArgError,
#define rb_eArgError (*rb_vm_specific_ptr(rb_vmkey_eArgError))
    rb_vmkey_eEOFError,
#define rb_eEOFError (*rb_vm_specific_ptr(rb_vmkey_eEOFError))
    rb_vmkey_eIndexError,
#define rb_eIndexError (*rb_vm_specific_ptr(rb_vmkey_eIndexError))
    rb_vmkey_eStopIteration,
#define rb_eStopIteration (*rb_vm_specific_ptr(rb_vmkey_eStopIteration))
    rb_vmkey_eKeyError,
#define rb_eKeyError (*rb_vm_specific_ptr(rb_vmkey_eKeyError))
    rb_vmkey_eRangeError,
#define rb_eRangeError (*rb_vm_specific_ptr(rb_vmkey_eRangeError))
    rb_vmkey_eIOError,
#define rb_eIOError (*rb_vm_specific_ptr(rb_vmkey_eIOError))
    rb_vmkey_eRuntimeError,
#define rb_eRuntimeError (*rb_vm_specific_ptr(rb_vmkey_eRuntimeError))
    rb_vmkey_eSecurityError,
#define rb_eSecurityError (*rb_vm_specific_ptr(rb_vmkey_eSecurityError))
    rb_vmkey_eSystemCallError,
#define rb_eSystemCallError (*rb_vm_specific_ptr(rb_vmkey_eSystemCallError))
    rb_vmkey_eThreadError,
#define rb_eThreadError (*rb_vm_specific_ptr(rb_vmkey_eThreadError))
    rb_vmkey_eTypeError,
#define rb_eTypeError (*rb_vm_specific_ptr(rb_vmkey_eTypeError))
    rb_vmkey_eZeroDivError,
#define rb_eZeroDivError (*rb_vm_specific_ptr(rb_vmkey_eZeroDivError))
    rb_vmkey_eNotImpError,
#define rb_eNotImpError (*rb_vm_specific_ptr(rb_vmkey_eNotImpError))
    rb_vmkey_eNoMemError,
#define rb_eNoMemError (*rb_vm_specific_ptr(rb_vmkey_eNoMemError))
    rb_vmkey_eNoMethodError,
#define rb_eNoMethodError (*rb_vm_specific_ptr(rb_vmkey_eNoMethodError))
    rb_vmkey_eFloatDomainError,
#define rb_eFloatDomainError (*rb_vm_specific_ptr(rb_vmkey_eFloatDomainError))
    rb_vmkey_eLocalJumpError,
#define rb_eLocalJumpError (*rb_vm_specific_ptr(rb_vmkey_eLocalJumpError))
    rb_vmkey_eSysStackError,
#define rb_eSysStackError (*rb_vm_specific_ptr(rb_vmkey_eSysStackError))
    rb_vmkey_eRegexpError,
#define rb_eRegexpError (*rb_vm_specific_ptr(rb_vmkey_eRegexpError))

    rb_vmkey_eScriptError,
#define rb_eScriptError (*rb_vm_specific_ptr(rb_vmkey_eScriptError))
    rb_vmkey_eNameError,
#define rb_eNameError (*rb_vm_specific_ptr(rb_vmkey_eNameError))
    rb_vmkey_eSyntaxError,
#define rb_eSyntaxError (*rb_vm_specific_ptr(rb_vmkey_eSyntaxError))
    rb_vmkey_eLoadError,
#define rb_eLoadError (*rb_vm_specific_ptr(rb_vmkey_eLoadError))

    rb_vmkey_stdin,
#define rb_stdin (*rb_vm_specific_ptr(rb_vmkey_stdin))
    rb_vmkey_stdout,
#define rb_stdout (*rb_vm_specific_ptr(rb_vmkey_stdout))
    rb_vmkey_stderr,
#define rb_stderr (*rb_vm_specific_ptr(rb_vmkey_stderr))
    rb_vmkey_argf,
#define rb_argf (*rb_vm_specific_ptr(rb_vmkey_argf))

    rb_vmkey_verbose,
    rb_vmkey_debug,
    rb_vmkey_progname,

    ruby_public_object_count
};

#endif	/* RUBY_PUBLIC_OBJECT_H */
