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

    DFA(std::string_view state_prefix = "s") noexcept
        : prefix{state_prefix}
    {

    }

    void set_initial_state(size_t state) noexcept
    {
        initial_state = to_state(state);
    }

    void add_acceptation_state(size_t state)
    {
        acceptation_state_set.insert(to_state(state));
    }

    void add_acceptation_states(std::initializer_list<size_t> states)
    {
        for (auto state: states)
        {
            acceptation_state_set.insert(to_state(state));
        }
    }

    void add_transition(size_t state0, size_t state1, symbol_type symbol) noexcept
    {
        auto s0 = to_state(state0);
        auto s1 = to_state(state1);
        state_set.emplace(s0);
        state_set.emplace(s1);
        transition_table.emplace(std::make_pair(s0, symbol), s1);
    }

    bool match(word_type word) noexcept
    {
        return ext_delta(initial_state, word).first;
    }

    void to_dot(std::ostream& output) noexcept
    {
        output << "digraph\n{\n";
        
        output << "  rankdir = LR;\n\n";

        for (const auto& state: state_set)
        {
            output << "  " << state << "[shape = ";

            if (acceptation_state_set.find(state) != acceptation_state_set.end())
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
    std::string to_state(size_t state) noexcept
    {
        return prefix + std::to_string(state);
    }

    result_type delta(const state_type& state, symbol_type symbol) noexcept
    {
        auto it = transition_table.find(std::make_pair(state, symbol));

        if (it == transition_table.end())
        {
            return std::make_pair(false, "");
        }
        
        return std::make_pair(true, it->second);
    }

    result_type ext_delta(const state_type& state, word_type word)
    {
        if (word.size() == 0)
        {
            return std::make_pair(true, state);
        }

        symbol_type symbol = word[word.size() - 1];
        word_type remaining_word = word.substr(0, word.size() - 1);

        result_type result = ext_delta(state, remaining_word);

        if (!result.first)
        {
            return result;
        }

        return delta(result.second, symbol);
    }

    state_set_type state_set;
    transition_table_type transition_table;
    state_set_type acceptation_state_set;
    state_type initial_state = "s0";
    const std::string prefix;
};