#pragma once

#include <unordered_map>
#include <unordered_set>
#include <typeindex>
#include <type_traits>
#include <vector>
#include <memory>

#include <cstddef>
#include <iostream>


namespace Engine::ECS::Detail
{


	// AND: すべての型を持つか
	template<typename... MustComponents>
	bool hasAllComponents(const std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>>& pools,
		uint32_t entityID)
	{
		//bool allFound = true;

		//(void)std::initializer_list<int>
		//{
		//	(allFound = allFound && [&]
		//		{
		//			std::type_index type = std::type_index(typeid(MustComponents));
		//			auto it = pools.find(type);
		//			return it != pools.end() && it->second.find(entityID) != it->second.end();
		//		}(), 0)...
		//};

		//return allFound;

		//return (... && [&] {
		//	auto poolIt = pools.find(std::type_index(typeid(MustComponents)));
		//	return poolIt != pools.end() && poolIt->second.find(entityID) != poolIt->second.end();
		//	}());


		return (...&& (// 短絡評価によって効率化
			pools.count(std::type_index(typeid(MustComponents))) &&// 短絡評価によって例外には入らない
			pools.at(std::type_index(typeid(MustComponents))).count(entityID)
			));
	}

	// OR: いずれかの型を持つか
	template<typename... AnyComponents>
	bool hasAnyComponents(const std::unordered_map<std::type_index, std::unordered_map<uint32_t, std::shared_ptr<void>>>& pools,
		uint32_t entityID)
	{
		//bool anyFound = false;

		//// 副作用を利用
		//(void)std::initializer_list<int>{// int型リスト
		//	(anyFound = anyFound || [&] // 副作用で計算結果を得る：anyFound || lambda() 
		//		{
		//			std::type_index type = std::type_index(typeid(AnyComponents));
		//			auto it = pools.find(type);
		//			return it != pools.end() && it->second.find(entityID) != it->second.end();
		//		}(), 0)...// カンマ演算子 (anyFound || lambda (), 0) => list要素をすべて (int)0にしてつぶす(中身は不要) voidで捨てる
		//};

		//return anyFound;

		//return (... || [&] {
		//	[pools, entityID]() {
		//		std::type_index type = std::type_index(typeid(AnyComponents));
		//		auto it = pools.find(type);
		//		return it != pools.end() && it->second.find(entityID) != it->second.end();
		//		}());

		return (... || (// 短絡評価によって効率化
			pools.count(std::type_index(typeid(AnyComponents))) &&// 短絡評価によって例外には入らない
			pools.at(std::type_index(typeid(AnyComponents))).count(entityID)
			));
	}
}