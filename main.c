//MAIN CODE
// Program 1
// Quiz Engine - Final Project: a file-based multiple-choice quiz. // States the program's purpose.
// Questions are shuffled, answered once, scored, reviewed, and summarized. // Summarizes the quiz steps.

#include <stdio.h>  // Provides file and console input/output functions.
#include <stdlib.h> // Provides random-number functions.
#include <stdbool.h> // Provides the bool, true, and false types.

int load_questions(char question[][200], char choiceA[][200], char choiceB[][200], // Declares the question-file loading function.
                   char choiceC[][200], char choiceD[][200], char *answer); // Declares the rest of its arrays and answer storage.
int validatesize(int total); // Declares the function that checks how many questions to ask.
void swap(int *a, int *b); // Declares the function that exchanges two integers.
void shuffle(int *order, int total); // Declares the function that randomizes question indexes.
void show_question(char question[][200], char choiceA[][200], char choiceB[][200], // Declares the function that displays a question and choices.
                   char choiceC[][200], char choiceD[][200], int q, int number); // Declares its remaining arrays and question numbers.
char make_uppercase(char letter); // Declares the function that converts lowercase letters to uppercase.
bool Valid_answer(int code); // Declares the function that checks whether a letter is A through D.
char get_answer(void); // Declares the function that reads a valid answer from the user.
bool check_answer(char user, char correct); // Declares the function that compares two answer letters.
void count_correct(bool *results, int size, int *score); // Declares the function that counts correct answers.
void find_percent(int score, int size, double *percent); // Declares the function that calculates the quiz percentage.
void print_results(int score, int size, double percent); // Declares the function that prints the score.
void review_wrong(char question[][200], int *order, char *userAnswer, // Declares the function that displays missed answers.
                  char *answer, bool *results, int size); // Declares the remaining data needed for the review.
void print_report(char question[][200], int *order, char *userAnswer, // Declares the function that prints every question's result.
                  char *answer, bool *results, int size, int score, double percent); // Declares the remaining report data.

int main(void) { // Starts the program.
    char question[50][200]; // Stores up to 50 question texts of up to 199 characters each.
    char choiceA[50][200]; // Stores choice A for each question.
    char choiceB[50][200]; // Stores choice B for each question.
    char choiceC[50][200]; // Stores choice C for each question.
    char choiceD[50][200]; // Stores choice D for each question.
    char answer[50]; // Stores the correct answer letter for each question.
    int total; // Holds the number of questions loaded from the file.
    int seed; // Holds the number used to initialize the random shuffle.
    char again = 'Y'; // Starts the quiz loop in the continue state.

    total = load_questions(question, choiceA, choiceB, choiceC, choiceD, answer); // Loads questions and their answers from questions.txt.
    if (total == 0) { // Checks whether the file was missing, empty, or invalid.
        printf("error loading questions.txt\n"); // Tells the user the question file could not be loaded.
        return 1; // Ends the program with an error status.
    } // Ends the file-loading error check.
    printf("%d questions loaded\n", total); // Shows how many questions are available.

    printf("Enter any number to shuffle the questions: "); // Prompts for the random seed.
    scanf("%d", &seed); // Reads the seed number entered by the user.
    srand((unsigned int)seed); // Initializes the random number generator with the user's seed.

    while (again == 'Y') { // Repeats the quiz while the user chooses Y.
        int size; // Holds how many questions this round will ask.
        int score; // Holds how many answers are correct.
        double percent; // Holds the score as a percentage.

        size = validatesize(total); // Asks the user how many available questions to answer.

        int order[50]; // Stores question indexes in shuffled order.
        char userAnswer[50]; // Stores the user's answer for each quiz position.
        bool results[50]; // Stores whether each answer was correct.

        shuffle(order, total); // Fills order with every question index in random order.

        for (int i = 0; i < size; i++) { // Goes through the requested number of questions.
            int q = order[i]; // Gets the question index for this quiz position.
            show_question(question, choiceA, choiceB, choiceC, choiceD, q, i + 1); // Displays that question and its four choices.
            userAnswer[i] = get_answer(); // Reads and stores the user's A-D answer.
            results[i] = check_answer(userAnswer[i], answer[q]); // Stores whether the user's answer matches the correct one.
        } // Ends the question-asking loop.

        count_correct(results, size, &score); // Counts the true values in results.
        find_percent(score, size, &percent); // Converts the score into a percentage.
        print_results(score, size, percent); // Prints the score and percentage.
        review_wrong(question, order, userAnswer, answer, results, size); // Shows each missed question and the correct letter.
        print_report(question, order, userAnswer, answer, results, size, score, percent); // Prints the complete results for the round.

        printf("\nTake another quiz? (Y/N): "); // Asks whether to start another round.
        scanf(" %c", &again); // Reads Y or N, skipping leftover spaces and newlines.
        again = make_uppercase(again); // Lets lowercase y or n work like uppercase input.
    } // Ends the repeat-quiz loop.

    printf("Goodbye!\n"); // Prints a closing message after the quiz loop ends.
    return 0; // Reports that the program finished successfully.
} // Ends main.

// Reads each question as seven lines: question, four choices, answer letter, and separator. // Documents the file layout.
int load_questions(char question[][200], char choiceA[][200], char choiceB[][200], // Defines the question-loading function.
                   char choiceC[][200], char choiceD[][200], char *answer) { // Receives the arrays where loaded data will be stored.
    FILE *file = fopen("questions.txt", "r"); // Opens questions.txt for reading.
    if (file == NULL) { // Checks whether opening the file failed.
        return 0; // Returns zero to indicate that no questions loaded.
    } // Ends the file-open check.

    int count = 0; // Tracks how many complete questions have been read.
    char line[200]; // Reuses this 200-character array for the answer and separator lines.

    // fgets(destination, size, file) reads one line, including its newline, into destination.
    // Here size is 200, so fgets reads at most 199 characters into each array.
    while (count < 50 && fgets(question[count], 200, file) != NULL) { // Reads the question line; NULL means the file has ended or could not be read.
        if (fgets(choiceA[count], 200, file) == NULL || // Reads the next line into choice A; NULL means the question entry is incomplete.
            fgets(choiceB[count], 200, file) == NULL || // Reads the next line into choice B; NULL means the question entry is incomplete.
            fgets(choiceC[count], 200, file) == NULL || // Reads the next line into choice C; NULL means the question entry is incomplete.
            fgets(choiceD[count], 200, file) == NULL || // Reads the next line into choice D; NULL means the question entry is incomplete.
            fgets(line, 200, file) == NULL) { // Reads the answer letter into the temporary line array.
            fclose(file); // Closes the file before returning on invalid input.
            return 0; // Reports that the question file was incomplete.
        } // Ends the check for missing question fields.

        if (!Valid_answer(make_uppercase(line[0]))) { // Checks that the first character is a letter from A to D.
            fclose(file); // Closes the file before returning on an invalid answer.
            return 0; // Reports that the question file contains an invalid answer.
        } // Ends the answer-letter validation.
        answer[count] = make_uppercase(line[0]); // Stores the correct answer as an uppercase letter.

        if (fgets(line, 200, file) == NULL) { // Reads the separator line after the answer; it is not stored as quiz data.
            fclose(file); // Closes the file if the separator is missing.
            return 0; // Reports an incomplete question entry.
        } // Ends the separator check.

        count++; // Marks this question as completely loaded.
    } // Ends the file-reading loop.

    fclose(file); // Closes the question file after reading it.
    return count; // Gives main the number of complete questions loaded.
} // Ends load_questions.

int validatesize(int total) { // Defines a function to read a valid number of questions.
    int size; // Stores the number entered by the user.
    printf("\nHow many questions (1 to %d): ", total); // Shows the allowed range.
    scanf("%d", &size); // Reads the user's number; enter a whole number.
    while (size < 1 || size > total) { // Repeats if the number is outside the allowed range.
        printf("Invalid number. Enter a value from 1 to %d: ", total); // Explains the valid range and prompts again.
        scanf("%d", &size); // Reads another whole number from the user.
    } // Ends the validation loop when size is valid.
    return size; // Sends the valid question count back to main.
} // Ends validatesize.

void swap(int *a, int *b) { // Defines a function to exchange two integers.
    int temp = *a; // Saves the first integer before overwriting it.
    *a = *b; // Copies the second integer into the first integer's location.
    *b = temp; // Copies the saved first integer into the second location.
} // Ends swap.

void shuffle(int *order, int total) { // Defines a function to randomize all question indexes.
    for (int i = 0; i < total; i++) { // Visits each position in the order array.
        order[i] = i; // Starts with the indexes in their original order.
    } // Ends the array-initialization loop.

    for (int i = total - 1; i > 0; i--) { // Moves backward through the indexes for Fisher-Yates shuffle.
        int j = rand() % (i + 1); // Chooses a random position from zero through i.
        swap(&order[i], &order[j]); // Exchanges the current index with the randomly chosen one.
    } // Ends the shuffle loop.
} // Ends shuffle.

void show_question(char question[][200], char choiceA[][200], char choiceB[][200], // Defines the question display function.
                   char choiceC[][200], char choiceD[][200], int q, int number) { // Receives the question index and its displayed number.
    printf("\nQuestion %d: %s", number, question[q]); // Prints the question; its file line already ends with a newline.
    printf("  %s", choiceA[q]); // Prints answer choice A, including its file newline.
    printf("  %s", choiceB[q]); // Prints answer choice B, including its file newline.
    printf("  %s", choiceC[q]); // Prints answer choice C, including its file newline.
    printf("  %s", choiceD[q]); // Prints answer choice D, including its file newline.
} // Ends show_question.

char make_uppercase(char letter) { // Defines a function to convert a lowercase letter to uppercase.
    int code = letter; // Gets the letter's numeric ASCII code.
    if (code >= 97 && code <= 122) { // Checks whether the code represents a lowercase a through z.
        code = code - 32; // Uses the ASCII difference to change lowercase to uppercase.
    } // Ends the lowercase check.
    letter = (char)code; // Converts the numeric code back to a character.
    return letter; // Returns the uppercase letter or the original character.
} // Ends make_uppercase.

bool Valid_answer(int code) { // Defines a function that accepts only A, B, C, or D.
    return code >= 65 && code <= 68; // ASCII codes 65 through 68 represent uppercase A through D.
} // Ends Valid_answer.

char get_answer(void) { // Defines a function that keeps asking until it gets A, B, C, or D.
    char letter; // Stores the character entered by the user.
    printf("Your answer: "); // Prompts the user to choose an answer.
    scanf(" %c", &letter); // Reads one character, skipping spaces and newlines.
    letter = make_uppercase(letter); // Accepts lowercase letters by converting them to uppercase.

    while (!Valid_answer(letter)) { // Repeats while the letter is outside A through D.
        printf("Invalid input, enter A, B, C or D: "); // Explains which answer letters are accepted.
        scanf(" %c", &letter); // Reads another character from the user.
        letter = make_uppercase(letter); // Converts the replacement letter to uppercase.
    } // Ends the invalid-answer retry loop.
    return letter; // Returns the valid uppercase answer.
} // Ends get_answer.

bool check_answer(char user, char correct) { // Defines a function that compares the two answer letters.
    int userCode = user; // Converts the user's letter to its ASCII number.
    int correctCode = correct; // Converts the correct letter to its ASCII number.
    return userCode == correctCode; // Returns true when the two ASCII values match.
} // Ends check_answer.

void count_correct(bool *results, int size, int *score) { // Defines a function that counts correct answers.
    *score = 0; // Starts the score at zero.
    for (int i = 0; i < size; i++) { // Checks each quiz result.
        if (results[i]) { // Tests whether this answer was correct.
            (*score)++; // Adds one point for a correct answer.
        } // Ends the correct-answer check.
    } // Ends the result-counting loop.
} // Ends count_correct.

void find_percent(int score, int size, double *percent) { // Defines a function that calculates the quiz percentage.
    *percent = (double)score / size * 100; // Divides correct answers by total and converts to percent.
} // Ends find_percent.

void print_results(int score, int size, double percent) { // Defines a function that prints the overall score.
    printf("\nScore: %d out of %d\n", score, size); // Prints the number correct and total questions.
    printf("Percent: %.2f%%\n", percent); // Prints the percentage with two decimal places.
} // Ends print_results.

void review_wrong(char question[][200], int *order, char *userAnswer, // Defines a function that reviews missed questions.
                  char *answer, bool *results, int size) { // Receives the answers, results, and number of questions.
    int missed = 0; // Counts how many incorrect answers are found.
    printf("\n--- Review ---\n"); // Prints a heading for the review section.
    for (int i = 0; i < size; i++) { // Checks each question asked in this round.
        if (!results[i]) { // Continues only when the answer was incorrect.
            int q = order[i]; // Finds the original question index for this quiz position.
            printf("\n%s", question[q]); // Prints the missed question, including its file newline.
            printf("Your answer: %c (%d)\n", userAnswer[i], userAnswer[i]); // Prints the user's letter and its ASCII value.
            printf("Correct answer: %c (%d)\n", answer[q], answer[q]); // Prints the correct letter and its ASCII value.
            missed++; // Records that one missed question was reviewed.
        } // Ends the incorrect-answer check.
    } // Ends the review loop.
    if (missed == 0) { // Checks whether every answer was correct.
        printf("Perfect score, nothing to review.\n"); // Tells the user there were no missed answers.
    } // Ends the perfect-score check.
} // Ends review_wrong.

void print_report(char question[][200], int *order, char *userAnswer, // Defines a function that prints a result for every question.
                  char *answer, bool *results, int size, int score, double percent) { // Receives quiz data and the overall score.
    printf("\n--- Quiz Report ---\n"); // Prints a heading for the full report.
    for (int i = 0; i < size; i++) { // Goes through each question asked in this round.
        int q = order[i]; // Finds the original question index for this position.
        printf("%d. %s", i + 1, question[q]); // Prints the quiz position and question, including its file newline.
        printf("   Your answer: %c | Correct answer: %c | %s\n", // Starts a line showing the user's and correct answers.
               userAnswer[i], answer[q], results[i] ? "Correct" : "Incorrect"); // Prints both letters and the result label.
    } // Ends the report loop.
    printf("Total: %d/%d (%.2f%%)\n", score, size, percent); // Prints the final score and percentage.
} // Ends print_report.
