// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"
//#inclue "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{


    Super::BeginPlay();

    int32 Number = 9;
    FBullCoWCount Count;


    Isograms = GetValidWords(Words);

    SetUpGame(); //function to set up game

    //PrintLine(TEXT("The HiddenWord is: %s"), *HiddenWord); // testing what the hidden number is

    PrintLine(TEXT("Welcome to Bull & Cows Game!"));
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());




}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    ClearScreen();
    ProcessGuess(Input);


}

void UBullCowCartridge::SetUpGame()
{
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{

    //if game is over, clearscreen() and redo SetupGame()
    if(bGameOver)
    {
        ClearScreen();
        SetUpGame();
        return;
    }

    //winning the game
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("You have won the game!"));
        EndGame();
        return;
    }

    if ( HiddenWord.Len() != Guess.Len())
    {
        PrintLine(TEXT("Incorrect length"));
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    Lives--;
    PrintLine(TEXT("Incorrect guess, you have lost a life"));

    //if no lives left
    if (Lives <= 0) {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
        EndGame();
    }

    //Show the player Bulls and Cows
    FBullCoWCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{

    for(int32 i = 0; i < Word.Len() - 1; i++)
    {
        for(int32 j = i+1; j < Word.Len(); j++)
        {
            if(Word[i] == Word[j])
            {
                return false;
            }
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList)
{
    TArray<FString> ValidWords;

    for (FString index : WordList)
    {
        if (index.Len() >= 4 && index.Len() <= 8 && IsIsogram(index))
            ValidWords.Emplace(index);
    }

    return ValidWords;
}

FBullCoWCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCoWCount Count;


    //if index Guess is same as index Hidden, Bullcount++
    for (int32 GIndex = 0; GIndex < Guess.Len(); GIndex++)
    {
        if (Guess[GIndex] == HiddenWord[GIndex])
        {
            Count.Bulls++;
            continue;
        }

        for(int32 HIndex = 0; HIndex < HiddenWord.Len(); HIndex++)
        {
            if (Guess[GIndex] == HiddenWord[HIndex])
            {
                Count.Cows++;
                break;
            }
        }

    }

    return Count;

}