local rhea    = require 'rhea'
local json    = require 'charon.json'
local Migrate = require 'rhea.Migrate'
local test    = {}
local result  = {}

function output(value)
  table.insert(result, value)
end

Migrate.output = output
rhea.output    = output

test.before = function()
  Migrate.dir = os.temp()
  result = {}
end

test.should_create_file_with_timestamp = function()
  local QDateTime   = require('QDateTime')
  Migrate.timestamp = QDateTime.currentDateTime():toString('yyyyMdhhmmss')
  local params = {}
  params[0] = 'migrate:generate'
  params[1] = 'alter path permission'
  rhea.run(params)
  --(Migrate.dir .. '/' .. Migrate.timestamp .. '_alter_path_permission.lua')
  assert( os.exists(Migrate.dir .. '/' .. Migrate.timestamp .. '_alter_path_permission.lua')  == true)
end

test.should_create_sql_file_with_paramter = function()
  local QDateTime   = require('QDateTime')
  Migrate.timestamp = QDateTime.currentDateTime():toString('yyyyMdhhmmss')
  local params = {}
  params[0] = 'migrate:generate'
  params[1] = 'alter path permission'
  params[2] = '--sql'

  rhea.run(params)
  --(Migrate.dir .. '/' .. Migrate.timestamp .. '_alter_path_permission.lua')
  assert( os.exists(Migrate.dir .. '/' .. Migrate.timestamp .. '_alter_path_permission.sql')  == true)
end

test.should_create_dir_file_if_not_exists = function()
  Migrate.dir = os.temp() .. '/' .. os.uuid()
  local QDateTime   = require('QDateTime')
  Migrate.timestamp = QDateTime.currentDateTime():toString('yyyyMdhhmmss')
  local params = {}
  params[0] = 'migrate:generate'
  params[1] = 'alter path permission'
  params[2] = '--sql'

  rhea.run(params)
  --(Migrate.dir .. '/' .. Migrate.timestamp .. '_alter_path_permission.lua')
  assert( os.exists(Migrate.dir .. '/' .. Migrate.timestamp .. '_alter_path_permission.sql')  == true)
end

test.should_error_if_parameter_is_empty = function()
  Migrate.dir = os.temp() .. '/' .. os.uuid()
  local QDateTime   = require('QDateTime')
  Migrate.timestamp = QDateTime.currentDateTime():toString('yyyyMdhhmmss')
  local params = {}
  params[0] = 'migrate:generate'
  params[1] = nil

  local status, message = pcall(rhea.run, params)
  assert( status == false )
  assert( message:contains('arg for migrate name') , message )
end

return test
