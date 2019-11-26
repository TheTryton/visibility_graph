#pragma once
#include <random>
#include <memory>
#include <vector>
#include <functional>
#include <exception>
#define MAX_H 20
namespace data_structures {


/*
███████╗██╗  ██╗██╗██████╗ ██╗     ██╗███████╗████████╗
██╔════╝██║ ██╔╝██║██╔══██╗██║     ██║██╔════╝╚══██╔══╝
███████╗█████╔╝ ██║██████╔╝██║     ██║███████╗   ██║
╚════██║██╔═██╗ ██║██╔═══╝ ██║     ██║╚════██║   ██║
███████║██║  ██╗██║██║     ███████╗██║███████║   ██║
╚══════╝╚═╝  ╚═╝╚═╝╚═╝     ╚══════╝╚═╝╚══════╝   ╚═╝

██████╗ ███████╗ ██████╗██╗      █████╗ ██████╗  █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
██╔══██╗██╔════╝██╔════╝██║     ██╔══██╗██╔══██╗██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
██║  ██║█████╗  ██║     ██║     ███████║██████╔╝███████║   ██║   ██║██║   ██║██╔██╗ ██║
██║  ██║██╔══╝  ██║     ██║     ██╔══██║██╔══██╗██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██████╔╝███████╗╚██████╗███████╗██║  ██║██║  ██║██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═════╝ ╚══════╝ ╚═════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
                                                                            */

        template<class T, class K, int MAX_HEIGHT = MAX_H>
        class skip_list
        {

                public:
                        class node
                                        {
                                                friend skip_list<T, K, MAX_HEIGHT>;

                                                private:
                                                        size_t height;
                                                        std::vector<std::shared_ptr<node>> neighbours;
                                                        T& value;

                                                public:
                                                        node(T& value);
                                                        node(size_t height, T& value);
                                                        ~node();
                                                        K get_key();
                                        };

                typedef std::shared_ptr<node> ptr;

                private:
                        std::shared_ptr<node> beginning;
                        std::function<K(T)> get_key_function;
                        static size_t get_random_height();
                        std::shared_ptr<node> get_preceeding(const K& key);
                        std::shared_ptr<node> get_preceeding(K&& key);
                public:
                        skip_list(std::function<K(const T&)> get_key_function);
                        ~skip_list();
                        void insert(T& element);
                        void insert(T&& element);
                        std::pair<bool, T> get_element(const K& key);
                        std::pair<bool, T> get_element(K&& key);

                        T& find_no_smaller_than(const K& key);
                        T& find_no_greater_than(const K& key);
                        void remove(const K& key);
                        void remove(K&& key);
                        void print();

        };
/*
███╗   ██╗ ██████╗ ██████╗ ███████╗     ██████╗██╗      █████╗ ███████╗███████╗
████╗  ██║██╔═══██╗██╔══██╗██╔════╝    ██╔════╝██║     ██╔══██╗██╔════╝██╔════╝
██╔██╗ ██║██║   ██║██║  ██║█████╗      ██║     ██║     ███████║███████╗███████╗
██║╚██╗██║██║   ██║██║  ██║██╔══╝      ██║     ██║     ██╔══██║╚════██║╚════██║
██║ ╚████║╚██████╔╝██████╔╝███████╗    ╚██████╗███████╗██║  ██║███████║███████║
╚═╝  ╚═══╝ ╚═════╝ ╚═════╝ ╚══════╝     ╚═════╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝

██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
 */

        template<class T, class K, int MAX_HEIGHT>
        inline skip_list<T, K, MAX_HEIGHT>::node::node(T & value):value(value), height(get_random_height())
        {
                neighbours = std::vector<ptr>(height + 1, nullptr);
        }

        template<class T, class K, int MAX_HEIGHT>
        inline skip_list<T, K, MAX_HEIGHT>::node::node(size_t height, T & value) : value(value), height(height)
        {
                neighbours = std::vector<ptr>(height + 1, nullptr);
        }

        template<class T, class K, int MAX_HEIGHT>
        inline skip_list<T, K, MAX_HEIGHT>::node::~node()
        {
                this->neighbours.clear();
        }

        template<class T, class K, int MAX_HEIGHT>
        inline K data_structures::skip_list<T, K, MAX_HEIGHT>::node::get_key()
        {
                return get_key_function(this->value);
        }
/*
███████╗██╗  ██╗██╗██████╗ ██╗     ██╗███████╗████████╗
██╔════╝██║ ██╔╝██║██╔══██╗██║     ██║██╔════╝╚══██╔══╝
███████╗█████╔╝ ██║██████╔╝██║     ██║███████╗   ██║
╚════██║██╔═██╗ ██║██╔═══╝ ██║     ██║╚════██║   ██║
███████║██║  ██╗██║██║     ███████╗██║███████║   ██║
╚══════╝╚═╝  ╚═╝╚═╝╚═╝     ╚══════╝╚═╝╚══════╝   ╚═╝

██╗███╗   ███╗██████╗ ██╗     ███████╗███╗   ███╗███████╗███╗   ██╗████████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
██║████╗ ████║██╔══██╗██║     ██╔════╝████╗ ████║██╔════╝████╗  ██║╚══██╔══╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
██║██╔████╔██║██████╔╝██║     █████╗  ██╔████╔██║█████╗  ██╔██╗ ██║   ██║   ███████║   ██║   ██║██║   ██║██╔██╗ ██║
██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══╝  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██║██║ ╚═╝ ██║██║     ███████╗███████╗██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   ██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚══════╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝
 */


        template<class T, class K, int MAX_HEIGHT>
        inline size_t skip_list<T, K, MAX_HEIGHT>::get_random_height()
        {
                        size_t height = 0;
                        std::random_device rd;
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<> dis(0, 1);
                        while (dis(gen) == 1 and height < MAX_HEIGHT)height++;

                        return height;
        }

        template<class t, class k, int max_height>
        inline std::shared_ptr<typename skip_list<t, k, max_height>::node> skip_list<t, k, max_height>::get_preceeding(k&& key)
        {
                std::unique_ptr<typename skip_list<t, k, max_height>::node> prev = this->beginning;
                int level = prev->height;
                ptr current;
                while (level >= 0)
                {
                        current = prev->neighbours[level];

                        while (get_key_function(current->value) < key)
                        {
                                prev = current;
                                current = current->neighbours[level];
                        }
                        level--;

                }
                return current;
        }

        template<class t, class k, int max_height>
        inline std::shared_ptr<typename skip_list<t, k, max_height>::node > skip_list<t, k, max_height>::get_preceeding(const k & key)
        {
                ptr prev = this->beginning;
                int level = prev->height;
                ptr current;
                while (level >= 0)
                {
                        current = prev->neighbours[level];

                        while (get_key_function(current->value) < key)
                        {
                                prev = current;
                                current = current->neighbours[level];
                        }
                        level--;

                }
                return current;
        }



        template<class T, class K, int MAX_HEIGHT>
        inline skip_list<T, K, MAX_HEIGHT>::skip_list(std::function<K(const T&)> get_key_function) :get_key_function(get_key_function)
        {
                ptr min_guard = ptr(new node(MAX_HEIGHT, K::min_value));
                ptr max_guard = ptr(new node(MAX_HEIGHT, K::max_value));
                this->beginning = min_guard;
                for (size_t i = 0; i <= MAX_HEIGHT; i++)
                {
                        min_guard->neighbours[i] = max_guard;
                }
        }


        template<class T, class K, int MAX_HEIGHT>
        inline data_structures::skip_list<T, K, MAX_HEIGHT>::~skip_list()
        {
                ptr current = this->beginning;
                while (current != nullptr)
                {
                        ptr next = current->neighbours[0];
                        current.reset();
                        current = next;
                }
        }


        template<class T, class K, int MAX_HEIGHT>
        inline void skip_list<T, K, MAX_HEIGHT>::insert(T& element)
        {
                K key = get_key_function(element);
                ptr new_node = ptr( new node(element) );
                int level = new_node->height;
                ptr current = this->beginning;
                ptr next;
                while (level >= 0)
                {
                        next = current->neighbours[level];

                        while (get_key_function(next->value) < key)
                        {
                                current = next;
                                next = next->neighbours[level];
                        }
                        current->neighbours[level] = new_node;
                        new_node->neighbours[level] = next;
                        level--;
                }
        }


        template<class T, class K, int MAX_HEIGHT>
        inline void skip_list<T, K, MAX_HEIGHT>::insert(T&& element)
        {
                K key = get_key_function(element);
                ptr new_node = ptr(new node(element));
                int level = new_node->height;
                ptr current = this->beginning;
                ptr next;
                while (level >= 0)
                {
                        next = current->neighbours[level];

                        while (get_key_function(next->value) < key)
                        {
                                current = next;
                                next = next->neighbours[level];
                        }
                        current->neighbours[level] = new_node;
                        new_node->neighbours[level] = next;
                        level--;
                }
        }


        template<class T, class K, int MAX_HEIGHT>
        inline std::pair<bool, T> skip_list<T, K, MAX_HEIGHT>::get_element(const K& key)
        {
                ptr current = get_preceeding(key);
                if (get_key_function(current->value) == key) return std::make_pair(true,current->value);
                else return std::make_pair(false, current->value);
        }


        template<class T, class K, int MAX_HEIGHT>
        inline std::pair<bool, T> skip_list<T, K, MAX_HEIGHT>::get_element(K&& key)
        {
                ptr current = get_preceeding(key);
                if (get_key_function(current->value) == key) return std::make_pair(true,current->value);
                else return std::make_pair(false,current->value);

        }


        template<class T, class K, int MAX_HEIGHT>
        inline void skip_list<T, K, MAX_HEIGHT>::remove(const K & key)
        {
                ptr current = this->beginning;
                int level = current->height;
                ptr next = current->neighbours[level];
                while (level >= 0)
                {

                        while (get_key_function(next->value) < key)
                        {
                                current = next;
                                next = next->neighbours[level];
                        }
                        if (get_key_function(next->value) == key)
                        {
                                current->neighbours[level] = next->neighbours[level];
                                next = current;
                                level--;
                        }
                        else
                        {
                                level--;
                                next = current->neighbours[level];
                        }
                }
        }


        template<class T, class K, int MAX_HEIGHT>
        inline void skip_list<T, K, MAX_HEIGHT>::remove(K && key)
        {
                ptr current = this->beginning;
                int level = current->height;
                ptr next = current->neighbours[level];
                while (level >= 0)
                {

                        while (get_key_function(next->value) < key)
                        {
                                current = next;
                                next = next->neighbours[level];
                        }
                        if (get_key_function(next->value) == key)
                        {
                                current->neighbours[level] = next->neighbours[level];
                                next = current;
                                level--;
                        }
                        else
                        {
                                level--;
                                next = current->neighbours[level];
                        }

                }

        }

        template<class T, class K, int MAX_HEIGHT>
        inline void skip_list<T, K, MAX_HEIGHT>::print(size_t from_level=MAX_HEIGHT)
        {
                for (int level = from_level; level >= 0; level--)
                {
                        auto current = this->beginning;
                        while (current != nullptr)
                        {
                                std::cout << current->value;
                                for (int i = 0; i < 10; i++)
                                {
                                        std::cout << "-";
                                }
                                current = current->neighbours[level];

                        }
                        std::cout << std::endl;
                }
        }

}
