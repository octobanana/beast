//
// Copyright (c) 2013-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BEAST_IMPL_STATIC_STRING_IPP
#define BEAST_IMPL_STATIC_STRING_IPP

#include <beast/core/detail/type_traits.hpp>

namespace beast {

//
// (constructor)
//

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string()
{
    n_ = 0;
    term();
}

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string(size_type count, CharT ch)
{
    assign(count, ch);
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
static_string<N, CharT, Traits>::
static_string(static_string<M, CharT, Traits> const& other,
    size_type pos)
{
    assign(other, pos);
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
static_string<N, CharT, Traits>::
static_string(static_string<M, CharT, Traits> const& other,
    size_type pos, size_type count)
{
    assign(other, pos, count);
}

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string(CharT const* s, size_type count)
{
    assign(s, count);
}

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string(CharT const* s)
{
    assign(s);
}

template<std::size_t N, class CharT, class Traits>
template<class InputIt>
static_string<N, CharT, Traits>::
static_string(InputIt first, InputIt last)
{
    assign(first, last);
}

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string(static_string const& s)
{
    assign(s);
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
static_string<N, CharT, Traits>::
static_string(static_string<M, CharT, Traits> const& s)
{
    assign(s);
}

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string(std::initializer_list<CharT> init)
{
    assign(init.begin(), init.end());
}

template<std::size_t N, class CharT, class Traits>
static_string<N, CharT, Traits>::
static_string(string_view_type sv)
{
    assign(sv);
}

template<std::size_t N, class CharT, class Traits>
template<class T, class>
static_string<N, CharT, Traits>::
static_string(T const& t, size_type pos, size_type n)
{
    assign(t, pos, n);
}

//
// (assignment)
//

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
assign(size_type count, CharT ch) ->
    static_string&
{
    if(count > max_size())
        throw detail::make_exception<std::length_error>(
            "count > max_size()", __FILE__, __LINE__);
    n_ = count;
    Traits::assign(&s_[0], n_, ch);
    term();
    return *this;
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
assign(static_string const& str) ->
    static_string&
{
    n_ = str.n_;
    Traits::copy(&s_[0], &str.s_[0], n_ + 1);
    return *this;
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
auto
static_string<N, CharT, Traits>::
assign(static_string<M, CharT, Traits> const& str,
        size_type pos, size_type count) ->
    static_string&
{
    auto const ss = str.substr(pos, count);
    return assign(ss.data(), ss.size());
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
assign(CharT const* s, size_type count) ->
    static_string&
{
    if(count > max_size())
        throw detail::make_exception<std::length_error>(
            "count > max_size()", __FILE__, __LINE__);
    n_ = count;
    Traits::copy(&s_[0], s, n_);
    term();
    return *this;
}

template<std::size_t N, class CharT, class Traits>
template<class InputIt>
auto
static_string<N, CharT, Traits>::
assign(InputIt first, InputIt last) ->
    static_string&
{
    std::size_t const n = std::distance(first, last);
    if(n > max_size())
        throw detail::make_exception<std::length_error>(
            "n > max_size()", __FILE__, __LINE__);
    n_ = n;
    for(auto it = &s_[0]; first != last; ++it, ++first)
        Traits::assign(*it, *first);
    term();
    return *this;
}

template<std::size_t N, class CharT, class Traits>
template<class T>
auto
static_string<N, CharT, Traits>::
assign(T const& t, size_type pos, size_type count) ->
    typename std::enable_if<std::is_convertible<T,
        string_view_type>::value, static_string&>::type
{
    auto const sv = string_view_type(t).substr(pos, count);
    if(sv.size() > max_size())
        throw detail::make_exception<std::length_error>(
            "sv.size() > max_size()", __FILE__, __LINE__);
    n_ = sv.size();
    Traits::copy(&s_[0], &sv[0], n_);
    term();
    return *this;
}

//
// Element access
//

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
at(size_type pos) ->
    reference
{
    if(pos >= size())
        throw detail::make_exception<std::out_of_range>(
            "pos >= size()", __FILE__, __LINE__);
    return s_[pos];
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
at(size_type pos) const ->
    const_reference
{
    if(pos >= size())
        throw detail::make_exception<std::out_of_range>(
            "pos >= size()", __FILE__, __LINE__);
    return s_[pos];
}

//
// Capacity
//

template<std::size_t N, class CharT, class Traits>
void
static_string<N, CharT, Traits>::
reserve(std::size_t n)
{
    if(n > max_size())
        throw detail::make_exception<std::length_error>(
            "n > max_size()", __FILE__, __LINE__);
}

//
// Operations
//

template<std::size_t N, class CharT, class Traits>
void
static_string<N, CharT, Traits>::
clear()
{
    n_ = 0;
    term();
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
insert(size_type index, size_type count, CharT ch) ->
    static_string&
{
    if(index > size())
        throw detail::make_exception<std::out_of_range>(
            "index > size()", __FILE__, __LINE__);
    insert(begin() + index, count, ch);
    return *this;
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
insert(size_type index, CharT const* s, size_type count) ->
    static_string&
{
    if(index > size())
        throw detail::make_exception<std::out_of_range>(
            "index > size()", __FILE__, __LINE__);
    if(size() + count > max_size())
        throw detail::make_exception<std::length_error>(
            "size() + count > max_size()", __FILE__, __LINE__);
    Traits::move(
        &s_[index + count], &s_[index], size() - index);
    n_ += count;
    Traits::copy(&s_[index], s, count);
    term();
    return *this;
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
auto
static_string<N, CharT, Traits>::
insert(size_type index,
    static_string<M, CharT, Traits> const& str,
        size_type index_str, size_type count) ->
    static_string&
{
    auto const ss = str.substr(index_str, count);
    return insert(index, ss.data(), ss.size());
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
insert(const_iterator pos, size_type count, CharT ch) ->
    iterator
{
    if(size() + count > max_size())
        throw detail::make_exception<std::length_error>(
            "size() + count() > max_size()", __FILE__, __LINE__);
    auto const index = pos - &s_[0];
    Traits::move(
        &s_[index + count], &s_[index], size() - index);
    n_ += count;
    Traits::assign(&s_[index], count, ch);
    term();
    return &s_[index];
}

template<std::size_t N, class CharT, class Traits>
template<class InputIt>
auto
static_string<N, CharT, Traits>::
insert(const_iterator pos, InputIt first, InputIt last) ->
    typename std::enable_if<
        detail::is_input_iterator<InputIt>::value,
            iterator>::type
{
    std::size_t const count = std::distance(first, last);
    if(size() + count > max_size())
        throw detail::make_exception<std::length_error>(
            "size() + count > max_size()", __FILE__, __LINE__);
    std::size_t const index = pos - begin();
    Traits::move(
        &s_[index + count], &s_[index], size() - index);
    n_ += count;
    for(auto it = begin() + index;
            first != last; ++it, ++first)
        Traits::assign(*it, *first);
    term();
    return begin() + index;
}

template<std::size_t N, class CharT, class Traits>
template<class T>
auto
static_string<N, CharT, Traits>::
insert(size_type index, const T& t,
        size_type index_str, size_type count) ->
    typename std::enable_if<std::is_convertible<
        T const&, string_view_type>::value &&
        ! std::is_convertible<T const&, CharT const*>::value,
            static_string&>::type
{
    auto const str =
        string_view_type(t).substr(index_str, count);
    return insert(index, str.data(), str.size());
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
erase(size_type index, size_type count) ->
    static_string&
{
    if(index > size())
        throw detail::make_exception<std::out_of_range>(
            "index > size()", __FILE__, __LINE__);
    auto const n = (std::min)(count, size() - index);
    Traits::move(
        &s_[index], &s_[index + n], size() - (index + n) + 1);
    n_ -= n;
    return *this;
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
erase(const_iterator pos) ->
    iterator
{
    erase(pos - begin(), 1);
    return begin() + (pos - begin());
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
erase(const_iterator first, const_iterator last) ->
    iterator
{
    erase(first - begin(),
        std::distance(first, last));
    return begin() + (first - begin());
}

template<std::size_t N, class CharT, class Traits>
void
static_string<N, CharT, Traits>::
push_back(CharT ch)
{
    if(size() >= max_size())
        throw detail::make_exception<std::length_error>(
            "size() >= max_size()", __FILE__, __LINE__);
    Traits::assign(s_[n_++], ch);
    term();
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
auto
static_string<N, CharT, Traits>::
append(static_string<M, CharT, Traits> const& str,
        size_type pos, size_type count) ->
    static_string&
{
    // Valid range is [0, size)
    if(pos >= str.size())
        throw detail::make_exception<std::out_of_range>(
            "pos > str.size()", __FILE__, __LINE__);
    string_view_type const ss{&str.s_[pos],
        (std::min)(count, str.size() - pos)};
    insert(size(), ss.data(), ss.size());
    return *this;
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
substr(size_type pos, size_type count) const ->
    string_view_type
{
    if(pos > size())
        throw detail::make_exception<std::out_of_range>(
            "pos > size()", __FILE__, __LINE__);
    return{&s_[pos], (std::min)(count, size() - pos)};
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
copy(CharT* dest, size_type count, size_type pos) const ->
    size_type
{
    auto const str = substr(pos, count);
    Traits::copy(dest, str.data(), str.size());
    return str.size();
}

template<std::size_t N, class CharT, class Traits>
void
static_string<N, CharT, Traits>::
resize(std::size_t n, CharT c)
{
    if(n > max_size())
        throw detail::make_exception<std::length_error>(
            "n > max_size()", __FILE__, __LINE__);
    if(n > n_)
        Traits::assign(&s_[n_], n - n_, c);
    n_ = n;
    term();
}

template<std::size_t N, class CharT, class Traits>
void
static_string<N, CharT, Traits>::
swap(static_string& str)
{
    static_string tmp(str);
    str.n_ = n_;
    Traits::copy(&str.s_[0], &s_[0], n_ + 1);
    n_ = tmp.n_;
    Traits::copy(&s_[0], &tmp.s_[0], n_ + 1);
}

template<std::size_t N, class CharT, class Traits>
template<std::size_t M>
void
static_string<N, CharT, Traits>::
swap(static_string<M, CharT, Traits>& str)
{
    if(size() > str.max_size())
        throw detail::make_exception<std::length_error>(
            "size() > str.max_size()", __FILE__, __LINE__);
    if(str.size() > max_size())
        throw detail::make_exception<std::length_error>(
            "str.size() > max_size()", __FILE__, __LINE__);
    static_string tmp(str);
    str.n_ = n_;
    Traits::copy(&str.s_[0], &s_[0], n_ + 1);
    n_ = tmp.n_;
    Traits::copy(&s_[0], &tmp.s_[0], n_ + 1);
}


template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
assign_char(CharT ch, std::true_type) ->
    static_string&
{
    n_ = 1;
    Traits::assign(s_[0], ch);
    term();
    return *this;
}

template<std::size_t N, class CharT, class Traits>
auto
static_string<N, CharT, Traits>::
assign_char(CharT ch, std::false_type) ->
    static_string&
{
    throw detail::make_exception<std::length_error>(
        "max_size() == 0", __FILE__, __LINE__);
}

} // beast

#endif