# ATLAS Programming guidelines

---

## Last updated: `Tue 1 Aug. 07:20`

---

# Table of contents

1. [Compiler specification](#compiler-specification)
    1. [Warning flags](#i-warning-flags)
    2. [codegen-flags](#ii-codegen-flags)
2. [Style](#style)
    1. [Naming style](#naming-style)
3. [Programing Rules](#programming-rules)
    1. [General notes](#general-notes)
    2. [Init style](#init-style)

---

# Compiler specification

This is written under the assumption that gcc is used. Other compilers (while not supported) can be used with a mirrored warning setup (Try to mirror all warnings that are shown here)

---

## i. Warning flags

``-Wall -Wextra -Wundef -Wnull-dereference -Wpedantic -pedantic-errors -Weffc++ -Wnoexcept -Wuninitialized -Wunused -Wunused-parameter -Winit-self -Wconversion -Wuseless-cast -Wextra-semi -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override -Wformat-signedness -Wno-format-zero-length -Wmissing-include-dirs -Wshift-overflow=2 -Walloc-zero -Walloca -Wsign-promo -Wconversion -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow -Wshadow=local -Wmultiple-inheritance -Wvirtual-inheritance -Wno-virtual-move-assign -Wunsafe-loop-optimizations -Wnormalized -Wpacked -Wredundant-decls -Wmismatched-tags -Wredundant-tags -Wctor-dtor-privacy -Wdeprecated-copy-dtor -Wstrict-null-sentinel -Wold-style-cast -Woverloaded-virtual -Wzero-as-null-pointer-constant -Wconditionally-supported -Werror=pedantic -Wwrite-strings -Wmultiple-inheritance -Wunused-const-variable=2 -Wdouble-promotion -Wpointer-arith -Wcast-align=strict -Wcast-qual -Wconversion -Wsign-conversion -Wimplicit-fallthrough=1 -Wmisleading-indentation -Wdangling-else -Wdate-time -Wformat=2 -Wformat-overflow=2 -Wformat-signedness -Wformat-truncation=2 -Wswitch-default -Wswitch-enum -Wstrict-overflow=5 -Wstringop-overflow=4 -Warray-bounds=2 -Wattribute-alias=2 -Wcatch-value=2 -Wplacement-new=2 -Wtrampolines -Winvalid-imported-macros -Winvalid-imported-macros``
> Note: These flags should be used within reason. If unable to use due to weird constraints then they can be SELECTIVELY disabled. Reasoning must be given and verified for the reason of being disabled.

---

## ii. Codegen flags

## Debug

``-Og -g -fstrict-aliasing -fno-omit-frame-pointer -fstack-check -ftrapv -fwrapv -fverbose-asm -femit-class-debug-always``
> * Recomended `-fanalyzer` should be used where possible

## System Release

``-DNDEBUG -Ofast -march=native -fgcse-las -fgcse-sm -fdeclone-ctor-dtor -fdevirtualize-speculatively -fdevirtualize-at-ltrans -ftree-loop-im -fivopts -ftree-loop-ivcanon -fira-hoist-pressure -fsched-pressure -fsched-spec-load -fipa-pta -flto=auto -s -ffat-lto-objects -fno-enforce-eh-specs -fstrict-enums``
> * Only should be run on the same system used for compilation

## Release

``-DNDEBUG -Ofast -fdeclone-ctor-dtor -flto=auto -s``
> * Default unless running on the same system as compilation (See System Release*)

## RelWithDebug

``-Ofast -march=native -g -fstrict-aliasing -fno-omit-frame-pointer -fstack-check -ftrapv -fwrapv -fverbose-asm -femit-class-debug-always``

---

## iii. Configuration

`-fmax-errors=3`: Whatever value you want set

`-Wpadded`: Disable if impossible to fix

`-fconcepts-diagnostics-depth=4`: If you do crazy template stuff and are debugging it you'll want this higher possibly.

# Style

## Naming Style

|                   | case type  | extra notes |
|-------------------|------------|-------------|
| functions/methods | cammelCase |             |
| class/struct      | PascalCase |             |
| variables         | snake_case |             |
| lambdas           | cammelCase |             |

## Variable names

Variables in a class should be prefixed with `m_`. Variables outside of a class are exempt from this. Variables within a `struct` with no methods are exempt from this. (POD)

# Programming rules

## General notes

- Avoid C style code at all costs (C style memory allocation and C style casts). Prefer using things like `static_cast`
- Avoid raw pointers at all cost. Use proper wrappers like `std::unique_ptr` and `std::shared_ptr`.
- Follow the Core CPP guidelines where possible.
- Const where possible. If the value isn't changed then it should be const.

## Init style

An init style like the following should be used `int my_int { 4 };` This is to assist with readability. As it can safely be guaranteed that `my_int` is created here in ANY situation at a quick glance.

---

# Nobleese Oblige

#### Thank you for your continued service.