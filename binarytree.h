#ifndef BTREE_H
#define BTREE_H

#include <ostream>
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
 * @brief tentativo di aggiunta di un nodo esistente
 * 
 */
class ExistingNodeException : public std::exception
{
    virtual const char *what() const throw()
    {
        return "Impossibile aggiungere il nodo::valore già esistente";
    }
};
class UnmatchableException : public std::exception {
    virtual const char *what() const throw()
    {
        return "Impossibile confrontare il dato, problema nella definizione del funtore di comparazine/eguaglianza!";
    }
};

/**
 * @brief Classe templata binarytree per un albero binario di ricerca (BST)
 * 
 * @tparam T Tipo di valori del BST
 * @tparam C funtore di comparazione per dati di tipo T
 * @tparam E funtore di eguaglianza per dati di tipo T
 */
template <typename T, typename C, typename E>
class binarytree
{
    /**
     * @brief struct che implemente un nodo dell'albero
     * 
     */
    struct node
    {
        /**
         * @brief valore contenuto nel nodo
         * 
         */
        const T value;
        /**
         * @brief puntatore al figlio sinistro
         * 
         */
        node *left;
        /**
         * @brief puntatore al figlio destro
         * 
         */
        node *right;
        /**
         * @brief puntatore al parent
         * 
         */
        node *parent;
        /**
         * @brief puntatore al nodo logicamente successivo
         * 
         */
        node *next;
        /**
         * @brief Costruttore di un nodo vuoto, 
         * @deprecated Non è possibile avere nodi vuoti.
         */
        //node() : left(nullptr), right(nullptr), parent(nullptr), next(nullptr) {}
        /**
         * @brief Costruttore di un nodo con valore v di tipo T
         * 
         * @param v dato contenuto nel nodo, di tipo T
         */
        explicit node(const T &v) : value(v), left(nullptr), right(nullptr), parent(nullptr), next(nullptr) {}
        /**
         * @brief Distruttore del nodo
         * 
         */
        ~node()
        {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            next = nullptr;
        }
    }; // node struct

    /**
     * @brief puntatore alla root del BST
     * 
     */
    node *_root;
    /**
     * @brief uint contenente la size dell'albero
     * 
     */
    uint _size;
    /**
     * @brief funtore di comparazione
     * 
     */
    C compare_t;
    /**
     * @brief funtore di eguaglianza
     * 
     */
    E equal_t;

    /**
     * @brief Dato un nodo in un BST, ne ritorna il successore logico
     * 
     * @param n         nodo in ingresso
     * @return node*    puntatore al nodo successivo
     */
    node *next(node *n)
    {
        //node *n = first;
        node *max = nullptr;
        if (n->right == nullptr)
        {
            while (n != nullptr and n->parent != nullptr)
            {
                if (compare_t(n->value, n->parent->value))
                {
                    n = n->parent;
                    return n;
                }
                else if (compare_t(n->parent->value, n->value))
                {
                    max = n;
                    n = n->parent;
                }
            }
            if (max != nullptr)
            {
                if (compare_t(n->value, max->value))
                { // controllo per il last element
                    return nullptr;
                }
                else
                    return n;
            }
            else
            {
                return nullptr;
            }
        }
        else
        {
            n = n->right;
            while (n->left != nullptr)
            {
                n = n->left;
            }
            return n;
        }
    }

    /**
     * @brief partendo dal nodo minimo di un BST, aggiorna il puntatore a next di ogni nodo
     * 
     */
    void nexter()
    {
        node *n = min();
        while (n != nullptr)
        {
            n->next = next(n);
            // std::cout << "nexting chain_> " << n->value << " ";
            n = n->next;
        }
    }

    /**
     * @brief cancellazione ricorsiva di un nodo e i suoi figli
     * 
     * @param n nodo padre da cancellare
     */
    void trim(const node *n)
    {
        if (n != nullptr)
        {
            trim(n->left);  // lancia la trim sul LeftTree
            trim(n->right); // lancia la trim sul RightTree
            delete n;
            n = nullptr;
            _size--;
        }
    }

    /**
     * @brief cancella ricorsivamente l'albero, chiamando trim(_root)
     * 
     */
    void trim_tree()
    {
        trim(_root);
    }

    /**
     * @brief helper ricorsivo per la copia di un albero
     * 
     * @param n nodo padre da copiare
     */
    void copy_helper(const node *n)
    {
            if (n != nullptr)
            {
                add(n->value);
                copy_helper(n->left);
                copy_helper(n->right);
            }
    }

    /**
     * @brief helper ricorsivo per la stampa IN ORDER di un albero
     * 
     * @param n nodo padre da stampare
     */
    void ptree_helper(const node *n) const
    {
        if (n != nullptr)
        {
            std::cout << " ( ";
            ptree_helper(n->left);
            std::cout << n->value;
            ptree_helper(n->right);
            std::cout << " ) ";
        }
    }

    /**
     * @brief helper ricorsivo per una stampa PREORDER VERBOSA di un albero
     * 
     * @param n nodo padre da stampare
     */
    void verbose_ptree_helper(const node *n) const
    {
        if (n != nullptr)
        {
            std::cout << " ( " << n->value << " ) --> ";
            if (n->left != nullptr)
                std::cout << "l:" << n->left->value << " __ ";
            if (n->right != nullptr)
                std::cout << "r:" << n->right->value << " __ ";
            if (n->parent != nullptr)
                std::cout << "parent:" << n->parent->value;
            std::cout << std::endl;
            verbose_ptree_helper(n->left);
            verbose_ptree_helper(n->right);
        }
    }

    /**
     * @brief calcola il minimo dell'albero
     *  
     * @return node*  puntatore al nodo minimo
     */
    node *min() const
    {

            if (_root == nullptr)
                return nullptr;
            node *n = _root;
            while (n->left != nullptr)
            {
                n = n->left;
            }
            return n;

    }

public:
    /**
     * @brief Construttore di default di un binarytree
     * 
     */
    binarytree() : _root(nullptr), _size(0) {}
    /**
     * @brief Costruttore di un binarytree con radice
     * 
     * @param element valore del nodo di _root
     */
    explicit binarytree(const T &element) : _root(nullptr), _size(0) { add(element); }
    /**
     * @brief overload dell'operatore di copia
     * 
     * @param other altro binarytree da copiare
     */
    binarytree(const binarytree &other) : _root(nullptr), _size(0)
    {

        node *n = other._root;
        try
        {
            copy_helper(n);
        }
        catch (...)
        {
            trim_tree();
            throw;
        }
    }
    /**
     * @brief overload dell'operatore di assegnamento
     * 
     * @param other altro binarytree da assegnare
     * @return binarytree& 
     */
    binarytree &operator=(const binarytree &other)
    {
        if (this != &other)
        {
            binarytree tmp(other);
            std::swap(_root, tmp._root);
            std::swap(_size, tmp._size);
        }
        return *this;
    }
    /**
     * @brief Distruttore del binarytree
     * 
     */
    ~binarytree()
    {
        trim_tree();
    }
    /**
     * @brief getter per la _size del binarytree
     * 
     * @return uint size del binarytree
     */
    uint size() const
    {
        return _size;
    }

    /**
     * @brief Controlla l'esistenza di un elemento in un BST
     * 
     * @param element elemento da controllare (per valore)
     * @return true se l'elemento è gia presente nel BST
     * @return false altrimenti
     * @throw UnmatchableException Se non riesco a confrontare il dato in ingresso
     */
    bool exist(const T &element) const
    {
        node *curr = _root;
        while (curr != nullptr)
        {
            if (equal_t(curr->value, element))
            {
                return true;
            }
            else if (compare_t(curr->value, element))
            {
                curr = curr->right;
            }
            else if (compare_t(element, curr->value))
            {
                curr = curr->left;
            }
            else throw UnmatchableException{};
        }
        return false;
    }
    /**
     * @brief Permetta l'aggiunta per valore di un nodo al BST
     * 
     * @param element   elemento d aggiungere
     * @throw ExistingNodeException() aggiunta di un nodo già esistente
     * @throw UnmatchableException() se non riesco a controntare il dato in ingresso
     */
    void add(const T &element)
    {
        node *nd = new node(element);
        bool dir; //1 right 0 left
        if (_root == nullptr)
        {
            _root = nd;
            _size++;
            return;
        }
        node *curr = _root;
        node *prev = _root;
        while (curr != nullptr)
        {
            if (equal_t(curr->value, element))
            {
                delete nd;
                nd = nullptr;
                trim_tree();
                throw ExistingNodeException{};
            }
            else if (compare_t(curr->value, element))
            {
                prev = curr;
                curr = curr->right;
                
                dir = true; //right
            }
            else if (compare_t(element, curr->value))
            {
                prev = curr;
                curr = curr->left;
                dir = false; //left
            }
            else {
                trim_tree();
                throw UnmatchableException{};
            }
        }

        if (dir == true) //right
        {
            prev->right = nd;
            nd->parent = prev;
        }
        else //left
        {
            prev->left = nd;
            nd->parent = prev;
        }
        _size++;
        nexter();
    }

    /**
     * @brief Stampa INORDER il BST (parentesizzato)
     * 
     */
    void ptree() const
    {
        node *n = _root;
        if (_root == nullptr)
            std::cout << "No root found!" << std::endl;
        else
        {
            std::cout << "Tree root: " << n->value << " | Tree size: " << size() << std::endl;
            ptree_helper(n);
            std::cout << std::endl
                      << " ----------------------------- " << std::endl;
        }
    }

    /**
     * @brief Stampa verbosamente PREORDER il BST
     * 
     */
    void ptree_verbose() const
    {
        node *n = _root;
        if (_root == nullptr)
            std::cout << "No root found!" << std::endl;
        else
        {
            std::cout << "Tree root: " << n->value << " | Tree size: " << size() << std::endl;
            verbose_ptree_helper(n);
            std::cout << " ----------------------------- " << std::endl;
        }
    }

    /**
     * @brief Permette di trovare un sotto-albero di un BST, a partire dal valore di un nodo del BST
     * 
     * @param ovalue valore radice del sotto-albero
     * @return binarytree<T, C, E> sub-BST del BST corrente
     * !!!    il BST di ritorno potrebbe essere vuoto se !(exist(ovalue)) nel BST  !!!
     */
    binarytree<T, C, E> subtree(T ovalue)
    {
        binarytree<T, C, E> temp;
        if (exist(ovalue))
        {
            node *curr = _root;
            while (curr != nullptr)
            {
                if (equal_t(curr->value, ovalue))
                {
                    try {
                    temp.copy_helper(curr);
                    } catch(...) {
                        temp.trim_tree();
                        throw ExistingNodeException{};
                        }
                    return temp;
                }
                else if (compare_t(curr->value, ovalue))
                {
                    curr = curr->right;
                }
                else if (compare_t(ovalue, curr->value))
                {
                    curr = curr->left;
                }
            }
        }
        return temp;
    }

    /**
     * @brief iteratore di sola lettura sui nodi di un binarytree
     * 
     */
    class const_iterator
    {
        /**
         * @brief permette l'accesso ai membri privati di binarytree
         * 
         */
        friend class binarytree;

        /**
         * @brief puntatore a nodo constatnte (accesso in sola lettura)
         * 
         */
        const node *_node;

    public:
        typedef std::forward_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef const T *pointer;
        typedef const T &reference;
        /**
         * @brief Costruttore di deafult per il const_iterator
         * 
         */
        const_iterator() : _node(nullptr) {}
        /**
         * @brief overload operatore di copia
         * 
         * @param other altro const_iterator
         */
        const_iterator(const const_iterator &other) : _node(other._node) {}
        /**
         * @brief overload operatore di assegnamento
         * 
         * @param other altro const_iterator
         * @return const_iterator& 
         */
        const_iterator &operator=(const const_iterator &other)
        {
            _node = other._node;
            return *this;
        }
        /**
         * @brief Distruttore di default
         * 
         */
        ~const_iterator()
        {
        }
        /**
         * @brief operatore di /accesso
         * 
         * @return reference 
         */
        reference operator*() const
        {
            return _node->value;
        }

        pointer operator->() const
        {
            return &(_node->value);
        }
        /**
         * @brief operatore post incremento
         * 
         * @param dummy 
         * @return const_iterator 
         */
        const_iterator operator++(int dummy)
        {
            const_iterator tmp(*this);
            _node = _node->next;
            return tmp;
        }
        /**
         * @brief operatore pre incremento
         * 
         * @return const_iterator& 
         */
        const_iterator &operator++()
        {
            _node = _node->next;
            return *this;
        }
        /**
         * @brief operatore di eguaglianza
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator==(const const_iterator &other) const
        {
            return (_node == other._node);
        }
        /**
         * @brief operatore di disuguaglianza
         * 
         * @param other 
         * @return true 
         * @return false 
         */
        bool operator!=(const const_iterator &other) const
        {
            return (_node != other._node);
        }

    private:
        /**
         * @brief costruttore privato per la costruzione di un const_iteratore a partire da un *nodo
         * 
         * @param n 
         */
        explicit const_iterator(const node *n) : _node(n) {}
    };

    /**
     * @brief Operatore di begin() per il const_iterator
     * 
     * @return const_iterator puntatore al min del BST
     */
    const_iterator begin() const
    {
        return const_iterator(min());
    }

    /**
     * @brief Operatore di end() per il const_iterator
     * 
     * @return const_iterator nullptr
     */
    const_iterator end() const
    {
        return const_iterator(nullptr);
    }
};

/**
 * @brief Overload dell'operatore di stream per la classe templata binarytree
 * 
 * @tparam T Tipo di valori del BST
 * @tparam C funtore di comparazione per dati di tipo T
 * @tparam E funtore di eguaglianza per dati di tipo T
 * @param os std::ostream
 * @param bt binarytree
 * @return std::ostream& os
 */
template <typename T, typename C, typename E>
std::ostream &operator<<(std::ostream &os,
                         const binarytree<T, C, E> &bt)
{
    typename binarytree<T, C, E>::const_iterator i, ie;
    i = bt.begin();
    ie = bt.end();
    while (i != ie)
    {
        os << *i << " ";
        ++i;
    }
    return os;
}

/**
 * @brief Funzione globale che stampa i valori di un binarytree che soddisfano il predicato
 * 
 * @tparam T Tipo di valori del BST
 * @tparam C funtore di comparazione per dati di tipo T
 * @tparam E funtore di eguaglianza per dati di tipo T
 * @tparam P funtore predicato per dati di tipo T
 * @param bt BST in input
 * @param predicate predicato da controllare
 */
template <typename T, typename C, typename E, typename P>
void printif(const binarytree<T, C, E> &bt, P predicate)
{

    typename binarytree<T, C, E>::const_iterator i, ie;

    i = bt.begin();
    ie = bt.end();

    while (i != ie)
    {
        if (predicate(*i))
        {
            std::cout << *i << " -- ";
        }
        ++i;
    }
    std::cout << " satisfy the predicate." << std::endl;
}

#endif