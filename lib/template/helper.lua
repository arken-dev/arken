-- http://stackoverflow.com/questions/4201142/rails-style-template-helpers-in-velocity
-- http://framework.zend.com/manual/current/en/modules/zend.view.helpers.html
-- http://framework.zend.com/manual/current/en/modules/zend.view.helpers.url.html

--[[
<?php echo $this->formText('email', 'you@example.com', array('size' => 32)) ?>
<%=        helper.formText('email', 'you@example.com', array('size' => 32)) %>


<% local img = helper.img("teste") -%>
<%= helper.link(img, {url = "teste"}) -%>
<%= helper.link(helper.img("icons/detalhes.png") , {url = "teste"}) -%>


<%= helper.link{img = "icons/teste.png", url = "teste"} -%>


<% local form = helper.form("pedido") -%>
<%= form.floatField("total", {default = 0.0})
<a href="<%= helper.url{ action = 'concluir' } -%>" data-remote='true'><%= helper.image("icone/detalhes") -%></a>
]]

local M = {}

M.test = function()
  return "helper !!! works !!! yes !!!"
end

return M
