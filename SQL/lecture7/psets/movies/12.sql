-- SQL query to list all movies Johnny Depp and Helena Bonham Carter starred
SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = 'Johnny Depp'
AND title IN (SELECT title FROM movies
JOIN stars ON movies.id = stars.movie_id
JOIN people ON stars.person_id = people.id
WHERE people.name = 'Helena Bonham Carter')
ORDER BY movies.year;