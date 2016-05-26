#pragma once

#include <vector>
#include <algorithm>
#include <iterator>

/////////////////////////////////////////////////////////////////////
// A utility class that should provide shortest sequence
// of insert, remove and move operations in order to
// synchronize source vector of items with targeted
// sequence.
// T - type of data in the vector
// Callback - class with callback methods:
//              Insert(T item, size_t index);
//              Remove(size_t index);
//              Move(size_t from, size_t to);
// Pred - A binary predicate used for item equality check

template<typename T, typename Callback, typename Pred = equal_to<T>>
class VectorSynchronizer
{
public:
	using Items = std::vector<T>;
	using Indices = std::vector<size_t>;

	VectorSynchronizer(const Items& source, Callback& callback) 
		: m_source(source)
		, m_callback(callback)
	{}

	virtual ~VectorSynchronizer() {}

	Items SynchronizeToDestination(const Items& targeted)
	{
		const auto& itemsToRemove = IndicesOfItemsToRemove(targeted);
		return DoSynchronization(targeted, itemsToRemove);
	}

protected:
	Items m_source;
	Callback& m_callback;

	virtual Items DoSynchronization(const Items& targeted, const Indices& itemsToRemove)
	{
		RemoveItems(itemsToRemove);
		ReorderItems(targeted);

		InsertMissingItems(targeted);
		return m_source;
	}

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
			m_callback.Remove(index);
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
		Items result;
		std::copy_if(targeted.cbegin(), targeted.cend(), std::back_inserter(result), [this](const T& item)
		{
			return std::find(m_source.cbegin(), m_source.cend(), item) == m_source.cend();
		});
		return result;
	}

	size_t InsertMissingItems(const Items& targeted)
	{
		const auto& itemsToAdd = ItemsToAdd(targeted);
		for (const auto& item : itemsToAdd)
		{
			size_t index = std::find(targeted.cbegin(), targeted.cend(), item) - targeted.begin();
			m_callback.Insert(item, index);
			m_source.insert(m_source.begin() + index, item);
		}
		return itemsToAdd.size();
	}

	void Move(size_t from, size_t to)
	{
		m_callback.Move(from, to);
		if (from > to)
			std::swap(from, to);

		const auto& origin = m_source.begin() + from;
		const auto& destination = m_source.begin() + to;
		std::rotate(origin, destination, destination + 1);
	}
};

// derived version which tries to avoid empty vector if all
// source vector items should be removed
template<typename T, typename Callback, typename Pred = equal_to<T>>
class VectorSynchronizerNoEmpty : public VectorSynchronizer<T, Callback, Pred>
{
public:
	VectorSynchronizerNoEmpty(const Items& source, Callback& callback) : VectorSynchronizer(source, callback)
	{}

protected:
	Items DoSynchronization(const Items& targeted, const Indices& itemsToRemove) override
	{
		if (m_source.size() > itemsToRemove.size())
			return VectorSynchronizer::DoSynchronization(targeted, itemsToRemove);

		size_t added = InsertMissingItems(targeted);
		// since missing items have been inserted to the begining
		// all items to follow can be removed
		Indices toRemove;
		while (added < m_source.size())
		{
			toRemove.push_back(added);
			++added;
		}
		RemoveItems(toRemove);
		return m_source;
	}
};