#####################
# Practical 0
# Name & Surname: James Basson
# Student number 26080975
#####################


# Question 1
Q1a <- (6 - 2) * 42^2                  # subtract, then multiply by 42 squared
Q1b <- (8^2+ 6)/(7-2)                  # 8 squared plus 6, divided by (7 - 2)
Q1c <- ((60+3)/7)^2 - 7 * 8            # (63/7) squared, minus 7 times 8



# Question 2
zar <- 1500                            # amount in South African Rand
Q2 <- (zar * 0.055) - 2                # apply 5.5% rate to zar, then subtract 2


# Question 3
## a
Q3a <- c(4, 8, 15, 16, 23, 42)         # create a numeric vector
## b
Q3b <- mean(Q3a)                       # calculate the mean of Q3a


# Question 4  
temp <- 28                             # temperature value to test
if (temp >= 25) {
  Q4 <- "hot"                          # temp is 25 or above -> hot
} else if (temp < 25) {
  Q4 <- "cold"                         # temp is below 25 -> cold
}


# Question 5
number <- 15                           # number to test for FizzBuzz
if (number %% 5 == 0 && number %% 3 == 0) {
  Q5 <- "FizzBuzz"                     # divisible by both 5 and 3
} else if (number %% 5 == 0) {
  Q5 <- "Buzz"                         # divisible by 5 only
} else if (number %% 3 == 0) {
  Q5 <- "fizz"                         # divisible by 3 only
}


# Question 6
balance <- 1000                        # starting balance
interest <- 0.08                       # annual interest rate
Q6 <- numeric(10)                      # empty vector to store balance each year
for (year in 1:10) {
  balance <- balance * (1 + interest)  # apply compound interest for the year
  Q6[year] <- balance                  # store the updated balance
}



# Question 7
titles <- read.csv("titles.csv")       # load the titles dataset from CSV


# Question 8
Q8 <- dim(titles)                      # get number of rows and columns


# Question 9
Q9 <- names(titles)                    # get column names


# Question 10
Q10 <- titles$title                    # extract the title column


# Question 11
Q11 <- head(titles, 10)                # preview the first 10 rows


# Question 12
Q12 <- 2026 - titles$release_year      # calculate age of each title in years

