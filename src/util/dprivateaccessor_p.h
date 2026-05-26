// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <QtCore/qcompilerdetection.h>

// Private member accessor using the explicit template instantiation technique.
//
// C++ Standard [temp.explicit]/12 states:
// "The usual access checking rules do not apply to names used to
// specify explicit instantiation definitions."
//
// This allows passing pointers to private/protected data members and
// member functions as template arguments in explicit instantiations,
// bypassing normal access control — without modifying the class definition
// and without the UB caused by "#define private public".
//
// NOTE: These helper structs must be in the SAME namespace as the Tag structs
// (global namespace, since the macros expand at file scope). If they were in a
// sub-namespace, the friend definition would create a different function than
// the friend declaration in the Tag struct, causing undefined-reference errors.

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Wnon-template-friend")

template<typename Tag>
struct DtkWidgetPrivateAccessor
{
    using MemberPtr = typename Tag::MemberPtr;
    friend MemberPtr get(Tag) noexcept;
};

template<typename Tag, typename Tag::MemberPtr Ptr>
struct DtkWidgetPrivateAccessorImpl : DtkWidgetPrivateAccessor<Tag>
{
    friend typename Tag::MemberPtr get(Tag) noexcept { return Ptr; }
};

QT_WARNING_POP

#define D_DECLARE_PRIVATE_MEMBER(TagName, ClassName, Member, MemberType) \
    struct TagName { \
        using MemberPtr = MemberType ClassName::*; \
        friend MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkWidgetPrivateAccessorImpl<TagName, &ClassName::Member>

#define D_DECLARE_PRIVATE_METHOD(TagName, ClassName, MethodName, RetType, ...) \
    struct TagName { \
        using MemberPtr = RetType (ClassName::*)(__VA_ARGS__); \
        friend MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkWidgetPrivateAccessorImpl<TagName, &ClassName::MethodName>

#define D_DECLARE_PRIVATE_CONST_METHOD(TagName, ClassName, MethodName, RetType, ...) \
    struct TagName { \
        using MemberPtr = RetType (ClassName::*)(__VA_ARGS__) const; \
        friend MemberPtr get(TagName) noexcept; \
    }; \
    template struct DtkWidgetPrivateAccessorImpl<TagName, &ClassName::MethodName>

// Trampoline: ensures get(tag) is called from a context with no class-scope
// get() member that might suppress ADL (C++ [basic.lookup.argdep] para 3).
namespace dtk_private_detail {
    template<typename Tag>
    inline typename Tag::MemberPtr access(Tag t) noexcept { return get(t); }
}

#define D_PRIVATE_MEMBER(obj, tag) ((obj).*dtk_private_detail::access(tag))
#define D_PRIVATE_CALL(obj, tag, ...) ((obj).*dtk_private_detail::access(tag))(__VA_ARGS__)
