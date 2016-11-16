-- Copyright (c) 2015 — 2016, Aapo Talvensaari

require 'json'

json.pretty = function(dt, lf, id, ac, ec)
    local s, e = (ec or json.encode)(dt)
    if not s then return s, e end
    lf, id, ac = lf or "\n", id or "  ", ac or " "
    local i, j, k, n, r, p, q  = 1, 0, 0, #s, {}, nil, nil
    local al = string.sub(ac, -1) == "\n"
    for x = 1, n do
        local c = string.sub(s, x, x)
        if not q and (c == "{" or c == "[") then
            r[i] = p == ":" and table.concat{ c, lf } or table.concat{ string.rep(id, j), c, lf }
            j = j + 1
        elseif not q and (c == "}" or c == "]") then
            j = j - 1
            if p == "{" or p == "[" then
                i = i - 1
                r[i] = table.concat{ string.rep(id, j), p, c }
            else
                r[i] = table.concat{ lf, string.rep(id, j), c }
            end
        elseif not q and c == "," then
            r[i] = table.concat{ c, lf }
            k = -1
        elseif not q and c == ":" then
            r[i] = table.concat{ c, ac }
            if al then
                i = i + 1
                r[i] = string.rep(id, j)
            end
        else
            if c == '"' and p ~= "\\" then
                q = not q and true or nil
            end
            if j ~= k then
                r[i] = string.rep(id, j)
                i, k = i + 1, j
            end
            r[i] = c
        end
        p, i = c, i + 1
    end
    return table.concat(r)
end
