#pragma once
#include "types.hpp"

#include <es_fe/var_base.hpp>

#include <es_util/tuple.hpp>
#include <es_util/type_traits.hpp>

#include <cassert>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace fe
{
template<class Element_, Local_index dim_ = 1, class... Bnd_conds>
class Var : public internal::Var_base<Element_, Bnd_conds...>
{
	static_assert(dim_ > 0);

private:
	using Base = internal::Var_base<Element_, Bnd_conds...>;

public:
	static constexpr Local_index dim()
	{
		return dim_;
	}

	template<class Tag>
	static constexpr Local_index n_dofs(Tag tag)
	{
		return dim() * Element_::n_dofs(tag);
	}

	template<class Tag>
	static constexpr Local_index n_total_dofs(Tag tag)
	{
		return dim() * Element_::n_total_dofs(tag);
	}
};

template<class Element_, class... Bnd_conds>
class Var_x : public internal::Var_base<Element_, Bnd_conds...>
{
public:
	Local_index dim() const
	{
		return dim_;
	}

	void set_dim(Local_index dim)
	{
		dim_ = dim;
	}

	template<typename Tag>
	Local_index n_dofs(Tag tag) const
	{
		return dim() * Element_::n_dofs(tag);
	}

	template<typename Tag>
	Local_index n_total_dofs(Tag tag) const
	{
		return dim() * Element_::n_total_dofs(tag);
	}

private:
	Local_index dim_ = 1;
};
} // namespace fe