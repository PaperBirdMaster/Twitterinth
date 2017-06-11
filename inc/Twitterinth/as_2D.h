#ifndef WRAPPER_AS_2D
#define WRAPPER_AS_2D

// STD includes.
#include <algorithm>

namespace Wrapper
{

template <typename indexable_t>
struct as_2D
{
	//!
	using index_t = decltype(sizeof(nullptr));
	//!
	using type_t = decltype((*(static_cast<indexable_t *>(nullptr)))[0]);

	struct rectangle
	{
		index_t x{}, y{}, width{}, height{};
	};

	/**
	*/
	as_2D(indexable_t &a_indexable, index_t a_width, index_t a_height) :
		m_data  { a_indexable },
		m_width { a_width     },
		m_height{ a_height    }
	{}

	/**
	*/
	as_2D(as_2D &&a_other) :
		m_data  { a_other.m_data   },
		m_width { a_other.m_width  },
		m_height{ a_other.m_height }
	{}

	/**
	*/
	type_t &operator()(index_t a_x, index_t a_y)
	{
		return m_data[a_x + (a_y * m_width)];
	}

	/**
	*/
	const type_t &operator()(index_t a_x, index_t a_y) const
	{
		return m_data[a_x + (a_y * m_width)];
	}

	/**
	*/
	index_t width() const
	{
		return m_width;
	}

	/**
	*/
	index_t height() const
	{
		return m_height;
	}

	/**
	*/
	index_t count() const
	{
		return m_width * m_height;
	}

	/**
	*/
	template<typename, class iterator_t>
	iterator_t copy_to(iterator_t a_destination) const
	{
		return std::copy(std::begin(m_data), std::end(m_data), a_destination);
	}

	/**
	*/
	template<typename, class iterator_t>
	iterator_t copy_to(const rectangle &a_rectangle, iterator_t a_destination) const
	{
		const auto &me = *this;

		for (index_t y = a_rectangle.y, last_y = a_rectangle.y + a_rectangle.height; ++y)
		{
			for (index_t x = a_rectangle.x, last_x = a_rectangle.x + a_rectangle.width; ++x)
			{
				*a_destination++ = me(x, y);
			}
		}

		return a_destination;
	}

//private:
	//!
	indexable_t &m_data;
	//!
	index_t m_width{};
	//!
	index_t m_height{};
};

}


#endif
