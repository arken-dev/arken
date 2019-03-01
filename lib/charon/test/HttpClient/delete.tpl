<% local urlIndex = self._url:indexOf('/', 8) -%>
DELETE <%= self._url:mid(urlIndex) -%> HTTP/1.1
User-Agent: libcurl-agent/1.0
<% for _, header in ipairs(self.headers) do -%>
<%= header .. '\r\n' -%>
<% end -%>
Host: <%= self._url:sub(8, urlIndex-1) .. "\r\n" -%>
Accept: */*
Content-Length: <%= tostring( #self._body ) .. "\r\n" -%>

<%= self._body -%>
