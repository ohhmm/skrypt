#pragma once
#include <omnn/math/System.h>
#include <omnn/math/VarHost.h>

#include <boost/typeof/typeof.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <string_view>


namespace skrypt {
	class Skrypt
		: public omnn::math::System
	{
		using base = omnn::math::System;

		::omnn::math::VarHost::ptr varHost = ::omnn::math::VarHost::make<std::string>();

		omnn::math::Valuable::va_names_t vars;
		bool echo = {};
		bool disjunctionParseMode = {};

	protected:
		void SetVarhost(decltype(varHost));
		constexpr bool DisjunctionParseMode() const { return disjunctionParseMode; }
		void DisjunctionParseMode(bool isDisjunctionMode) { disjunctionParseMode = isDisjunctionMode; }
		::omnn::math::Product disjunction;
		bool ParseTotal(std::istream&, std::string_view&, std::function<void(::omnn::math::Valuable&&)>);

	public:
		using value_type = omnn::math::Valuable;

		Skrypt() {}
		Skrypt(const boost::filesystem::path&);
		Skrypt(std::istream&);

		auto GetVarHost() const { return varHost; }

		using base::Add;
		
		bool Add(std::string_view);
        bool ParseNextLine(std::istream&, std::string_view&);
		void PrintVarKnowns(const omnn::math::Variable&);
        void ProcessQuestionLine(std::string_view&);

		/// <summary>
		/// Loads .skrypt file and returns InitialVarNames for input
		/// </summary>
		/// <returns></returns>
		const omnn::math::Valuable::va_names_t& Load(std::istream&);

		/// <summary>
		/// Loads .skrypt file and returns InitialVarNames for input
		/// </summary>
		/// <returns></returns>
		const omnn::math::Valuable::va_names_t& Load(const boost::filesystem::path&);

		void Echo(bool e) { echo = e; }
	};
}
