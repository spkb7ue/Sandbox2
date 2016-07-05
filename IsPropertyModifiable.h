#pragma once

namespace rabbit
{

/**
* Can be used to create a const/non-const type at compile time.
* An alternative to this struct is a combination of std::add_const
* and std::decay, but these features are not supported by all compilers
* Usage:
* IsPropertyModifiable<T, true>::type is same as T
* IsPropertyModifiable<T, true>::type is same as const T
* @tparam T class of interest
* @tparam canBeModified whether T can be modified
*/
template<typename T, bool canBeModified>
struct IsPropertyModifiable{};

template<typename T>
struct IsPropertyModifiable<T,true>
{
    typedef T type;
};

template<typename T>
struct IsPropertyModifiable<T,false>
{
    typedef const T type;
};

}
