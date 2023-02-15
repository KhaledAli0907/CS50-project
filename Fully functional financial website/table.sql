CREATE TABLE purchase (
    user_id INTEGER NOT NULL,
    symbol TEXT,
    shares INTEGER,
    price NUMBER,
    time_stamp TIME,
    name TEXT
);

CREATE TABLE own (
    user_id INTEGER NOT NULL ,
    symbol TEXT,
    shares INTEGER,
    name TEXT
);