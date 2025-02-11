#!/usr/bin/env arken

local arg1 = ( arg[1] and os.exists(arg[1]) ) and arg[1] or "src/arken"
local verbose = (arg[1] == '--verbose' or arg[2] == '--verbose' ) and true or false

-- Enabled checks:
local options = {
-- android-cloexec-accept
-- android-cloexec-accept4
-- android-cloexec-creat
-- android-cloexec-dup
-- android-cloexec-epoll-create
-- android-cloexec-epoll-create1
-- android-cloexec-fopen
-- android-cloexec-inotify-init
-- android-cloexec-inotify-init1
-- android-cloexec-memfd-create
-- android-cloexec-open
-- android-cloexec-socket
-- boost-use-to-string
-- bugprone-argument-comment
-- bugprone-assert-side-effect
-- bugprone-bool-pointer-implicit-conversion
-- bugprone-copy-constructor-init
-- bugprone-dangling-handle
-- bugprone-fold-init-type
-- bugprone-forward-declaration-namespace
-- bugprone-inaccurate-erase
-- bugprone-integer-division
-- bugprone-misplaced-operator-in-strlen-in-alloc
-- bugprone-move-forwarding-reference
-- bugprone-multiple-statement-macro
-- bugprone-string-constructor
-- bugprone-suspicious-memset-usage
-- bugprone-undefined-memory-manipulation
-- bugprone-use-after-move
-- bugprone-virtual-near-miss
-- cert-dcl03-c
-- cert-dcl21-cpp
-- cert-dcl50-cpp
-- cert-dcl54-cpp
-- cert-dcl58-cpp
-- cert-dcl59-cpp
-- cert-env33-c
-- cert-err09-cpp
-- cert-err34-c
-- cert-err52-cpp
-- cert-err58-cpp
-- cert-err60-cpp
-- cert-err61-cpp
-- cert-fio38-c
-- cert-flp30-c
-- cert-msc30-c
-- cert-msc50-cpp
-- cert-oop11-cpp
-- clang-analyzer-apiModeling.google.GTest
-- clang-analyzer-core.CallAndMessage
-- clang-analyzer-core.DivideZero
-- clang-analyzer-core.DynamicTypePropagation
-- clang-analyzer-core.NonNullParamChecker
-- clang-analyzer-core.NonnilStringConstants
-- clang-analyzer-core.NullDereference
-- clang-analyzer-core.StackAddressEscape
-- clang-analyzer-core.UndefinedBinaryOperatorResult
-- clang-analyzer-core.VLASize
-- clang-analyzer-core.builtin.BuiltinFunctions
-- clang-analyzer-core.builtin.NoReturnFunctions
-- clang-analyzer-core.uninitialized.ArraySubscript
-- clang-analyzer-core.uninitialized.Assign
-- clang-analyzer-core.uninitialized.Branch
-- clang-analyzer-core.uninitialized.CapturedBlockVariable
-- clang-analyzer-core.uninitialized.UndefReturn
-- clang-analyzer-cplusplus.NewDelete
-- clang-analyzer-cplusplus.NewDeleteLeaks
-- clang-analyzer-cplusplus.SelfAssignment
-- clang-analyzer-deadcode.DeadStores
-- clang-analyzer-llvm.Conventions
-- clang-analyzer-nullability.NullPassedToNonnull
-- clang-analyzer-nullability.NullReturnedFromNonnull
-- clang-analyzer-nullability.NullableDereferenced
-- clang-analyzer-nullability.NullablePassedToNonnull
-- clang-analyzer-nullability.NullableReturnedFromNonnull
-- clang-analyzer-optin.cplusplus.VirtualCall
-- clang-analyzer-optin.mpi.MPI-Checker
-- clang-analyzer-optin.osx.cocoa.localizability.EmptyLocalizationContextChecker
-- clang-analyzer-optin.osx.cocoa.localizability.NonLocalizedStringChecker
-- clang-analyzer-optin.performance.Padding
-- clang-analyzer-optin.portability.UnixAPI
-- clang-analyzer-osx.API
-- clang-analyzer-osx.NumberObjectConversion
-- clang-analyzer-osx.ObjCProperty
-- clang-analyzer-osx.SecKeychainAPI
-- clang-analyzer-osx.cocoa.AtSync
-- clang-analyzer-osx.cocoa.ClassRelease
-- clang-analyzer-osx.cocoa.Dealloc
-- clang-analyzer-osx.cocoa.IncompatibleMethodTypes
-- clang-analyzer-osx.cocoa.Loops
-- clang-analyzer-osx.cocoa.MissingSuperCall
-- clang-analyzer-osx.cocoa.NSAutoreleasePool
-- clang-analyzer-osx.cocoa.NSError
-- clang-analyzer-osx.cocoa.NilArg
-- clang-analyzer-osx.cocoa.NonNilReturnValue
-- clang-analyzer-osx.cocoa.ObjCGenerics
-- clang-analyzer-osx.cocoa.RetainCount
-- clang-analyzer-osx.cocoa.SelfInit
-- clang-analyzer-osx.cocoa.SuperDealloc
-- clang-analyzer-osx.cocoa.UnusedIvars
-- clang-analyzer-osx.cocoa.VariadicMethodTypes
-- clang-analyzer-osx.coreFoundation.CFError
-- clang-analyzer-osx.coreFoundation.CFNumber
-- clang-analyzer-osx.coreFoundation.CFRetainRelease
-- clang-analyzer-osx.coreFoundation.containers.OutOfBounds
-- clang-analyzer-osx.coreFoundation.containers.PointerSizedValues
-- clang-analyzer-security.FloatLoopCounter
-- clang-analyzer-security.insecureAPI.UncheckedReturn
-- clang-analyzer-security.insecureAPI.getpw
-- clang-analyzer-security.insecureAPI.gets
-- clang-analyzer-security.insecureAPI.mkstemp
-- clang-analyzer-security.insecureAPI.mktemp
-- clang-analyzer-security.insecureAPI.rand
-- clang-analyzer-security.insecureAPI.strcpy
-- clang-analyzer-security.insecureAPI.vfork
-- clang-analyzer-unix.API
-- clang-analyzer-unix.Malloc
-- clang-analyzer-unix.MallocSizeof
-- clang-analyzer-unix.MismatchedDeallocator
-- clang-analyzer-unix.StdCLibraryFunctions
-- clang-analyzer-unix.Vfork
-- clang-analyzer-unix.cstring.BadSizeArg
-- clang-analyzer-unix.cstring.NullArg
-- clang-analyzer-valist.CopyToSelf
-- clang-analyzer-valist.Uninitialized
-- clang-analyzer-valist.Unterminated
-- cppcoreguidelines-c-copy-assignment-signature
-- cppcoreguidelines-interfaces-global-init
  'cppcoreguidelines-no-malloc',
-- cppcoreguidelines-owning-memory
-- cppcoreguidelines-pro-bounds-array-to-pointer-decay
-- cppcoreguidelines-pro-bounds-constant-array-index
-- cppcoreguidelines-pro-bounds-pointer-arithmetic
  'cppcoreguidelines-pro-type-const-cast',
  'cppcoreguidelines-pro-type-cstyle-cast',
-- cppcoreguidelines-pro-type-member-init
-- cppcoreguidelines-pro-type-reinterpret-cast
-- cppcoreguidelines-pro-type-static-cast-downcast
-- cppcoreguidelines-pro-type-union-access
-- cppcoreguidelines-pro-type-vararg
-- cppcoreguidelines-slicing
-- cppcoreguidelines-special-member-functions
-- fuchsia-default-arguments
-- fuchsia-overloaded-operator
-- fuchsia-virtual-inheritance
-- google-build-explicit-make-pair
  'google-build-namespaces',
  'google-build-using-namespace',
-- google-default-arguments
-- google-explicit-constructor
-- google-global-names-in-headers
-- google-objc-avoid-throwing-exception
-- google-objc-global-variable-declaration
-- google-readability-braces-around-statements
-- google-readability-casting
-- google-readability-function-size
  'google-readability-namespace-comments',
-- google-readability-redundant-smartptr-get
-- google-readability-todo
-- google-runtime-int
-- google-runtime-member-string-references
-- google-runtime-operator
-- google-runtime-references
-- hicpp-braces-around-statements
-- hicpp-deprecated-headers
-- hicpp-exception-baseclass
-- hicpp-explicit-conversions
-- hicpp-function-size
-- hicpp-invalid-access-moved
-- hicpp-member-init
-- hicpp-move-const-arg
-- hicpp-named-parameter
-- hicpp-new-delete-operators
-- hicpp-no-array-decay
-- hicpp-no-assembler
-- hicpp-no-malloc
-- hicpp-noexcept-move
-- hicpp-signed-bitwise
-- hicpp-special-member-functions
-- hicpp-static-assert
-- hicpp-undelegated-constructor
-- hicpp-use-auto
-- hicpp-use-emplace
-- hicpp-use-equals-default
-- hicpp-use-equals-delete
-- hicpp-use-noexcept
-- hicpp-use-nullptr
-- hicpp-use-override
-- hicpp-vararg
-- llvm-header-guard
-- llvm-include-order
-- llvm-namespace-comment
-- llvm-twine-local
-- misc-definitions-in-headers
-- misc-forwarding-reference-overload
-- misc-incorrect-roundings
-- misc-lambda-function-name
-- misc-macro-parentheses
-- misc-macro-repeated-side-effects
-- misc-misplaced-const
-- misc-misplaced-widening-cast
-- misc-new-delete-overloads
-- misc-non-copyable-objects
-- misc-redundant-expression
-- misc-sizeof-container
-- misc-sizeof-expression
-- misc-static-assert
-- misc-string-compare
-- misc-string-integer-assignment
-- misc-string-literal-with-embedded-nul
-- misc-suspicious-enum-usage
-- misc-suspicious-missing-comma
-- misc-suspicious-semicolon
-- misc-suspicious-string-compare
-- misc-swapped-arguments
-- misc-throw-by-value-catch-by-reference
-- misc-unconventional-assign-operator
-- misc-undelegated-constructor
-- misc-uniqueptr-reset-release
-- misc-unused-alias-decls
-- misc-unused-parameters
-- misc-unused-raii
-- misc-unused-using-decls
-- modernize-avoid-bind
-- modernize-deprecated-headers
-- modernize-loop-convert
-- modernize-make-shared
-- modernize-make-unique
-- modernize-pass-by-value
-- modernize-raw-string-literal
-- modernize-redundant-void-arg
-- modernize-replace-auto-ptr
-- modernize-replace-random-shuffle
-- modernize-return-braced-init-list
-- modernize-shrink-to-fit
-- modernize-unary-static-assert
  'modernize-use-auto',
-- modernize-use-bool-literals
-- modernize-use-default-member-init
-- modernize-use-emplace
-- modernize-use-equals-default
-- modernize-use-equals-delete
-- modernize-use-noexcept
  'modernize-use-nullptr',
-- modernize-use-override
-- modernize-use-transparent-functors
  'modernize-use-using',
-- mpi-buffer-deref
-- mpi-type-mismatch
-- objc-avoid-nserror-init
-- objc-avoid-spinlock
-- objc-forbidden-subclassing
-- objc-property-declaration
-- performance-faster-string-find
-- performance-for-range-copy
-- performance-implicit-conversion-in-loop
-- performance-inefficient-algorithm
-- performance-inefficient-string-concatenation
-- performance-inefficient-vector-operation
-- performance-move-const-arg
-- performance-move-constructor-init
-- performance-noexcept-move-constructor
-- performance-type-promotion-in-math-fn
-- performance-unnecessary-copy-initialization
-- performance-unnecessary-value-param
-- readability-avoid-const-params-in-decls
-- readability-braces-around-statements
-- readability-container-size-empty
-- readability-delete-null-pointer
-- readability-deleted-default
-- readability-else-after-return
-- readability-function-size
-- readability-identifier-naming
-- readability-implicit-bool-conversion
-- readability-inconsistent-declaration-parameter-name
-- readability-misleading-indentation
-- readability-misplaced-array-index
-- readability-named-parameter
-- readability-non-const-parameter
-- readability-redundant-control-flow
-- readability-redundant-declaration
-- readability-redundant-function-ptr-dereference
-- readability-redundant-member-init
-- readability-redundant-smartptr-get
-- readability-redundant-string-cstr
-- readability-redundant-string-init
-- readability-simplify-boolean-expr
-- readability-static-accessed-through-instance
-- readability-static-definition-in-anonymous-namespace
-- readability-uniqueptr-delete-release
}

local checks = table.concat(options, ', ')

local tidy = [[
clang-tidy-6.0 %s \
  -header-filter=arken/.* \
  -checks='%s' \
  -- \
  -std=c++11 \
  -I src/vendors/mongrel2 \
  -I src/embedded \
  -I src/lua/json \
  -I include \
  -I deps/include \
  -I /opt/objectdata/Qt-5.7.0/include \
  -I /opt/objectdata/Qt-5.7.0/include/QtCore \
  -I /opt/objectdata/Qt-5.6.0/include/QtGui \
  -I /opt/objectdata/Qt-5.7.0/include/QtNetwork \
  -I /opt/objectdata/Qt-5.7.0/include/QtWidgets \
  -I /usr/include/glib-2.0 \
  -I /usr/lib/x86_64-linux-gnu/glib-2.0/include \
  -I /usr/include/gdk-pixbuf-2.0 \
  -I /usr/include/gdk-pixbuf-2.0/gdk-pixbuf \
  -fPIC \
   2> /dev/null
]]

local tidy = function(fileName)
  local cmd = string.format(tidy, fileName, checks)
  if verbose then
    print(cmd)
    print(fileName)
  end
  os.execute(cmd)
end

if os.isfile(arg1) then
  tidy(arg1)
else
  local list = os.find(arg1, "cpp$", true)
  for fileName in list:each() do
    tidy(fileName)
  end
end
