CREATE TABLE IF NOT EXISTS person_migrate (
  id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, name VARCHAR(250), observation TEXT,
  created_at TEXT, updated_at TEXT, total REAL
)
