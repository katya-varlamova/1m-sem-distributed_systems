\c persons
drop table Person;
create table if not exists Person (
    id serial primary key,
    login varchar(80) unique,
    password bytea,
    name varchar(80),
    birthDate date,
    gender varchar(50),
    contact varchar(30),
    accountData varchar(600),
    photoLink varchar(600));
