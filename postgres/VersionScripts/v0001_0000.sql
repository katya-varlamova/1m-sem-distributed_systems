\c persons
drop table Person;
create table if not exists Person (
    id serial primary key,
    name varchar(80),
    address varchar(50),
    work varchar(50),
    age integer);
