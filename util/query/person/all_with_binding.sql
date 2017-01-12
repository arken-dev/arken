SELECT person.* FROM person LEFT JOIN orders ON orders.person_id = person.id AND orders.user_id = $user_id
