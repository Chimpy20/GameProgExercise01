#pragma once

namespace containers
{

template <class T>
class List;

class NodeBase
{
public:
	NodeBase() :
		m_next( this ),
		m_prev( this )
	{
	}
	~NodeBase()
	{
		unlink();
	}

	NodeBase* m_next;
	NodeBase* m_prev;

	void push_back( NodeBase* newNode )
	{
		newNode->m_next = this;
		newNode->m_prev = m_prev;
		m_prev->m_next = newNode;
		m_prev = newNode;
	}

	void unlink()
	{
		NodeBase* next = m_next, * prev = m_prev;
		next->m_prev = prev;
		prev->m_next = next;
		m_next = this;
		m_prev = this;
	}
};

template<class T>
class Node : public NodeBase
{
public:
	Node() : m_data() {}
	Node( T t ) : m_data( t ) {}

	T				m_data;
};

template<typename T>
class ListIterator
{
public:
	NodeBase* m_node;

	ListIterator() : m_node() {}
	ListIterator( NodeBase* node )
		: m_node( node ) {}

	T& operator*() const
	{
		return static_cast<Node<T>*>( m_node )->m_data;
	}

	T* operator->() const
	{
		return &static_cast<Node<T>*>( m_node )->m_data;
	}

	ListIterator& operator++()
	{
		m_node = m_node->m_next;
		return *this;
	}

	ListIterator operator++( int )
	{
		ListIterator tmp = *this;
		m_node = m_node->m_next;
		return tmp;
	}

	bool operator==( ListIterator other ) const { return m_node == other.m_node; }
	bool operator!=( ListIterator other ) const { return m_node != other.m_node; }
};

template<typename T>
class ListConstIterator
{
public:
	const NodeBase* m_node;

	ListConstIterator() : m_node() {}
	ListConstIterator( const NodeBase* node ) : m_node( node ) {}
	ListConstIterator( const ListConstIterator<T>& node ) : m_node( node.m_node ) {}

	const T& operator*() const
	{
		return static_cast<const Node<T>*>( m_node )->m_data;
	}

	const T* operator->() const
	{
		return &static_cast<const Node<T>*>( m_node )->m_data;
	}

	ListConstIterator& operator++()
	{
		m_node = m_node->m_next;
		return *this;
	}

	ListConstIterator operator++( int )
	{
		ListConstIterator tmp = *this;
		m_node = m_node->m_next;
		return tmp;
	}

	bool operator==( ListConstIterator other ) const { return m_node == other.m_node; }
	bool operator!=( ListConstIterator other ) const { return m_node != other.m_node; }
};

template <class T>
class List
{
private:
	Node<T>			m_head;
	size_t			m_size;

public:
	typedef ListIterator<T> iterator;
	typedef ListConstIterator<T> const_iterator;

	List() :
		m_size( 0 )
	{
	}

	~List()
	{
		clear();
	}

	inline bool empty() const
	{
		return m_head.m_next == &m_head;
	}

	inline iterator begin()
	{
		return m_head.m_next;
	}

	inline const_iterator begin() const
	{
		return m_head.m_next;
	}

	inline iterator end()
	{
		return &m_head;
	}

	inline const_iterator end() const
	{
		return &m_head;
	}

	inline void push_back( T t )
	{
		Node<T>* newNode = new Node<T>( t );
		m_head.push_back( newNode );
		++m_size;
	}

	inline iterator erase( iterator i )
	{
		iterator next = i.m_node->m_next;
		if( next == nullptr )
			next = end();
		delete i.m_node;
		ASSERT( m_size > 0, "Trying to erase from a non-empty list\n" );
		--m_size;
		return next;
	}

	inline void clear()
	{
		while( !empty() )
			erase( begin() );

		ASSERT( m_size == 0, "Finished erase but size is still %u.\n", m_size );
		m_size = 0;
	}

	inline size_t size() const
	{
		return m_size;
	}

	inline void sort()
	{
		Node<T>* start = static_cast<Node<T>*>( m_head.m_next );
		while( start != &m_head )
		{
			Node<T>* highestVal = start;
			Node<T>* node = static_cast<Node<T>*>( start->m_next );
			while( node != &m_head )
			{
				if( node->m_data > highestVal->m_data )
				{
					highestVal = node;
				}
				node = static_cast<Node<T>*>( node->m_next );
			}
			T tmp = start->m_data;
			start->m_data = highestVal->m_data;
			highestVal->m_data = tmp;

			start = static_cast<Node<T>*>( start->m_next );
		}
	}
};

}