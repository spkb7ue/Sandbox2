#pragma once

namespace rabbit
{

/**
* @brief A binary node which can be used to build a tree data structure.
* Required for partitioning the 3D model into bounding volume hierarchies.
* @tparam TA simply binary node holding data of type T
*/
template <typename T>
class BNode
{
public:

    BNode(const T& data, BNode<T>* parent = nullptr);
    void SetLeft(BNode<T>* left);
    void SetRight(BNode<T>* right);
    void SetChildren(BNode<T>* left, BNode<T>* right);
	void SetParent(BNode<T>* parent){ m_parent = parent; }
    BNode<T>* GetParent()const;
    BNode<T>* GetLeft()const;
    BNode<T>* GetRight()const;
    const T& Data()const;
    T& Data();

private:

  T m_data;
  BNode<T> *m_parent;
  BNode<T> *m_left;
  BNode<T> *m_right;
};

template<typename T>
void BNode<T>::SetChildren(BNode<T>* left, BNode<T>* right)
{
    m_left = left;
    m_right = right;
}

template<typename T>
BNode<T>::BNode(const T& data, BNode<T>* parent):
    m_data(data),
    m_parent(parent),
    m_left(nullptr),
    m_right(nullptr){}

template<typename T>
void BNode<T>::SetLeft(BNode<T>* left){
    m_left = left;
}

template<typename T>
void BNode<T>::SetRight(BNode<T>* right){
    m_right = right;
}

template<typename T>
BNode<T>* BNode<T>::GetParent()const{
    return m_parent;
}

template<typename T>
BNode<T>* BNode<T>::GetLeft()const{
  return m_left;
}

template<typename T>
BNode<T>* BNode<T>::GetRight()const{
  return m_right;
}

template<typename T>
const T& BNode<T>::Data()const{
    return m_data;
}

template<typename T>
T& BNode<T>::Data(){
    return m_data;
}


} // namespace rabbit
