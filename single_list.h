#pragma single_list

#include <iostream>
#include <cassert>
#include <initializer_list>

/*
    Метод push_back() - добавляет элемент в конец списка. Хранится указатель m_last на последний элемент для быстрой вставки.
    Метод push_front() - добавляет элемент в начало списка.
    Метод show() - выводит элементы списка
    Метод insert_after() - принимает итератор и элемент. Вставляет элемент после элемента, на который указывает итератор. Если нулевой, вставляет элемент вперед.
    Метод erase_after() - удаляет элемент, следующий за утератором. Если нулевой итератор то удаляет первый элемент.
    Метод pop_fron() - удаляет первый элемент.
    Метод find() - ищет элемент, если элемента нет в списке возвращает nullptr.
*/

template<class Type>
class My_list
{
    struct Element
    {
        Type m_data;
        Element *m_next;

        Element(const Type &data): m_data {std::move(data)}, m_next {nullptr}
        {
        }
    };

    Element *m_first {nullptr};
    Element *m_last {nullptr};
    size_t m_size {0};

    void copy(const My_list &list)
    {
        Element *current_element = list.m_first;

        size_t time_size {list.get_size()};

        while(time_size)
        {
            push_back(current_element->m_data);
            current_element = current_element->m_next;
            --time_size;
        }
    }

    public:

    My_list(): m_first {nullptr}, m_last {nullptr}, m_size {0}
    {
    }

    My_list(std::initializer_list<Type> list)
    {
        for(const auto &element : list)
        {
            push_back(std::move(element));
        }
    }

    My_list(const My_list &list)
    {
        copy(std::move(list));
    }

    ~My_list()
    {
        clear();
    }

    class Iterator
    {
        Element *m_ptr{nullptr};

        Element* get_ptr() const
        {
            return m_ptr;
        }

        public:

        friend My_list;

        Iterator(Element *ptr): m_ptr {ptr}
        {
        }

        Type& operator*()
        {
            assert(m_ptr != nullptr);

            return m_ptr->m_data;
        }

        Iterator operator++()
        {
            assert(m_ptr != nullptr);
            m_ptr = m_ptr->m_next;

            return *this;
        }

        Iterator operator++(int)
        {
            assert(m_ptr != nullptr);

            auto copy_ptr = *this;
            m_ptr = m_ptr->m_next;

            return copy_ptr;
        }

        bool operator==(Iterator iter) const
        {
            return m_ptr == iter.m_ptr;
        }

        bool operator!=(Iterator iter) const
        {
            return !(*this == iter);
        }
    };

    bool isEmpty() const
    {
        return m_first == nullptr;
    }

    size_t get_size() const
    {
        return m_size;
    }

    void push_back(const Type &elm)
    {
        if(isEmpty())
        {
            m_first = new Element(std::move(elm));
            m_last = m_first;
        }
        else
        {
            Element *current_element = new Element(std::move(elm));
            m_last->m_next = current_element;
            m_last = current_element;
        }

        ++m_size;
    }

    void push_front(const Type &elm)
    {
        if(isEmpty())
        {
            m_first = new Element(std::move(elm));
        }
        else
        {
            Element *current_element = this->m_first;
            m_first = new Element(std::move(elm));
            m_first->m_next = current_element;
        }
        ++m_size;
    }

    void clear()
    {
        if(isEmpty())
        {
            return;
        }
        else
        {
            Element *current_element {nullptr};

            while(this->m_size)
            {
                current_element = this->m_first;
                this->m_first = this->m_first->m_next;
                delete current_element;
                --m_size;
            }
        }
        m_first = nullptr;
        m_last = nullptr;
    }

    My_list& operator=(const My_list &list)
    {
        if(this == &list)
        {
            return *this;
        }

        clear();
        copy(std::move(list));

        return *this;
    }

    Iterator begin()
    {
        return Iterator {m_first};
    }

    Iterator end()
    {
        return Iterator {nullptr};
    }

    Iterator find(const Type &elm)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it == elm)
            {
                return it;
            }
        }
        return Iterator {nullptr};
    }

    void insert_after(Iterator iter, Type data)
    {
        auto ptr = iter.get_ptr();

        if(!ptr)
        {
            push_front(std::move(data));
            return;
        }

        Element *new_element = new Element(std::move(data));

        if(ptr->m_next == nullptr)
        {
            m_last = new_element;
        }

        new_element->m_next = ptr->m_next;
        ptr->m_next = new_element;
        ++m_size;
    }

    void erase_after(Iterator iter)
    {
        auto ptr = iter.get_ptr();

        if(ptr)
        {
            auto delete_items = ptr->m_next;
            if(ptr->m_next)
            {
                ptr->m_next = ptr->m_next->m_next;

                if(ptr->m_next == nullptr)
                {
                    m_last = ptr;
                }

                --m_size;
            }
            delete delete_items;
        }
        else
        {
            if(m_first == nullptr)
            {
                return;
            }

            pop_front();
        }
    }

    void pop_front()
    {
        if(isEmpty())
        {
            return;
        }

        --m_size;

        if(m_first == m_last)
        {
            delete m_first;
            m_first = nullptr;
            m_last = nullptr;

            return;
        }

        Element *delete_items = m_first;
        m_first = m_first->m_next;

        delete delete_items;
    }

    void show()
    {
        std::cout << "[ ";
        if(isEmpty())
        {
            std::cout << 0 << ' ';
        }
        else
        {
            for(auto it = begin(); it != end(); ++it)
            {
                std::cout << *it << ' ';
            }
        }
        std::cout << "]\n";
    }
};
