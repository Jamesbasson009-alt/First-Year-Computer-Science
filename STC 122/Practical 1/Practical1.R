#####################
# Practical 1
# Name & Surname: James Basson
# Student number: 26080975
#####################

# Import the data
titles <- read.csv("titles.csv")


# Question 1

Q1a <- max(titles$imdb_score)

Q1b <- min(titles$imdb_score)

Q1c <- quantile(titles$imdb_score, 0.25)

Q1d <- quantile(titles$imdb_score, 0.50)

Q1e <- quantile(titles$imdb_score, 0.75)

mean_score <- mean(titles$imdb_score)
Q1f <- "C"

# Question 2

Q2 <- titles$title[which.max(titles$imdb_score)]

# Question 3

Q3a <- sum(titles$imdb_score > 8)

Q3b <- mean(titles$imdb_score >= 8)


# Question 4

hist(titles$imdb_score,
     breaks = 50,
     main = "Distribution of IMDB Scores",
     xlab = "IMDB Score",
     ylab = "Frequency")

hist(titles$imdb_score, breaks = 5,   main = "IMDB Scores (breaks = 5)",   xlab = "IMDB Score")
hist(titles$imdb_score, breaks = 10,  main = "IMDB Scores (breaks = 10)",  xlab = "IMDB Score")
hist(titles$imdb_score, breaks = 50,  main = "IMDB Scores (breaks = 50)",  xlab = "IMDB Score")
hist(titles$imdb_score, breaks = 100, main = "IMDB Scores (breaks = 100)", xlab = "IMDB Score")

# Question 5

Q5 <- subset(titles, type == "MOVIE")

# Question 6

Q6 <- mean(Q5$runtime)

# Question 7

Q7 <- sd(Q5$runtime)

# Question 8

lower_bound <- Q6 - 1.2 * Q7
upper_bound <- Q6 + 1.2 * Q7

in_range <- Q5$runtime >= lower_bound & Q5$runtime <= upper_bound

Q8 <- mean(in_range) * 100

# Question 9

boxplot(imdb_score ~ type, data = titles,
        main = "IMDB Ratings: Movies vs TV Shows",
        xlab = "Type",
        ylab = "IMDB Score",
        col = c("lightblue", "lightgreen"))
legend("topright",
       legend = c("Movie", "TV Show"),
       fill = c("lightblue", "lightgreen"))

# Question 10

Q10 <- "B"

# Question 11

Q11 <- table(titles$age_certification, titles$type)

# Question 12

Q12 <- sum(titles$age_certification == "G" & titles$type == "MOVIE")

# Question 13

barplot(table(titles$age_certification),
        main = "Distribution of Age Certifications",
        xlab = "Age Certification",
        ylab = "Count")