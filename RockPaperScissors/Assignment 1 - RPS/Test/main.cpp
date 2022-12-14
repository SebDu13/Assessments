#include <iostream>
#include "../PaperRockScissorsEngine.hpp"
#include "acutest.h"
#include "fixture.hpp"
#include <vector>
#include <algorithm>

namespace Test
{

void test_Symbol(void)
{
    using namespace Game;

    TEST_CHECK(Symbol(Symbol::Type::PAPER) < Symbol(Symbol::Type::SCISSORS));
    TEST_CHECK(!(Symbol(Symbol::Type::PAPER) > Symbol(Symbol::Type::SCISSORS)));
    TEST_CHECK(Symbol(Symbol::Type::PAPER) > Symbol(Symbol::Type::ROCK));
    TEST_CHECK(!(Symbol(Symbol::Type::PAPER) < Symbol(Symbol::Type::ROCK)));
    TEST_CHECK(Symbol(Symbol::Type::PAPER) == Symbol(Symbol::Type::PAPER));
    TEST_CHECK(!(Symbol(Symbol::Type::PAPER) != Symbol(Symbol::Type::PAPER)));

    TEST_CHECK(Symbol(Symbol::Type::SCISSORS) < Symbol(Symbol::Type::ROCK));
    TEST_CHECK(!(Symbol(Symbol::Type::SCISSORS) > Symbol(Symbol::Type::ROCK)));
    TEST_CHECK(Symbol(Symbol::Type::SCISSORS) > Symbol(Symbol::Type::PAPER));
    TEST_CHECK(!(Symbol(Symbol::Type::SCISSORS) < Symbol(Symbol::Type::PAPER)));
    TEST_CHECK(Symbol(Symbol::Type::SCISSORS) == Symbol(Symbol::Type::SCISSORS));
    TEST_CHECK(!(Symbol(Symbol::Type::SCISSORS) != Symbol(Symbol::Type::SCISSORS)));

    TEST_CHECK(Symbol(Symbol::Type::ROCK) < Symbol(Symbol::Type::PAPER));
    TEST_CHECK(!(Symbol(Symbol::Type::ROCK) > Symbol(Symbol::Type::PAPER)));
    TEST_CHECK(Symbol(Symbol::Type::ROCK) > Symbol(Symbol::Type::SCISSORS));
    TEST_CHECK(!(Symbol(Symbol::Type::ROCK) < Symbol(Symbol::Type::SCISSORS)));
    TEST_CHECK(Symbol(Symbol::Type::ROCK) == Symbol(Symbol::Type::ROCK));
    TEST_CHECK(!(Symbol(Symbol::Type::ROCK) != Symbol(Symbol::Type::ROCK)));
}

void test_PaperRockScissorsEngine(void)
{
    using namespace Game;

    auto player1 = std::make_shared<Test::PlayerTest>();
    auto player2 = std::make_shared<Test::PlayerTest>();

    TEST_ASSERT(player1 != nullptr);
    TEST_ASSERT(player2 != nullptr);

    TEST_EXCEPTION(PaperRockScissorsEngine(player1, nullptr), std::runtime_error);
    TEST_EXCEPTION(PaperRockScissorsEngine(nullptr, player1), std::runtime_error);
    TEST_EXCEPTION(PaperRockScissorsEngine(nullptr, nullptr), std::runtime_error);


    PaperRockScissorsEngineTest engine(player1, player2);
    
    std::cout.setstate(std::ios_base::failbit);

    std::vector<Symbol::Type> symbols = {Symbol::Type::ROCK, Symbol::Type::PAPER, Symbol::Type::SCISSORS};
    for(int i = 0; i < symbols.size(); ++i)
    {
        for(int j = 0; j < symbols.size(); ++j)
        {
            player1->symbol = symbols[i];
            player2->symbol = symbols[j];

            std::shared_ptr<Test::PlayerTest> winner;
            if(player1->symbol == player2->symbol)
                winner = nullptr;
            else if(player1->symbol > player2->symbol)
                winner = player1;
            else
                winner = player2;

            TEST_CHECK(engine.playRound() == winner);
        }
    }

    TEST_CHECK(player1->score == 3);
    TEST_CHECK(player2->score == 3);

    player1->symbol = Symbol::Type::ROCK;
    player2->symbol = Symbol::Type::SCISSORS;
    player1->score = 0;
    player2->score = 0;
    engine.run(5);
    TEST_CHECK(player1->score == 5);
    TEST_CHECK(player2->score == 0);

    std::cout.clear();
}
}

TEST_LIST = {
   { "Symbol", Test::test_Symbol },
   { "PaperRockScissorsEngine", Test::test_PaperRockScissorsEngine },
   { NULL, NULL }     /* zeroed record marking the end of the list */
};
