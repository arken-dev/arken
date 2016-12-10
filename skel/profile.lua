package.path  = package.path .. ";app/models/?.lua"
package.path  = package.path .. ";app/controllers/?.lua"
package.path  = package.path .. ";app/helpers/?.lua"
package.path  = package.path .. ";lib/?.lua"

CHARON_ENV    = os.getenv("CHARON_ENV") or "development"

-------------------------------------------------------------------------------
-- PRODUCTION
-------------------------------------------------------------------------------

local template = require 'template'

if CHARON_ENV == 'production' then
  template.filter = function(file_name, buffer)
    if file_name:endsWith('.html') then
      return buffer:simplified()
    else
      return buffer
    end
  end
end
