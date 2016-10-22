//
// Created by aleksandr on 20.10.16.
//

#ifndef PROOFCHECKER_HASH_H
#define PROOFCHECKER_HASH_H

#include "ast.h"
#include <functional>

namespace hash_numbers
{
    const size_t neg_number = 484459;
    const size_t var_number = 616129;
    const std::pair<size_t, size_t> conj_numbers(762491, 761003);
    const std::pair<size_t, size_t> disj_numbers(1022179, 1033289);
    const std::pair<size_t, size_t> impl_numbers(1047587, 1251529);
}

namespace std
{
    template<>
    struct hash<ast>
    {
    public:
        size_t operator()(const ast& x) const
        {
            return recursive_hash_counter(x.root);
        }
    private:
        size_t recursive_hash_counter(std::shared_ptr<parser::node> nod) const
        {
            if (nod->left != NULL && nod->right != NULL)
            {
                //bin op
                switch (nod->type)
                {
                    case conj:
                        return recursive_hash_counter(nod->left) * hash_numbers::conj_numbers.first +
                               recursive_hash_counter(nod->right) * hash_numbers::conj_numbers.second;
                    case disj:
                        return recursive_hash_counter(nod->left) * hash_numbers::disj_numbers.first +
                               recursive_hash_counter(nod->right) * hash_numbers::disj_numbers.second;
                    case impl:
                        return recursive_hash_counter(nod->left) * hash_numbers::impl_numbers.first +
                               recursive_hash_counter(nod->right) * hash_numbers::impl_numbers.second;
                    default:
                        assert(false);
                        break;
                }
            }
            else if (nod->left != NULL)
            {
                //neg
                return recursive_hash_counter(nod->left) * hash_numbers::neg_number;
            } else
            {
                //var, leave
                return hash<std::string>{}(nod->var_name) * hash_numbers::var_number;
            }
        }
    };
}

#endif //PROOFCHECKER_HASH_H
