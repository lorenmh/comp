#include "lexertl/generator.hpp"
#include <iostream>
#include "lexertl/lookup.hpp"
#include "lexertl/match_results.hpp"

int main()
{
    lexertl::rules rules;
    lexertl::state_machine sm;

    rules.push("[0-9]+", 1);
    rules.push("[a-z]+", 2);
    lexertl::generator::build(rules, sm);

    std::string input("abc012Ad3e4");
    lexertl::smatch results(input.begin(), input.end());

    // Read ahead
    lexertl::lookup(sm, results);

    std::cout << "npos: " << results.npos() << '\n';

    while (results.id != 0)
    {
        std::cout << "Id: " << results.id << ", Token: '" <<
            results.str () << "'\n";
        lexertl::lookup(sm, results);
    }

    return 0;
}
