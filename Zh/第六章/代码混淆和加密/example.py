def check_number(x):
    if x > 0:
        print("Positive")
    elif x == 0:
        print("Zero")
    else:
        print("Negative")

if __name__ == "__main__":
    number = int(input("Enter a number: "))
    check_number(number)
