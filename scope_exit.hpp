#ifndef SCOPE_EXIT_HPP
#define SCOPE_EXIT_HPP

#include <utility>

template <typename EF>
class scope_exit {
    EF cb_;
    bool cancelled_{false};

public:
    scope_exit() = delete;
    scope_exit(const scope_exit &) = delete;
    scope_exit & operator=(const scope_exit &) = delete;
    scope_exit & operator=(scope_exit &&) = delete;

    template <typename EFP>
    explicit scope_exit(EFP && cb)
        : cb_{std::forward<EFP>(cb)}
    {
    }

    scope_exit(scope_exit && other)
        : cb_{std::move(other.cb_)}
        , cancelled_{other.cancelled_}
    {
        other.release();
    }

    ~scope_exit()
    {
        if (!cancelled_) {
            cb_();
        }
    }

    void release() noexcept { cancelled_ = true; }
};

template <typename EF>
auto make_scope_exit(EF && exit_function)
{
    return scope_exit<std::decay_t<EF>>(std::forward<EF>(exit_function));
}

#endif