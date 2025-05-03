#pragma once

#include <array>
#include <vector>
#include <memory>
#include <type_traits>
#include <cassert>

template <typename T, size_t PageSize>
class SparseSets final
{
public:
    static_assert(std::is_integral_v<T>, "T must be an integral type");

    SparseSets() = default;
    SparseSets(const SparseSets&) = delete;
    SparseSets& operator=(const SparseSets&) = delete;
    SparseSets(SparseSets&&) = default;
    SparseSets& operator=(SparseSets&&) = default;

    void Add(T t)
    {
        density.push_back(t);
        auto p = page(t);
        if (p >= sparse.size())
        {
            for (size_t i = sparse.size(); i <= p; i++)
            {
                sparse.emplace_back(std::make_unique<std::array<T, PageSize>>());
                sparse[i]->fill(null);
            }
        }
        index(t) = density.size() - 1;
    }

    void Remove(T t)
    {
        if (!Has(t))
            return;
        auto& idx = index(t);
        if (idx != density.size() - 1)
        {
            auto last = density.back();
            index(last) = idx;
            std::swap(density[idx], density.back());
        }
        idx = null;
        density.pop_back();
    }

    bool Has(T t) const
    {
        assert(t != null);
        auto p = page(t);
        auto o = offset(t);
        return p < sparse.size() && sparse[p]->at(o) != null;
    }

    void Clear()
    {
        density.clear();
        sparse.clear();
    }

    inline auto begin() { return density.begin(); }
    inline auto end() { return density.end(); }

private:

    inline size_t page(T t) const { return t / PageSize; }
    inline size_t offset(T t) const { return t % PageSize; }

    inline T index(T t) const { return sparse[page(t)]->at(offset(t)); }
    inline T& index(T t) { return sparse[page(t)]->at(offset(t)); }

private:

    std::vector<T> density;
    std::vector<std::unique_ptr<std::array<T, PageSize>>> sparse;
    static constexpr T null = std::numeric_limits<T>::max();

};