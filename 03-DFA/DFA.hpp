#pragma once

#include <cstdint>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

/** Unsafe implementation of a finite automaton. 
 * 
 * This is unsafe because it is up to the user to ensure that the state
 * numbers to set as initial state and acceptation states should be part
 * of the transition table.
 */
template <typename SymbolT, typename SymbolHash = std::hash<SymbolT>>
class DFA
{
public:
    using state_type  = std::string;
    using symbol_type = SymbolT;
    using pair_type   = std::pair<state_type, symbol_type>;
    using result_type = std::pair<bool, state_type>;
    using word_type   = std::basic_string_view<symbol_type>;

    struct PairHash
    {
        size_t operator () (const pair_type& p) const noexcept
        {
            return std::hash<state_type>{}(p.first) ^ SymbolHash{}(p.second);
        }
    };

    using transition_table_type = std::unordered_map<pair_type, state_type, PairHash>;
    using state_set_type        = std::unordered_set<state_type>;

    DFA(std::string_view state_prefix = "q") noexcept
        : prefix{state_prefix}
    {

    }

    void set_initial_state(size_t q) noexcept
    {
        q0 = to_state(q);
    }

    void add_acceptation_state(size_t q)
    {
        f_set.insert(to_state(q));
    }

    void add_acceptation_states(std::initializer_list<size_t> qs)
    {
        for (auto q: qs)
        {
            f_set.insert(to_state(q));
        }
    }

    void add_transition(size_t q0, size_t q1, symbol_type s) noexcept
    {
        auto s0 = to_state(q0);
        auto s1 = to_state(q1);
        s_set.emplace(s0);
        s_set.emplace(s1);
        transition_table.emplace(std::make_pair(s0, s), s1);
    }

    bool match(word_type w) noexcept
    {
        return ext_delta(q0, w).first;
    }

    void to_dot(std::ostream& output) noexcept
    {
        output << "digraph\n{\n";
        
        output << "  rankdir = LR;\n\n";

        for (const auto& p: s_set)
        {
            output << "  " << p << "[shape = ";

            if (f_set.find(p) != f_set.end())
            {
                output << "double";
            }
            
            output << "circle];\n";
        }

        for (const auto& p: transition_table)
        {
            output << "  " << p.first.first << " -> " << p.second  
                   << "[label = \"" << p.first.second << "\"]\n";
        }

        output << "}";
    }

private:
    std::string to_state(size_t s) noexcept
    {
        return prefix + std::to_string(s);
    }

    result_type delta(const state_type& q, symbol_type s) noexcept
    {
        auto it = transition_table.find(std::make_pair(q, s));

        if (it == transition_table.end())
        {
            return std::make_pair(false, "");
        }
        
        return std::make_pair(true, it->second);
    }

    result_type ext_delta(const state_type& q, word_type w)
    {
        if (w.size() == 0)
        {
            return std::make_pair(true, q);
        }

        symbol_type a = w[w.size() - 1];
        word_type rw = w.substr(0, w.size() - 1);

        result_type result = ext_delta(q, rw);

        if (!result.first)
        {
            return result;
        }

        return delta(result.second, a);
    }

    state_set_type s_set;
    transition_table_type transition_table;
    state_set_type f_set;
    state_type q0 = "q0";
    const std::string prefix;
};