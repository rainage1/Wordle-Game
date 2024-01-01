#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "WordleGameApplication.h"

/**
 * Author:    Raina Ge
 * Created:   10/01/2023
 *
 * This file contains the logic behind the Wordle game and uses WT to create a UI.
 **/

// Function name, description, parameter descriptions, return description.

/**
 * WordleApp
 * This is the constructor that creates the Wordle game by calling on the setUpGame function
 * env: Reference to the type Wt::WEnvironment to access information about the environment the application is running on
 * return: NA
 **/
WordleApp::WordleApp(const Wt::WEnvironment& env) : Wt::WApplication(env) {
    setTitle("Wordle Game");
    setupGame();
}

/**
 * setUpGame
 * This function creates the UI and calls on the function handleGuess once user inputs their guess
 * return: NA
 **/
void WordleApp::setupGame() {
	//Calls on loadWordList to load the valid words
    std::vector<std::string> validWords;
    loadWordList(validWords);

    //Chooses a random word from the list of words
    srand(time(0));
    int randomIndex = rand() % validWords.size();
    targetWord_ = validWords[randomIndex];

    remainingAttempts_ = 6;

    //Prints the previous guesses
    previousGuessesOutput_ = root()->addWidget(std::make_unique<Wt::WText>());

    std::string hintMessage = "Enter guess, 5 letters maximum: ";
    //User inputs guess of maximum length 5
    hintOutput_ = root()->addWidget(std::make_unique<Wt::WText>(hintMessage));
    guessInput_ = root()->addWidget(std::make_unique<Wt::WLineEdit>());
    guessInput_->setFocus();
    guessInput_->setMaxLength(5);

    guessButton_ = root()->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    // Calls on handleGuess function if guess button is pressed.
    guessButton_->clicked().connect(this, &WordleApp::handleGuess);
    guessButton_->setMargin(5, Wt::Side::Left);

    root()->addWidget(std::make_unique<Wt::WBreak>());
}

/**
 * loadWordList
 * This function accesses the file containing the list of words and adds it to an array if the word is 5 letters long
 * validWords: Reference to the array of words that the wordle game can choose from
 * return: NA
 **/
void WordleApp::loadWordList(std::vector<std::string>& validWords) {
    const std::string filename = "word_list.txt";

    std::ifstream wordFile(filename);
    if (!wordFile.is_open()) {
        std::cerr << "Error: Unable to open " << filename << std::endl;
        return;
    }

    std::string word;
    while (std::getline(wordFile, word)) {
        if (word.length() == 5)
            validWords.push_back(word);
    }

    wordFile.close();
}

/**
 * handleGuess
 * This function compares the user guess to the target word and displays the letters of the guess a grey, yellow, or green in the UI.
 * return: NA
 **/
void WordleApp::handleGuess() {

    std::string guess = guessInput_->text().toUTF8();
    std::cout << "Guessed word: " << guess << std::endl;

    //User is out of guesses
    if (remainingAttempts_ == 0) {
        hintOutput_->setText("You are out of guesses! Play again?");
        guessInput_->setDisabled(true);
        return;
    }

    if (guess.length() != 5) {
        return;
    }

    std::string hint = colouredHint(guess);
    remainingAttempts_--;

    // Store the guess and its hint
    previousGuesses_.emplace_back(guess, hint);

    // Update the display of previous guesses
    displayPreviousGuesses();

    //User guessed the word
    if (toUpperCase(guess).compare(targetWord_) == 0) {
        hintOutput_->setText("You guessed correctly! Play again?");
        guessButton_ -> setText("Restart");
        guessButton_ -> setDisabled(true);
        guessInput_->setDisabled(true);
        return;
    }
}

/**
 * toUpperCase
 * This function changes the user guess to uppercase
 * str: Reference variable of the user guess
 * return: string of the user guess in upper case
 **/
std::string WordleApp::toUpperCase(const std::string& str) {
    std::string upper;
    for (char c : str) {
        upper += std::toupper(c);
    }
    return upper;
}

/**
 * colouredHint
 * This function takes the user guess and compares it to the target word. If the letter is in the right spot, the letter becomes green, grey if the letter is in the wrong spot, red if the letter is not in the word.
 * validWords: Reference to the user guess
 * return: string of the guess with the coloured letters
 **/

std::string WordleApp::colouredHint(const std::string& guess) {
    std::string colouredHint;

    //Gets the user guess in upper case
    std::string upperGuess = toUpperCase(guess);

    //Loops through each letter to check if it equals the target letter or if the letter is in the word
    for (size_t i = 0; i < targetWord_.length(); ++i) {
        char targetChar = targetWord_[i];
        char guessChar = upperGuess[i];

        std::string colourClass;
        // Determines the colour of the letter
        if (guessChar == targetChar)
            colourClass = "green";
        else if (targetWord_.find(guessChar) != std::string::npos)
            colourClass = "grey";
        else
            colourClass = "red";
        colouredHint += "<span style='colour: " + colourClass + "'>" + guessChar + "</span>";
    }

    return colouredHint;
}

/**
 * generatecolouredHint
 * This function displays the user's previous guesses in the UI
 * return: NA
 **/
void WordleApp::displayPreviousGuesses() {
    previousGuessesOutput_->setText("");

    // Displays each coloured hint
    for (const auto& guessWithHint : previousGuesses_) {
        const std::string& guess = guessWithHint.first;
        const std::string& hint = guessWithHint.second;
        std::string guessDisplay = "<div>" + hint + "</div>";
        previousGuessesOutput_->setText(previousGuessesOutput_->text() + guessDisplay);
    }
}
