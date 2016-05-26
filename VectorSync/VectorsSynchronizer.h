#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

template<typename T>
class VectorsSynchronizer
{
	using Items = std::vector<T>;
	using Indices = std::vector<size_t>;

public:
	VectorsSynchronizer(const Items& source) : m_source(source) {}
	~VectorsSynchronizer() {}

	Items SynchronizeToDestination(const Items& targeted)
	{
		const auto& itemsToRemove = IndicesOfItemsToRemove(targeted);
		RemoveItems(itemsToRemove);
		ReorderItems(targeted);

		AddMissingItems(targeted);
		ReorderItems(targeted);
		return m_source;
	}

	const Items& GetSource() const { return m_source; }

private:
	Items m_source;

	Indices IndicesOfItemsToRemove(const Items& targeted) const
	{
		Indices result;
		for (auto it = m_source.cbegin(); it != m_source.end(); ++it)
		{
			if (std::find(targeted.cbegin(), targeted.cend(), *it) == targeted.cend())
				result.push_back(it - m_source.cbegin());
		}
		return result;
	}

	void RemoveItems(const Indices& toRemove)
	{
		for (auto it = toRemove.crbegin(); it != toRemove.crend(); ++it)
		{
			size_t index = *it;
			const auto& offset = m_source.begin() + index;
			m_source.erase(offset);
		}
	}

	void ReorderItems(const Items& targeted)
	{
		size_t moveToIndex = 0;
		auto found = targeted.cbegin();
		do
		{
			found = std::find_first_of(found, targeted.cend(), m_source.cbegin(), m_source.cend());
			if (found == targeted.cend())
				break;
			if (*found != m_source[moveToIndex])
			{
				size_t offset = std::find(m_source.cbegin(), m_source.cend(), *found) - m_source.cbegin();
				Move(offset, moveToIndex);
			}
			++moveToIndex;
			++found;
		} while (found != targeted.cend());
	}

	Items ItemsToAdd(const Items& targeted) const
	{
		return ItemsInFirstButNotInSecond(targeted, m_source);
	}

	Items ItemsInFirstButNotInSecond(const Items& first, const Items& second) const
	{
		Items result;
		std::copy_if(first.cbegin(), first.cend(), std::back_inserter(result), [&second](const T& item)
		{
			return std::find(second.cbegin(), second.cend(), item) == second.cend();
		});
		return result;
	}

	void AddMissingItems(const Items& targeted)
	{
		const auto& itemsToAdd = ItemsToAdd(targeted);
		m_source.insert(m_source.end(), itemsToAdd.cbegin(), itemsToAdd.cend());
	}

	void Move(size_t from, size_t to)
	{
		if (from > to)
			std::swap(from, to);

		const auto& origin = m_source.begin() + from;
		const auto& destination = m_source.begin() + to;
		std::rotate(origin, destination, destination + 1);
	}
};

