-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Create a suspects table
--CREATE TABLE susbcets (
   -- name TEXT
--);

-- quarry to read the descrption of the Crime
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7
AND day = 28
AND street = 'Humphrey Street';

-- quarry to read the witnesses interviews
SELECT name, transcript FROM interviews
WHERE year = 2021 AND month = 7
AND day = 28 AND transcript LIKE '%bakery%';

-- See the bakery security logs
SELECT people.name, bakery_security_logs.license_plate,
bakery_security_logs.activity, bakery_security_logs.hour, bakery_security_logs.minute
FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE year = 2021 AND month = 7
AND day = 28 AND hour = 10
AND minute BETWEEN 15 and 25 AND activity = 'exit';

-- See the atm trancations
SELECT people.name, atm_transactions.amount
FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE year = 2021 AND month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';

-- See the caller phone calls which lasted less than minute
SELECT people.name, phone_calls.caller FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE year = 2021 AND month = 7
AND day = 28 AND duration < 60;

-- See the recivers names
SELECT people.name, phone_calls.receiver FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE year = 2021 AND month = 7
AND day = 28 AND duration < 60;

-- insert into suspects list
--INSERT INTO suspects VALUES (bruce, diana, iman, luca);

-- See the name of the FarmVille airport
SELECT abbreviation, full_name, city FROM airports;

-- Find the next day's flights
SELECT airports.city, flights.hour, flights.minute FROM airports
join flights ON airports.id = flights.origin_airport_id
WHERE airports.abbreviation = 'CSF' AND year = 2021
AND month = 7 AND day = 29 ORDER BY flights.hour;

-- Find the Passengers names on the 8:20 flight
SELECT people.name FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
WHERE year = 2021 AND month = 7 AND
day = 29 AND hour = 8 AND minute = 20
ORDER BY people.name;

-- Update the suspects list
--DELETE FROM suspects WHERE name = 'iman' AND name = 'diana' AND name ='luca';

-- the theif's name is bruce because he's the only one that made a phone call, withdrawed, and booked a ticket within the time period
-- figure out where did he escape
SELECT airports.city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
JOIN passengers ON flights.id = passengers.flight_id
JOIN people ON passengers.passport_number = people.passport_number
WHERE people.name = 'Bruce';

-- figure out who helped him
SELECT people.name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE year = 2021 AND month = 7
AND day = 28 AND duration < 60 AND phone_calls.caller = '(367) 555-5533';



