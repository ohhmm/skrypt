#include <omnn/math/System.h>
#include <omnn/math/VarHost.h>

#include <boost/algorithm/string.hpp>
#include <boost/typeof/typeof.hpp>

#include <string_view>

namespace boost::filesystem {
	class path;
}

namespace skrypt {
	class Skrypt
		: public omnn::math::System
	{
		using base = omnn::math::System;

		BOOST_AUTO(varHost, ::omnn::math::VarHost::make<std::string>());

		omnn::math::Valuable::va_names_t vars;


	public:
		Skrypt(const boost::filesystem::path&);

		bool Add(std::string_view);

		/// <summary>
		/// Loads .skrypt file and returns InitialVarNames for input
		/// </summary>
		/// <returns></returns>
		const omnn::math::Valuable::va_names_t& Load(const boost::filesystem::path&);
	};
}
