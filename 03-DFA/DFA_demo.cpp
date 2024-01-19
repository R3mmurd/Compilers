#include <iostream>
#include <fstream>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <DFA.hpp>

using fa_type = DFA<char>;

void fill_range(char s0, char s1, int q0, int q1, fa_type& fa) noexcept
{
    for (char s = s0; s <= s1; ++s)
    {
        fa.add_transition(q0, q1, s);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " word\n";
        return EXIT_FAILURE;
    }

    std::vector<std::string> dfa_names{"for", "identifier", "integer"};

    std::vector<fa_type> automata(3);

    // Create automata for RE for
    automata[0].add_transition(0, 1, 'f');
    automata[0].add_transition(1, 2, 'o');
    automata[0].add_transition(2, 3, 'r');
    automata[0].set_initial_state(0);
    automata[0].add_acceptation_state(3);
    
    // Create automata for RE [a-z][a-z0-9]*
    fill_range('a', 'z', 0, 1, automata[1]);
    fill_range('a', 'z', 1, 2, automata[1]);
    fill_range('0', '9', 1, 2, automata[1]);
    fill_range('a', 'z', 2, 2, automata[1]);
    fill_range('0', '9', 2, 2, automata[1]);
    automata[1].set_initial_state(0);
    automata[1].add_acceptation_state(2);

    // Create automata for RE ([1-9][0-9]*)|0
    automata[2].add_transition(0, 3, '0');
    fill_range('1', '9', 0, 1, automata[2]);
    fill_range('0', '9', 1, 2, automata[2]);
    fill_range('0', '9', 2, 2, automata[2]);
    automata[2].set_initial_state(0);
    automata[2].add_acceptation_states({1, 2, 3});

    for (size_t i = 0; i < automata.size(); ++i)
    {
        std::ofstream out{dfa_names[i] + ".dot"};
        automata[i].to_dot(out);
        out.close();
    }

    for (size_t i = 0; i < automata.size(); ++i)
    {
        if (automata[i].match(argv[1]))
        {
            std::cout << argv[1] << " matches with " << dfa_names[i] << std::endl;
            return EXIT_SUCCESS;
        }
    }

    

    std::cout << "Not match found for " << argv[1] << std::endl;
    
    return EXIT_SUCCESS;
}