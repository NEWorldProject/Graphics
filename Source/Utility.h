#pragma once

struct NonCopyable {
    NonCopyable() = default;
    NonCopyable(NonCopyable&&) = default;
    NonCopyable& operator = (NonCopyable&&) = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator = (const NonCopyable&)= delete;
    ~NonCopyable() = default;
};

struct NonCopyableVirtualBase {
    NonCopyableVirtualBase() = default;
    NonCopyableVirtualBase(NonCopyableVirtualBase&&) = default;
    NonCopyableVirtualBase& operator = (NonCopyableVirtualBase&&) = default;
    NonCopyableVirtualBase(const NonCopyableVirtualBase&) = delete;
    NonCopyableVirtualBase& operator = (const NonCopyableVirtualBase&)= delete;
    virtual ~NonCopyableVirtualBase() = default;
};
