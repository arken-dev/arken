/**

Copyright 2011 Stefan Peters, all rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

Redistributions of source code must retain the above copyright notice, 
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE ORIGINAL AUTHOR ``AS IS'' AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE ORIGINAL AUTHOR OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include <stdlib.h>
#include <sys/select.h>

#include "pgsql.h"

/** private helper functions **/

/* server NOTICE message handler */
static void pg_notice(void *, const char *message) {
	fprintf(stderr, "debug.notice [%s]\n", message);
}

/* check and return pointer */
static void *luaL_checkpointer(lua_State* L, int i) {
	luaL_checktype(L, i, LUA_TLIGHTUSERDATA);
	return lua_touserdata(L, i);
}

/* check and return connection object parameter */
static con_t *luaL_checkconn(lua_State* L, int i) {
	luaL_checkudata(L, i, TYPE_CONNECTION);
   	return lua_toconn(L, i);
}

/* check and return result object parameter */
static rs_t *luaL_checkresult(lua_State* L, int i) {
	luaL_checkudata(L, i, TYPE_RESULT);
   	return lua_toresult(L, i);
}

/* push a connection object on the stack */
static void lua_pushconn(lua_State *L, PGconn *con) {
	con_t *p = lua_newconn(L);
	luaL_getmetatable(L, TYPE_CONNECTION);
	lua_setmetatable(L, -2);
	p->ptr = con;
	p->open = 1;
#ifdef DEBUG
	fprintf(stderr, "debug.lua_pushconn ptr [%p]\n", (void *)p->ptr);
	fprintf(stderr, "debug.lua_pushconn open [%d]\n", p->open);
#endif
}

/* push a result object on the stack */
static void lua_pushresult(lua_State *L, PGresult *rs) {
	rs_t *p = lua_newresult(L);
	luaL_getmetatable(L, TYPE_RESULT);
	lua_setmetatable(L, -2);
	p->ptr = rs;
	p->open = 1;
	p->row = 0;
#ifdef DEBUG
	fprintf(stderr, "debug.lua_pushresult ptr [%p]\n", (void *)p->ptr);
	fprintf(stderr, "debug.lua_pushresult open [%d]\n", p->open);
	fprintf(stderr, "debug.lua_pushresult row [%d]\n", p->row);
#endif
}

/* get one value from PGresult and push it onto the Lua stack */
static void lua_pushpgdata(lua_State *L, PGresult *rs, int row, int col) {
	const char *val;
	double temp;
	/* grab the value */
	if (PQgetisnull(rs, row, col)) {
		/* tasty NULLs - take that PHP! */
		lua_pushnil(L);
	} else {
		val = PQgetvalue(rs, row, col);
		switch (PQftype(rs, col)) {
		case BOOLOID:
			/* map postgresql default bool format */
			if (val[0] == 't') {
				lua_pushboolean(L, 1);
			} else {
				lua_pushboolean(L, 0);
			}
			break;
		case INT2OID:
		case INT4OID:
		case INT8OID:
		case FLOAT4OID:
		case FLOAT8OID:
			/* convert using Lua string -> number conversion for reliability */
			lua_pushstring(L, val);
			temp = lua_tonumber(L, -1);
			lua_pop(L, 1);
			lua_pushnumber(L, temp);
			break;
		case NUMERICOID:
		default:
			/* it's all just a string after that */
			lua_pushstring(L, val);
			break;
		}
	}
}

/* push a table onto the stack containing a row of data from PGresult - by edo1 */
static void lua_pushpgrow(lua_State *L, PGresult *rs, int row) {
	int col;
	int cols = PQnfields(rs);
	lua_createtable(L, cols, cols);
	for (col = 0; col < cols; col++) {
		/* grab the data */
		lua_pushpgdata(L, rs, row, col);
		/* give us an indexed ... */
		lua_pushvalue(L, -1);
		lua_rawseti(L, -3, col + 1);
		/* ... and assoc array */
		lua_pushstring(L, PQfname(rs, col));
		lua_pushvalue(L, -2);
		lua_settable(L, -4);
		lua_pop(L, 1);
	}
}

/* push a table onto the stack containing a row of data from PGresult - by edo1 */
static void lua_pushpgrow_assoc(lua_State *L, PGresult *rs, int row) {
	int col;
	int cols = PQnfields(rs);
	lua_createtable(L, cols, cols);
	for (col = 0; col < cols; col++) {
		/* grab the data */
		lua_pushpgdata(L, rs, row, col);
		/* give us an indexed ... */
		//lua_pushvalue(L, -1);
		//lua_rawseti(L, -3, col + 1);
		/* ... and assoc array */
		lua_pushstring(L, PQfname(rs, col));
		lua_pushvalue(L, -2);
		lua_settable(L, -4);
		lua_pop(L, 1);
	}
}
/** module registration **/

/* base functions */
static const luaL_Reg R_pg_functions[] = {
	{"connect", L_connect},
	{NULL, NULL}
};

/* connection objects methods */
static const luaL_Reg R_con_methods[] = {
	{"escape", L_con_escape},
	{"exec", L_con_exec},
	{"notifywait", L_con_notifywait},
	{"close", L_con_close},
	{NULL, NULL}
};

/* result object methods */
static const luaL_Reg R_res_methods[] = {
	{"count", L_res_count},
	{"fetch", L_res_fetch},
	{"cols", L_res_cols},
	{"rows", L_res_rows},
	{"clear", L_res_clear},
	{NULL, NULL}
};

/* open the library - used by require() */
extern "C" {
LUALIB_API int luaopen_oberon_pgsql(lua_State *L) {
	/* register the base functions and module tags */
	luaL_register(L, "pg", R_pg_functions);
	lua_pushliteral(L,"version");			/** version */
	lua_pushliteral(L,MYVERSION);
	lua_settable(L,-3);
	/* register the connection object type */
	luaL_newmetatable(L, TYPE_CONNECTION);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(L, NULL, R_con_methods);
	lua_pushcfunction(L, L_con_gc);
	lua_setfield(L, -2, "__gc");
	lua_pop(L, 1);
	/* register the result object type */
	luaL_newmetatable(L, TYPE_RESULT);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(L, NULL, R_res_methods);
	lua_pushcfunction(L, L_res_gc);
	lua_setfield(L, -2, "__gc");
	lua_pop(L, 1);
	/* return the library handle */
	return 1;
}
}

/** exported functions **/

/* pg.connect - connect to a database */
LUALIB_API int L_connect(lua_State *L) {
	PGconn *con = NULL;
	const char *info = (const char*)luaL_checkstring(L, 1);
	con = PQconnectdb(info);
	if (PQstatus(con) == CONNECTION_OK) {
		PQsetNoticeProcessor(con, pg_notice, (void *)con);
		lua_pushconn(L, con);
		lua_pushnil(L);
	} else {
		lua_pushnil(L);
		lua_pushstring(L, PQerrorMessage(con));
		PQfinish(con);
	}
	return 2;
}


/** PgSQL.Connection object **/

/* con:escape - escape a string for use in queries */
LUALIB_API int L_con_escape(lua_State *L) {
	size_t len;
	const char *src; char *dst;
	/* con_t *con = luaL_checkconn(L, 1); */
	src = luaL_checklstring(L, 2, &len);
	dst = (char *) calloc(len * 2 + 1, sizeof(char));
	/* PQescapeStringConn(con->ptr, dst, src, len, NULL); */
	PQescapeString(dst, src, len);
	lua_pushstring(L, dst);
	free(dst);
	return 1;
}

/* con:exec - execute a sql command, with or without parameters */
LUALIB_API int L_con_exec(lua_State *L) {
	PGresult *rs = NULL;
	const char **param = NULL;
	int param_count;
	char *bool_t[2] = {(char*)"FALSE", (char*)"TRUE"};
	con_t *con = luaL_checkconn(L, 1);
	const char *sql = luaL_checkstring(L, 2);
	int i;
#ifdef DEBUG
	fprintf(stderr, "debug.lua_con_exec ptr [%p]\n", (void *)con->ptr);
	fprintf(stderr, "debug.lua_con_exec open [%d]\n", con->open);
	fprintf(stderr, "debug.lua_con_exec sql [%s]\n", sql);
#endif
	if (PQstatus(con->ptr) == CONNECTION_OK) {
		if (lua_gettop(L) == 2) {
			/* no parameters, just an 'ol fashioned query */
			rs = PQexec(con->ptr, sql);
		} else {
			/* parameterized query */
			luaL_checktype(L, 3, LUA_TTABLE);
			if (lua_gettop(L) >= 4) {
				/* parameter count given, use it */
				param_count = luaL_checkinteger(L, 4);
			} else {
				/* parameter count not given, trust in the force (luaL_getn) */
				param_count = luaL_getn(L, 3);
			}
			/* clear-allocate params for PQexecParams */
			if (param_count > 0) param = (const char **) calloc(param_count, sizeof(char *));
			/* load params from Lua table into C array */
			for (i = 0; i < param_count; i++) {
				lua_rawgeti(L, 3, i + 1);
				if (lua_type(L, -1) == LUA_TBOOLEAN) {
					/* convert boolean into "TRUE" or "FALSE" */
					param[i] = bool_t[lua_toboolean(L, -1)];
				} else {
					param[i] = lua_tostring(L, -1);
				}
			}
			rs = PQexecParams(con->ptr, sql, param_count, NULL, param, NULL, NULL, 0);
			if (param) {
				lua_pop(L, param_count);	
				free(param);
			}
		}
		if (rs) {
			ExecStatusType status = PQresultStatus(rs);
			if (status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK) {
				lua_pushresult(L, rs);
				lua_pushnil(L);
			} else {
				lua_pushnil(L);
				lua_pushstring(L, PQresultErrorMessage(rs));
				PQclear(rs);
  	 		}
		} else {
  			lua_pushnil(L);
   		 	lua_pushliteral(L, "FATAL error");
  		}
	} else {
		lua_pushnil(L);
		lua_pushliteral(L, "Connection Failure");
	}
	return 2;
}

/* con:notifywait - wait for any NOTIFY message from server - by edo1 */
LUALIB_API int L_con_notifywait(lua_State *L) {
	con_t *con = luaL_checkconn(L, 1);
	int sock;
	fd_set input_mask;
	struct timeval tv;
	struct timeval *tvp;
	PGnotify *notify;
	int nnotifies = 0;
	if (lua_gettop(L) >= 2) {
		lua_Number t = lua_tonumber(L,2);
		tv.tv_sec = t;
		tv.tv_usec = (t - tv.tv_sec) * 1000000;
		tvp = &tv;
	} else {
		tvp = NULL;
	}
	sock = PQsocket(con->ptr);
	/* Now check for input */
	do {
		PQconsumeInput(con->ptr);
		while ((notify = PQnotifies(con->ptr)) != NULL)
		{
			PQfreemem(notify);
			nnotifies++;
		}
		if (nnotifies > 0) {
			tv.tv_sec = 0;
			tv.tv_usec = 0;
			tvp = &tv;
		}
		FD_ZERO(&input_mask);
		FD_SET(sock, &input_mask);
	} while (select(sock + 1, &input_mask, NULL, NULL, tvp) > 0);
	lua_pushinteger(L, nnotifies);
	return 1;
}

/* con:close - close the connection and free the client resources */
LUALIB_API int L_con_close(lua_State *L) {
	con_t *con = luaL_checkconn(L, 1);
	if (con->open) {
		con->open = false;
		PQfinish(con->ptr);
	}
	return 0;
}

/* connection object garbage collector */
LUALIB_API int L_con_gc(lua_State *L) {
	if (lua_isuserdata(L, 1)) {
		con_t *con = lua_toconn(L, 1);
#ifdef DEBUG
		fprintf(stderr, "debug.lua_con_gc ptr [%p]\n", (void *)con->ptr);
		fprintf(stderr, "debug.lua_con_gc open [%d]\n", con->open);
#endif
		if (con->open) {
			con->open = false;
			PQfinish(con->ptr);
		}
	}
	return 0;
}


/** PgSQL.Result object **/

/* rs:count - the number of rows returned OR affected by the sql command */
LUALIB_API int L_res_count(lua_State *L) {
	int n;
	rs_t *rs = luaL_checkresult(L, 1);
	if (PQresultStatus(rs->ptr) == PGRES_TUPLES_OK) {
		lua_pushinteger(L, PQntuples(rs->ptr));
	} else if (PQresultStatus(rs->ptr) == PGRES_COMMAND_OK) {
		lua_pushstring(L, PQcmdTuples(rs->ptr));
		n = lua_tonumber(L, -1);
		lua_pop(L, 1);
		lua_pushinteger(L, n);
	}
	return 1;
}

/* rs:fetch - tranditional 'fetch' interface */
LUALIB_API int L_res_fetch(lua_State *L) {
	rs_t *rs = luaL_checkresult(L, 1);
	int rows = PQntuples(rs->ptr);
	if (rs->row < rows) {
		lua_pushpgrow_assoc(L, rs->ptr, rs->row);
		/* next row */
		rs->row++;
		return 1;
	} else {
		/* no more values to return */
		return 0;
	}
}

/* rs:cols generator */
LUALIB_API int L_res_cols(lua_State *L) {
	rs_t *rs = luaL_checkresult(L, 1);
	lua_pushlightuserdata(L, rs->ptr);
	/* start at column 0 */
	lua_pushinteger(L, 0);
	lua_pushcclosure(L, L_res_col_iter, 2);
	return 1;
}

/* rs:cols iterator */
LUALIB_API int L_res_col_iter (lua_State *L) {
	int cols; int col;
	PGresult *rs;
	rs = (PGresult *)luaL_checkpointer(L, lua_upvalueindex(1));
	/* current column */
	col = luaL_checkinteger(L, lua_upvalueindex(2));
	/* number of columns */
	cols = PQnfields(rs);
	if (col < cols) {
		lua_pushinteger(L, col + 1);
		lua_pushstring(L, PQfname(rs, col));
		lua_pushinteger(L, col + 1); /* next column */
		lua_replace(L, lua_upvalueindex(2));
		return 2;
	} else return 0;  /* no more values to return */
}

/* rs:rows generator */
LUALIB_API int L_res_rows(lua_State *L) {
	rs_t *rs = luaL_checkresult(L, 1);
	lua_pushlightuserdata(L, rs->ptr);
	/* start at row 0 */
	lua_pushinteger(L, 0);
	lua_pushcclosure(L, L_res_row_iter, 2);
	return 1;
}

/* rs:rows iterator */
LUALIB_API int L_res_row_iter (lua_State *L) {
	int row;
	int rows;
	PGresult *rs;
	rs = (PGresult *)luaL_checkpointer(L, lua_upvalueindex(1));
	/* current row */
	row = luaL_checkinteger(L, lua_upvalueindex(2));
	rows = PQntuples(rs);
	if (row < rows) {
		lua_pushpgrow(L, rs, row);
		/* next row */
		lua_pushinteger(L, row + 1);
		lua_replace(L, lua_upvalueindex(2));
		return 1;
	} else {
        /* no more values to return */
		return 0;
	}
}

/* rs:clear - free the result set */
LUALIB_API int L_res_clear(lua_State *L) {
	rs_t *rs = luaL_checkresult(L, 1);
	if (rs->open) {
		rs->open = false;
		PQclear(rs->ptr);
	}
	return 0;
}

/* result object garbage collector */
LUALIB_API int L_res_gc(lua_State *L) {
	if (lua_isuserdata(L, 1)) {
		rs_t *rs = luaL_checkresult(L, 1);
#ifdef DEBUG
		fprintf(stderr, "debug.lua_rs_gc ptr [%p]\n", (void *)rs->ptr);
		fprintf(stderr, "debug.lua_rs_gc open [%d]\n", rs->open);
#endif
		if (rs->open) {
			rs->open = false;
			PQclear(rs->ptr);
		}
	}
	return 0;
}
