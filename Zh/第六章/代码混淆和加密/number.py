def getInput():
    return int(input("Enter a number: "))

def main():
    secretNumber = 42
    guess = getInput()
    if guess == secretNumber:
        print("Congratulations! You guessed the secret number!")
    else:
        print("Sorry, wrong guess!")

if __name__ == "__main__":
    main()
