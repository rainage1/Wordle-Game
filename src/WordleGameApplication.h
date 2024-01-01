#ifndef WORDLE_APP_H
#define WORDLE_APP_H

#include <Wt/WApplication.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <vector>

class WordleApp : public Wt::WApplication {
public:
    WordleApp(const Wt::WEnvironment& env);
    void loadWordList(std::vector<std::string>& validWords);
    void setupGame();
    void handleGuess();
    std::string toUpperCase(const std::string& str);
    std::string colouredHint(const std::string& guess);
    void displayPreviousGuesses();

private:
    std::string targetWord_;
    int remainingAttempts_;
    Wt::WLineEdit* guessInput_;
    Wt::WText* hintOutput_;
    Wt::WText* previousGuessesOutput_;
    Wt::WPushButton* guessButton_;
    std::vector<std::pair<std::string, std::string>> previousGuesses_;
};

#endif // WORDLE_APP_H
