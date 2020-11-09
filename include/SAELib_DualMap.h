#pragma once

#include <unordered_map>

namespace sae
{

	template <typename LT, typename RT>
	struct unordered_dualmap
	{
	public:
		using left_type = LT;
		using right_type = RT;

		right_type& ltor(const left_type& _l)
		{
			return this->l_to_r_.at(_l);
		};
		const right_type& ltor(const left_type& _l) const
		{
			return this->l_to_r_.at(_l);
		};

		left_type& rtol(const right_type& _l)
		{
			return this->r_to_l_.at(_l);
		};
		const left_type& rtol(const right_type& _l) const
		{
			return this->r_to_l_.at(_l);
		};

		void insert(const std::pair<left_type, right_type>& _pair)
		{
			this->l_to_r_.insert(_pair);
			this->r_to_l_.insert({ _pair.second, _pair.first });
		};

		bool contains_left(const left_type& _l) const
		{
			return this->l_to_r_.count(_l) != 0;
		};
		bool contains_right(const right_type& _r) const
		{
			return this->r_to_l_.count(_r) != 0;
		};





		unordered_dualmap() = default;
		unordered_dualmap(std::initializer_list<std::pair<left_type, right_type>> _pairs)
		{
			for (const auto& p : _pairs)
			{
				this->insert(p);
			};
		};

	private:
		std::unordered_map<LT, RT> l_to_r_{};
		std::unordered_map<RT, LT> r_to_l_{};

	};




}

