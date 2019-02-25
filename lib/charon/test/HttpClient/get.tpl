<% local urlIndex = self._url:indexOf('/', 8) -%>
GET <%= self._url:mid(urlIndex) -%> HTTP/1.1
User-Agent: libcurl-agent/1.0
Host: <%= self._url:sub(8, urlIndex-1) .. "\r\n" -%>
Accept: */*
Content-Length: <%= tostring( #self._body ) .. "\r\n" -%>
Content-Type: application/x-www-form-urlencoded

<%= self._body -%>
