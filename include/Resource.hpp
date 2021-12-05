#pragma once
#include <functional>

template<
    typename Signature,
    typename ValueType,
    typename ResourceTraits,
    typename ...AcquirerArguments
>
struct Resource {
    using self_type = Resource;
    using signature_type = Signature;
    using value_type = ValueType;
    using traits_type = ResourceTraits;
    
    Resource(AcquirerArguments&&... acquirerArguments)
        : value_(traits_type::getAcquirer()(std::forward<AcquirerArguments&&>(acquirerArguments)...)) {
    }
    
    ~Resource() {
        const value_type undefined = traits_type::getUndefinedValue();
        if (undefined != value_) {
            traits_type::getReleaser()(value_);
            value_ = undefined;
        }
    }
    
    const value_type& get() const noexcept {
        return value_;
    }
    
    Resource(const self_type&) = delete;
    self_type& operator=(const self_type&) = delete;
    
private:
    value_type value_;
};

template<
    typename Signature,
    typename ValueType,
    typename ...AcquirerArguments
>
struct ResourceTraits {
    using self_type = ResourceTraits;
    using signature_type = Signature;
    using value_type = ValueType;
    using acquirer_type = std::function<value_type(AcquirerArguments&&...)>;
    using releaser_type = std::function<void(value_type&)>;
    
    static value_type getUndefinedValue() { return value_type(); }
    static acquirer_type getAcquirer() { return acquirer_type(); }
    static releaser_type getReleaser() { return releaser_type(); }
};
