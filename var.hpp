#pragma once
#include "types.hpp"
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
namespace internal
{
template<class Element_, class... Bnd_conds_>
class Var_base
{
public:
	using Element = Element_;
	static constexpr bool has_bnd_cond = sizeof...(Bnd_conds_) > 0;
	static constexpr Local_index space_dim = Element::dim;

	// TODO : remove
	static constexpr Local_index n_total_dofs = Element::n_total_face_dofs;

private:
	using Bnd_conds_tuple = std::tuple<std::unique_ptr<Bnd_conds_>...>;

public:
	template<std::size_t i = 0, typename... Args>
	void set_bnd_cond(Args&&... args)
	{
		using Bnd_cond = typename std::tuple_element_t<i, Bnd_conds_tuple>::element_type;
		std::get<i>(bnd_conds_) = std::make_unique<Bnd_cond>(std::forward<Args>(args)...);
	}

	template<class Func>
	void for_each_ess_bnd_cond(Func func) const
	{
		es_util::tuple_for_each([&func](const auto& bnd_cond)
			{
				using Bnd_cond = es_util::Remove_cvref<decltype(*bnd_cond)>;

				assert(bnd_cond);
				if constexpr (Bnd_cond::is_essential())
					func(*bnd_cond);
			}, bnd_conds_);
	}

	template<class Func>
	void for_each_non_ess_bnd_cond(Func func) const
	{
		es_util::tuple_for_each([&func](const auto& bnd_cond)
			{
				using Bnd_cond = es_util::Remove_cvref<decltype(*bnd_cond)>;

				assert(bnd_cond);
				if constexpr (!Bnd_cond::is_essential())
					func(*bnd_cond);
			}, bnd_conds_);
	}

	const std::string& name() const
	{
		return name_;
	}

	void set_name(std::string name)
	{
		name_ = std::move(name);
	}

private:
	Bnd_conds_tuple bnd_conds_;
	std::string name_ = "<un-named>";
};
}

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
}