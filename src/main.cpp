#include <Wt/WApplication.h>
#include "WordleGameApplication.h"

/**
 * Author:    Raina Ge
 * Created:   10/01/2023
 *
 * This file contains the main which creates a Wordle Game.
 **/
int main(int argc, char **argv) {
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
        return std::make_unique<WordleApp>(env);
    });
}
