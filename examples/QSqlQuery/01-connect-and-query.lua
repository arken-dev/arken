local QSqlDatabase = require 'QSqlDatabase'
local QSqlQuery    = require 'QSqlQuery'

db = QSqlDatabase.addDatabase("QPSQL");
db:setHostName("localhost");
db:setDatabaseName("fusion_development");
db:setUserName("postgres");
db:setPassword("");
ok = db:open();
print(ok)
query = db:query()
result = query:exec("SELECT attname FROM pg_attribute")
while(query:next()) do
  local attname = query:value("attname")
end

