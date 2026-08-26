-------------------------------------------------------------------------------
-- PATH
-------------------------------------------------------------------------------

package.path  = package.path .. ";app/models/?.lua"
package.path  = package.path .. ";app/controllers/?.lua"
package.path  = package.path .. ";lib/?.lua"

-------------------------------------------------------------------------------
-- GLOBALS
-------------------------------------------------------------------------------

ARKEN_TASK = false
APP_PATH   = os.pwd()
ARKEN_ENV  = os.getenv("ARKEN_ENV") or "development"
Object     = require('arken.oop.Object')
Class      = require('arken.oop.Class')

-------------------------------------------------------------------------------
-- LOCALE
-------------------------------------------------------------------------------

os.setlocale("C", "numeric")
