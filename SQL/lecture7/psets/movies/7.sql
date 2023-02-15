--SQL query to list all movies released in 2010 and their ratings
SELECT title, rating FROM movies
JOIN ratings ON movies.id = ratings.movie_id
WHERE year = 2010 ORDER BY rating DESC, title ;