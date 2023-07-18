

To build on linux it's pretty simple compared to windows.
## Important note
We are a bleeding edge build. We use c++20 (min gcc 12) features that are required for Atlas to function. 
Aswell as using a semi-latest build of Qt. We do not support builds of older versions. 
Please do not ask for support if you are unable to obtain at the MINIMUM the packages we require. Bring this up to your distro maintaners instead.

## More notes
We don't actively support clang and probably never will (it should generally work though). If you wish to add fixes for the clang compiler to properly build our app you can make a PR

## Git Requirements
All submodules are required for linux builds (`git submodule --init --recursive`)

## Package requirements
***NOTE***: While we give versions here. We only actively bug fix the latest ones. Please don't send us on an adventure trying to fix a bug for a library a few versions behind.
- At minimum a compiler that can support c++20 (`GCC 12` is the 'offical' compiler for Atlas)
- At minimum Qt6 `6.4.2`. Older might work (`6.x.x` only or above) but we don't actively support it
- At minimum sqlite3 `3.41.x`

## Build types
Atlas has a few build types you can use
| Type | Notes |
|----|-----|
| `Debug` | This is obvious |
| `System` | If you are building for yourself use this |
| `Release` | If you are building to run on multiple machines use this |
| `RelWithDebInfo` | Release + Debug |

## Extra flags
- `-DATLAS_PROFILE_ENABLE=ON` Will enable profiling for the app using (tracy)[]

## To build
1. Prep build dir:`cmake . -DCMAKE_BUILD_TYPE=(see build type) -B build`
2. Build target atlas: `cmake --build build --target Atlas -j8` (`-jN` can be changed to allow for more threads).
