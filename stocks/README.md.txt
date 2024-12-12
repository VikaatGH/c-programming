
# Stock Profit and Loss Calculator

## Description
This program calculates the maximum possible profit and loss from stock trading. Stock prices are provided sequentially, and we calculate profits and losses based on a given time interval. A stock can be bought once and sold once within the interval, without short selling allowed.

## Input
The program takes stock prices followed by queries to calculate possible profit and loss.

Stock prices are provided as + price, e.g., + 200.
Queries use the format ? from to, e.g., ? 5 10.
Input ends when EOF is reached.

## Output
For each query, the program will print:

- The maximum possible profit (with the time of purchase and sale).
- The maximum possible loss.
- If no profit or loss can be achieved in the given interval, the result is N/A. 

## Incorrect input
If the input is invalid, the program prints an error message and exits.
Errors include:

Unrecognized input (not starting with + or ?).
Invalid stock prices (non-numeric, negative).
Invalid queries (missing numbers, negative start, end of interval greater than the number of known stock prices, or start of the interval is greater than the end).

If a program detects an error, it stops asking for further input, prints the error message, and exits. The error must be detected immediately after reading the value (do not delay validation until the entire input is read). The error message should be printed to the standard output, not to the standard error output.

## Running example 
+ 20                 # price at time 0
+ 30                 # price at time 1
+ 40                 # price at time 2
+ 10                 # price at time 3
? 0 2                # count max profit/loss for interval <0;2>
? 0 3                # count max profit/loss for interval <0;3>
+ 50                 # price at time 4
? 0 3                # count max profit/loss for interval <0;3>
? 1 4                # count max profit/loss for interval <1;4>

## Testing
The stocks folder contains several test cases and a script to run those tests. 
