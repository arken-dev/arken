{
  "development" : {
    "adapter"  : "ActiveRecord.PostgresAdapter",
    "host"     : "localhost",
    "database" : "arken_development",
    "user"     : "postgres",
    "password" : "abc123"
  },
  "production" : {
    "adapter"  : "ActiveRecord.PostgresAdapter",
    "host"     : "localhost",
    "database" : "arken_development",
    "user"     : "postgres",
    "password" : "abc123"
  },
  "test" : {
    "adapter"  : "ActiveRecord.PostgresAdapter",
    "host"     : "localhost",
    "database" : "arken_test<%= self.number or '' -%>",
    "user"     : "postgres",
    "password" : "abc123"
  }
}
