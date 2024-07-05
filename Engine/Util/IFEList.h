#pragma once
#include <list>
#include <iterator>
#include <algorithm>
#include <stdexcept>

namespace IFE
{
    namespace List
    {
        // 2�̃C�e���[�^���w���v�f����������֐�
        template<typename T>
        void Swap(typename std::list<T>::iterator it1, typename std::list<T>::iterator it2) {
            if (it1 == it2) return; // �����C�e���[�^�Ȃ牽�����Ȃ�

            // �v�f������
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

            // �ړ����̃C�e���[�^�擾
            auto it_from = std::next(lst.begin(), from_index);

            // �ړ���̃C�e���[�^�擾
            auto it_to = std::next(lst.begin(), to_index);

            // �v�f���ړ�
            lst.splice(it_to, lst, it_from);
        }
    }
}