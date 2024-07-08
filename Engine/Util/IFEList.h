#pragma once
#include <list>
#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace IFE
{
    namespace List
    {
        // 2つのイテレータが指す要素を交換する関数
        template<typename T>
        void Swap(typename std::list<T>::iterator it1, typename std::list<T>::iterator it2) {
            if (it1 == it2) return; // 同じイテレータなら何もしない

            // 要素を交換
            std::iter_swap(it1, it2);
        }

        template<typename T>
        std::list<T>::iterator GetIteratorAt(std::list<T>& lst, size_t index) {
            if (index >= lst.size()) {
                throw std::out_of_range("Index out of range");
            }
            return std::next(lst.begin(), index);
        }


        template<typename T>
        void MoveElement(std::list<T>& lst, size_t from_index, size_t to_index) {
            if (from_index >= lst.size() || to_index > lst.size()) {
                throw std::out_of_range("Index out of range");
            }

            // 移動元のイテレータ取得
            auto it_from = std::next(lst.begin(), from_index);

            // 移動先のイテレータ取得
            auto it_to = std::next(lst.begin(), to_index);

            // 要素を移動
            lst.splice(it_to, lst, it_from);
        }
    }
}